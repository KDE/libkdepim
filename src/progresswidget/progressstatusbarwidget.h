/*
   SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \class KPIM::ProgressStatusBarWidget
 * \inmodule Libkdepim
 * \inheaderfile KPIM/ProgressStatusBarWidget
 *
 * \brief The ProgressStatusBarWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KDEPIM_EXPORT ProgressStatusBarWidget : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for ProgressStatusBarWidget.
     * \param alignWidget the widget to align with
     * \param parent the parent widget
     * \param showTypeProgressItem the progress item type to show
     */
    explicit ProgressStatusBarWidget(QWidget *alignWidget, QWidget *parent = nullptr, unsigned int showTypeProgressItem = 0);
    /*!
     * \brief Destructor for ProgressStatusBarWidget.
     */
    ~ProgressStatusBarWidget() override;

    /*!
     * \brief Returns the status bar progress widget.
     * \return the StatusbarProgressWidget instance
     */
    [[nodiscard]] KPIM::StatusbarProgressWidget *littleProgress() const;

private:
    std::unique_ptr<ProgressStatusBarWidgetPrivate> const d;
};
}
