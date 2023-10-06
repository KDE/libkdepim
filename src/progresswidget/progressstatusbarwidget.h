/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "kdepim_export.h"
#include <QObject>
#include <memory>
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
    ~ProgressStatusBarWidget() override;

    [[nodiscard]] KPIM::StatusbarProgressWidget *littleProgress() const;

private:
    std::unique_ptr<ProgressStatusBarWidgetPrivate> const d;
};
}
