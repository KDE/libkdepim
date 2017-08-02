/*
  Copyright (c) 2015-2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "addressline/completionorder/completionordereditor.h"
#include "addresseelineeditmanager.h"
#include "addresseelineedit_p.h"
#include "addresseelineedit.h"
#include "kmailcompletion.h"
#include "libkdepimakonadi_debug.h"

#include <QMap>
#include <QTimer>
#include <kcolorscheme.h>
#include <kcompletionbox.h>
#include <AkonadiCore/Session>
#include <Akonadi/Contact/ContactSearchJob>
#include <Akonadi/Contact/ContactGroupSearchJob>
#include <AkonadiCore/CollectionFetchJob>
#include <AkonadiCore/ItemFetchScope>
#include <AkonadiCore/Job>
#include <QApplication>
#include <QToolButton>
#include <KConfigGroup>
#include <KLocalizedString>
#include <AkonadiSearch/PIM/contactcompleter.h>

#include <ldap/ldapclientsearch.h>
#include <addressline/addresslineedit/baloocompletionemail.h>
#include <akonadi/contact/contactsearchjob.h>

using namespace KPIM;
AddresseeLineEditPrivate::AddresseeLineEditPrivate(KPIM::AddresseeLineEdit *qq, bool enableCompletion)
    : QObject(qq)
    , q(qq)
    , m_recentAddressConfig(nullptr)
    , m_useCompletion(enableCompletion)
    , m_completionInitialized(false)
    , m_smartPaste(false)
    , m_addressBookConnected(false)
    , m_lastSearchMode(false)
    , m_searchExtended(false)
    , m_useSemicolonAsSeparator(false)
    , m_showOU(false)
    , m_enableBalooSearch(true)
    , m_enableAkonadiSearch(true)
    , mExpandIntern(true)
    , mAutoGroupExpand(false)
    , mShowRecentAddresses(true)
    , mCanDeleteLineEdit(true)
{
    m_delayedQueryTimer.setSingleShot(true);
    connect(&m_delayedQueryTimer, &QTimer::timeout, this, &AddresseeLineEditPrivate::slotTriggerDelayedQueries);
}

AddresseeLineEditPrivate::~AddresseeLineEditPrivate()
{
    if (AddresseeLineEditManager::self()->addressLineEdit() == q) {
        AddresseeLineEditManager::self()->stopLDAPLookup();
    }
}

void AddresseeLineEditPrivate::restartTime(const QString &searchString)
{
    if (useCompletion()) {
        AddresseeLineEditManager::self()->restartLdap(searchString, q);
    }
}

static const QString s_completionItemIndentString = QStringLiteral("     ");

class SourceWithWeight
{
public:
    int weight;           // the weight of the source
    int index;            // index into AddresseeLineEditStatic::self()->completionSources
    QString sourceName;   // the name of the source, e.g. "LDAP Server"

    bool operator<(const SourceWithWeight &other) const
    {
        if (weight > other.weight) {
            return true;
        }

        if (weight < other.weight) {
            return false;
        }

        return sourceName < other.sourceName;
    }
};

void AddresseeLineEditPrivate::init()
{
    m_toolButton = new QToolButton(q);
    m_toolButton->setVisible(false);
    m_toolButton->setCursor(Qt::ArrowCursor);
    const int size = q->sizeHint().height() - 5;
    m_toolButton->setFixedSize(size, size);
    int padding = (q->sizeHint().height() - size) / 2;
    m_toolButton->move(2, padding);
    m_toolButton->setStyleSheet(QStringLiteral("QToolButton { border: none; }"));
    connect(m_toolButton, &QToolButton::clicked, q, &AddresseeLineEdit::iconClicked);

    if (!AddresseeLineEditManager::self()) {
        AddresseeLineEditManager::self()->completion()->setOrder(KCompletion::Weighted);
        AddresseeLineEditManager::self()->completion()->setIgnoreCase(true);
    }

    if (m_useCompletion) {
        AddresseeLineEditManager::self()->initializeLdap();
        AddresseeLineEditManager::self()->setBalooCompletionSource(q->addCompletionSource(i18nc("@title:group", "Contacts found in your data"), -1));

        AddresseeLineEditManager::self()->updateLDAPWeights();
        if (!m_completionInitialized) {
            q->setCompletionObject(AddresseeLineEditManager::self()->completion(), false);
            connect(q, &KLineEdit::completion,
                    this, &AddresseeLineEditPrivate::slotCompletion);
            connect(q, SIGNAL(returnPressed(QString)),
                    this, SLOT(slotReturnPressed(QString)));

            KCompletionBox *box = q->completionBox();
            connect(box, SIGNAL(activated(QString)),
                    this, SLOT(slotPopupCompletion(QString)));
            connect(box, &KCompletionBox::userCancelled,
                    this, &AddresseeLineEditPrivate::slotUserCancelled);
            connect(AddresseeLineEditManager::self()->ldapTimer(), &QTimer::timeout, this, &AddresseeLineEditPrivate::slotStartLDAPLookup);
            connect(AddresseeLineEditManager::self()->ldapSearch(), SIGNAL(searchData(KLDAP::LdapResult::List)),
                    SLOT(slotLDAPSearchData(KLDAP::LdapResult::List)));

            m_completionInitialized = true;
        }

        KConfigGroup group(KSharedConfig::openConfig(), "AddressLineEdit");
        m_showOU = group.readEntry("ShowOU", false);
        mAutoGroupExpand = group.readEntry("AutoGroupExpand", false);

        loadBalooBlackList();
    }
    connect(q, &AddresseeLineEdit::textCompleted, q, &AddresseeLineEdit::slotEditingFinished);
    connect(q, &AddresseeLineEdit::editingFinished, q, &AddresseeLineEdit::slotEditingFinished);
}

void AddresseeLineEditPrivate::setIcon(const QIcon &icon, const QString &tooltip)
{
    if (icon.isNull()) {
        m_toolButton->setVisible(false);
        q->setStyleSheet(QString());
    } else {
        m_toolButton->setIcon(icon);
        m_toolButton->setToolTip(tooltip);
        const int padding = m_toolButton->width() - q->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        q->setStyleSheet(QStringLiteral("QLineEdit { padding-left: %1px }").arg(padding));
        m_toolButton->setVisible(true);
    }
}

QStringList AddresseeLineEdit::cleanupEmailList(const QStringList &inputList)
{
    return d->cleanupEmailList(inputList);
}

QStringList AddresseeLineEditPrivate::cleanupEmailList(const QStringList &inputList)
{
    KPIM::BalooCompletionEmail completionEmail;
    completionEmail.setEmailList(inputList);
    completionEmail.setBlackList(m_balooBlackList);
    completionEmail.setExcludeDomain(m_domainExcludeList);
    const QStringList listEmail = completionEmail.cleanupEmailList();
    return listEmail;
}

void AddresseeLineEditPrivate::searchInBaloo()
{
    const QString trimmedString = m_searchString.trimmed();
    Akonadi::Search::PIM::ContactCompleter com(trimmedString, 20);
    const QStringList listEmail = cleanupEmailList(com.complete());
    for (const QString &email : listEmail) {
        addCompletionItem(email, 1, AddresseeLineEditManager::self()->balooCompletionSource());
    }
    doCompletion(m_lastSearchMode);
}

void AddresseeLineEditPrivate::alternateColor()
{
    const KColorScheme colorScheme(QPalette::Active, KColorScheme::View);
    m_alternateColor = colorScheme.background(KColorScheme::AlternateBackground).color();
}

void AddresseeLineEditPrivate::setCompletedItems(const QStringList &items, bool autoSuggest)
{
    KCompletionBox *completionBox = q->completionBox();

    if (!items.isEmpty()
        && !(items.count() == 1 && m_searchString == items.first())) {
        completionBox->clear();
        const int numberOfItems(items.count());
        for (int i = 0; i < numberOfItems; ++i) {
            QListWidgetItem *item = new QListWidgetItem(items.at(i), completionBox);
            if (!items.at(i).startsWith(s_completionItemIndentString)) {
                if (!m_alternateColor.isValid()) {
                    alternateColor();
                }
                item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
                item->setBackgroundColor(m_alternateColor);
            }
            completionBox->addItem(item);
        }
        if (!completionBox->isVisible()) {
            if (!m_searchString.isEmpty()) {
                completionBox->setCancelledText(m_searchString);
            }
            completionBox->popup();
            // we have to install the event filter after popup(), since that
            // calls show(), and that's where KCompletionBox installs its filter.
            // We want to be first, though, so do it now.
            if (AddresseeLineEditManager::self()->completion()->order() == KCompletion::Weighted) {
                qApp->installEventFilter(q);
            }
        }

        QListWidgetItem *item = completionBox->item(1);
        if (item) {
            completionBox->blockSignals(true);
            completionBox->setCurrentItem(item);
            item->setSelected(true);
            completionBox->blockSignals(false);
        }

        if (autoSuggest) {
            const int index = items.first().indexOf(m_searchString);
            const QString newText = items.first().mid(index);
            q->callSetUserSelection(false);
            q->callSetCompletedText(newText, true);
        }
    } else {
        if (completionBox && completionBox->isVisible()) {
            completionBox->hide();
            completionBox->setItems(QStringList());
        }
    }
}

void AddresseeLineEditPrivate::addCompletionItem(const QString &string, int weight, int completionItemSource, const QStringList *keyWords)
{
    // Check if there is an exact match for item already, and use the
    // maximum weight if so. Since there's no way to get the information
    // from KCompletion, we have to keep our own QMap.
    // We also update the source since the item should always be shown from the source with the highest weight

    AddresseeLineEditManager::CompletionItemsMap::iterator it = AddresseeLineEditManager::self()->completionItemMap.find(string);
    if (it != AddresseeLineEditManager::self()->completionItemMap.end()) {
        weight = qMax((*it).first, weight);
        (*it).first = weight;
        (*it).second = completionItemSource;
    } else {
        AddresseeLineEditManager::self()->completionItemMap.insert(string, qMakePair(weight, completionItemSource));
    }

    AddresseeLineEditManager::self()->completion()->addItem(string, weight);
    if (keyWords && !keyWords->isEmpty()) {
        AddresseeLineEditManager::self()->completion()->addItemWithKeys(string, weight, keyWords);    // see kmailcompletion.cpp
    }
}

const QStringList KPIM::AddresseeLineEditPrivate::adjustedCompletionItems(bool fullSearch)
{
    QStringList items = fullSearch
                        ? AddresseeLineEditManager::self()->completion()->allMatches(m_searchString)
                        : AddresseeLineEditManager::self()->completion()->substringCompletion(m_searchString);

    //force items to be sorted by email
    items.sort();

    // For weighted mode, the algorithm is the following:
    // In the first loop, we add each item to its section (there is one section per completion source)
    // We also add spaces in front of the items.
    // The sections are appended to the items list.
    // In the second loop, we then walk through the sections and add all the items in there to the
    // sorted item list, which is the final result.
    //
    // The algo for non-weighted mode is different.

    int lastSourceIndex = -1;
    unsigned int i = 0;

    // Maps indices of the items list, which are section headers/source items,
    // to a QStringList which are the items of that section/source.
    QMap<int, QStringList> sections;
    QStringList sortedItems;
    for (QStringList::Iterator it = items.begin(); it != items.end(); ++it, ++i) {
        AddresseeLineEditManager::CompletionItemsMap::const_iterator cit = AddresseeLineEditManager::self()->completionItemMap.constFind(*it);
        if (cit == AddresseeLineEditManager::self()->completionItemMap.constEnd()) {
            continue;
        }

        const int index = (*cit).second;

        if (AddresseeLineEditManager::self()->completion()->order() == KCompletion::Weighted) {
            if (lastSourceIndex == -1 || lastSourceIndex != index) {
                const QString sourceLabel(AddresseeLineEditManager::self()->completionSources.at(index));
                if (sections.find(index) == sections.end()) {
                    it = items.insert(it, sourceLabel);
                    ++it; //skip new item
                }
                lastSourceIndex = index;
            }

            (*it) = (*it).prepend(s_completionItemIndentString);
            // remove preferred email sort <blank> added in  addContact()
            (*it).replace(QLatin1String("  <"), QStringLiteral(" <"));
        }
        sections[ index ].append(*it);

        if (AddresseeLineEditManager::self()->completion()->order() == KCompletion::Sorted) {
            sortedItems.append(*it);
        }
    }

    if (AddresseeLineEditManager::self()->completion()->order() == KCompletion::Weighted) {
        // Sort the sections
        QList<SourceWithWeight> sourcesAndWeights;
        const int numberOfCompletionSources(AddresseeLineEditManager::self()->completionSources.count());
        sourcesAndWeights.reserve(numberOfCompletionSources);
        for (int i = 0; i < numberOfCompletionSources; ++i) {
            SourceWithWeight sww;
            sww.sourceName = AddresseeLineEditManager::self()->completionSources.at(i);
            sww.weight = AddresseeLineEditManager::self()->completionSourceWeights[sww.sourceName];
            sww.index = i;
            sourcesAndWeights.append(sww);
        }

        std::sort(sourcesAndWeights.begin(), sourcesAndWeights.end());
        // Add the sections and their items to the final sortedItems result list
        const int numberOfSources(sourcesAndWeights.size());
        for (int i = 0; i < numberOfSources; ++i) {
            const SourceWithWeight source = sourcesAndWeights.at(i);
            const QStringList sectionItems = sections[source.index];
            if (!sectionItems.isEmpty()) {
                sortedItems.append(source.sourceName);
                for (const QString &itemInSection : sectionItems) {
                    sortedItems.append(itemInSection);
                }
            }
        }
    } else {
        sortedItems.sort();
    }

    return sortedItems;
}

void AddresseeLineEditPrivate::updateSearchString()
{
    m_searchString = q->text();

    int n = -1;
    bool inQuote = false;
    const int searchStringLength = m_searchString.length();
    for (int i = 0; i < searchStringLength; ++i) {
        const QChar searchChar = m_searchString.at(i);
        if (searchChar == QLatin1Char('"')) {
            inQuote = !inQuote;
        }

        if (searchChar == QLatin1Char('\\')
            && (i + 1) < searchStringLength && m_searchString.at(i + 1) == QLatin1Char('"')) {
            ++i;
        }

        if (inQuote) {
            continue;
        }

        if (i < searchStringLength
            && (searchChar == QLatin1Char(',')
                || (m_useSemicolonAsSeparator && searchChar == QLatin1Char(';')))) {
            n = i;
        }
    }

    if (n >= 0) {
        ++n; // Go past the ","

        const int len = m_searchString.length();

        // Increment past any whitespace...
        while (n < len && m_searchString.at(n).isSpace()) {
            ++n;
        }

        m_previousAddresses = m_searchString.left(n);
        m_searchString = m_searchString.mid(n).trimmed();
    } else {
        m_previousAddresses.clear();
    }
}

void AddresseeLineEditPrivate::slotTriggerDelayedQueries()
{
    const QString strSearch = m_searchString.trimmed();
    if (strSearch.size() <= 2) {
        return;
    }

    if (m_enableBalooSearch) {
        searchInBaloo();
    }

    // We send a contactsearch job through akonadi.
    // This not only searches baloo but also servers if remote search is enabled
    if (m_enableAkonadiSearch) {
        akonadiPerformSearch();
    }
}

void AddresseeLineEditPrivate::startSearches()
{
    if (!m_delayedQueryTimer.isActive()) {
        m_delayedQueryTimer.start(50);
    }
}

void AddresseeLineEditPrivate::akonadiPerformSearch()
{
    qCDebug(LIBKDEPIMAKONADI_LOG) << "searching akonadi with:" << m_searchString;

    // first, kill all job still in flight, they are no longer current
    for (const QWeakPointer<Akonadi::Job> &job : qAsConst(AddresseeLineEditManager::self()->akonadiJobsInFlight)) {
        if (!job.isNull()) {
            job.data()->kill();
        }
    }
    AddresseeLineEditManager::self()->akonadiJobsInFlight.clear();

    // now start new jobs
    Akonadi::ContactSearchJob *contactJob = new Akonadi::ContactSearchJob(AddresseeLineEditManager::self()->akonadiSession());
    contactJob->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
    contactJob->setQuery(Akonadi::ContactSearchJob::NameOrEmail, m_searchString,
                         Akonadi::ContactSearchJob::ContainsWordBoundaryMatch);
    connect(contactJob, &Akonadi::ItemSearchJob::itemsReceived,
            this, &AddresseeLineEditPrivate::slotAkonadiHandleItems);
    connect(contactJob, &KJob::result,
            this, &AddresseeLineEditPrivate::slotAkonadiSearchResult);

    Akonadi::ContactGroupSearchJob *groupJob = new Akonadi::ContactGroupSearchJob(AddresseeLineEditManager::self()->akonadiSession());
    groupJob->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
    groupJob->setQuery(Akonadi::ContactGroupSearchJob::Name, m_searchString,
                       Akonadi::ContactGroupSearchJob::ContainsMatch);
    connect(contactJob, &Akonadi::ItemSearchJob::itemsReceived,
            this, &AddresseeLineEditPrivate::slotAkonadiHandleItems);
    connect(groupJob, &KJob::result,
            this, &AddresseeLineEditPrivate::slotAkonadiSearchResult);

    AddresseeLineEditManager::self()->akonadiJobsInFlight.append(contactJob);
    AddresseeLineEditManager::self()->akonadiJobsInFlight.append(groupJob);
    akonadiHandlePending();
}

void AddresseeLineEditPrivate::akonadiHandlePending()
{
    qCDebug(LIBKDEPIMAKONADI_LOG) << "Pending items: " << AddresseeLineEditManager::self()->akonadiPendingItems.size();
    Akonadi::Item::List::iterator it = AddresseeLineEditManager::self()->akonadiPendingItems.begin();
    while (it != AddresseeLineEditManager::self()->akonadiPendingItems.end()) {
        const Akonadi::Item item = *it;

        const AddresseeLineEditManager::collectionInfo sourceIndex
            = AddresseeLineEditManager::self()->akonadiCollectionToCompletionSourceMap.value(item.parentCollection().id(), AddresseeLineEditManager::collectionInfo());
        if (sourceIndex.index >= 0) {
            qCDebug(LIBKDEPIMAKONADI_LOG) << "identified collection: " << AddresseeLineEditManager::self()->completionSources[sourceIndex.index];
            if (sourceIndex.enabled) {
                q->addItem(item, 1, sourceIndex.index);
            }

            // remove from the pending
            it = AddresseeLineEditManager::self()->akonadiPendingItems.erase(it);
        } else {
            ++it;
        }
    }
}

void AddresseeLineEditPrivate::doCompletion(bool ctrlT)
{
    m_lastSearchMode = ctrlT;

    const KCompletion::CompletionMode mode = q->completionMode();

    if (mode == KCompletion::CompletionNone) {
        return;
    }

    AddresseeLineEditManager::self()->completion()->setOrder(KCompletion::Weighted);

    // cursor at end of string - or Ctrl+T pressed for substring completion?
    if (ctrlT) {
        const QStringList completions = adjustedCompletionItems(false);

        if (completions.count() == 1) {
            q->setText(m_previousAddresses + completions.first().trimmed());
        }

        // Make sure the completion popup is closed if no matching items were found
        setCompletedItems(completions, true);

        q->cursorAtEnd();
        q->setCompletionMode(mode);   //set back to previous mode
        return;
    }

    switch (mode) {
    case KCompletion::CompletionPopupAuto:
        if (m_searchString.isEmpty()) {
            break;
        }
    //else: fall-through to the CompletionPopup case
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
        Q_FALLTHROUGH();
#endif

    case KCompletion::CompletionPopup:
    {
        const QStringList items = adjustedCompletionItems(false);
        setCompletedItems(items, false);
        break;
    }

    case KCompletion::CompletionShell:
    {
        const QString match = AddresseeLineEditManager::self()->completion()->makeCompletion(m_searchString);
        if (!match.isNull() && match != m_searchString) {
            q->setText(m_previousAddresses + match);
            q->setModified(true);
            q->cursorAtEnd();
        }
        break;
    }

    case KCompletion::CompletionMan: // Short-Auto in fact
    case KCompletion::CompletionAuto:
        //force autoSuggest in KLineEdit::keyPressed or setCompletedText will have no effect
        q->setCompletionMode(q->completionMode());

        if (!m_searchString.isEmpty()) {
            //if only our \" is left, remove it since user has not typed it either
            if (m_searchExtended && m_searchString == QLatin1String("\"")) {
                m_searchExtended = false;
                m_searchString.clear();
                q->setText(m_previousAddresses);
                break;
            }

            QString match = AddresseeLineEditManager::self()->completion()->makeCompletion(m_searchString);

            if (!match.isEmpty()) {
                if (match != m_searchString) {
                    const QString adds = m_previousAddresses + match;
                    q->callSetCompletedText(adds);
                }
            } else {
                if (!m_searchString.startsWith(QLatin1Char('\"'))) {
                    //try with quoted text, if user has not type one already
                    match = AddresseeLineEditManager::self()->completion()->makeCompletion(QLatin1String("\"") + m_searchString);
                    if (!match.isEmpty() && match != m_searchString) {
                        m_searchString = QLatin1String("\"") + m_searchString;
                        m_searchExtended = true;
                        q->setText(m_previousAddresses + m_searchString);
                        q->callSetCompletedText(m_previousAddresses + match);
                    }
                } else if (m_searchExtended) {
                    //our added \" does not work anymore, remove it
                    m_searchString = m_searchString.mid(1);
                    m_searchExtended = false;
                    q->setText(m_previousAddresses + m_searchString);
                    //now try again
                    match = AddresseeLineEditManager::self()->completion()->makeCompletion(m_searchString);
                    if (!match.isEmpty() && match != m_searchString) {
                        const QString adds = m_previousAddresses + match;
                        q->setCompletedText(adds);
                    }
                }
            }
        }
        break;

    case KCompletion::CompletionNone:
    default: // fall through
        break;
    }
}

void AddresseeLineEditPrivate::slotCompletion()
{
    // Called by KLineEdit's keyPressEvent for CompletionModes
    // Auto,Popup -> new text, update search string.
    // not called for CompletionShell, this is been taken care of
    // in AddresseeLineEdit::keyPressEvent

    updateSearchString();
    if (q->completionBox()) {
        q->completionBox()->setCancelledText(m_searchString);
    }

    startSearches();
    doCompletion(false);
}

void AddresseeLineEditPrivate::slotPopupCompletion(const QString &completion)
{
    QString c = completion.trimmed();
    if (c.endsWith(QLatin1Char(')'))) {
        c = completion.mid(0, completion.lastIndexOf(QLatin1String(" ("))).trimmed();
    }
    q->setText(m_previousAddresses + c);
    q->cursorAtEnd();
    updateSearchString();
    q->emitTextCompleted();
}

void AddresseeLineEditPrivate::slotReturnPressed(const QString &)
{
    if (!q->completionBox()->selectedItems().isEmpty()) {
        slotPopupCompletion(q->completionBox()->selectedItems().constFirst()->text());
    }
}

void AddresseeLineEditPrivate::slotStartLDAPLookup()
{
    if (AddresseeLineEditManager::self()->isOnline()) {
        const KCompletion::CompletionMode mode = q->completionMode();
        if (mode == KCompletion::CompletionNone) {
            return;
        }
        if (!AddresseeLineEditManager::self()->ldapSearch()->isAvailable()) {
            return;
        }
        if (AddresseeLineEditManager::self()->addressLineEdit() != q) {
            return;
        }
        AddresseeLineEditManager::self()->startLoadingLDAPEntries();
    }
}

void AddresseeLineEditPrivate::slotLDAPSearchData(const KLDAP::LdapResult::List &results)
{
    if (results.isEmpty() || AddresseeLineEditManager::self()->addressLineEdit() != q) {
        return;
    }

    for (const KLDAP::LdapResult &result : results) {
        KContacts::Addressee contact;
        contact.setNameFromString(result.name);
        contact.setEmails(result.email);
        QString ou;

        if (m_showOU) {
            const int depth = result.dn.depth();
            for (int i = 0; i < depth; ++i) {
                const QString rdnStr = result.dn.rdnString(i);
                if (rdnStr.startsWith(QStringLiteral("ou="), Qt::CaseInsensitive)) {
                    ou = rdnStr.mid(3);
                    break;
                }
            }
        }

        if (!AddresseeLineEditManager::self()->isLdapClientToCompletionSourceMapContains(result.clientNumber)) {
            AddresseeLineEditManager::self()->updateLDAPWeights(); // we got results from a new source, so update the completion sources
        }

        q->addContact(contact, result.completionWeight,
                      AddresseeLineEditManager::self()->ldapClientToCompletionSourceValue(result.clientNumber), ou);
    }

    if ((q->hasFocus() || q->completionBox()->hasFocus())
        && q->completionMode() != KCompletion::CompletionNone
        && q->completionMode() != KCompletion::CompletionShell) {
        q->setText(m_previousAddresses + m_searchString);
        // only complete again if the user didn't change the selection while
        // we were waiting; otherwise the completion box will be closed
        const QListWidgetItem *current = q->completionBox()->currentItem();
        if (!current || m_searchString.trimmed() != current->text().trimmed()) {
            doCompletion(m_lastSearchMode);
        }
    }
}

void AddresseeLineEditPrivate::slotEditCompletionOrder()
{
    init(); // for AddresseeLineEditStatic::self()->ldapSearch
    if (m_useCompletion) {
        QPointer<CompletionOrderEditor> dlg = new CompletionOrderEditor(AddresseeLineEditManager::self()->ldapSearch(), nullptr);
        if (dlg->exec()) {
            AddresseeLineEditManager::self()->updateCompletionOrder();
        }
        delete dlg;
    }
}

KLDAP::LdapClientSearch *AddresseeLineEditPrivate::ldapSearch()
{
    init(); // for AddresseeLineEditStatic::self()->ldapSearch
    return AddresseeLineEditManager::self()->ldapSearch();
}

void AddresseeLineEditPrivate::slotUserCancelled(const QString &cancelText)
{
    if (AddresseeLineEditManager::self()->addressLineEdit() == q) {
        AddresseeLineEditManager::self()->stopLDAPLookup();
    }

    q->callUserCancelled(m_previousAddresses + cancelText);   // in KLineEdit
}

void AddresseeLineEditPrivate::slotAkonadiHandleItems(const Akonadi::Item::List &items)
{
    /* We have to fetch the collections of the items, so that
       the source name can be correctly labeled.*/
    for (const Akonadi::Item &item : items) {
        // check the local cache of collections
        const Akonadi::Collection::Id colId = item.parentCollection().id();
        const AddresseeLineEditManager::collectionInfo sourceIndex
            = AddresseeLineEditManager::self()->akonadiCollectionToCompletionSourceMap.value(colId, AddresseeLineEditManager::collectionInfo());
        if (sourceIndex.index == -1) {
            qCDebug(LIBKDEPIMAKONADI_LOG) << "Fetching New collection: " << colId;
            // the collection isn't there, start the fetch job.
            Akonadi::CollectionFetchJob *collectionJob
                = new Akonadi::CollectionFetchJob(item.parentCollection(),
                                                  Akonadi::CollectionFetchJob::Base,
                                                  AddresseeLineEditManager::self()->akonadiSession());
            connect(collectionJob, &Akonadi::CollectionFetchJob::collectionsReceived,
                    this, &AddresseeLineEditPrivate::slotAkonadiCollectionsReceived);
            /* we don't want to start multiple fetch jobs for the same collection,
            so insert the collection with an index value of -2 */
            AddresseeLineEditManager::collectionInfo info;
            info.index = -2;
            AddresseeLineEditManager::self()->akonadiCollectionToCompletionSourceMap.insert(colId, info);
            AddresseeLineEditManager::self()->akonadiPendingItems.append(item);
        } else if (sourceIndex.index == -2) {
            /* fetch job already started, don't need to start another one,
            so just append the item as pending */
            AddresseeLineEditManager::self()->akonadiPendingItems.append(item);
        } else {
            if (sourceIndex.enabled) {
                q->addItem(item, 1, sourceIndex.index);
            }
        }
    }

    if (!items.isEmpty()) {
        const QListWidgetItem *current = q->completionBox()->currentItem();
        if (!current || m_searchString.trimmed() != current->text().trimmed()) {
            doCompletion(m_lastSearchMode);
        }
    }
}

