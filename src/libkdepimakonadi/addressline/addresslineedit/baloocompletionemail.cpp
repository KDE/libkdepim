/*
  Copyright (c) 2015-2019 Montel Laurent <montel@kde.org>

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

#include "baloocompletionemail.h"
#include <QMap>
#include <KEmailAddress>
#include "libkdepimakonadi_debug.h"

using namespace KPIM;

BalooCompletionEmail::BalooCompletionEmail()
{
}

void BalooCompletionEmail::setEmailList(const QStringList &lst)
{
    mListEmail = lst;
}

void BalooCompletionEmail::setExcludeDomain(const QStringList &lst)
{
    mExcludeDomain = lst;
}

void BalooCompletionEmail::setBlackList(const QStringList &lst)
{
    mBlackList = lst;
}

QStringList BalooCompletionEmail::cleanupEmailList()
{
    if (mListEmail.isEmpty()) {
        return mListEmail;
    }
    QMap<QString, QString> hashEmail;
    for (QString email :  qAsConst(mListEmail)) {
        if (!mBlackList.contains(email)) {
            QString address;
            email = stripEmail(email, address);
            if (address.isEmpty()) {
                address = email;
            }
            bool excludeMail = false;
            for (const QString &excludeDomain : qAsConst(mExcludeDomain)) {
                if (!excludeDomain.isEmpty()) {
                    if (address.endsWith(excludeDomain)) {
                        excludeMail = true;
                        continue;
                    }
                }
            }
            const QString addressLower = address.toLower();
            if (!excludeMail && !hashEmail.contains(addressLower)) {
                hashEmail.insert(addressLower, email);
            }
        }
    }
    return hashEmail.values();
}

/* stips the name of an email address email
 *
 * 'a' <a@example.com> -> a <a@example.com>
 * "a" <a@example.com> -> a <a@example.com>
 * "\"'a'\"" <a@example.com> -> a <a@example.com>
 *
 * but "\"'a" <a@example.com> -> "\"'a" <a@example.com>
 * cause the start and end is not the same.
 */
QString BalooCompletionEmail::stripEmail(const QString &email, QString &address)
{
    QString displayName, addrSpec, comment;
    if (KEmailAddress::AddressOk == KEmailAddress::splitAddress(email, displayName, addrSpec, comment)) {
        address = addrSpec;
        while (1) {
            if ((displayName.startsWith(QLatin1String("\\\"")) && displayName.endsWith(QLatin1String("\\\"")))) {
                displayName = displayName.mid(2, displayName.length() - 4).trimmed();
            } else if ((displayName.startsWith(QLatin1Char('\'')) && displayName.endsWith(QLatin1Char('\'')))
                       || (displayName.startsWith(QLatin1Char('"')) && displayName.endsWith(QLatin1Char('"')))) {
                displayName = displayName.mid(1, displayName.length() - 2).trimmed();
            } else {
                break;
            }
        }
        return KEmailAddress::normalizedAddress(displayName, addrSpec, comment);
    } else {
        return email;
    }
}
