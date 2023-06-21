/*
    SPDX-FileCopyrightText: 2010 Casey Link <unnamedrambler@gmail.com>
    SPDX-FileCopyrightText: 2009-2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

    Refactored from earlier code by:
    SPDX-FileCopyrightText: 2010 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2004 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multiplyingline.h"

#include <QKeyEvent>

using namespace KPIM;

MultiplyingLine::MultiplyingLine(QWidget *parent)
    : QWidget(parent)
{
}

void MultiplyingLine::slotReturnPressed()
{
    Q_EMIT returnPressed(this);
}

void MultiplyingLine::slotPropagateDeletion()
{
    Q_EMIT deleteLine(this);
}

void MultiplyingLine::slotFocusUp()
{
    Q_EMIT upPressed(this);
}

void MultiplyingLine::slotFocusDown()
{
    Q_EMIT downPressed(this);
}

void MultiplyingLine::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Up) {
        Q_EMIT upPressed(this);
    } else if (ev->key() == Qt::Key_Down) {
        Q_EMIT downPressed(this);
    }
}

void MultiplyingLine::aboutToBeDeleted()
{
}

#include "moc_multiplyingline.cpp"