void AddresseeLineEditPrivate::slotAkonadiSearchResult(KJob *job)
{
    if (job->error()) {
        qCWarning(LIBKDEPIMAKONADI_LOG) << "Akonadi search job failed: " << job->errorString();
    }
    const int index = AddresseeLineEditManager::self()->akonadiJobsInFlight.indexOf(qobject_cast<Akonadi::Job *>(job));
    if (index != -1) {
        AddresseeLineEditManager::self()->akonadiJobsInFlight.remove(index);
    }
}

void AddresseeLineEditPrivate::slotAkonadiCollectionsReceived(
    const Akonadi::Collection::List &collections)
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimcompletionorder"));
    KConfigGroup groupCompletionWeights(config, "CompletionWeights");
    KConfigGroup groupCompletionEnabled(config, "CompletionEnabled");
    for (const Akonadi::Collection &collection : collections) {
        if (collection.isValid()) {
            const QString sourceString = collection.displayName();
            const Akonadi::Collection::Id colId = collection.id();
            const int weight = groupCompletionWeights.readEntry(QString::number(colId), 1);
            const int index = q->addCompletionSource(sourceString, weight);
            AddresseeLineEditManager::collectionInfo info(index, groupCompletionEnabled.readEntry(QString::number(colId), true));
            qCDebug(LIBKDEPIMAKONADI_LOG) << "\treceived: " << sourceString << "index: " << index << " enabled: " << info.enabled;
            AddresseeLineEditManager::self()->akonadiCollectionToCompletionSourceMap.insert(colId, info);
        }
    }

    // now that we have added the new collections, recheck our list of pending contacts
    akonadiHandlePending();
    // do completion
    const QListWidgetItem *current = q->completionBox()->currentItem();
    if (!current || m_searchString.trimmed() != current->text().trimmed()) {
        doCompletion(m_lastSearchMode);
    }
}

