/*
  Copyright (c) 2015-2018 Montel Laurent <montel@kde.org>

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

#ifndef ADDRESSEELINEEDITMANAGER_H
#define ADDRESSEELINEEDITMANAGER_H

#include <QMap>
#include <AkonadiCore/Collection>

#include <AkonadiCore/Item>
#include <AkonadiCore/Session>
#include <QVector>
#include <QPointer>
#include <ldap/ldapclientsearch.h>
#include <QColor>

class QTimer;
class QNetworkConfigurationManager;
namespace KPIM {
class KMailCompletion;
class AddresseeLineEdit;
class AddresseeLineEditAkonadi;
class AddresseeLineEditLdap;
class AddresseeLineEditBaloo;
class AddresseeLineEditManager
{
public:
    typedef QMap< QString, QPair<int, int> > CompletionItemsMap;

    AddresseeLineEditManager();

    ~AddresseeLineEditManager();

    static AddresseeLineEditManager *self();

    void updateCollectionWeights();

    void updateLDAPWeights();
    void updateCompletionOrder();

    int addCompletionSource(const QString &source, int weight);
    void removeCompletionSource(const QString &source);

    CompletionItemsMap completionItemMap;
    QStringList completionSources;

    // The weights associated with the completion sources in s_static->completionSources.
    // Both are maintained by addCompletionSource(), don't attempt to modifiy those yourself.
    QMap<QString, int> completionSourceWeights;

    // holds the cached mapping from akonadi collection id to the completion source index
    struct collectionInfo {
        collectionInfo()
            : index(-1)
            , enabled(true)
        {
        }

        collectionInfo(int idx, bool _enabled)
            : index(idx)
            , enabled(_enabled)
        {
        }

        int index;
        bool enabled;
    };

    QMap<Akonadi::Collection::Id, collectionInfo> akonadiCollectionToCompletionSourceMap;
    // a list of akonadi items (contacts) that have not had their collection fetched yet
    Akonadi::Item::List akonadiPendingItems;
    QVector<QPointer<Akonadi::Job> > akonadiJobsInFlight;

    //Ldap
    void initializeLdap();
    QString ldapText() const;
    void setLdapText(const QString &ldapText);
    KLDAP::LdapClientSearch *ldapSearch() const;
    QTimer *ldapTimer() const;
    bool isLdapClientToCompletionSourceMapContains(int value) const;
    int ldapClientToCompletionSourceValue(int value) const;
    void stopLDAPLookup();
    void startLoadingLDAPEntries();
    void restartLdap(const QString &searchString, AddresseeLineEdit *addressLine);

    //Baloo
    int balooCompletionSource() const;
    void setBalooCompletionSource(int value);

    //Akonadi
    Akonadi::Session *akonadiSession() const;

    KMailCompletion *completion() const;

    AddresseeLineEdit *addressLineEdit() const;
    void setAddressLineEdit(AddresseeLineEdit *addressLineEdit);
    bool isOnline() const;

    void loadBalooBlackList();
    QStringList cleanupEmailList(const QStringList &inputList);
    QStringList balooBlackList() const;

    bool showOU() const;
    void setShowOU(bool showOU);

    bool autoGroupExpand() const;
    void setAutoGroupExpand(bool checked);

    QColor alternateColor() const;

private:
    mutable QColor mAlternateColor;
    bool mShowOU = false;
    bool mAutoGroupExpand = false;
    KMailCompletion *mCompletion = nullptr;
    AddresseeLineEditAkonadi *mAddresseeLineEditAkonadi = nullptr;
    AddresseeLineEditLdap *mAddressessLineEditLdap = nullptr;
    AddresseeLineEditBaloo *mAddressessLineEditBaloo = nullptr;
    QNetworkConfigurationManager *mNetworkConfigMgr = nullptr;
};
}
#endif // ADDRESSEELINEEDITMANAGER_H
