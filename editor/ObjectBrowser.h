#ifndef OBJECT_BROWSER_H_INCLUDED
#define OBJECT_BROWSER_H_INCLUDED

#include <QTreeWidget>

class ObjectBrowser : public QTabWidget {
	Q_OBJECT

	public:
		explicit ObjectBrowser(QWidget *parent = 0);
		~ObjectBrowser();
};

#endif