#include "PreviewWidget.h"

PreviewWidget::PreviewWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::DepthBuffer | QGL::DirectRendering), parent) {
	context = new QGLContext(QGLFormat());
	context->create();
	makeCurrent();
}

PreviewWidget::~PreviewWidget() {
	delete context;
}