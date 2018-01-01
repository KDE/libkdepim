/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

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

#include "progressstatusbarwidgettest.h"
#include "../progressstatusbarwidget.h"
#include <QTest>
#include "../progressdialog.h"
#include "../statusbarprogresswidget.h"
QTEST_MAIN(ProgressStatusBarWidgetTest)

ProgressStatusBarWidgetTest::ProgressStatusBarWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ProgressStatusBarWidgetTest::shouldHaveDefaultValue()
{
    QWidget *widget = new QWidget;
    KPIM::ProgressStatusBarWidget w(widget, widget);
    widget->show();

    KPIM::ProgressDialog *progressDialog = widget->findChild<KPIM::ProgressDialog *>(QStringLiteral("progressDialog"));
    QVERIFY(progressDialog);
    QVERIFY(progressDialog->isHidden());

    KPIM::StatusbarProgressWidget *mLittleProgress = widget->findChild<KPIM::StatusbarProgressWidget *>(QStringLiteral("littleprogress"));
    QVERIFY(mLittleProgress);
    QVERIFY(mLittleProgress->isVisible());

    QCOMPARE(mLittleProgress, w.littleProgress());
}
