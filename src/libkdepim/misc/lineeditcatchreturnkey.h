/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kdepim_export.h"
#include <QObject>
class QLineEdit;
namespace KPIM
{
/**
 * @brief The LineEditCatchReturnKey class
 * @author Laurent Montel <montel@kde.org>
 */
class KDEPIM_EXPORT LineEditCatchReturnKey : public QObject
{
    Q_OBJECT
public:
    explicit LineEditCatchReturnKey(QLineEdit *lineEdit, QObject *parent = nullptr);
    ~LineEditCatchReturnKey() override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLineEdit *mLineEdit = nullptr;
};
}
