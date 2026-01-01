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
     * \since 4.12
     */
    explicit ProgressIndicatorLabel(const QString &labelStr, QWidget *parent = nullptr);
    /*!
     */
    explicit ProgressIndicatorLabel(QWidget *parent = nullptr);
    /*!
     */
    ~ProgressIndicatorLabel() override;

    /*!
     */
    void setActiveLabel(const QString &label);

public Q_SLOTS:
    /*!
     */
    void start();
    /*!
     */
    void stop();

private:
    friend class ProgressIndicatorLabelPrivate;
    std::unique_ptr<ProgressIndicatorLabelPrivate> const d;
};
}
