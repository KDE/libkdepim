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

#include "addresseelineeditmanager.h"
#include "addresseelineeditakonadi.h"
#include "addresseelineeditindex.h"
#include "addresseelineeditldap.h"
#include "kmailcompletion.h"

#include <Libkdepim/LdapClient>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QCoreApplication>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KColorScheme>
#include <AkonadiCore/Session>

using namespace KPIM;

Q_GLOBAL_STATIC(AddresseeLineEditManager, sInstance)

AddresseeLineEditManager::AddresseeLineEditManager()
    : mShowOU(false)
    , mAutoGroupExpand(false)
    , mCompletion(new KMailCompletion)
    , mAddresseeLineEditAkonadi(new AddresseeLineEditAkonadi(this))
    , mAddressessLineEditLdap(new AddresseeLineEditLdap(this))
    , mAddressessLineEditIndex(new AddresseeLineEditIndex(this))
    , mNetworkConfigMgr(new QNetworkConfigurationManager(QCoreApplication::instance()))
{
    KConfigGroup group(KSharedConfig::openConfig(), "AddressLineEdit");
    mShowOU = group.readEntry("ShowOU", false);
    mAutoGroupExpand = group.readEntry("AutoGroupExpand", false);
}

AddresseeLineEditManager::~AddresseeLineEditManager()
{
    delete mCompletion;

    delete mAddresseeLineEditAkonadi;
    delete mAddressessLineEditLdap;
    delete mAddressessLineEditIndex;
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

int AddresseeLineEditManager::indexCompletionSource() const
{
    return mAddressessLineEditIndex->indexCompletionSource();
}

void AddresseeLineEditManager::setIndexCompletionSource(int value)
{
    mAddressessLineEditIndex->setIndexCompletionSource(value);
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

void AddresseeLineEditManager::loadIndexBlackList()
{
    mAddressessLineEditIndex->loadIndexBlackList();
}

QStringList AddresseeLineEditManager::cleanupEmailList(const QStringList &inputList)
{
    return mAddressessLineEditIndex->cleanupEmailList(inputList);
}

QStringList AddresseeLineEditManager::indexBlackList() const
{
    return mAddressessLineEditIndex->indexBlackList();
}

bool AddresseeLineEditManager::showOU() const
{
    return mShowOU;
}

void AddresseeLineEditManager::setShowOU(bool checked)
{
    if (checked != mShowOU) {
        KConfigGroup group(KSharedConfig::openConfig(), "AddressLineEdit");
        group.writeEntry("ShowOU", checked);
        mShowOU = checked;
    }
}

bool AddresseeLineEditManager::autoGroupExpand() const
{
    return mAutoGroupExpand;
}

void AddresseeLineEditManager::setAutoGroupExpand(bool checked)
{
    if (mAutoGroupExpand != checked) {
        mAutoGroupExpand = checked;
        KConfigGroup group(KSharedConfig::openConfig(), "AddressLineEdit");
        group.writeEntry("AutoGroupExpand", mAutoGroupExpand);
    }
}

QColor AddresseeLineEditManager::alternateColor() const
{
    if (!mAlternateColor.isValid()) {
        const KColorScheme colorScheme(QPalette::Active, KColorScheme::View);
        mAlternateColor = colorScheme.background(KColorScheme::AlternateBackground).color();
    }
    return mAlternateColor;
}
