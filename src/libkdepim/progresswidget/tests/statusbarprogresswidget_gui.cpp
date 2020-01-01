/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "statusbarprogresswidget_gui.h"
#include "../statusbarprogresswidget.h"

#include <QApplication>
#include <QCommandLineParser>

StatusbarProgressWidget_Gui::StatusbarProgressWidget_Gui(QWidget *parent)
    : QWidget(parent)
{
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);
    StatusbarProgressWidget_Gui *w = new StatusbarProgressWidget_Gui;
    w->resize(800, 600);
    w->show();
    app.exec();
    delete w;
    return 0;
}
