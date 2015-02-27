#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include "Shaderloader.cpp"

void renderFunc(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
		glVertex3f(-2,-2,-5.0);
		glVertex3f(2,0.0,-5.0);
		glVertex3f(0.0,2,-5.0);
	glEnd();

    glutSwapBuffers();
}

void changeSize(int w, int h) {
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv){

    glutInit(&argc, &argv);
    glewExperimental = GL_TRUE;
    glutInitContextVersion (4, 0);
    glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitWindowSize(1280, 720);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGLTest");

    glutDisplayFunc(renderFunc);
    glutReshapeFunc(changeSize);

    glutMainLoop();

}
