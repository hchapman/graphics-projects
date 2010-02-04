#ifndef __MainWindow_h__
#define __MainWindow_h__

#include <QMainWindow>
#include "RenderWidget0.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void startAnimation();
	void stopAnimation();

private:
	void createMenus();

	QMenu *fileMenu;
	QMenu *animationMenu;
	QAction *exitAct;
	QAction *startAct;
	QAction *stopAct;

	RenderWidget0 *renderWidget;
};

#endif