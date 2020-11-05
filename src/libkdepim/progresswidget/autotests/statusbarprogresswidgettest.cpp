/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    auto *progress = new KPIM::ProgressDialog(widget, widget);
    KPIM::StatusbarProgressWidget w(progress, widget);
    auto *boxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("boxLayout"));
    QVERIFY(boxLayout);
    QCOMPARE(boxLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    QCOMPARE(boxLayout->spacing(), 0);

    auto *mButton = w.findChild<QPushButton *>(QStringLiteral("button"));
    QVERIFY(mButton);
    QVERIFY(mButton->text().isEmpty());

    auto *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedwidget"));
    QVERIFY(mStackedWidget);

    auto *mSslLabel = w.findChild<KPIM::SSLLabel *>(QStringLiteral("ssllabel"));
    QVERIFY(mSslLabel);

    auto *mProgressBar = w.findChild<QProgressBar *>(QStringLiteral("progressbar"));
    QVERIFY(mProgressBar);

    auto *mLabel = w.findChild<QLabel *>(QStringLiteral("emptylabel"));
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
