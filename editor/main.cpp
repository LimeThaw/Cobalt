#include <QApplication>
#include "MainWindow.h"

#include "process_qss.cpp"

using namespace ce;

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWindow window;

	// Style variables
	map<string, string> svars;
	svars.insert(pair<string, string>("color-0", "#29506D"));
	svars.insert(pair<string, string>("color-1", "#718EA4"));
	svars.insert(pair<string, string>("color-2", "#496D89"));
	svars.insert(pair<string, string>("color-3", "#123652"));
	svars.insert(pair<string, string>("color-4", "#042037"));
	map<string, string> gray;
	gray.insert(pair<string, string>("color-0", "#60676D"));
	gray.insert(pair<string, string>("color-1", "#A1A4A7"));
	gray.insert(pair<string, string>("color-2", "#7C8287"));
	gray.insert(pair<string, string>("light-color-2", "#8C9297"));
	gray.insert(pair<string, string>("color-3", "#475056"));
	gray.insert(pair<string, string>("color-4", "#2C333A"));

	QFile styleSheet("stylesheet.css");
	styleSheet.open(QFile::ReadOnly);
	app.setStyleSheet(process_qss(gray, styleSheet.readAll().data()).c_str());

	window.show();
	return app.exec();
}