void AddresseeLineEditPrivate::slotShowOUChanged(bool checked)
{
    if (checked != m_showOU) {
        KConfigGroup group(KSharedConfig::openConfig(), "AddressLineEdit");
        group.writeEntry("ShowOU", checked);
        m_showOU = checked;
    }
}

void AddresseeLineEditPrivate::updateBalooBlackList()
{
    loadBalooBlackList();
    q->removeCompletionSource(i18nc("@title:group", "Contacts found in your data"));
    AddresseeLineEditManager::self()->setBalooCompletionSource(q->addCompletionSource(i18nc("@title:group", "Contacts found in your data"), -1));
}

void AddresseeLineEditPrivate::updateCompletionOrder()
{
    AddresseeLineEditManager::self()->updateCompletionOrder();
}

bool AddresseeLineEditPrivate::canDeleteLineEdit() const
{
    return mCanDeleteLineEdit;
}

void AddresseeLineEditPrivate::setCanDeleteLineEdit(bool inprogressToConfigureCompletion)
{
    mCanDeleteLineEdit = inprogressToConfigureCompletion;
}

KConfig *AddresseeLineEditPrivate::recentAddressConfig() const
{
    return m_recentAddressConfig;
}

bool AddresseeLineEditPrivate::showRecentAddresses() const
{
    return mShowRecentAddresses;
}

