/*
  Copyright (c) 2015-2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#include "blacklistbalooemailcompletionwidget.h"
#include "blacklistbalooemaillist.h"
#include "blacklistbalooemailsearchjob.h"
#include "blacklistbalooemailutil.h"
#include "blacklistbalooemailwarning.h"
#include "misc/lineeditcatchreturnkey.h"

#include <KLocalizedString>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KListWidgetSearchLine>
#include "libkdepimakonadi_debug.h"

using namespace KPIM;
BlackListBalooEmailCompletionWidget::BlackListBalooEmailCompletionWidget(QWidget *parent)
    : QWidget(parent)
    , mLimit(500)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    mainLayout->addLayout(searchLayout);

    QLabel *lab = new QLabel(i18n("Search email:"), this);
    lab->setObjectName(QStringLiteral("search_label"));
    searchLayout->addWidget(lab);

    mSearchLineEdit = new QLineEdit(this);
    mSearchLineEdit->setPlaceholderText(i18n("Research is done from 3 characters"));
    mSearchLineEdit->setFocus();
    mSearchLineEdit->setClearButtonEnabled(true);
    new KPIM::LineEditCatchReturnKey(mSearchLineEdit, this);
    mSearchLineEdit->setObjectName(QStringLiteral("search_lineedit"));
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &BlackListBalooEmailCompletionWidget::slotCheckIfUpdateBlackListIsNeeded);
    searchLayout->addWidget(mSearchLineEdit);

    mSearchButton = new QPushButton(QIcon::fromTheme(QStringLiteral("edit-find")), i18n("Search"), this);
    mSearchButton->setObjectName(QStringLiteral("search_button"));
    connect(mSearchButton, &QAbstractButton::clicked, this, &BlackListBalooEmailCompletionWidget::slotCheckIfUpdateBlackListIsNeeded);
    mSearchButton->setEnabled(false);
    searchLayout->addWidget(mSearchButton);

    mShowAllBlackListedEmails = new QPushButton(i18n("Show Blacklisted Emails"), this);
    mShowAllBlackListedEmails->setObjectName(QStringLiteral("show_blacklisted_email_button"));
    connect(mShowAllBlackListedEmails, &QAbstractButton::clicked, this, &BlackListBalooEmailCompletionWidget::slotShowAllBlacklistedEmail);
    searchLayout->addWidget(mShowAllBlackListedEmails);

    mEmailList = new BlackListBalooEmailList(this);
    mEmailList->setObjectName(QStringLiteral("email_list"));
    mainLayout->addWidget(mEmailList);

    mBlackListWarning = new BlackListBalooEmailWarning(this);
    mBlackListWarning->setObjectName(QStringLiteral("backlistwarning"));
    connect(mBlackListWarning, &BlackListBalooEmailWarning::newSearch, this, &BlackListBalooEmailCompletionWidget::slotSearch);
    connect(mBlackListWarning, &BlackListBalooEmailWarning::saveChanges, this, &BlackListBalooEmailCompletionWidget::slotSaveChanges);
    mainLayout->addWidget(mBlackListWarning);

    QHBoxLayout *searchLineLayout = new QHBoxLayout;
    mainLayout->addLayout(searchLineLayout);

    QHBoxLayout *selectElementLayout = new QHBoxLayout;
    searchLineLayout->addLayout(selectElementLayout);

    mSelectButton = new QPushButton(i18n("&Select"), this);
    mSelectButton->setObjectName(QStringLiteral("select_email"));
    connect(mSelectButton, &QAbstractButton::clicked, this, &BlackListBalooEmailCompletionWidget::slotSelectEmails);
    selectElementLayout->addWidget(mSelectButton);

    mUnselectButton = new QPushButton(i18n("&Unselect"), this);
    mUnselectButton->setObjectName(QStringLiteral("unselect_email"));
    connect(mUnselectButton, &QAbstractButton::clicked, this, &BlackListBalooEmailCompletionWidget::slotUnselectEmails);
    selectElementLayout->addWidget(mUnselectButton);

    mMoreResult = new QLabel(i18n("<qt><a href=\"more_result\">More result...</a></qt>"), this);
    mMoreResult->setObjectName(QStringLiteral("moreresultlabel"));
    selectElementLayout->addWidget(mMoreResult);

    mMoreResult->setContextMenuPolicy(Qt::NoContextMenu);
    connect(mMoreResult, &QLabel::linkActivated, this, &BlackListBalooEmailCompletionWidget::slotLinkClicked);
    mMoreResult->setVisible(false);
    selectElementLayout->addStretch(1);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &BlackListBalooEmailCompletionWidget::slotSearchLineEditChanged);

    mSearchInResultLineEdit = new KListWidgetSearchLine(this, mEmailList);
    mSearchInResultLineEdit->setObjectName(QStringLiteral("searchinresultlineedit"));
    mSearchInResultLineEdit->setClearButtonEnabled(true);
    mSearchInResultLineEdit->setPlaceholderText(i18n("Search in result..."));

    searchLineLayout->addStretch(0);
    mNumberOfEmailsFound = new QLabel(this);
    mNumberOfEmailsFound->setObjectName(QStringLiteral("numberofemailsfound"));

    searchLineLayout->addWidget(mNumberOfEmailsFound);
    searchLineLayout->addWidget(mSearchInResultLineEdit);

    QHBoxLayout *excludeDomainLayout = new QHBoxLayout;
    excludeDomainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(excludeDomainLayout);

    QLabel *excludeDomainLabel = new QLabel(i18n("Exclude domain names:"), this);
    excludeDomainLabel->setObjectName(QStringLiteral("domain_label"));
    excludeDomainLayout->addWidget(excludeDomainLabel);

    mExcludeDomainLineEdit = new QLineEdit(this);
    excludeDomainLayout->addWidget(mExcludeDomainLineEdit);
    mExcludeDomainLineEdit->setObjectName(QStringLiteral("domain_lineedit"));
    mExcludeDomainLineEdit->setClearButtonEnabled(true);
    new KPIM::LineEditCatchReturnKey(mExcludeDomainLineEdit, this);
    mExcludeDomainLineEdit->setPlaceholderText(i18n("Separate domain with \'%1\'", QLatin1Char(',')));

    connect(mEmailList, &QListWidget::itemSelectionChanged, this, &BlackListBalooEmailCompletionWidget::slotSelectionChanged);
    slotSelectionChanged();
}

BlackListBalooEmailCompletionWidget::~BlackListBalooEmailCompletionWidget()
{
}

void BlackListBalooEmailCompletionWidget::slotSelectionChanged()
{
    bool selectionIsNotEmpty = !mEmailList->selectedItems().isEmpty();
    mSelectButton->setEnabled(selectionIsNotEmpty);
    mUnselectButton->setEnabled(selectionIsNotEmpty);
}

void BlackListBalooEmailCompletionWidget::load()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    const QStringList lst = group.readEntry("ExcludeDomain", QStringList());
    mEmailList->setExcludeDomain(lst);
    mExcludeDomainLineEdit->setText(lst.join(QLatin1Char(',')));
    mOriginalExcludeDomain = lst;
    slotSelectionChanged();
}

void BlackListBalooEmailCompletionWidget::slotUnselectEmails()
{
    const QList<QListWidgetItem *> lstSelected = mEmailList->selectedItems();
    for (QListWidgetItem *item : lstSelected) {
        item->setCheckState(Qt::Unchecked);
    }
}

void BlackListBalooEmailCompletionWidget::slotSelectEmails()
{
    const QList<QListWidgetItem *> lstSelected = mEmailList->selectedItems();
    for (QListWidgetItem *item : lstSelected) {
        item->setCheckState(Qt::Checked);
    }
}

void BlackListBalooEmailCompletionWidget::slotSearchLineEditChanged(const QString &text)
{
    mSearchButton->setEnabled(text.trimmed().count() > 2);
    hideMoreResultAndChangeLimit();
}

void BlackListBalooEmailCompletionWidget::hideMoreResultAndChangeLimit()
{
    mMoreResult->setVisible(false);
    mLimit = 500;
}

void BlackListBalooEmailCompletionWidget::slotSearch()
{
    const QString searchEmail = mSearchLineEdit->text().trimmed();
    if (searchEmail.length() > 2) {
        mSearchInResultLineEdit->clear();
        KPIM::BlackListBalooEmailSearchJob *job = new KPIM::BlackListBalooEmailSearchJob(this);
        job->setSearchEmail(searchEmail);
        job->setLimit(mLimit);
        connect(job, &BlackListBalooEmailSearchJob::emailsFound, this, &BlackListBalooEmailCompletionWidget::slotEmailFound);
        if (!job->start()) {
            qCWarning(LIBKDEPIMAKONADI_LOG) << "Impossible to start search job";
        }
    }
}

void BlackListBalooEmailCompletionWidget::slotEmailFound(const QStringList &list)
{
    const int numberOfEmails = mEmailList->setEmailFound(list);
    mMoreResult->setVisible(numberOfEmails == mLimit);
    mEmailList->scrollToBottom();
    if (numberOfEmails == 0) {
        mNumberOfEmailsFound->setText(i18n("No email found."));
    } else {
        mNumberOfEmailsFound->setText(i18np("1 email found", "%1 emails found", numberOfEmails));
    }
}

void BlackListBalooEmailCompletionWidget::setEmailBlackList(const QStringList &list)
{
    mEmailList->setEmailBlackList(list);
}

void BlackListBalooEmailCompletionWidget::slotSaveChanges()
{
    //TODO avoid to save a lot.
    const QHash<QString, bool> result = mEmailList->blackListItemChanged();
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    QStringList blackList = group.readEntry("BalooBackList", QStringList());
    KPIM::BlackListBalooEmailUtil util;
    util.initialBlackList(blackList);
    util.newBlackList(result);
    blackList = util.createNewBlackList();
    group.writeEntry("BalooBackList", blackList);
    group.sync();
    mEmailList->setEmailBlackList(blackList);
    slotSearch();
}

void BlackListBalooEmailCompletionWidget::slotCheckIfUpdateBlackListIsNeeded()
{
    const QHash<QString, bool> result = mEmailList->blackListItemChanged();
    if (result.isEmpty()) {
        slotSearch();
    } else {
        mBlackListWarning->animatedShow();
    }
}

void BlackListBalooEmailCompletionWidget::save()
{
    const QString domain = mExcludeDomainLineEdit->text().remove(QLatin1Char(' '));
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    const QStringList newExcludeDomain = domain.split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    const QStringList newExcludeDomain = domain.split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
    bool needToSave = (mOriginalExcludeDomain != newExcludeDomain);
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    const QHash<QString, bool> result = mEmailList->blackListItemChanged();
    if (!result.isEmpty()) {
        needToSave = true;
        QStringList blackList = group.readEntry("BalooBackList", QStringList());
        KPIM::BlackListBalooEmailUtil util;
        util.initialBlackList(blackList);
        util.newBlackList(result);
        blackList = util.createNewBlackList();
        group.writeEntry("BalooBackList", blackList);
    }
    if (needToSave) {
        group.writeEntry("ExcludeDomain", newExcludeDomain);
        group.sync();
    }
    config->reparseConfiguration();
}

void BlackListBalooEmailCompletionWidget::slotLinkClicked(const QString &link)
{
    if (link == QLatin1String("more_result")) {
        mLimit += 200;
        slotSearch();
    }
}

void BlackListBalooEmailCompletionWidget::slotShowAllBlacklistedEmail()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    const QStringList balooBlackList = group.readEntry("BalooBackList", QStringList());
    slotEmailFound(balooBlackList);
}
