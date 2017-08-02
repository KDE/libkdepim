/*
  Copyright (c) 2015-2016 Montel Laurent <montel@kde.org>

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

#ifndef ADDRESSEELINEEDIT_P_H
#define ADDRESSEELINEEDIT_P_H
#include <QStringList>
#include <QObject>
#include <QTimer>
#include <QList>
#include <QColor>
#include <ContactGroupSearchJob>
#include "Libkdepim/LdapClient"
#include "ldap/ldapclientsearch.h"
#include <AkonadiCore/Item>
#include <AkonadiCore/Collection>
#include <kcontacts/contactgroup.h>

class KConfig;
class QToolButton;

namespace KPIM {
class AddresseeLineEdit;
class AddresseeLineEditPrivate : public QObject
{
    Q_OBJECT
public:
    AddresseeLineEditPrivate(AddresseeLineEdit *qq, bool enableCompletion);
    ~AddresseeLineEditPrivate();
    void loadBalooBlackList();
    void alternateColor();
    void init();
    void setCompletedItems(const QStringList &items, bool autoSuggest);
    void addCompletionItem(const QString &string, int weight, int source, const QStringList *keyWords = nullptr);
    const QStringList adjustedCompletionItems(bool fullSearch);
    void updateSearchString();
    void startSearches();
    void akonadiPerformSearch();
    void akonadiHandlePending();
    void doCompletion(bool ctrlT);

    bool showOU() const;
    void setShowOU(bool showOU);

    void setIcon(const QIcon &icon, const QString &tooltip);

    bool useCompletion() const;
    void setUseCompletion(bool useCompletion);

    bool completionInitialized() const;
    void setCompletionInitialized(bool completionInitialized);

    bool smartPaste() const;
    void setSmartPaste(bool smartPaste);

    bool searchExtended() const;
    void setSearchExtended(bool searchExtended);

    QString searchString() const;
    void setSearchString(const QString &searchString);

    bool enableBalooSearch() const;
    void setEnableBalooSearch(bool enableBalooSearch);

    bool useSemicolonAsSeparator() const;
    void setUseSemicolonAsSeparator(bool useSemicolonAsSeparator);

    bool enableAkonadiSearch() const;
    void setEnableAkonadiSearch(bool enableAkonadiSearch);

    void removeCompletionSource(const QString &source);
    int addCompletionSource(const QString &source, int weight);
    void restartTime(const QString &searchString);
    void updateBalooBlackList();
    void updateCompletionOrder();

    KLDAP::LdapClientSearch *ldapSearch();

    void setExpandIntern(bool);
    bool expandIntern() const;

    bool autoGroupExpand() const;
    void setAutoGroupExpand(bool autoGroupExpand);

    QList<KJob *> mightBeGroupJobs() const;
    void setMightBeGroupJobs(const QList<KJob *> &mightBeGroupJobs);

    KContacts::ContactGroup::List groups() const;
    void setGroups(const KContacts::ContactGroup::List &groups);

    void mightBeGroupJobsClear();
    void groupsClear();
    void addGroups(const KContacts::ContactGroup::List &lst);
    void mightBeGroupJobsRemoveOne(Akonadi::ContactGroupSearchJob *search);
    void mightBeGroupJobsAdd(Akonadi::ContactGroupSearchJob *job);
    bool groupsIsEmpty() const;
    void setShowRecentAddresses(bool b);
    bool showRecentAddresses() const;
    void setRecentAddressConfig(KConfig *config);

    KConfig *recentAddressConfig() const;

    bool canDeleteLineEdit() const;
    void setCanDeleteLineEdit(bool inprogressToConfigureCompletion);

public Q_SLOTS:
    void slotToggleExpandGroups();
    void slotShowOUChanged(bool);

private Q_SLOTS:
    void slotCompletion();
    void slotPopupCompletion(const QString &);
    void slotReturnPressed(const QString &);
    void slotStartLDAPLookup();
    void slotLDAPSearchData(const KLDAP::LdapResult::List &);
    void slotEditCompletionOrder();
    void slotUserCancelled(const QString &);
    void slotAkonadiHandleItems(const Akonadi::Item::List &items);
    void slotAkonadiSearchResult(KJob *);
    void slotAkonadiCollectionsReceived(const Akonadi::Collection::List &);
    void searchInBaloo();
    void slotTriggerDelayedQueries();

private:
    AddresseeLineEdit *q;
    QToolButton *m_toolButton;
    QTimer m_delayedQueryTimer;
    QColor m_alternateColor;
    QString m_previousAddresses;
    QString m_searchString;
    QList<KJob *> mMightBeGroupJobs;
    KContacts::ContactGroup::List mGroups;
    KConfig *m_recentAddressConfig;
    bool m_useCompletion;
    bool m_completionInitialized;
    bool m_smartPaste;
    bool m_addressBookConnected;
    bool m_lastSearchMode;
    bool m_searchExtended; //has \" been added?
    bool m_useSemicolonAsSeparator;
    bool mShowOU;
    bool m_enableBalooSearch;
    bool m_enableAkonadiSearch;
    bool mExpandIntern;
    bool mAutoGroupExpand;
    bool mShowRecentAddresses;
    bool mCanDeleteLineEdit;
};
}

#endif // ADDRESSEELINEEDIT_P_H
