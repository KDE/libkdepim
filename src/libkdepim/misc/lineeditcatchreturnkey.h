/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LINEEDITCATCHRETURNKEY_H
#define LINEEDITCATCHRETURNKEY_H

#include <QObject>
#include "kdepim_export.h"
class QLineEdit;
namespace KPIM {
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
#endif // LINEEDITCATCHRETURNKEY_H
