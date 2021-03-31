/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "kdepim_export.h"
#include <QObject>

namespace KPIM
{
class StatusbarProgressWidget;
class ProgressStatusBarWidgetPrivate;
/**
 * @brief The ProgressStatusBarWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KDEPIM_EXPORT ProgressStatusBarWidget : public QObject
{
    Q_OBJECT
public:
    explicit ProgressStatusBarWidget(QWidget *alignWidget, QWidget *parent = nullptr, unsigned int showTypeProgressItem = 0);
    ~ProgressStatusBarWidget();

    Q_REQUIRED_RESULT KPIM::StatusbarProgressWidget *littleProgress() const;

private:
    ProgressStatusBarWidgetPrivate *const d;
};
}

