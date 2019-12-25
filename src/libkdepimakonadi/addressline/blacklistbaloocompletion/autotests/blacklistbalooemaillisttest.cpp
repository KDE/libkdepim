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

#include "blacklistbalooemaillisttest.h"
#include "../blacklistbalooemaillist.h"
#include <QTest>

BlackListBalooEmailListTest::BlackListBalooEmailListTest(QObject *parent)
    : QObject(parent)
{
}

BlackListBalooEmailListTest::~BlackListBalooEmailListTest()
{
}

void BlackListBalooEmailListTest::shouldHaveDefaultValue()
{
    KPIM::BlackListBalooEmailList blackList;
    QVERIFY(blackList.count() == 0);
}

void BlackListBalooEmailListTest::shouldFillListEmail()
{
    KPIM::BlackListBalooEmailList blackList;
    QCOMPARE(blackList.setEmailFound(QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org") << QStringLiteral("bli@kde.org")), 3);
    QCOMPARE(blackList.count(), 3);
    for (int i = 0; i < blackList.count(); ++i) {
        QListWidgetItem *item = blackList.item(i);
        QVERIFY(item);
        KPIM::BlackListBalooEmailListItem *blackListItem = static_cast<KPIM::BlackListBalooEmailListItem *>(item);
        QVERIFY(!blackListItem->initializeStatus());
        QCOMPARE(blackListItem->checkState(), Qt::Unchecked);
    }
    QVERIFY(blackList.blackListItemChanged().isEmpty());
}

void BlackListBalooEmailListTest::shouldFillListWithAlreadyBlackListedEmail()
{
    KPIM::BlackListBalooEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org") << QStringLiteral("bli@kde.org");
    blackList.setEmailBlackList(emails);
    QCOMPARE(blackList.setEmailFound(emails), 3);

    QCOMPARE(blackList.count(), 3);
    for (int i = 0; i < blackList.count(); ++i) {
        QListWidgetItem *item = blackList.item(i);
        QVERIFY(item);
        KPIM::BlackListBalooEmailListItem *blackListItem = static_cast<KPIM::BlackListBalooEmailListItem *>(item);
        QVERIFY(blackListItem->initializeStatus());
        QCOMPARE(blackListItem->checkState(), Qt::Checked);
    }

    QVERIFY(blackList.blackListItemChanged().isEmpty());
}

void BlackListBalooEmailListTest::shouldReturnChangedItems()
{
    KPIM::BlackListBalooEmailList blackList;
    const QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org") << QStringLiteral("bli@kde.org");
    blackList.setEmailBlackList(emails);
    QCOMPARE(blackList.setEmailFound(emails), 3);
    QListWidgetItem *item = blackList.item(1);
    QVERIFY(item);
    item->setCheckState(Qt::Unchecked);
    QVERIFY(!blackList.blackListItemChanged().isEmpty());
}

void BlackListBalooEmailListTest::shouldNotAddDuplicateEmails()
{
    KPIM::BlackListBalooEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bli@kde.org") << QStringLiteral("bli@kde.org");
    blackList.setEmailBlackList(emails);
    QCOMPARE(blackList.setEmailFound(emails), 2);

    QCOMPARE(blackList.count(), 2);
}

void BlackListBalooEmailListTest::shouldExcludeDomain()
{
    KPIM::BlackListBalooEmailList blackList;
    blackList.setExcludeDomain(QStringList() << QStringLiteral("kde.org") << QStringLiteral("toto.fr"));
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bli@fr.fr") << QStringLiteral("bli@toto.fr");
    blackList.setEmailBlackList(emails);
    QCOMPARE(blackList.setEmailFound(emails), 1);

    QCOMPARE(blackList.count(), 1);

    blackList.setExcludeDomain(QStringList() << QStringLiteral("kde.org") << QStringLiteral("toto.fr"));
    emails = QStringList() << QStringLiteral("<foo@kde.org>") << QStringLiteral("bli@fr.fr") << QStringLiteral("bli@toto.fr");
    blackList.setEmailBlackList(emails);
    QCOMPARE(blackList.setEmailFound(emails), 1);
    QCOMPARE(blackList.count(), 1);
}

void BlackListBalooEmailListTest::shouldAvoidSameEmailWithDifferentCase()
{
    KPIM::BlackListBalooEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("Foo@kde.org") << QStringLiteral("foo@kde.ORG");
    QCOMPARE(blackList.setEmailFound(emails), 1);

    QCOMPARE(blackList.count(), 1);
}

void BlackListBalooEmailListTest::shouldAvoidSameEmailWithDisplayNameOrNot()
{
    KPIM::BlackListBalooEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org")
                                       << QStringLiteral("Bla Blo <Foo@kde.org>");
    QCOMPARE(blackList.setEmailFound(emails), 1);

    QCOMPARE(blackList.count(), 1);
}

QTEST_MAIN(BlackListBalooEmailListTest)
