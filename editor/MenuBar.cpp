#include "MenuBar.h"
using namespace ce;

MenuBar::MenuBar(MainWindow *window, QWidget *parent) : QMenuBar(parent) {

		// Add "File" menu
		QMenu *fileMenu = addMenu("File");
			// Add "New" option
			QAction *newAction = fileMenu->addAction("New");
			newAction->setShortcuts(QKeySequence::New);
			newAction->setObjectName("newAction");
			// Add "Open" option
			QAction *openAction = fileMenu->addAction("Open");
			openAction->setShortcuts(QKeySequence::Open);
			openAction->setObjectName("openAction");
			// Add "Save" option
			QAction *saveAction = fileMenu->addAction("Save");
			saveAction->setShortcuts(QKeySequence::Save);
			saveAction->setObjectName("saveAction");
			// Add "Close" option
			QAction *closeAction = fileMenu->addAction("Close");
			closeAction->setShortcuts(QKeySequence::Quit);
			closeAction->setObjectName("closeAction");

		// Add "Import" menu
		QMenu *importMenu = addMenu("Import");
			// Add "Mesh" option
			QAction *meshAction = importMenu->addAction("Mesh");
			meshAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
			meshAction->setObjectName("meshAction");
			// Add "Texture" option
			QAction *textureAction = importMenu->addAction("Texture");
			textureAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
			textureAction->setObjectName("textureAction");

		// Add "Add" menu
		QMenu *addMenu = this->addMenu("Add");
			// Add "Point light" option
			QAction *plAction = addMenu->addAction("Point light");
			plAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
			plAction->setObjectName("plAction");
			// Add "Directional light" option
			QAction *dlAction = addMenu->addAction("Directional light");
			dlAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
			dlAction->setObjectName("dlAction");
			// Add "Group node" option
			QAction *gnAction = addMenu->addAction("Group node");
			gnAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
			gnAction->setObjectName("gnAction");

		// Add "Help" menu
		QMenu *helpMenu = this->addMenu("Help");
			// Add "Wiki" option
			QAction *wikiAction = helpMenu->addAction("Wiki");
			wikiAction->setShortcut(QKeySequence(Qt::Key_F11));
			wikiAction->setObjectName("wikiAction");
			// Add "About" option
			QAction *aboutAction = helpMenu->addAction("About");
			aboutAction->setShortcut(QKeySequence(Qt::Key_F10));
			aboutAction->setObjectName("aboutAction");

}

MenuBar::~MenuBar() {}