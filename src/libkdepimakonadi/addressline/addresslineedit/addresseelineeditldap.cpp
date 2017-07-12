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

AddresseeLineEditLdap::AddresseeLineEditLdap(AddresseeLineEditStatic *addressLineStatic)
    : mLdapTimer(nullptr)
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
