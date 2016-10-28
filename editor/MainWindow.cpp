#include "MainWindow.h"
using namespace ce;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	// Create window with title and size
	setWindowTitle("Cobalt Editor");
	resize(1280, 720);

	// Add menu bar
	MenuBar *menuBar = new MenuBar();
	setMenuBar(menuBar);

	// Put the main container in place
	QWidget *containerWidget = new QWidget();

	QGridLayout *layout = new QGridLayout();
	layout->setContentsMargins(10, 10, 10, 10);

	// Add the object browser widget
	ObjectBrowser *objectBrowser = new ObjectBrowser();
	objectBrowser->setFixedWidth(300);
	layout->addWidget(objectBrowser, 1, 1, 1, 1);

	// Add preview widget
	QGLWidget *previewWidget = new QGLWidget();
	layout->addWidget(previewWidget, 1, 2, 1, 1);

	// Add console widget
	QTextEdit *consoleWidget = new QTextEdit();
	consoleWidget->setFixedHeight(150);
	consoleWidget->setReadOnly(true);
	for(char i = 0; i < 2; ++i) {
	consoleWidget->append("Lorem Ipsum");	// Sample text
	consoleWidget->append("Lorem Ipsum numero deux");
	}
	layout->addWidget(consoleWidget, 2, 1, 1, 2);

	// Add properties widget
	PropertiesWidget *propertiesWidget = new PropertiesWidget();
	layout->addWidget(propertiesWidget, 1, 3, 2, 1);

	containerWidget->setLayout(layout);
	setCentralWidget(containerWidget);
}

MainWindow::~MainWindow() {}

bool MainWindow::event(QEvent *event) {

	if(event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

		if(keyEvent->key() == Qt::Key_Escape) {
			this->close();
			return true;
		}
	}

	return QMainWindow::event(event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *e) {
	QMenu menu(this);
	menu.addAction("wasd");
	menu.addAction("asdf");
	menu.exec(e->globalPos());
}