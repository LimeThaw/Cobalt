#include "PropertiesWidget.h"

PropertiesWidget::PropertiesWidget(QWidget *parent) : QTableWidget(parent) {
	verticalHeader()->setVisible(false);
	setFixedWidth(300);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setAlternatingRowColors(true);
	setRowCount(10);
	setColumnCount(2);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setHorizontalHeaderItem(0, new QTableWidgetItem("Property"));
	setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
	setItem(0, 0, new QTableWidgetItem("Name"));
	setItem(0, 1, new QTableWidgetItem("Value"));
	setItem(1, 0, new QTableWidgetItem("Lorem"));
	setItem(1, 1, new QTableWidgetItem("Ipsum"));
	setEditTriggers(QAbstractItemView::NoEditTriggers);
}

PropertiesWidget::~PropertiesWidget() {}