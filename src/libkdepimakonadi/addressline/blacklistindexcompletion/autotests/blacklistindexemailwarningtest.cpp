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

#include "blacklistindexemailwarningtest.h"
#include "../blacklistindexemailwarning.h"
#include <QTest>
#include <QSignalSpy>
#include <QAction>

BlackListIndexEmailWarningTest::BlackListIndexEmailWarningTest(QObject *parent)
    : QObject(parent)
{
}

BlackListIndexEmailWarningTest::~BlackListIndexEmailWarningTest()
{
}

void BlackListIndexEmailWarningTest::shouldHaveDefaultValue()
{
    KPIM::BlackListIndexEmailWarning warning;
    QVERIFY(!warning.isVisible());
    QVERIFY(!warning.isCloseButtonVisible());
    QVERIFY(warning.wordWrap());

    QAction *save = warning.findChild<QAction *>(QStringLiteral("saveblacklist"));
    QVERIFY(save);

    QAction *search = warning.findChild<QAction *>(QStringLiteral("search"));
    QVERIFY(search);
}

void BlackListIndexEmailWarningTest::shouldEmitSaveChanges()
{
    KPIM::BlackListIndexEmailWarning warning;
    QSignalSpy spy1(&warning, SIGNAL(saveChanges()));

    QAction *save = warning.findChild<QAction *>(QStringLiteral("saveblacklist"));
    save->trigger();
    QCOMPARE(spy1.count(), 1);
}

void BlackListIndexEmailWarningTest::shouldEmitNewSearch()
{
    KPIM::BlackListIndexEmailWarning warning;
    QSignalSpy spy1(&warning, SIGNAL(newSearch()));

    QAction *search = warning.findChild<QAction *>(QStringLiteral("search"));
    search->trigger();
    QCOMPARE(spy1.count(), 1);
}

QTEST_MAIN(BlackListIndexEmailWarningTest)
