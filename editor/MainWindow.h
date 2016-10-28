#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QListWidget>
#include <QGLWidget>
#include <QTextEdit>
#include <QLayout>

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
			void contextMenuEvent(QContextMenuEvent *e);

	};

}

#endif