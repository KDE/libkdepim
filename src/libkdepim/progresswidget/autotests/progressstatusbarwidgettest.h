/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PROGRESSSTATUSBARWIDGETTEST_H
#define PROGRESSSTATUSBARWIDGETTEST_H

#include <QObject>

class ProgressStatusBarWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ProgressStatusBarWidgetTest(QObject *parent = nullptr);
    ~ProgressStatusBarWidgetTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // PROGRESSSTATUSBARWIDGETTEST_H
