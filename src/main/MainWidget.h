#ifndef __MainWidget_h__
#define __MainWidget_h__

#include <QWidget>
#include <QPushButton>

class MainWidget : public QPushButton
{
public:
	MainWidget(QWidget *parent = 0, Qt::WFlags flags = 0);

	QSizePolicy sizePolicy();
	void resizeEvent(QResizeEvent * event);
};

#endif