void AddresseeLineEditPrivate::setRecentAddressConfig(KConfig *config)
{
    m_recentAddressConfig = config;
}

KContacts::ContactGroup::List AddresseeLineEditPrivate::groups() const
{
    return mGroups;
}

void AddresseeLineEditPrivate::setGroups(const KContacts::ContactGroup::List &groups)
{
    mGroups = groups;
}

QList<KJob *> AddresseeLineEditPrivate::mightBeGroupJobs() const
{
    return mMightBeGroupJobs;
}

void AddresseeLineEditPrivate::setMightBeGroupJobs(const QList<KJob *> &mightBeGroupJobs)
{
    mMightBeGroupJobs = mightBeGroupJobs;
}

bool AddresseeLineEditPrivate::autoGroupExpand() const
{
    return mAutoGroupExpand;
}

void AddresseeLineEditPrivate::setAutoGroupExpand(bool autoGroupExpand)
{
    mAutoGroupExpand = autoGroupExpand;
}

QStringList AddresseeLineEditPrivate::balooBlackList() const
{
    return m_balooBlackList;
}

void AddresseeLineEditPrivate::setExpandIntern(bool b)
{
    mExpandIntern = b;
}

bool AddresseeLineEditPrivate::expandIntern() const
{
    return mExpandIntern;
}

