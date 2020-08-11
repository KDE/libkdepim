/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "progressstatusbarwidget.h"

#include "statusbarprogresswidget.h"
#include "progressdialog.h"

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
    KPIM::ProgressDialog *progressDialog = new KPIM::ProgressDialog(alignWidget, parent);
    progressDialog->setObjectName(QStringLiteral("progressDialog"));
    progressDialog->setShowTypeProgressItem(showTypeProgressItem);
    progressDialog->hide();

    d->mLittleProgress = new KPIM::StatusbarProgressWidget(progressDialog, alignWidget);
    d->mLittleProgress->setObjectName(QStringLiteral("littleprogress"));
    d->mLittleProgress->setShowTypeProgressItem(showTypeProgressItem);
    d->mLittleProgress->show();
}

ProgressStatusBarWidget::~ProgressStatusBarWidget()
{
    delete d;
}

KPIM::StatusbarProgressWidget *ProgressStatusBarWidget::littleProgress() const
{
    return d->mLittleProgress;
}
