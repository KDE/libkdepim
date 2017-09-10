/*
   Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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
    QTest::newRow("onemailwithdot") << QStringLiteral("foo at kde dot org") << QStringLiteral("foo@kde.org");
    QTest::newRow("mailto") << QStringLiteral("mailto:foo@kde.org") << QStringLiteral("foo@kde.org");
    QTest::newRow("multimail") << QStringLiteral("foo@kde.org,\r\n     bla@kde.org,blo@kde.org") << QStringLiteral("foo@kde.org,      bla@kde.org,blo@kde.org");
    QTest::newRow("multimail-2") << QStringLiteral("foo@kde.org,\r\n     bla@kde.org,blo@kde.org,   ") << QStringLiteral("foo@kde.org,      bla@kde.org,blo@kde.org");
    QTest::newRow("multimail-3") << QStringLiteral("foo@kde.org,\r\n     bla@kde.org,blo@kde.org,\n") << QStringLiteral("foo@kde.org,      bla@kde.org,blo@kde.org");
    QTest::newRow("multimail-4") << QStringLiteral("foo@kde.org,\r\n     bla@kde.org,blo@kde.org,    \n") << QStringLiteral("foo@kde.org,      bla@kde.org,blo@kde.org");
    QTest::newRow("multimail-5") << QStringLiteral("foo (at) kde.org,\r\n     bla (at) kde.org,blo (at) kde.org") << QStringLiteral("foo@kde.org,      bla@kde.org,blo@kde.org");
    QTest::newRow("multimail-6") << QStringLiteral("mailto:foo@kde.org,\r\n     bla@kde.org,blo@kde.org") << QStringLiteral("foo@kde.org,      bla@kde.org,blo@kde.org");
    QTest::newRow("multimail-7") << QStringLiteral("bla <foo@kde.org>,\r\n     bli <bla@kde.org>,blo <blo@kde.org>") << QStringLiteral("bla <foo@kde.org>,      bli <bla@kde.org>,blo <blo@kde.org>");
    QTest::newRow("multimail-8") << QStringLiteral("\"bla, v\" <foo@kde.org>,\r\n     \"bli, u\" <bla@kde.org>,\"blo, k\" <blo@kde.org>") << QStringLiteral(
        "\"bla, v\" <foo@kde.org>,      \"bli, u\" <bla@kde.org>,\"blo, k\" <blo@kde.org>");
}

void AddresseeLineEditUtilTest::shouldAdaptPasteMails()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(KPIM::AddresseeLineEditUtil::adaptPasteMails(input), output);
}

QTEST_MAIN(AddresseeLineEditUtilTest)
