#include <QGLWidget>
#include <QGLContext>

class PreviewWidget : public QGLWidget {
	public:
	    PreviewWidget(QWidget *parent = 0);
		~PreviewWidget();

	private:
		QGLContext *context;
};