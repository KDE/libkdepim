/*
    SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kdepim_export.h"

#include <QWidget>
namespace KPIM
{
class ProgressIndicatorLabelPrivate;
/*!
 * \class KPIM::ProgressIndicatorLabel
 * \inmodule Libkdepim
 * \inheaderfile KPIM/ProgressIndicatorLabel
 *
 * \brief The ProgressIndicatorLabel class
 * \author Laurent Montel <montel@kde.org>
 */
class KDEPIM_EXPORT ProgressIndicatorLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for ProgressIndicatorLabel with a label string.
     * \param labelStr the label text to display
     * \param parent the parent widget
     * \since 4.12
     */
    explicit ProgressIndicatorLabel(const QString &labelStr, QWidget *parent = nullptr);
    /*!
     * \brief Constructor for ProgressIndicatorLabel without a label.
     * \param parent the parent widget
     */
    explicit ProgressIndicatorLabel(QWidget *parent = nullptr);
    /*!
     * \brief Destructor for ProgressIndicatorLabel.
     */
    ~ProgressIndicatorLabel() override;

    /*!
     * \brief Sets the active label text.
     * \param label the new label text
     */
    void setActiveLabel(const QString &label);

public Q_SLOTS:
    /*!
     * \brief Starts the progress indicator animation.
     */
    void start();
    /*!
     * \brief Stops the progress indicator animation.
     */
    void stop();

private:
    friend class ProgressIndicatorLabelPrivate;
    std::unique_ptr<ProgressIndicatorLabelPrivate> const d;
};
}
