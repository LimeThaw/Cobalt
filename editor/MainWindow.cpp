#include "MainWindow.h"
using namespace ce;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	// Create window with title and size
	setWindowTitle("Cobalt Editor");
	resize(1280, 720);

	// Add menu bar
	MenuBar *menuBar = new MenuBar(this);
	setMenuBar(menuBar);

	// Put the main container in place
	containerWidget = new QWidget();

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
	consoleWidget = new QTextEdit();
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

	connectActions();

	scene = nullptr;
}

MainWindow::~MainWindow() {
	if(scene != nullptr) delete scene;
}

bool MainWindow::event(QEvent *event) {

	if(event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

		switch(keyEvent->key()) {
			case Qt::Key_Escape: // Esc
				this->close();
				break;
			case Qt::Key_N: // Ctrl-N
				if(QApplication::keyboardModifiers() & Qt::ControlModifier) newScene();
				break;
			case Qt::Key_O: // Ctrl-O
				if(QApplication::keyboardModifiers() & Qt::ControlModifier) openScene();
				break;
			case Qt::Key_S: // Ctrl-S
				if(QApplication::keyboardModifiers() & Qt::ControlModifier) saveScene();
				break;
			case Qt::Key_Q: // Ctrl-Q
				if(QApplication::keyboardModifiers() & Qt::ControlModifier) close();
				break;
			case Qt::Key_M: // Ctrl-M
				if(QApplication::keyboardModifiers() & Qt::ControlModifier) loadMesh();
				break;
			case Qt::Key_T: // Ctrl-T
				if(QApplication::keyboardModifiers() & Qt::ControlModifier) loadTexture();
				break;
			default:
				break;
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

void MainWindow::newScene() {
	//if(scene != nullptr) delete scene;
	//scene = new cs::std_scene();
	consoleWidget->append("Created new scene");
}

void MainWindow::openScene() {
	//if(scene != nullptr) delete scene;
	//scene = new cs::std_scene();
	//scene->load(path);
	consoleWidget->append("Loaded scene path");
}

void MainWindow::saveScene() {
	//if(scene == nullptr) return;
	//scene->save(path);
	consoleWidget->append("Saved scene to path");
}

void MainWindow::loadMesh() {
	//open filepicker
	//mesh* temp = new mesh(path, filename(path));
	//scene->append_node(temp);
	//to_delete.push_back(temp);
	consoleWidget->append("Imported mesh filename(path) from path");
}

void MainWindow::loadTexture() {
	//open filepicker
	//texture2D* temp = new texture2D(path, filename(path));
	//textures.push_back(temp);
	//to_delete.push_back(temp);
	consoleWidget->append("Imported texture filename(path) from path");
}

// private
void MainWindow::connectActions() {
	// Connect "New" action
	QAction *action = findChild<QAction*>("newAction");
	connect(action, &QAction::triggered, this, &MainWindow::newScene);

	// Connect "Save" action
	action = findChild<QAction*>("saveAction");
	connect(action, &QAction::triggered, this, &MainWindow::saveScene);

	// Connect "Open" action
	action = findChild<QAction*>("openAction");
	connect(action, &QAction::triggered, this, &MainWindow::openScene);

	// Connect "Close" action
	action = findChild<QAction*>("closeAction");
	connect(action, &QAction::triggered, this, &MainWindow::close);

	// Connect "Mesh" action
	action = findChild<QAction*>("meshAction");
	connect(action, &QAction::triggered, this, &MainWindow::loadMesh);

	// Connect "Mesh" action
	action = findChild<QAction*>("textureAction");
	connect(action, &QAction::triggered, this, &MainWindow::loadTexture);
}