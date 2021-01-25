/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    auto w = new StatusbarProgressWidget_Gui;
    w->resize(800, 600);
    w->show();
    app.exec();
    delete w;
    return 0;
}
