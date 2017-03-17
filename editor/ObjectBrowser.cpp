#include "ObjectBrowser.h"

ObjectBrowser::ObjectBrowser(QWidget *parent) : QTabWidget(parent) {
	// Add Groups tab
	QTreeWidget *nodeTab = new QTreeWidget();
	nodeTab->setAlternatingRowColors(true);

	QTreeWidgetItem *node = new QTreeWidgetItem(nodeTab);
	node->setText(0, "Sample node");
	QTreeWidgetItem *mesh = new QTreeWidgetItem();
	mesh->setText(0, "Sample mesh");
	node->addChild(mesh);
	mesh = new QTreeWidgetItem();
	mesh->setText(0, "Lorem Ipsum");
	node->addChild(mesh);
	node = new QTreeWidgetItem(nodeTab);
	node->setText(0, "Deuxieme node");
	mesh = new QTreeWidgetItem();
	mesh->setText(0, "Another mesh");
	node->addChild(mesh);
	mesh = new QTreeWidgetItem();
	mesh->setText(0, "Tralalalalala");
	node->addChild(mesh);
	node = new QTreeWidgetItem(nodeTab);
	node->setText(0, "Deuxieme node");

	addTab(nodeTab, "Nodes");
	// Add Material tab
	QTreeWidget *matTab = new QTreeWidget();
	addTab(matTab, "Materials");
	// Add Texture tab
	QTreeWidget *textureTab = new QTreeWidget();
	addTab(textureTab, "Textures");
}
ObjectBrowser::~ObjectBrowser() {}