bool AddresseeLineEditPrivate::useSemicolonAsSeparator() const
{
    return m_useSemicolonAsSeparator;
}

void AddresseeLineEditPrivate::setUseSemicolonAsSeparator(bool useSemicolonAsSeparator)
{
    m_useSemicolonAsSeparator = useSemicolonAsSeparator;
}

bool AddresseeLineEditPrivate::enableBalooSearch() const
{
    return m_enableBalooSearch;
}

void AddresseeLineEditPrivate::setEnableBalooSearch(bool enableBalooSearch)
{
    m_enableBalooSearch = enableBalooSearch;
}

bool AddresseeLineEditPrivate::enableAkonadiSearch() const
{
    return m_enableAkonadiSearch;
}

void AddresseeLineEditPrivate::setEnableAkonadiSearch(bool enableAkonadiSearch)
{
    m_enableAkonadiSearch = enableAkonadiSearch;
}

QString AddresseeLineEditPrivate::searchString() const
{
    return m_searchString;
}

void AddresseeLineEditPrivate::setSearchString(const QString &searchString)
{
    m_searchString = searchString;
}

bool AddresseeLineEditPrivate::searchExtended() const
{
    return m_searchExtended;
}

void AddresseeLineEditPrivate::setSearchExtended(bool searchExtended)
{
    m_searchExtended = searchExtended;
}

