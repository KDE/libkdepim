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

#include "addresseelineeditmanager.h"
#include "addresseelineeditakonadi.h"
#include "addresseelineeditbaloo.h"
#include "addresseelineeditldap.h"
#include "kmailcompletion.h"

#include <Libkdepim/LdapClient>
#include <KLocalizedString>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QCoreApplication>
#include <KLDAP/LdapServer>
#include <AkonadiCore/Session>
using namespace KPIM;

Q_GLOBAL_STATIC(AddresseeLineEditManager, sInstance)

AddresseeLineEditManager::AddresseeLineEditManager()
    : mCompletion(new KMailCompletion)
    , mAddresseeLineEditAkonadi(new AddresseeLineEditAkonadi(this))
    , mAddressessLineEditLdap(new AddresseeLineEditLdap(this))
    , mAddressessLineEditBaloo(new AddresseeLineEditBaloo(this))
    , mNetworkConfigMgr(new QNetworkConfigurationManager(QCoreApplication::instance()))
{
}

AddresseeLineEditManager::~AddresseeLineEditManager()
{
    delete mCompletion;

    delete mAddresseeLineEditAkonadi;
    delete mAddressessLineEditLdap;
    delete mAddressessLineEditBaloo;
}

AddresseeLineEditManager *AddresseeLineEditManager::self()
{
    return sInstance;
}

void AddresseeLineEditManager::updateCompletionOrder()
{
    updateLDAPWeights();
    updateCollectionWeights();
}

void AddresseeLineEditManager::updateCollectionWeights()
{
    akonadiCollectionToCompletionSourceMap.clear();
}

void AddresseeLineEditManager::updateLDAPWeights()
{
    mAddressessLineEditLdap->updateLDAPWeights();
}

int AddresseeLineEditManager::addCompletionSource(const QString &source, int weight)
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

void AddresseeLineEditManager::removeCompletionSource(const QString &source)
{
    QMap<QString, int>::iterator it = completionSourceWeights.find(source);
    if (it != completionSourceWeights.end()) {
        completionSourceWeights.remove(source);
        mCompletion->clear();
    }
}

Akonadi::Session *AddresseeLineEditManager::akonadiSession() const
{
    return mAddresseeLineEditAkonadi->akonadiSession();
}

KMailCompletion *AddresseeLineEditManager::completion() const
{
    return mCompletion;
}

AddresseeLineEdit *AddresseeLineEditManager::addressLineEdit() const
{
    return mAddressessLineEditLdap->addressLineEdit();
}

void AddresseeLineEditManager::setAddressLineEdit(AddresseeLineEdit *addressLineEdit)
{
    mAddressessLineEditLdap->setAddressLineEdit(addressLineEdit);
}

bool AddresseeLineEditManager::isOnline() const
{
    return mNetworkConfigMgr->isOnline();
}

KLDAP::LdapClientSearch *AddresseeLineEditManager::ldapSearch() const
{
    return mAddressessLineEditLdap->ldapSearch();
}

QTimer *AddresseeLineEditManager::ldapTimer() const
{
    return mAddressessLineEditLdap->ldapTimer();
}

int AddresseeLineEditManager::ldapClientToCompletionSourceValue(int value) const
{
    return mAddressessLineEditLdap->ldapClientToCompletionSourceValue(value);
}

bool AddresseeLineEditManager::isLdapClientToCompletionSourceMapContains(int value) const
{
    return mAddressessLineEditLdap->isLdapClientToCompletionSourceMapContains(value);
}

int AddresseeLineEditManager::balooCompletionSource() const
{
    return mAddressessLineEditBaloo->balooCompletionSource();
}

void AddresseeLineEditManager::setBalooCompletionSource(int value)
{
    mAddressessLineEditBaloo->setBalooCompletionSource(value);
}

void AddresseeLineEditManager::initializeLdap()
{
    mAddressessLineEditLdap->init();
}

QString AddresseeLineEditManager::ldapText() const
{
    return mAddressessLineEditLdap->ldapText();
}

void AddresseeLineEditManager::setLdapText(const QString &ldapText)
{
    mAddressessLineEditLdap->setLdapText(ldapText);
}

void AddresseeLineEditManager::stopLDAPLookup()
{
    mAddressessLineEditLdap->stopLDAPLookup();
}

void AddresseeLineEditManager::startLoadingLDAPEntries()
{
    mAddressessLineEditLdap->startLoadingLDAPEntries();
}

void AddresseeLineEditManager::restartLdap(const QString &searchString, AddresseeLineEdit *addressLine)
{
    mAddressessLineEditLdap->restartLdap(searchString, addressLine);
}
