/*
 CSCI 480 Computer Graphics
 Assignment 1: Height Fields
 C++ starter code
 */

#include <stdlib.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <pic.h>
#include <string.h>
#include <math.h>

int g_iMenuId;

int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;

/* state of the world */
float g_vLandRotate[3] = {0.0, 0.0, 0.0};
float g_vLandTranslate[3] = {0.0, 0.0, 0.0};
float g_vLandScale[3] = {1.0, 1.0, 1.0};

/* see <your pic directory>/pic.h for type Pic */
Pic * g_pHeightData;

int _value=150;

int drag_x_origin;
int drag_y_origin;
int dragging = 0;
int zoom = 0;

float _angle = 30.0f;
float camera_angle_v = 0.0f;
float camera_angle_h = 0.0f;
float zoom_in = 0.0f;
float zoom_out = 0.0f;

float _xposition=0.0f;
float _yposition=0.0f;

unsigned char mode='t';


/* Write a screenshot to the specified filename */
void saveScreenshot (int val)
{
    
    char buffer[20], buffer2[20];
        strcpy(buffer, ".jpg");
        sprintf(buffer2, "%d", val);
    char *filename = strcat(buffer2, buffer);
    
    int i,j;
    Pic *in = NULL;
    
    
    if (filename == NULL)
        return;
    
    /* Allocate a picture buffer */
    in = pic_alloc(640, 480, 3, NULL);
    
    //printf("File to save to: %s\n", filename);
    
    for (i=479; i>=0; i--) {
        glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
                     &in->pix[i*in->nx*in->bpp]);
    }
    
    if (jpeg_write(filename, in)){
        //printf("File saved Successfully\n");
    }
    else
        printf("Error in Saving\n");
    
    pic_free(in);
    if(_value<300)
    glutTimerFunc(66, saveScreenshot, _value++);
}

void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
   // glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 640.0/480.0, 0.01, 10.0);
}

void menufunc(int value)
{
    switch (value)
    {
        case 0:
            exit(0);
            break;
    }
}

void doIdle()
{
    /* do some stuff... */
    
    /* make the screen update */
    glutPostRedisplay();
}

/* converts mouse drags into information about
 rotation/translation/scaling */
void mousedrag(int x, int y)
{
    int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
    
    
    switch (g_ControlState)
    {
        case TRANSLATE:
            if (g_iLeftMouseButton)
            {
                g_vLandTranslate[0] += vMouseDelta[0]*0.01;
                g_vLandTranslate[1] -= vMouseDelta[1]*0.01;
                
                if (abs(x - (int)drag_x_origin) > 10) {
                _yposition = x-drag_x_origin-0.1;
                }
                
                else if (abs(y - (int)drag_y_origin) > 10){
                _xposition = y-drag_y_origin-0.1;
                }
            }
            if (g_iMiddleMouseButton)
            {
                g_vLandTranslate[2] += vMouseDelta[1]*0.01;
            }
            break;
        case ROTATE:
            if (g_iLeftMouseButton)
            {
                g_vLandRotate[0] += vMouseDelta[1];
                g_vLandRotate[1] += vMouseDelta[0];
                
                camera_angle_v += (y - drag_y_origin)*0.3;
                camera_angle_h += (x - drag_x_origin)*0.3;
                drag_x_origin = x;
                drag_y_origin = y;
                
            }
            if (g_iMiddleMouseButton)
            {
                g_vLandRotate[2] += vMouseDelta[1];
                
                camera_angle_v -= (y - drag_y_origin)*0.3;
                camera_angle_h -= (x - drag_x_origin)*0.3;
                drag_x_origin = x;
                drag_y_origin = y;
                
            }
            break;
        case SCALE:
            if (g_iLeftMouseButton)
            {
                g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
                g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
                zoom_in +=0.1;
            }
            if (g_iMiddleMouseButton)
            {
                g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
                zoom_in -=0.1;
                
                
                
            }
            break;
    }
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
    
}

void mousebutton(int button, int state, int x, int y)
{
    
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            g_iLeftMouseButton = (state==GLUT_DOWN);
            dragging=1;
            drag_x_origin = x;
            drag_y_origin = y;
            break;
        case GLUT_MIDDLE_BUTTON:
            g_iMiddleMouseButton = (state==GLUT_DOWN);
            break;
        case GLUT_RIGHT_BUTTON:
            g_iRightMouseButton = (state==GLUT_DOWN);
            break;
            
    }
    
    switch(glutGetModifiers())
    {
        case GLUT_ACTIVE_CTRL:
            g_ControlState = TRANSLATE;
            break;
        case GLUT_ACTIVE_SHIFT:
            g_ControlState = SCALE;
            break;
        default:
            g_ControlState = ROTATE;
            break;
    }
    
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
}

