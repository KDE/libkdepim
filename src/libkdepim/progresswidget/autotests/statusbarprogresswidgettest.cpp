/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

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

#include "statusbarprogresswidgettest.h"
#include "../statusbarprogresswidget.h"
#include "../progressdialog.h"
#include "../progressmanager.h"
#include <QTest>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QProgressBar>
#include <QStandardPaths>
#include <progresswidget/ssllabel.h>
QTEST_MAIN(StatusbarProgressWidgetTest)

StatusbarProgressWidgetTest::StatusbarProgressWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void StatusbarProgressWidgetTest::shouldHaveDefaultValue()
{
    QWidget *widget = new QWidget;
    KPIM::ProgressDialog *progress = new KPIM::ProgressDialog(widget, widget);
    KPIM::StatusbarProgressWidget w(progress, widget);
    QHBoxLayout *boxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("boxLayout"));
    QVERIFY(boxLayout);
    QCOMPARE(boxLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    QCOMPARE(boxLayout->spacing(), 0);

    QPushButton *mButton = w.findChild<QPushButton *>(QStringLiteral("button"));
    QVERIFY(mButton);
    QVERIFY(mButton->text().isEmpty());

    QStackedWidget *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedwidget"));
    QVERIFY(mStackedWidget);

    KPIM::SSLLabel *mSslLabel = w.findChild<KPIM::SSLLabel *>(QStringLiteral("ssllabel"));
    QVERIFY(mSslLabel);

    QProgressBar *mProgressBar = w.findChild<QProgressBar *>(QStringLiteral("progressbar"));
    QVERIFY(mProgressBar);

    QLabel *mLabel = w.findChild<QLabel *>(QStringLiteral("emptylabel"));
    QVERIFY(mLabel);
}

void StatusbarProgressWidgetTest::shouldShowItems()
{
    /*
    QWidget *widget = new QWidget;
    KPIM::ProgressDialog *progress = new KPIM::ProgressDialog(widget, widget);
    KPIM::StatusbarProgressWidget w(progress, widget);
    w.show();
    QPushButton *mButton = w.findChild<QPushButton *>(QStringLiteral("button"));

    QProgressBar *mProgressBar = w.findChild<QProgressBar *>(QStringLiteral("progressbar"));
    QVERIFY(!mProgressBar->isVisible());

    KPIM::ProgressManager::createProgressItem(KPIM::ProgressManager::getUniqueID(), QStringLiteral("Title"), QString(), true);
    QVERIFY(mProgressBar->isVisible());
    */
}
