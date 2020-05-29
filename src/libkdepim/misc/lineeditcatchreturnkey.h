/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    ~LineEditCatchReturnKey();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLineEdit *mLineEdit = nullptr;
};
}
#endif // LINEEDITCATCHRETURNKEY_H