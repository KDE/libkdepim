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

#ifndef ADDRESSEELINEEDITLDAP_H
#define ADDRESSEELINEEDITLDAP_H

#include <QTimer>
#include <QMap>

namespace KLDAP
{
class LdapClientSearch;
}

namespace KPIM {
class AddresseeLineEditManager;
class AddresseeLineEdit;
class AddresseeLineEditLdap : public QObject
{
public:
    explicit AddresseeLineEditLdap(AddresseeLineEditManager *addressLineStatic, QObject *parent = nullptr);
    ~AddresseeLineEditLdap();

    void init();

    void updateLDAPWeights();

    QMap<int, int> ldapClientToCompletionSourceMap() const;
    bool isLdapClientToCompletionSourceMapContains(int value) const;
    int ldapClientToCompletionSourceValue(int value) const;

    KLDAP::LdapClientSearch *ldapSearch() const;

    QTimer *ldapTimer() const;
    QString ldapText() const;
    void setLdapText(const QString &ldapText);

    AddresseeLineEdit *addressLineEdit() const;
    void setAddressLineEdit(AddresseeLineEdit *addressLineEdit);

private:
    // maps LDAP client indices to completion source indices
    // the assumption that they are always the first n indices in s_static->completion
    // does not hold when clients are added later on
    QMap<int, int> mLdapClientToCompletionSourceMap;
    QString mLdapText;
    QTimer *mLdapTimer;
    AddresseeLineEdit *mAddressLineEdit;
    KLDAP::LdapClientSearch *mLdapSearch;

    AddresseeLineEditManager *mAddressLineStatic;
};
}

#endif // ADDRESSEELINEEDITLDAP_H
