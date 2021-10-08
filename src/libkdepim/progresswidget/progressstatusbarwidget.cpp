/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "progressstatusbarwidget.h"

#include "progressdialog.h"
#include "statusbarprogresswidget.h"

using namespace KPIM;
class KPIM::ProgressStatusBarWidgetPrivate
{
public:
    ProgressStatusBarWidgetPrivate()
    {
    }

    KPIM::StatusbarProgressWidget *mLittleProgress = nullptr;
};

ProgressStatusBarWidget::ProgressStatusBarWidget(QWidget *alignWidget, QWidget *parent, unsigned int showTypeProgressItem)
    : QObject(parent)
    , d(new KPIM::ProgressStatusBarWidgetPrivate)
{
    auto progressDialog = new KPIM::ProgressDialog(alignWidget, parent);
    progressDialog->setObjectName(QStringLiteral("progressDialog"));
    progressDialog->setShowTypeProgressItem(showTypeProgressItem);
    progressDialog->hide();

    d->mLittleProgress = new KPIM::StatusbarProgressWidget(progressDialog, alignWidget);
    d->mLittleProgress->setObjectName(QStringLiteral("littleprogress"));
    d->mLittleProgress->setShowTypeProgressItem(showTypeProgressItem);
    d->mLittleProgress->show();
}

ProgressStatusBarWidget::~ProgressStatusBarWidget() = default;

KPIM::StatusbarProgressWidget *ProgressStatusBarWidget::littleProgress() const
{
    return d->mLittleProgress;
}
