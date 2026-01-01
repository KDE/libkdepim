/*
   SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "progressstatusbarwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "progressdialog.h"
#include "statusbarprogresswidget.h"

using namespace KPIM;
class KPIM::ProgressStatusBarWidgetPrivate
{
public:
    ProgressStatusBarWidgetPrivate() = default;

    KPIM::StatusbarProgressWidget *mLittleProgress = nullptr;
};

ProgressStatusBarWidget::ProgressStatusBarWidget(QWidget *alignWidget, QWidget *parent, unsigned int showTypeProgressItem)
    : QObject(parent)
    , d(new KPIM::ProgressStatusBarWidgetPrivate)
{
    auto progressDialog = new KPIM::ProgressDialog(alignWidget, parent);
    progressDialog->setObjectName("progressDialog"_L1);
    progressDialog->setShowTypeProgressItem(showTypeProgressItem);
    progressDialog->hide();

    d->mLittleProgress = new KPIM::StatusbarProgressWidget(progressDialog, alignWidget);
    d->mLittleProgress->setObjectName("littleprogress"_L1);
    d->mLittleProgress->setShowTypeProgressItem(showTypeProgressItem);
    d->mLittleProgress->show();
}

ProgressStatusBarWidget::~ProgressStatusBarWidget() = default;

KPIM::StatusbarProgressWidget *ProgressStatusBarWidget::littleProgress() const
{
    return d->mLittleProgress;
}

#include "moc_progressstatusbarwidget.cpp"
