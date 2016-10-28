#include "MenuBar.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent) {

		// Add "File" menu
		QMenu *fileMenu = addMenu("File");
			// Add "Open" option
			QAction *openAction = fileMenu->addAction("Open");
			// Add "Save" option
			QAction *saveAction = fileMenu->addAction("Save");
			// Add "Close" option
			QAction *closeAction = fileMenu->addAction("Close");

		// Add "Import" menu
		QMenu *importMenu = addMenu("Import");
			// Add "Mesh" option
			QAction *meshAction = importMenu->addAction("Mesh");
			// Add "Texture" option
			QAction *textureAction = importMenu->addAction("Texture");

		// Add "Add" menu
		QMenu *addMenu = this->addMenu("Add");
			// Add "Point light" option
			QAction *plAction = addMenu->addAction("Point light");
			// Add "Directional light" option
			QAction *dlAction = addMenu->addAction("Directional light");
			// Add "Group node" option
			QAction *gnAction = addMenu->addAction("Group node");

		// Add "Help" menu
		QMenu *helpMenu = this->addMenu("Help");
			// Add "Wiki" option
			QAction *wikiAction = helpMenu->addAction("Wiki");
			// Add "About" option
			QAction *aboutAction = helpMenu->addAction("About");

}

MenuBar::~MenuBar() {}