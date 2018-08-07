/*
  Copyright (c) 2015-2018 Montel Laurent <montel@kde.org>

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

#include "blacklistindexemaillisttest.h"
#include "../blacklistindexemaillist.h"
#include <qtest.h>

BlackListIndexEmailListTest::BlackListIndexEmailListTest(QObject *parent)
    : QObject(parent)
{
}

BlackListIndexEmailListTest::~BlackListIndexEmailListTest()
{
}

void BlackListIndexEmailListTest::shouldHaveDefaultValue()
{
    KPIM::BlackListIndexEmailList blackList;
    QVERIFY(blackList.count() == 0);
}

void BlackListIndexEmailListTest::shouldFillListEmail()
{
    KPIM::BlackListIndexEmailList blackList;
    blackList.setEmailFound(QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org") << QStringLiteral("bli@kde.org"));
    QCOMPARE(blackList.count(), 3);
    for (int i = 0; i < blackList.count(); ++i) {
        QListWidgetItem *item = blackList.item(i);
        QVERIFY(item);
        KPIM::BlackListIndexEmailListItem *blackListItem = static_cast<KPIM::BlackListIndexEmailListItem *>(item);
        QVERIFY(!blackListItem->initializeStatus());
        QCOMPARE(blackListItem->checkState(), Qt::Unchecked);
    }
    QVERIFY(blackList.blackListItemChanged().isEmpty());
}

void BlackListIndexEmailListTest::shouldFillListWithAlreadyBlackListedEmail()
{
    KPIM::BlackListIndexEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org") << QStringLiteral("bli@kde.org");
    blackList.setEmailBlackList(emails);
    blackList.setEmailFound(emails);

    QCOMPARE(blackList.count(), 3);
    for (int i = 0; i < blackList.count(); ++i) {
        QListWidgetItem *item = blackList.item(i);
        QVERIFY(item);
        KPIM::BlackListIndexEmailListItem *blackListItem = static_cast<KPIM::BlackListIndexEmailListItem *>(item);
        QVERIFY(blackListItem->initializeStatus());
        QCOMPARE(blackListItem->checkState(), Qt::Checked);
    }

    QVERIFY(blackList.blackListItemChanged().isEmpty());
}

void BlackListIndexEmailListTest::shouldReturnChangedItems()
{
    KPIM::BlackListIndexEmailList blackList;
    const QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org") << QStringLiteral("bli@kde.org");
    blackList.setEmailBlackList(emails);
    blackList.setEmailFound(emails);
    QListWidgetItem *item = blackList.item(1);
    QVERIFY(item);
    item->setCheckState(Qt::Unchecked);
    QVERIFY(!blackList.blackListItemChanged().isEmpty());
}

void BlackListIndexEmailListTest::shouldNotAddDuplicateEmails()
{
    KPIM::BlackListIndexEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bli@kde.org") << QStringLiteral("bli@kde.org");
    blackList.setEmailBlackList(emails);
    blackList.setEmailFound(emails);

    QCOMPARE(blackList.count(), 2);
}

void BlackListIndexEmailListTest::shouldExcludeDomain()
{
    KPIM::BlackListIndexEmailList blackList;
    blackList.setExcludeDomain(QStringList() << QStringLiteral("kde.org") << QStringLiteral("toto.fr"));
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bli@fr.fr") << QStringLiteral("bli@toto.fr");
    blackList.setEmailBlackList(emails);
    blackList.setEmailFound(emails);

    QCOMPARE(blackList.count(), 1);

    blackList.setExcludeDomain(QStringList() << QStringLiteral("kde.org") << QStringLiteral("toto.fr"));
    emails = QStringList() << QStringLiteral("<foo@kde.org>") << QStringLiteral("bli@fr.fr") << QStringLiteral("bli@toto.fr");
    blackList.setEmailBlackList(emails);
    blackList.setEmailFound(emails);
    QCOMPARE(blackList.count(), 1);
}

void BlackListIndexEmailListTest::shouldAvoidSameEmailWithDifferentCase()
{
    KPIM::BlackListIndexEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("Foo@kde.org") << QStringLiteral("foo@kde.ORG");
    blackList.setEmailFound(emails);

    QCOMPARE(blackList.count(), 1);
}

void BlackListIndexEmailListTest::shouldAvoidSameEmailWithDisplayNameOrNot()
{
    KPIM::BlackListIndexEmailList blackList;
    QStringList emails = QStringList() << QStringLiteral("foo@kde.org")
                                       << QStringLiteral("Bla Blo <Foo@kde.org>");
    blackList.setEmailFound(emails);

    QCOMPARE(blackList.count(), 1);
}

QTEST_MAIN(BlackListIndexEmailListTest)
