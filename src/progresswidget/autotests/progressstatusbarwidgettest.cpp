/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "progressstatusbarwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../progressdialog.h"
#include "../progressstatusbarwidget.h"
#include "../statusbarprogresswidget.h"
#include <QTest>
QTEST_MAIN(ProgressStatusBarWidgetTest)

ProgressStatusBarWidgetTest::ProgressStatusBarWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ProgressStatusBarWidgetTest::shouldHaveDefaultValue()
{
    auto widget = new QWidget;
    KPIM::ProgressStatusBarWidget w(widget, widget);
    widget->show();

    auto progressDialog = widget->findChild<KPIM::ProgressDialog *>(u"progressDialog"_s);
    QVERIFY(progressDialog);
    QVERIFY(progressDialog->isHidden());

    auto mLittleProgress = widget->findChild<KPIM::StatusbarProgressWidget *>(u"littleprogress"_s);
    QVERIFY(mLittleProgress);
    QVERIFY(mLittleProgress->isVisible());

    QCOMPARE(mLittleProgress, w.littleProgress());
}

#include "moc_progressstatusbarwidgettest.cpp"
