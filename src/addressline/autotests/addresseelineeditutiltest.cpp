/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

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

#include "addresseelineeditutiltest.h"
#include "../addresslineedit/addresseelineeditutil.h"
#include <QTest>

AddresseeLineEditUtilTest::AddresseeLineEditUtilTest(QObject *parent)
    : QObject(parent)
{

}

AddresseeLineEditUtilTest::~AddresseeLineEditUtilTest()
{

}

void AddresseeLineEditUtilTest::shouldAdaptPasteMails_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("onemail") << QStringLiteral("foo@kde.org") << QStringLiteral("foo@kde.org");
    QTest::newRow("onemailwithat") << QStringLiteral("foo (at) kde.org") << QStringLiteral("foo@kde.org");
    QTest::newRow("text") << QStringLiteral("fookde.org") << QStringLiteral("fookde.org");
}

void AddresseeLineEditUtilTest::shouldAdaptPasteMails()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(KPIM::AddresseeLineEditUtil::adaptPasteMails(input), output);
}

QTEST_MAIN(AddresseeLineEditUtilTest)
