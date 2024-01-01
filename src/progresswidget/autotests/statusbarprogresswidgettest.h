/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class StatusbarProgressWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit StatusbarProgressWidgetTest(QObject *parent = nullptr);
    ~StatusbarProgressWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldShowItems();
};
