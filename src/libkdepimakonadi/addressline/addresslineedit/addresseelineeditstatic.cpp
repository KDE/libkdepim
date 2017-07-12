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

#include "addresseelineeditstatic.h"
#include "addresseelineeditakonadi.h"
#include "addresseelineeditbaloo.h"
#include "addresseelineeditldap.h"
#include "kmailcompletion.h"

#include <Libkdepim/LdapClient>
#include <KLocalizedString>
#include <QTimer>
#include <KLDAP/LdapServer>
#include <AkonadiCore/Session>
using namespace KPIM;

AddresseeLineEditStatic::AddresseeLineEditStatic()
    : completion(new KMailCompletion)
    , addressLineEdit(nullptr)
    , mAddresseeLineEditAkonadi(new AddresseeLineEditAkonadi(this))
    , mAddressessLineEditLdap(new AddresseeLineEditLdap(this))
    , mAddressessLineEditBaloo(new AddresseeLineEditBaloo(this))
{
}

AddresseeLineEditStatic::~AddresseeLineEditStatic()
{
    delete completion;

    delete mAddresseeLineEditAkonadi;
    delete mAddressessLineEditLdap;
    delete mAddressessLineEditBaloo;
}

void AddresseeLineEditStatic::updateCompletionOrder()
{
    updateLDAPWeights();
    updateCollectionWeights();
}

void AddresseeLineEditStatic::updateCollectionWeights()
{
    akonadiCollectionToCompletionSourceMap.clear();
}

void AddresseeLineEditStatic::updateLDAPWeights()
{
    mAddressessLineEditLdap->updateLDAPWeights();
}

int AddresseeLineEditStatic::addCompletionSource(const QString &source, int weight)
{
    QMap<QString, int>::iterator it = completionSourceWeights.find(source);
    if (it == completionSourceWeights.end()) {
        completionSourceWeights.insert(source, weight);
    } else {
        completionSourceWeights[source] = weight;
    }

    const int sourceIndex = completionSources.indexOf(source);
    if (sourceIndex == -1) {
        completionSources.append(source);
        return completionSources.size() - 1;
    } else {
        return sourceIndex;
    }
}

void AddresseeLineEditStatic::removeCompletionSource(const QString &source)
{
    QMap<QString, int>::iterator it = completionSourceWeights.find(source);
    if (it != completionSourceWeights.end()) {
        completionSourceWeights.remove(source);
        completion->clear();
    }
}

Akonadi::Session *AddresseeLineEditStatic::akonadiSession()
{
    return mAddresseeLineEditAkonadi->akonadiSession();
}

KLDAP::LdapClientSearch *AddresseeLineEditStatic::ldapSearch() const
{
    return mAddressessLineEditLdap->ldapSearch();
}

QTimer *AddresseeLineEditStatic::ldapTimer() const
{
    return mAddressessLineEditLdap->ldapTimer();
}

int AddresseeLineEditStatic::balooCompletionSource() const
{
    return mAddressessLineEditBaloo->balooCompletionSource();
}

void AddresseeLineEditStatic::setBalooCompletionSource(int value)
{
    mAddressessLineEditBaloo->setBalooCompletionSource(value);
}

void AddresseeLineEditStatic::initializeLdap()
{
    mAddressessLineEditLdap->init();
}

QString AddresseeLineEditStatic::ldapText() const
{
    return mAddressessLineEditLdap->ldapText();
}

void AddresseeLineEditStatic::setLdapText(const QString &ldapText)
{
    mAddressessLineEditLdap->setLdapText(ldapText);
}

