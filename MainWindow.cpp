// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _full_screen(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->installEventFilter(this);
    setWindowTitle(tr("Bouncing Goal"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGraphicsView & MainWindow::getView()
{
    return *ui->graphicsView;
}

void MainWindow::show()
{
    _full_screen = false;
    QMainWindow::show();
}

void MainWindow::showNormal()
{
    _full_screen = false;
    QMainWindow::showNormal();
}

void MainWindow::showFullScreen()
{
    _full_screen = true;
    QMainWindow::showFullScreen();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch ( event->key() )
    {
    case Qt::Key_F:
        if ( _full_screen )
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
        break;
    default:
        break;
    }

    emit keyReleased(event->key());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    emit windowResized();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent = NULL; // event data, if this is a keystroke event
    bool result = false;        // return true to consume the keystroke

    if (event->type() == QEvent::KeyPress)
    {
        keyEvent = dynamic_cast<QKeyEvent*>(event);
        this->keyPressEvent(keyEvent);
        result = true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        keyEvent = dynamic_cast<QKeyEvent*>(event);
        this->keyReleaseEvent(keyEvent);
        result = true;
    }
    else
    {
        // Standard event processing
        result = QObject::eventFilter(obj, event);
    }

    return result;
}
