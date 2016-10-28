#ifndef MENU_BAR_H_INCLUDED
#define MENU_BAR_H_INCLUDED

#include <QMenuBar>

class MenuBar : public QMenuBar {
	Q_OBJECT

	public:
		explicit MenuBar(QWidget *parent = 0);
		~MenuBar();
};

#endif