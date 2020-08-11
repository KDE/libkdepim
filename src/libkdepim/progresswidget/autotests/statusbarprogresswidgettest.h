/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef STATUSBARPROGRESSWIDGETTEST_H
#define STATUSBARPROGRESSWIDGETTEST_H

#include <QObject>

class StatusbarProgressWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit StatusbarProgressWidgetTest(QObject *parent = nullptr);
    ~StatusbarProgressWidgetTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldShowItems();
};

#endif // STATUSBARPROGRESSWIDGETTEST_H
