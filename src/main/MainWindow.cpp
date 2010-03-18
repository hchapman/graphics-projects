#include <QtGui>
#include "MainWindow.h"

MainWindow::MainWindow()
{
	createMenus();
	setWindowTitle("CSCI330SP09 Rendering Engine");

	// the menu bar (21 pixels) is part of the widget size...
	//resize(512,512+21);
    resize(200,200+21);
	
	renderWidget = new RenderWidget0();
	setCentralWidget(renderWidget);
}

void MainWindow::createMenus()
{
	exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	startAct = new QAction(tr("St&art animation"), this);
    startAct->setShortcut(tr("Ctrl+a"));
    connect(startAct, SIGNAL(triggered()), this, SLOT(startAnimation()));
	
	stopAct = new QAction(tr("St&op animation"), this);
    stopAct->setShortcut(tr("Ctrl+o"));
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stopAnimation()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    animationMenu = menuBar()->addMenu(tr("&Animation"));
    animationMenu->addAction(startAct);
    animationMenu->addAction(stopAct);
}

void MainWindow::startAnimation()
{
	renderWidget->startAnimation();
}

void MainWindow::stopAnimation()
{
	renderWidget->stopAnimation();
}