bool AddresseeLineEditPrivate::smartPaste() const
{
    return m_smartPaste;
}

void AddresseeLineEditPrivate::setSmartPaste(bool smartPaste)
{
    m_smartPaste = smartPaste;
}

bool AddresseeLineEditPrivate::completionInitialized() const
{
    return m_completionInitialized;
}

void AddresseeLineEditPrivate::setCompletionInitialized(bool completionInitialized)
{
    m_completionInitialized = completionInitialized;
}

bool AddresseeLineEditPrivate::useCompletion() const
{
    return m_useCompletion;
}

void AddresseeLineEditPrivate::setUseCompletion(bool useCompletion)
{
    m_useCompletion = useCompletion;
}

bool AddresseeLineEditPrivate::showOU() const
{
    return m_showOU;
}

void AddresseeLineEditPrivate::setShowOU(bool showOU)
{
    m_showOU = showOU;
}

void AddresseeLineEditPrivate::loadBalooBlackList()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    m_balooBlackList = group.readEntry("BalooBackList", QStringList());
    m_domainExcludeList = group.readEntry("ExcludeDomain", QStringList());
}

void AddresseeLineEditPrivate::removeCompletionSource(const QString &source)
{
    AddresseeLineEditManager::self()->removeCompletionSource(source);
}

int AddresseeLineEditPrivate::addCompletionSource(const QString &source, int weight)
{
    return AddresseeLineEditManager::self()->addCompletionSource(source, weight);
}

void AddresseeLineEditPrivate::mightBeGroupJobsClear()
{
    mMightBeGroupJobs.clear();
}

bool AddresseeLineEditPrivate::groupsIsEmpty() const
{
    return mGroups.isEmpty();
}

void AddresseeLineEditPrivate::setShowRecentAddresses(bool b)
{
    mShowRecentAddresses = b;
}

void AddresseeLineEditPrivate::groupsClear()
{
    mGroups.clear();
}

void AddresseeLineEditPrivate::addGroups(const KContacts::ContactGroup::List &lst)
{
    mGroups << lst;
}

void AddresseeLineEditPrivate::mightBeGroupJobsRemoveOne(Akonadi::ContactGroupSearchJob *search)
{
    mMightBeGroupJobs.removeOne(search);
}

void AddresseeLineEditPrivate::mightBeGroupJobsAdd(Akonadi::ContactGroupSearchJob *job)
{
    mMightBeGroupJobs.append(job);
}
