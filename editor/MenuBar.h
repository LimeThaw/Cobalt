#ifndef MENU_BAR_H_INCLUDED
#define MENU_BAR_H_INCLUDED

#include <QMenuBar>

namespace ce {

class MainWindow;

class MenuBar : public QMenuBar {
	Q_OBJECT

	public:
		explicit MenuBar(MainWindow *window, QWidget *parent = 0);
		~MenuBar();

	private:
		MainWindow *window;
};

}

#endif