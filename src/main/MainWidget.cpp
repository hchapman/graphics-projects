#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent, Qt::WFlags flags)
    : QPushButton("ok", parent)
{
	
}

QSizePolicy MainWidget::sizePolicy()
{
	return QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void MainWidget::resizeEvent(QResizeEvent * event) 
{
	qDebug("resize");
}