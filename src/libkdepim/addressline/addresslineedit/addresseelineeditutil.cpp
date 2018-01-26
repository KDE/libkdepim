/*
   Copyright (C) 2016-2018 Laurent Montel <montel@kde.org>

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

#include "addresseelineeditutil.h"
#include <QRegularExpression>
#include <QUrl>

QString KPIM::AddresseeLineEditUtil::adaptPasteMails(QString str)
{
    QString newText = str;
    // remove newlines in the to-be-pasted string
    QStringList lines = newText.split(QRegularExpression(QStringLiteral("\r?\n")), QString::SkipEmptyParts);
    QStringList::iterator end(lines.end());
    for (QStringList::iterator it = lines.begin(); it != end; ++it) {
        // remove trailing commas and whitespace
        (*it).remove(QRegularExpression(QStringLiteral(",?\\s*$")));
    }
    newText = lines.join(QStringLiteral(", "));

    if (newText.startsWith(QLatin1String("mailto:"))) {
        const QUrl url(newText);
        newText = url.path();
    } else if (newText.contains(QLatin1String(" at "))) {
        // Anti-spam stuff
        newText.replace(QStringLiteral(" at "), QStringLiteral("@"));
        newText.replace(QStringLiteral(" dot "), QStringLiteral("."));
    } else if (newText.contains(QLatin1String("(at)"))) {
        newText.replace(QRegularExpression(QStringLiteral("\\s*\\(at\\)\\s*")), QStringLiteral("@"));
    }
    return newText;
}
