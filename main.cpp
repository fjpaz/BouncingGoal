// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "MainWindow.h"
#include "World.h"

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Bouncing Goal");

    MainWindow window;
    window.showNormal();

    World world(window);

    return app.exec();
}
