#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../engine/library/standard.h"

#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QListWidget>
#include <QGLWidget>
#include <QTextEdit>
#include <QLayout>
#include <QApplication>

#include "AboutWindow.h"
#include "MenuBar.h"
#include "ObjectBrowser.h"
#include "PropertiesWidget.h"

namespace ce {

	class MainWindow : public QMainWindow {
		Q_OBJECT

		public:
			explicit MainWindow(QWidget *parent = 0);
			~MainWindow();

			bool event(QEvent *event) Q_DECL_OVERRIDE;
			void contextMenuEvent(QContextMenuEvent *e) Q_DECL_OVERRIDE;

		public slots:
			void newScene();
			void openScene();
			void saveScene();
			void loadMesh();
			void loadTexture();
			void add();
			void openWiki();
			void showAbout();

		private:
			void connectActions();
			cs::std_scene *scene;
			QTextEdit *consoleWidget;
			QWidget *containerWidget;
			AboutWindow* awin;

	};

}

#endif