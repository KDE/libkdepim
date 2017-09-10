/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "emailvalidatortest.h"
#include <Libkdepim/EmailValidator>
#include <QTest>

QTEST_MAIN(EmailValidatorTest)
Q_DECLARE_METATYPE(QValidator::State)
EmailValidatorTest::EmailValidatorTest(QObject *parent)
    : QObject(parent)
{
}

void EmailValidatorTest::shouldValidateEmail_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<QValidator::State>("state");
    QTest::newRow("empty") << QString() << QValidator::Intermediate;
    QTest::newRow("email") << QStringLiteral("foo@kde.org") << QValidator::Acceptable;
    QTest::newRow("notanemail") << QStringLiteral("foo") << QValidator::Intermediate;
    QTest::newRow("space") << QStringLiteral("foo ") << QValidator::Invalid;
    QTest::newRow("space1") << QStringLiteral(" foo") << QValidator::Invalid;
    QTest::newRow("email2") << QStringLiteral("<foo@kde.org>") << QValidator::Intermediate;
    QTest::newRow("email3") << QStringLiteral("\"bla\" <foo@kde.org>") << QValidator::Invalid;
}

void EmailValidatorTest::shouldValidateEmail()
{
    QFETCH(QString, email);
    QFETCH(QValidator::State, state);
    KPIM::EmailValidator val(nullptr);
    int pos;
    QCOMPARE(val.validate(email, pos), state);
}
