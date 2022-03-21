/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class StatusbarProgressWidget_Gui : public QWidget
{
    Q_OBJECT
public:
    explicit StatusbarProgressWidget_Gui(QWidget *parent = nullptr);
    ~StatusbarProgressWidget_Gui() override = default;
};
