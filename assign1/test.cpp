#include "stdio.h"
#include "GL/glut.h"

void render()
{
	glBegin(GL_TRIANGLES);
		glColor3f(  1.0, 0.0, 0.0);
		glVertex3f(-0.5, 0.0, 0.0);

		glColor3f(  0.0, 0.0, 1.0);
		glVertex3f( 0.0, 0.5, 0.0);

		glColor3f(  0.0, 1.0, 0.0);
		glVertex3f( 0.5, 0.0, 0.0);
	glEnd();

	glutSwapBuffers();

	return;
}

int main(int argc, char **argv)
{
        glutInit(&argc, argv);
        glutInitWindowPosition(100,100);
        glutInitWindowSize(500,500);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        glutCreateWindow("testwin");

        glutDisplayFunc(render);
	glutMainLoop();

	return 0;
}