void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    
    
    glLoadIdentity();
    gluPerspective(45.0, (double)(w/h), 0.01, 1000.0);
}

void handleKeypress(unsigned char key, int x, int y)
{
    switch(key) {
        case 'p': mode='p';
            break;
        case 'l': mode='l';
            break;
        case 't': mode='t';
            break;
        case 27: exit(0);
    }
    
}

void update(int value)
{
    _angle +=2.0f;
    if(_angle>360)
    {
        _angle-=360;
    }
    
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

void display()
 {
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
     gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
     
     
     //TRANSLATION - Move object on X or Y axis using mouse drag + CTRL key
     glTranslatef(g_vLandTranslate[0], g_vLandTranslate[1],0.0f);
     
     //ROTATION - Rotate image along X and Y plane using mouse drag
     glRotated(g_vLandRotate[0], 1.0, 0.0, 0.0);
     glRotated(g_vLandRotate[1], 0.0, 1.0, 0.0);
     glRotated(g_vLandRotate[2], 0.0, 0.0, 1.0);
     
     
     //ZOOM - Zoom in or zoom out using mouse drag + SHIFT key
     glScalef(g_vLandScale[1]+g_vLandScale[0],g_vLandScale[1]+g_vLandScale[0],g_vLandScale[1]+g_vLandScale[0]);



    glPushMatrix();
     glBegin(GL_LINES);
    glTranslatef(-1.0,-1.0,-1.0);
     glColor3f(1.0, 0.0, 0.0);
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(1.0, 0.0, 0.0);
     
     glColor3f(0.0, 1.0, 0.0);
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(0.0, 1.0, 0.0);
     
     glColor3f(0.0, 0.0, 1.0);
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(0.0, 0.0, 1.0);
        // glTranslatef(1.0,1.0,1.0);
     glEnd();
     glPopMatrix();
     
     
     glPushMatrix();
    
     

     glTranslatef(0.5,0.5,0.0);
    
     glRotatef(-45.0,0.0, 0.0, 1.0);
     glTranslatef(-0.5,-0.5,0.0);
     
     glTranslatef(0.5,0.5,0.0);
     
     
    // glScalef(0.5,1.0,2.0);
     
     glColor3f(1.0, 1.0, 1.0);
     glutWireCube(0.5);
     glPopMatrix();
     
     glFlush();
     glutSwapBuffers();

 }


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    //glFrustum(-100.0, 100.0, -100.0, 100.0, 0.01, 100.0);
   gluPerspective(25.0, (GLfloat)w/(GLfloat)h, 0.1, 400.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    //Initial Setup
    glOrtho(0.0, g_pHeightData->nx, g_pHeightData->ny, 0.0, 0.01, 1000.0);
    glTranslatef(g_pHeightData->nx/4, g_pHeightData->ny/4, g_pHeightData->ny/3);
    //glRotatef(-90.0f,0.0f,0.0f,1.0f);
    
    //TRANSLATION - Move object on X or Y axis using mouse drag + CTRL key
    glTranslatef(_yposition, _xposition,0.0f);
    
    //ZOOM - Zoom in or zoom out using mouse drag + SHIFT key
    glScalef(0.5f+zoom_in,0.5f+zoom_in,0.5f);
    //glTranslatef(zoom_in*100,zoom_in*100,zoom_in*10);

    //ROTATION - Rotate image along X and Y plane using mouse drag
    glRotated(camera_angle_v, 1.0, 0.0, 0.0);
    glRotated(camera_angle_h, 0.0, 0.0, 1.0);
    
    
    //To display the image using points
    if(mode=='p' && g_pHeightData->bpp==1)
    {
        
        glBegin(GL_POINTS);
        for(int j=0;j<g_pHeightData->ny-1;j++)
        {
            for(int i=0;i<g_pHeightData->nx-1;i++)
            {
                unsigned int heightVal1 = PIC_PIXEL(g_pHeightData,i,j,0);
                glColor3f( 0.0f,(float)(heightVal1/255.0),1.0-(float)(heightVal1/255.0));
                glVertex3f(i, j,heightVal1);
            }
        }
        glEnd();
    }
    
    if(mode=='p' && g_pHeightData->bpp==3)
    {
        
        glBegin(GL_POINTS);
        for(int j=0;j<g_pHeightData->ny-1;j++)
        {
            for(int i=0;i<g_pHeightData->nx-1;i++)
            {
                unsigned int heightVal01 = PIC_PIXEL(g_pHeightData,i,j,0);
                unsigned int heightVal11 = PIC_PIXEL(g_pHeightData,i,j,1);
                unsigned int heightVal21 = PIC_PIXEL(g_pHeightData,i,j,2);
                glColor3f( (float)(heightVal01/255.0),(float)(heightVal11/255.0),(float)(heightVal21/255.0));
                glVertex3f(i, j,pow((heightVal01*heightVal11*heightVal21), 1/3.0));            }
        }
        glEnd();
    }
    
    //To display the image using lines
    if(mode=='l' && g_pHeightData->bpp==1)
    {
        
        glBegin(GL_LINES);
        for(int j=0;j<g_pHeightData->ny-1;j++)
        {
            for(int i=0;i<g_pHeightData->nx-1;i++)
            {
                unsigned int heightVal1 = PIC_PIXEL(g_pHeightData,i,j,0);
                unsigned int heightVal2 = PIC_PIXEL(g_pHeightData,i+1,j,0);
                unsigned int heightVal3 = PIC_PIXEL(g_pHeightData,i,j+1,0);
                
                
                glColor3f( 0.0f,(float)(heightVal1/255.0),1.0-(float)(heightVal1/255.0));
                glVertex3f(i, j,heightVal1);
                glColor3f( 0.0f,(float)(heightVal2/255.0),1.0-(float)(heightVal2/255.0));
                glVertex3f(i+1, j,heightVal2);
                glColor3f( 0.0f,(float)(heightVal2/255.0),1.0-(float)(heightVal2/255.0));
                glVertex3f(i+1, j,heightVal2);
                glColor3f( 0.0f,(float)(heightVal3/255.0),1.0-(float)(heightVal3/255.0));
                glVertex3f(i, j+1,heightVal3);
            }
        }
        glEnd();
    }
    
    //To display the image using lines
    if(mode=='l' && g_pHeightData->bpp==3)
    {
        
        glBegin(GL_LINES);
        for(int j=0;j<g_pHeightData->ny-1;j++)
        {
            for(int i=0;i<g_pHeightData->nx-1;i++)
            {
                unsigned int heightVal01 = PIC_PIXEL(g_pHeightData,i,j,0);
                unsigned int heightVal02 = PIC_PIXEL(g_pHeightData,i+1,j,0);
                unsigned int heightVal03 = PIC_PIXEL(g_pHeightData,i,j+1,0);
                
                unsigned int heightVal11 = PIC_PIXEL(g_pHeightData,i,j,1);
                unsigned int heightVal12 = PIC_PIXEL(g_pHeightData,i+1,j,1);
                unsigned int heightVal13 = PIC_PIXEL(g_pHeightData,i,j+1,1);
                
                unsigned int heightVal21 = PIC_PIXEL(g_pHeightData,i,j,2);
                unsigned int heightVal22 = PIC_PIXEL(g_pHeightData,i+1,j,2);
                unsigned int heightVal23 = PIC_PIXEL(g_pHeightData,i,j+1,2);
                
                
                glColor3f( (float)(heightVal01/255.0),(float)(heightVal11/255.0),(float)(heightVal21/255.0));
                glVertex3f(i, j,pow((heightVal01*heightVal11*heightVal21), 1/3.0));
                glColor3f( (float)(heightVal02/255.0),(float)(heightVal12/255.0),(float)(heightVal22/255.0));
                glVertex3f(i+1, j,pow((heightVal02*heightVal12*heightVal22), 1/3.0));
                glColor3f( (float)(heightVal02/255.0),(float)(heightVal12/255.0),(float)(heightVal22/255.0));
                glVertex3f(i+1, j,pow((heightVal02*heightVal12*heightVal22), 1/3.0));
                glColor3f( (float)(heightVal03/255.0),(float)(heightVal13/255.0),(float)(heightVal23/255.0));
                glVertex3f(i, j+1,pow((heightVal03*heightVal13*heightVal23), 1/3.0));
            }
        }
        glEnd();
    }
    
    
    //To display the image using triangles (default)
    if (mode=='t' && g_pHeightData->bpp==1)
    {
        glBegin(GL_TRIANGLES);
        for(int j=0;j<g_pHeightData->ny-1;j++)
        {
            for(int i=0;i<g_pHeightData->nx-1;i++)
            {
                
                unsigned int heightVal1 = PIC_PIXEL(g_pHeightData,i,j,0);
                unsigned int heightVal2 = PIC_PIXEL(g_pHeightData,i+1,j,0);
                unsigned int heightVal3 = PIC_PIXEL(g_pHeightData,i,j+1,0);
                unsigned int heightVal4 = PIC_PIXEL(g_pHeightData,i+1,j+1,0);
                
                glColor3f( 0.0f,(float)(heightVal1/255.0),1.0-(float)(heightVal1/255.0));
                glVertex3f(i, j,heightVal1);
                glColor3f( 0.0f,(float)(heightVal3/255.0),1.0-(float)(heightVal3/255.0));
                glVertex3f(i, j+1,heightVal3);
                glColor3f( 0.0f,(float)(heightVal4/255.0),1.0-(float)(heightVal4/255.0));
                glVertex3f(i+1, j+1,heightVal4);
                
                
                glColor3f( 0.0f,(float)(heightVal1/255.0),1.0-(float)(heightVal1/255.0));
                glVertex3f(i, j,heightVal1);
                glColor3f( 0.0f,(float)(heightVal4/255.0),1.0-(float)(heightVal4/255.0));
                glVertex3f(i+1, j+1,heightVal4);
                glColor3f( 0.0f,(float)(heightVal2/255.0),1.0-(float)(heightVal2/255.0));
                glVertex3f(i+1, j,heightVal2);
                
                
            }
            
        }
        glEnd();
    }
    
    
    //To display the image using triangles (default)
    if (mode=='t' && g_pHeightData->bpp==3)
    {
        int chan=0;
        glBegin(GL_TRIANGLES);
        for(int j=0;j<g_pHeightData->ny-1;j++)
        {
            for(int i=0;i<g_pHeightData->nx-1;i++)
            {
                
                unsigned int heightVal01 = PIC_PIXEL(g_pHeightData,i,j,0);
                unsigned int heightVal02 = PIC_PIXEL(g_pHeightData,i+1,j,0);
                unsigned int heightVal03 = PIC_PIXEL(g_pHeightData,i,j+1,0);
                unsigned int heightVal04 = PIC_PIXEL(g_pHeightData,i+1,j+1,0);
                
                unsigned int heightVal11 = PIC_PIXEL(g_pHeightData,i,j,1);
                unsigned int heightVal12 = PIC_PIXEL(g_pHeightData,i+1,j,1);
                unsigned int heightVal13 = PIC_PIXEL(g_pHeightData,i,j+1,1);
                unsigned int heightVal14 = PIC_PIXEL(g_pHeightData,i+1,j+1,1);
                
                unsigned int heightVal21 = PIC_PIXEL(g_pHeightData,i,j,2);
                unsigned int heightVal22 = PIC_PIXEL(g_pHeightData,i+1,j,2);
                unsigned int heightVal23 = PIC_PIXEL(g_pHeightData,i,j+1,2);
                unsigned int heightVal24 = PIC_PIXEL(g_pHeightData,i+1,j+1,2);
                
                
                glColor3f( (float)(heightVal01/255.0),(float)(heightVal11/255.0),(float)(heightVal21/255.0));
                glVertex3f(i, j,pow((heightVal01*heightVal11*heightVal21), 1/3.0));
                glColor3f( (float)(heightVal03/255.0),(float)(heightVal13/255.0),(float)(heightVal23/255.0));
                glVertex3f(i, j+1,pow((heightVal03*heightVal13*heightVal23), 1/3.0));
                glColor3f( (float)(heightVal04/255.0),(float)(heightVal14/255.0),(float)(heightVal24/255.0));
                glVertex3f(i+1, j+1,pow((heightVal04*heightVal14*heightVal24), 1/3.0));
                
                
                glColor3f( (float)(heightVal01/255.0),(float)(heightVal11/255.0),(float)(heightVal21/255.0));
                glVertex3f(i, j,pow((heightVal01*heightVal11*heightVal21), 1/3.0));
                glColor3f( (float)(heightVal04/255.0),(float)(heightVal14/255.0),(float)(heightVal24/255.0));
                glVertex3f(i+1, j+1,pow((heightVal04*heightVal14*heightVal24), 1/3.0));
                glColor3f( (float)(heightVal02/255.0),(float)(heightVal12/255.0),(float)(heightVal22/255.0));
                glVertex3f(i+1, j,pow((heightVal02*heightVal12*heightVal22), 1/3.0));
                
                
            }
            
        }
        glEnd();
    }
    glPopMatrix();
    
    glutSwapBuffers();
}*/


int main (int argc, char ** argv)
{
    if (argc<2)
    {
        printf ("usage: %s heightfield.jpg\n", argv[0]);
        exit(1);
    }
    
    g_pHeightData = jpeg_read(argv[1], NULL);
    if (!g_pHeightData)
    {
        printf ("error reading %s.\n", argv[1]);
        exit(1);
    }
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    
    //Create the window
    glutCreateWindow("Height Field Image");
    myinit();
    //glClearDepth(1.0);
    
    //Display the window
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    
    // allow the user to quit using the right mouse button menu
    /*g_iMenuId = glutCreateMenu(menufunc);
    glutSetMenu(g_iMenuId);
    glutAddMenuEntry("Quit",0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);*/

    glutIdleFunc(doIdle);
    
    // callback for mouse drags
    glutMotionFunc(mousedrag);
    // callback for idle mouse movement
    glutPassiveMotionFunc(mouseidle);
    // callback for mouse button changes
    glutMouseFunc(mousebutton);
    

    //saveScreenshot(150);
    
    glutMainLoop();
    return(0);
}
