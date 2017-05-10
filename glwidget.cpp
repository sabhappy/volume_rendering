#include "glwidget.h"
#include <cmath>
#include <iostream>
#include "stb_image.h"

int iTex = 3;

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



    //texture
    if(iTex == 0){
        int width, height, numComponents;
        std::string filename = "/Users/sabrinaheppner/Documents/workspace_Qt/test/side1.png";
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

        if(data == NULL)
                std::cerr << "Unable to load texture: " << filename<< std::endl;
        else
                printf("%s",data);
       // else
       //     std::cerr << "Texture: " << filename<< ", width" << width<< ", height "<<height<< std::endl;

       glTexImage2D(GL_TEXTURE_2D,    // 2D texture
                         0,                // level 0 (for mipmapping)
                         GL_RGBA,           // pixel format of texture
                         width,height,     // texture size
                         0,                // border size 0
                         GL_RGBA,           // pixel format of data supplied
                         GL_UNSIGNED_BYTE, // pixel storage type of data supplied
                         data);           // pointer to data chunk
    }

    if(iTex == 1){
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
        glTexImage2D(GL_TEXTURE_2D,    // 2D texture
                     0,                // level 0 (for mipmapping)
                     GL_RGBA,           // pixel format of texture
                     checkImageHeight,checkImageWidth,     // texture size
                     0,                // border size 0
                     GL_RGBA,           // pixel format of data supplied
                     GL_UNSIGNED_BYTE, // pixel storage type of data supplied
                     checkImage);           // pointer to data chunk
    }


    if(iTex == 2){
        int checkImageColorHeight = 2;
        int checkImageColorWidth = 2;
        GLubyte checkImageColor[checkImageColorHeight][checkImageColorWidth][4];

        checkImageColor[0][0][0] = (GLubyte) 255;
        checkImageColor[0][0][1] = (GLubyte) 0;
        checkImageColor[0][0][2] = (GLubyte) 0;
        checkImageColor[0][0][3] = (GLubyte) 100;

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


        glTexImage2D(GL_TEXTURE_2D,    // 2D texture
                     0,                // level 0 (for mipmapping)
                     GL_RGBA,           // pixel format of texture
                     checkImageColorHeight,checkImageColorWidth,     // texture size
                     0,                // border size 0
                     GL_RGBA,           // pixel format of data supplied
                     GL_UNSIGNED_BYTE, // pixel storage type of data supplied
                     checkImageColor);           // pointer to data chunk
    }

    if(iTex == 3){
            int width, height, numComponents;
            std::string filename = "/Users/sabrinaheppner/Documents/workspace_Qt/textures/billboardblueflowers.png";
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

            if(data == NULL)
                    std::cerr << "Unable to load texture: " << filename<< std::endl;
           // else
           //     std::cerr << "Texture: " << filename<< ", width" << width<< ", height "<<height<< std::endl;

           glTexImage2D(GL_TEXTURE_2D,    // 2D texture
                             0,                // level 0 (for mipmapping)
                             GL_RGBA,           // pixel format of texture
                             width,height,     // texture size
                             0,                // border size 0
                             GL_RGBA,           // pixel format of data supplied
                             GL_UNSIGNED_BYTE, // pixel storage type of data supplied
                             data);           // pointer to data chunk
        }


   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
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
    perspectiveGL(90.0,1,0.1,10.0);

    // setup model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



    glEnable(GL_TEXTURE_2D);

    //quad
    double p0[] = {-1, -1, -3};
    double p1[] = { 1, -1, -3};
    double p2[] = { 1, 1, -3};
    double p3[] = {-1, 1, -3};
    glBegin(GL_QUADS);
       glTexCoord2f(0,1);
       glVertex3d(p0[0],p0[1],p0[2]);
       glTexCoord2f(1,1);
       glVertex3d(p1[0],p1[1],p1[2]);
       glTexCoord2f(1,0);
       glVertex3d(p2[0],p2[1],p2[2]);
       glTexCoord2f(0,0);
       glVertex3d(p3[0],p3[1],p3[2]);
    glEnd();


    // define local rotated coordinate system
    static double angle=0.0; // rotation angle in degrees
    static const double omega=180.0; // rotation speed in degrees/s
    glTranslated(0.0,0.0,-2.0);
    glRotated(angle,0.0,1.0,0.0);

    // angle delta equals time delta times omega
    double fps = 60;
    double dt=1.0/fps;
    angle+=dt*omega;
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

