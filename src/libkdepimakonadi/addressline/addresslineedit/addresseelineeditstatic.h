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

#ifndef ADDRESSEELINEEDITSTATIC_H
#define ADDRESSEELINEEDITSTATIC_H

#include <QMap>
#include <AkonadiCore/Collection>

#include <AkonadiCore/Item>
#include <AkonadiCore/Session>
#include <QVector>
#include <QWeakPointer>
#include <ldap/ldapclientsearch.h>
#include <qstringlist.h>
class QTimer;
namespace KPIM {
class KMailCompletion;
class AddresseeLineEdit;
class AddresseeLineEditAkonadi;
class AddresseeLineEditLdap;
class AddresseeLineEditBaloo;
class AddresseeLineEditStatic
{
public:
    typedef QMap< QString, QPair<int, int> > CompletionItemsMap;

    AddresseeLineEditStatic();

    ~AddresseeLineEditStatic();


    void updateCollectionWeights();

    void updateLDAPWeights();
    void updateCompletionOrder();

    int addCompletionSource(const QString &source, int weight);
    void removeCompletionSource(const QString &source);




    KMailCompletion *completion;
    CompletionItemsMap completionItemMap;
    QStringList completionSources;

    AddresseeLineEdit *addressLineEdit;
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
            : index(idx),
              enabled(_enabled)
        {

        }

        int index;
        bool enabled;
    };

    QMap<Akonadi::Collection::Id, collectionInfo> akonadiCollectionToCompletionSourceMap;
    // a list of akonadi items (contacts) that have not had their collection fetched yet
    Akonadi::Item::List akonadiPendingItems;
    QVector<QWeakPointer<Akonadi::Job> > akonadiJobsInFlight;

    //Ldap
    void initializeLdap();
    QString ldapText() const;
    void setLdapText(const QString &ldapText);
    KLDAP::LdapClientSearch *ldapSearch() const;
    QTimer *ldapTimer() const;
    bool isLdapClientToCompletionSourceMapContains(int value) const;
    int ldapClientToCompletionSourceValue(int value) const;

    //Baloo
    int balooCompletionSource() const;
    void setBalooCompletionSource(int value);

    //Akonadi
    Akonadi::Session *akonadiSession();


private:
    AddresseeLineEditAkonadi *mAddresseeLineEditAkonadi;
    AddresseeLineEditLdap *mAddressessLineEditLdap;
    AddresseeLineEditBaloo *mAddressessLineEditBaloo;
};
}
#endif // ADDRESSEELINEEDITSTATIC_H
