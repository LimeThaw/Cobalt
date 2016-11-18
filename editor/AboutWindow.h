// Created 2016-11-18 by LimeThaw

#include <QWindow>
#include <QLabel>

namespace ce {

class AboutWindow : public QMainWindow {
	public:
		AboutWindow() : QMainWindow() {
			resize(300, 400);
			//setFlags(Qt::Dialog);

			QWidget *mainWidget = new QWidget();
			setCentralWidget(mainWidget);
			QVBoxLayout *layout = new QVBoxLayout();
			mainWidget->setLayout(layout);

			QFile textFile("about.txt");
			if(textFile.open(QFile::ReadOnly)) {
				QLabel *label = new QLabel(this);
				label->setAlignment(Qt::AlignCenter);
				label->setText(textFile.readAll().data());
				layout->addWidget(label);
			}
		}


		bool event(QEvent *event) {

			if(event->type() == QEvent::KeyPress) {
				QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

				switch(keyEvent->key()) {
					case Qt::Key_Escape: // Esc
						this->close();
						break;
					default:
						break;
				}
			}

			return QMainWindow::event(event);
		}
};

}