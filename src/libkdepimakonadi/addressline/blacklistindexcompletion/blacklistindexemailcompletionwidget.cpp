/*
  Copyright (c) 2015-2018 Montel Laurent <montel@kde.org>

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

#include "blacklistindexemailcompletionwidget.h"
#include "blacklistindexemaillist.h"
#include "blacklistindexemailsearchjob.h"
#include "blacklistindexemailutil.h"
#include "blacklistindexemailwarning.h"

#include <KLocalizedString>
#include <KLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KListWidgetSearchLine>
#include "libkdepimakonadi_debug.h"

using namespace KPIM;
BlackListIndexEmailCompletionWidget::BlackListIndexEmailCompletionWidget(QWidget *parent)
    : QWidget(parent)
    , mLimit(500)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    mainLayout->addLayout(searchLayout);

    QLabel *lab = new QLabel(i18n("Search email:"), this);
    lab->setObjectName(QStringLiteral("search_label"));
    searchLayout->addWidget(lab);

    mSearchLineEdit = new KLineEdit(this);
    mSearchLineEdit->setPlaceholderText(i18n("Research is done from 3 characters"));
    mSearchLineEdit->setFocus();
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setTrapReturnKey(true);
    mSearchLineEdit->setObjectName(QStringLiteral("search_lineedit"));
    connect(mSearchLineEdit, &KLineEdit::returnPressed, this, &BlackListIndexEmailCompletionWidget::slotCheckIfUpdateBlackListIsNeeded);
    searchLayout->addWidget(mSearchLineEdit);

    mSearchButton = new QPushButton(QIcon::fromTheme(QStringLiteral("edit-find")), i18n("Search"), this);
    mSearchButton->setObjectName(QStringLiteral("search_button"));
    connect(mSearchButton, &QAbstractButton::clicked, this, &BlackListIndexEmailCompletionWidget::slotCheckIfUpdateBlackListIsNeeded);
    mSearchButton->setEnabled(false);
    searchLayout->addWidget(mSearchButton);

    mShowAllBlackListedEmails = new QPushButton(i18n("Show Blacklisted Emails"), this);
    mShowAllBlackListedEmails->setObjectName(QStringLiteral("show_blacklisted_email_button"));
    connect(mShowAllBlackListedEmails, &QAbstractButton::clicked, this, &BlackListIndexEmailCompletionWidget::slotShowAllBlacklistedEmail);
    searchLayout->addWidget(mShowAllBlackListedEmails);

    mEmailList = new BlackListIndexEmailList(this);
    mEmailList->setObjectName(QStringLiteral("email_list"));
    mainLayout->addWidget(mEmailList);

    mBlackListWarning = new BlackListIndexEmailWarning(this);
    mBlackListWarning->setObjectName(QStringLiteral("backlistwarning"));
    connect(mBlackListWarning, &BlackListIndexEmailWarning::newSearch, this, &BlackListIndexEmailCompletionWidget::slotSearch);
    connect(mBlackListWarning, &BlackListIndexEmailWarning::saveChanges, this, &BlackListIndexEmailCompletionWidget::slotSaveChanges);
    mainLayout->addWidget(mBlackListWarning);


    QHBoxLayout *searchLineLayout = new QHBoxLayout;
    mainLayout->addLayout(searchLineLayout);

    QHBoxLayout *selectElementLayout = new QHBoxLayout;
    searchLineLayout->addLayout(selectElementLayout);

    mSelectButton = new QPushButton(i18n("&Select"), this);
    mSelectButton->setObjectName(QStringLiteral("select_email"));
    connect(mSelectButton, &QAbstractButton::clicked, this, &BlackListIndexEmailCompletionWidget::slotSelectEmails);
    selectElementLayout->addWidget(mSelectButton);

    mUnselectButton = new QPushButton(i18n("&Unselect"), this);
    mUnselectButton->setObjectName(QStringLiteral("unselect_email"));
    connect(mUnselectButton, &QAbstractButton::clicked, this, &BlackListIndexEmailCompletionWidget::slotUnselectEmails);
    selectElementLayout->addWidget(mUnselectButton);

    mMoreResult = new QLabel(i18n("<qt><a href=\"more_result\">More result...</a></qt>"), this);
    mMoreResult->setObjectName(QStringLiteral("moreresultlabel"));
    selectElementLayout->addWidget(mMoreResult);

    mMoreResult->setContextMenuPolicy(Qt::NoContextMenu);
    connect(mMoreResult, &QLabel::linkActivated, this, &BlackListIndexEmailCompletionWidget::slotLinkClicked);
    mMoreResult->setVisible(false);
    selectElementLayout->addStretch(1);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &BlackListIndexEmailCompletionWidget::slotSearchLineEditChanged);

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
    excludeDomainLayout->setMargin(0);
    mainLayout->addLayout(excludeDomainLayout);

    QLabel *excludeDomainLabel = new QLabel(i18n("Exclude domain names:"), this);
    excludeDomainLabel->setObjectName(QStringLiteral("domain_label"));
    excludeDomainLayout->addWidget(excludeDomainLabel);

    mExcludeDomainLineEdit = new KLineEdit(this);
    excludeDomainLayout->addWidget(mExcludeDomainLineEdit);
    mExcludeDomainLineEdit->setObjectName(QStringLiteral("domain_lineedit"));
    mExcludeDomainLineEdit->setClearButtonEnabled(true);
    mExcludeDomainLineEdit->setTrapReturnKey(true);
    mExcludeDomainLineEdit->setPlaceholderText(i18n("Separate domain with \'%1\'", QLatin1Char(',')));


    connect(mEmailList, &QListWidget::itemSelectionChanged, this, &BlackListIndexEmailCompletionWidget::slotSelectionChanged);
    slotSelectionChanged();
}

BlackListIndexEmailCompletionWidget::~BlackListIndexEmailCompletionWidget()
{
}

void BlackListIndexEmailCompletionWidget::slotSelectionChanged()
{
    bool selectionIsNotEmpty = !mEmailList->selectedItems().isEmpty();
    mSelectButton->setEnabled(selectionIsNotEmpty);
    mUnselectButton->setEnabled(selectionIsNotEmpty);
}

void BlackListIndexEmailCompletionWidget::load()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    const QStringList lst = group.readEntry("ExcludeDomain", QStringList());
    mEmailList->setExcludeDomain(lst);
    mExcludeDomainLineEdit->setText(lst.join(QLatin1Char(',')));
    mOriginalExcludeDomain = lst;
    slotSelectionChanged();
}

void BlackListIndexEmailCompletionWidget::slotUnselectEmails()
{
    const QList<QListWidgetItem *> lstSelected = mEmailList->selectedItems();
    for (QListWidgetItem *item : lstSelected) {
        item->setCheckState(Qt::Unchecked);
    }
}

void BlackListIndexEmailCompletionWidget::slotSelectEmails()
{
    const QList<QListWidgetItem *> lstSelected = mEmailList->selectedItems();
    for (QListWidgetItem *item : lstSelected) {
        item->setCheckState(Qt::Checked);
    }
}

void BlackListIndexEmailCompletionWidget::slotSearchLineEditChanged(const QString &text)
{
    mSearchButton->setEnabled(text.trimmed().count() > 2);
    hideMoreResultAndChangeLimit();
}

void BlackListIndexEmailCompletionWidget::hideMoreResultAndChangeLimit()
{
    mMoreResult->setVisible(false);
    mLimit = 500;
}

void BlackListIndexEmailCompletionWidget::slotSearch()
{
    const QString searchEmail = mSearchLineEdit->text().trimmed();
    if (searchEmail.length() > 2) {
        mSearchInResultLineEdit->clear();
        KPIM::BlackListIndexEmailSearchJob *job = new KPIM::BlackListIndexEmailSearchJob(this);
        job->setSearchEmail(searchEmail);
        job->setLimit(mLimit);
        connect(job, &BlackListIndexEmailSearchJob::emailsFound, this, &BlackListIndexEmailCompletionWidget::slotEmailFound);
        job->start();
    }
}

void BlackListIndexEmailCompletionWidget::slotEmailFound(const QStringList &list)
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

void BlackListIndexEmailCompletionWidget::setEmailBlackList(const QStringList &list)
{
    mEmailList->setEmailBlackList(list);
}

void BlackListIndexEmailCompletionWidget::slotSaveChanges()
{
    //TODO avoid to save a lot.
    const QHash<QString, bool> result = mEmailList->blackListItemChanged();
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    QStringList blackList = group.readEntry("IndexBackList", QStringList());
    KPIM::BlackListIndexEmailUtil util;
    util.initialBlackList(blackList);
    util.newBlackList(result);
    blackList = util.createNewBlackList();
    group.writeEntry("IndexBackList", blackList);
    group.sync();
    mEmailList->setEmailBlackList(blackList);
    slotSearch();
}

void BlackListIndexEmailCompletionWidget::slotCheckIfUpdateBlackListIsNeeded()
{
    const QHash<QString, bool> result = mEmailList->blackListItemChanged();
    if (result.isEmpty()) {
        slotSearch();
    } else {
        mBlackListWarning->animatedShow();
    }
}

void BlackListIndexEmailCompletionWidget::save()
{
    const QString domain = mExcludeDomainLineEdit->text().remove(QLatin1Char(' '));
    const QStringList newExcludeDomain = domain.split(QLatin1Char(','), QString::SkipEmptyParts);
    bool needToSave = (mOriginalExcludeDomain != newExcludeDomain);
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    const QHash<QString, bool> result = mEmailList->blackListItemChanged();
    if (!result.isEmpty()) {
        needToSave = true;
        QStringList blackList = group.readEntry("IndexBackList", QStringList());
        KPIM::BlackListIndexEmailUtil util;
        util.initialBlackList(blackList);
        util.newBlackList(result);
        blackList = util.createNewBlackList();
        group.writeEntry("IndexBackList", blackList);
    }
    if (needToSave) {
        group.writeEntry("ExcludeDomain", newExcludeDomain);
        group.sync();
    }
    config->reparseConfiguration();
}

void BlackListIndexEmailCompletionWidget::slotLinkClicked(const QString &link)
{
    if (link == QLatin1String("more_result")) {
        mLimit += 200;
        slotSearch();
    }
}

void BlackListIndexEmailCompletionWidget::slotShowAllBlacklistedEmail()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kpimbalooblacklist"));
    KConfigGroup group(config, "AddressLineEdit");
    const QStringList balooBlackList = group.readEntry("IndexBackList", QStringList());
    slotEmailFound(balooBlackList);
}
