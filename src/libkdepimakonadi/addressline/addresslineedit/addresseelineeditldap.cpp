/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "addresseelineeditldap.h"
#include "addresseelineeditstatic.h"
#include <Libkdepim/LdapClientSearch>
#include <Libkdepim/LdapClient>
#include <KLDAP/LdapServer>
#include <KLocalizedString>

using namespace KPIM;

AddresseeLineEditLdap::AddresseeLineEditLdap(AddresseeLineEditStatic *addressLineStatic, QObject *parent)
    : QObject(parent)
    , mLdapTimer(nullptr)
    , mLdapSearch(nullptr)
    , mAddressLineStatic(addressLineStatic)
{

}

AddresseeLineEditLdap::~AddresseeLineEditLdap()
{

}

void AddresseeLineEditLdap::updateLDAPWeights()
{
    /* Add completion sources for all ldap server, 0 to n. Added first so
       * that they map to the LdapClient::clientNumber() */
    mLdapSearch->updateCompletionWeights();
    int clientIndex = 0;
    for (const KLDAP::LdapClient *client : mLdapSearch->clients()) {
        const int sourceIndex
            = mAddressLineStatic->addCompletionSource(i18n("LDAP server: %1", client->server().host()),
                                  client->completionWeight());
        mLdapClientToCompletionSourceMap.insert(clientIndex, sourceIndex);
        ++clientIndex;
    }
}

QMap<int, int> AddresseeLineEditLdap::ldapClientToCompletionSourceMap() const
{
    return mLdapClientToCompletionSourceMap;
}

KLDAP::LdapClientSearch *AddresseeLineEditLdap::ldapSearch() const
{
    return mLdapSearch;
}

void AddresseeLineEditLdap::init()
{
    if (!mLdapTimer) {
        mLdapTimer = new QTimer(this);
        mLdapSearch = new KLDAP::LdapClientSearch(this);

        /* The reasoning behind this filter is:
         * If it's a person, or a distlist, show it, even if it doesn't have an email address.
         * If it's not a person, or a distlist, only show it if it has an email attribute.
         * This allows both resource accounts with an email address which are not a person and
         * person entries without an email address to show up, while still not showing things
         * like structural entries in the ldap tree.
         */

#if 0
        mLdapSearch->setFilter(QStringLiteral("&(|(objectclass=person)(objectclass=groupOfNames)(mail=*))"
                                                       "(|(cn=%1*)(mail=%1*)(mail=*@%1*)(givenName=%1*)(sn=%1*))"));
#endif
        //Fix bug 323272 "Exchange doesn't like any queries beginning with *."
        mLdapSearch->setFilter(QStringLiteral("&(|(objectclass=person)(objectclass=groupOfNames)(mail=*))"
                                                       "(|(cn=%1*)(mail=%1*)(givenName=%1*)(sn=%1*))"));
    }
}

QTimer *AddresseeLineEditLdap::ldapTimer() const
{
    return mLdapTimer;
}
