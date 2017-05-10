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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void GLWidget::paintGL()
{
    // clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // setup perspective matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspectiveGL(90.0,(float)width()/height(),0.1,10.0);

    // setup model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //quad

    GLubyte checkerboard[8*8] =
       {255,0,255,0,255,0,255,0,
        0,255,0,255,0,255,0,255,
        255,0,255,0,255,0,255,0,
        0,255,0,255,0,255,0,255,
        255,0,255,0,255,0,255,0,
        0,255,0,255,0,255,0,255,
        255,0,255,0,255,0,255,0,
        0,255,0,255,0,255,0,255};

    int checkImageHeight = 64;
    int checkImageWidth = 64;
    GLubyte checkImage[checkImageHeight][checkImageWidth][4];

    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
       for (j = 0; j < checkImageWidth; j++) {
          c = ((((i&0x8)==0)^((j&0x8))==0))*255;
          checkImage[i][j][0] = (GLubyte) c;
          checkImage[i][j][1] = (GLubyte) c;
          checkImage[i][j][2] = (GLubyte) c;
          checkImage[i][j][3] = (GLubyte) 255;
       }
    }

    int checkImageColorHeight = 2;
    int checkImageColorWidth = 2;
    GLubyte checkImageColor[checkImageColorHeight][checkImageColorWidth][4];

    checkImageColor[0][0][0] = (GLubyte) 255;
    checkImageColor[0][0][1] = (GLubyte) 0;
    checkImageColor[0][0][2] = (GLubyte) 0;
    checkImageColor[0][0][3] = (GLubyte) 255;

    checkImageColor[0][1][0] = (GLubyte) 0;
    checkImageColor[0][1][1] = (GLubyte) 255;
    checkImageColor[0][1][2] = (GLubyte) 0;
    checkImageColor[0][1][3] = (GLubyte) 255;

    checkImageColor[1][0][0] = (GLubyte) 0;
    checkImageColor[1][0][1] = (GLubyte) 0;
    checkImageColor[1][0][2] = (GLubyte) 255;
    checkImageColor[1][0][3] = (GLubyte) 255;

    checkImageColor[1][1][0] = (GLubyte) 255;
    checkImageColor[1][1][1] = (GLubyte) 0;
    checkImageColor[1][1][2] = (GLubyte) 255;
    checkImageColor[1][1][3] = (GLubyte) 255;

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D,    // 2D texture
                 0,                // level 0 (for mipmapping)
                 GL_RGBA,           // pixel format of texture
                 checkImageHeight,checkImageWidth,     // texture size
                 0,                // border size 0
                 GL_RGBA,           // pixel format of data supplied
                 GL_UNSIGNED_BYTE, // pixel storage type of data supplied
                 checkImage);           // pointer to data chunk

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    double x_0 = -1;
    double x_1 = 1;
    double x_2 = 1;
    double x_3 = -1;

    double y_0 = -1;
    double y_1 = -1;
    double y_2 = 1;
    double y_3 = 1;

    double z_0 = -3;
    double z_1 = -3;
    double z_2 = -3;
    double z_3 = -3;
    glBegin(GL_QUADS);
       glTexCoord2f(0,0);
       glVertex3d(x_0,y_0,z_0);
       glTexCoord2f(1,0);
       glVertex3d(x_1,y_1,z_1);
       glTexCoord2f(1,1);
       glVertex3d(x_2,y_2,z_2);
       glTexCoord2f(0,1);
       glVertex3d(x_3,y_3,z_3);
    glEnd();


    // define local rotated coordinate system
    static double angle=0.0; // rotation angle in degrees
    static const double omega=180.0; // rotation speed in degrees/s
    glTranslated(0.0,0.0,-2.0);
    glRotated(angle,0.0,1.0,0.0);

    // render triangles
/*
    glBegin(GL_TRIANGLES);
       glColor3f(0.0f,0.75f,0.0f);
       glVertex3d(-0.5,-0.5,0.0);
       glVertex3d(0.5,-0.5,0.0);
       glVertex3d(0.0,0.5,0.0);
    glEnd();
*/


    // angle delta equals time delta times omega
    double fps = 60;
    double dt=1.0/fps;
    angle+=dt*omega;
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

