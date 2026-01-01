/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusbarprogresswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../progressdialog.h"
#include "../progressmanager.h"
#include "../statusbarprogresswidget.h"
#include "progresswidget/ssllabel.h"
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(StatusbarProgressWidgetTest)

StatusbarProgressWidgetTest::StatusbarProgressWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void StatusbarProgressWidgetTest::shouldHaveDefaultValue()
{
    auto widget = new QWidget;
    auto progress = new KPIM::ProgressDialog(widget, widget);
    KPIM::StatusbarProgressWidget w(progress, widget);
    auto boxLayout = w.findChild<QHBoxLayout *>(u"boxLayout"_s);
    QVERIFY(boxLayout);
    QCOMPARE(boxLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    QCOMPARE(boxLayout->spacing(), 0);

    auto mButton = w.findChild<QPushButton *>(u"button"_s);
    QVERIFY(mButton);
    QVERIFY(mButton->text().isEmpty());

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"stackedwidget"_s);
    QVERIFY(mStackedWidget);

    auto mSslLabel = w.findChild<KPIM::SSLLabel *>(u"ssllabel"_s);
    QVERIFY(mSslLabel);

    auto mProgressBar = w.findChild<QProgressBar *>(u"progressbar"_s);
    QVERIFY(mProgressBar);

    auto mLabel = w.findChild<QLabel *>(u"emptylabel"_s);
    QVERIFY(mLabel);
}

void StatusbarProgressWidgetTest::shouldShowItems()
{
    /*
    QWidget *widget = new QWidget;
    KPIM::ProgressDialog *progress = new KPIM::ProgressDialog(widget, widget);
    KPIM::StatusbarProgressWidget w(progress, widget);
    w.show();
    QPushButton *mButton = w.findChild<QPushButton *>(u"button"_s);

    QProgressBar *mProgressBar = w.findChild<QProgressBar *>(u"progressbar"_s);
    QVERIFY(!mProgressBar->isVisible());

    KPIM::ProgressManager::createProgressItem(KPIM::ProgressManager::getUniqueID(), u"Title"_s, QString(), true);
    QVERIFY(mProgressBar->isVisible());
    */
}

#include "moc_statusbarprogresswidgettest.cpp"
