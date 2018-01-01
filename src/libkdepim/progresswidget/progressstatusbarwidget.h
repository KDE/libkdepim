/*
   Copyright (C) 2014-2018 Laurent Montel <montel@kde.org>

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
#ifndef PROGRESSSTATUSBARWIDGET_H
#define PROGRESSSTATUSBARWIDGET_H

#include <QObject>
#include "kdepim_export.h"

namespace KPIM {
class StatusbarProgressWidget;
class ProgressStatusBarWidgetPrivate;
class KDEPIM_EXPORT ProgressStatusBarWidget : public QObject
{
    Q_OBJECT
public:
    explicit ProgressStatusBarWidget(QWidget *alignWidget, QWidget *parent = nullptr, unsigned int showTypeProgressItem = 0);
    ~ProgressStatusBarWidget();

    KPIM::StatusbarProgressWidget *littleProgress() const;

private:
    ProgressStatusBarWidgetPrivate *const d;
};
}

#endif // PROGRESSSTATUSBARWIDGET_H
