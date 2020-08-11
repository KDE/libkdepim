/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef STATUSBARPROGRESSWIDGET_GUI_H
#define STATUSBARPROGRESSWIDGET_GUI_H

#include <QWidget>

class StatusbarProgressWidget_Gui : public QWidget
{
    Q_OBJECT
public:
    explicit StatusbarProgressWidget_Gui(QWidget *parent = nullptr);
    ~StatusbarProgressWidget_Gui() = default;
};

#endif // STATUSBARPROGRESSWIDGET_GUI_H
