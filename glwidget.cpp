#include "glwidget.h"
#include <cmath>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(10);
}


void GLWidget::initializeGL()
{
    glClearColor(0.9, 0.9, 0.9, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    glRotatef(1, 0, 0, 1);
    glRotatef(2, 0, 1, 0);
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-0.5, -0.5, 0);
        glColor3f(0, 1, 0);
        glVertex3f( 0.5, -0.5, 0);
        glColor3f(0, 0, 1);
        glVertex3f( 0.0,  0.5, 0);
    glEnd();

    const float PI = 3.141592f;
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles
    GLfloat radius = 0.3f;
    int gradation = 20;

    glColor3f(0.5, 0, 1);
    for (alpha = 0.0; alpha < PI; alpha += PI/gradation)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (beta = 0.0; beta < 2.01*PI; beta += PI/gradation)
        {
            x = radius*cos(beta)*sin(alpha);
            y = radius*sin(beta)*sin(alpha);
            z = radius*cos(alpha);
            glVertex3f(x, y, z);
            x = radius*cos(beta)*sin(alpha + PI/gradation);
            y = radius*sin(beta)*sin(alpha + PI/gradation);
            z = radius*cos(alpha + PI/gradation);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

