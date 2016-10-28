#ifndef PROPERTIES_WIDGET_H_INCLUDED
#define PROPERTIES_WIDGET_H_INCLUDED

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

class PropertiesWidget : public QTableWidget {
	Q_OBJECT

	public:
		explicit PropertiesWidget(QWidget *parent = 0);
		~PropertiesWidget();
};

#endif