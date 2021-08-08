#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <bits/stdc++.h>
#include "BmpLoader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

unsigned int ID[1000];
int direction = 1, seesawDir = 1, radius = 20,xDir = 1, yDir = 1, planeDir = 1,shcpt=1;
const int L = 10,cpt = 9,ntheta = 20,nt = 40;	//number of slices along x-direction
const double PI = 3.141592654;
GLboolean spinFlag = false, swingFlag = false, rideOneFlag = false, rideTwoFlag = false, seesawFlag = false;
double angle = 0.0, speed = 1, swingAngle = 0.0,swingSpeed = 1, thresAngle = 30.0, rideOneAng = 0.0, rideOneSpeed = 1, rideTwoAng = 0.0, rideTwoSpeed = 1;
double seesawAngle = 0.0,seesawSpeed = 0.9, seesawThresAngle = 30.0,rotAngle = 0, planeSpeed = 5,planeAngle = 0,unAng = 0;
GLboolean cableFlag = false, dropFlag = false, planeFlag = false,planeFlag1 = false,unFlag = false;
double cableTx = -265, thresCableTx = 265, dropTy = 16;
double eyeX = 0, eyeY = 3, eyeZ = 336, dx =0, dy =0, dz =-1;
GLboolean spotLight = false, ambL0 = false,diffL0 = false, specL0 = false;
GLboolean Light1 = false, ambL1 = false,diffL1 = false, specL1 = false;
GLboolean Light2 = false, ambL2 = false,diffL2 = false, specL2 = false;
GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 }, light_specular[] = { 0.5,0.5,0.5, 1.0 };
GLfloat light_ambient0[]  = { 0.5, 0.5, 0.5, 1.0 }, light_diffuse0[]  = { 1.0, 1.0, 1.0, 1.0 };  //blue spotlight1
GLfloat light_ambient1[]  = { 0.5, 0.5, 0.5, 1.0 }, light_diffuse1[]  = { 1.0, 1.0, 1.0, 1.0 };  //white light1
GLfloat light_ambient2[]  = { 0.5, 0.5, 0.5, 1.0 }, light_diffuse2[]  = { 1.0, 1.0, 1.0, 1.0 };  //white light2

static GLfloat surface[8][3]
{
    {0.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
};

static GLubyte quadIndices[6][4] =
{
    {1,5,4,0},   //bottom clockwise
    {3,7,6,2},   //top
    {0,4,7,3},   //left clockwise
    {2,6,5,1},   //right
    {4,5,6,7},   //front
    {3,2,1,0},   //back clockwise
};

static void getNormal3p(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1; Uy = y2-y1; Uz = z2-z1;
    Vx = x3-x1; Vy = y3-y1; Vz = z3-z1;
    Nx = Uy*Vz - Uz*Vy; Ny = Uz*Vx - Ux*Vz; Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void Material(GLfloat rCol, GLfloat gCol, GLfloat bCol)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { rCol*0.5, gCol*0.5, bCol*0.5, 1.0 };
    GLfloat mat_diffuse[] = { rCol, gCol, bCol, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
}

void MyCube()
{
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(surface[quadIndices[i][0]][0], surface[quadIndices[i][0]][1], surface[quadIndices[i][0]][2],
                    surface[quadIndices[i][1]][0], surface[quadIndices[i][1]][1], surface[quadIndices[i][1]][2],
                    surface[quadIndices[i][2]][0], surface[quadIndices[i][2]][1], surface[quadIndices[i][2]][2]);

        glVertex3fv(&surface[quadIndices[i][0]][0]); glTexCoord2f(1,1);
        glVertex3fv(&surface[quadIndices[i][1]][0]); glTexCoord2f(1,0);
        glVertex3fv(&surface[quadIndices[i][2]][0]); glTexCoord2f(0,0);
        glVertex3fv(&surface[quadIndices[i][3]][0]); glTexCoord2f(0,1);
    }
    glEnd();
}

static GLfloat p_vertex[5][3] =
{
    {-4.0, -20.0, -4.0},
    {-4.0, -20.0, 4.0},
    {4.0, -20.0, 4.0},
    {4.0, -20.0, -4.0},
    {0.0, 0.0, 0.0},
};

static GLubyte p_triangleIndices[4][3] =
{
    {4, 1, 2}, //front
    {4, 2, 3}, //right
    {4, 3, 0}, //back
    {4, 0, 1}  //left
};

static GLubyte p_quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

void drawPyramid()
{
    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        getNormal3p(p_vertex[p_triangleIndices[i][0]][0], p_vertex[p_triangleIndices[i][0]][1], p_vertex[p_triangleIndices[i][0]][2],
                    p_vertex[p_triangleIndices[i][1]][0], p_vertex[p_triangleIndices[i][1]][1], p_vertex[p_triangleIndices[i][1]][2],
                    p_vertex[p_triangleIndices[i][2]][0], p_vertex[p_triangleIndices[i][2]][1], p_vertex[p_triangleIndices[i][2]][2]);

        glVertex3fv(&p_vertex[p_triangleIndices[i][0]][0]); glTexCoord2f(0,0);
        glVertex3fv(&p_vertex[p_triangleIndices[i][1]][0]); glTexCoord2f(1,0);
        glVertex3fv(&p_vertex[p_triangleIndices[i][2]][0]); glTexCoord2f(1,1);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i <1; i++)
    {
        getNormal3p(p_vertex[p_quadIndices[i][0]][0], p_vertex[p_quadIndices[i][0]][1], p_vertex[p_quadIndices[i][0]][2],
                    p_vertex[p_quadIndices[i][1]][0], p_vertex[p_quadIndices[i][1]][1], p_vertex[p_quadIndices[i][1]][2],
                    p_vertex[p_quadIndices[i][2]][0], p_vertex[p_quadIndices[i][2]][1], p_vertex[p_quadIndices[i][2]][2]);

        glVertex3fv(&p_vertex[p_quadIndices[i][0]][0]);  glTexCoord2f(1,1);
        glVertex3fv(&p_vertex[p_quadIndices[i][1]][0]);  glTexCoord2f(1,0);
        glVertex3fv(&p_vertex[p_quadIndices[i][2]][0]);  glTexCoord2f(0,0);
        glVertex3fv(&p_vertex[p_quadIndices[i][3]][0]);  glTexCoord2f(0,1);
    }
    glEnd();
}
void drawSkyBox()
{
    Material(1.0,1.0,1.0);
    //up anticlockwise
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-1000,475,-400);
	glTexCoord2f(1, 1); glVertex3f(-1000,475,1100);
	glTexCoord2f(1, 0); glVertex3f(1000,475,1100);
	glTexCoord2f(0, 0); glVertex3f(1000,475,-400);
	glEnd();

    //right anticlockwise
	glBindTexture(GL_TEXTURE_2D,2);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(1000,475,-400);
	glTexCoord2f(1, 1); glVertex3f(1000,475,1100);
	glTexCoord2f(1, 0); glVertex3f(1000,-125,1100);
	glTexCoord2f(0, 0); glVertex3f(1000,-125,-400);
	glEnd();

    //left clockwise
	glBindTexture(GL_TEXTURE_2D,3);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(-1000,-125,-400);
	glTexCoord2f(0, 0); glVertex3f(-1000,-125,1100);
	glTexCoord2f(0, 1); glVertex3f(-1000,475,1100);
	glTexCoord2f(1, 1); glVertex3f(-1000,475,-400);
	glEnd();

    //back clockwise
    glBindTexture(GL_TEXTURE_2D,4);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-1000,475,-400);
	glTexCoord2f(1, 1); glVertex3f(1000,475,-400);
	glTexCoord2f(1, 0); glVertex3f(1000,-125,-400);
	glTexCoord2f(0, 0); glVertex3f(-1000,-125,-400);
	glEnd();

    //front anticlockwise
	glBindTexture(GL_TEXTURE_2D,5);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(-1000,-125,1100);
	glTexCoord2f(0, 0); glVertex3f(1000,-125,1100);
	glTexCoord2f(0, 1); glVertex3f(1000,475,1100);
	glTexCoord2f(1, 1); glVertex3f(-1000,475,1100);
	glEnd();

    //down clockwise
	glBindTexture(GL_TEXTURE_2D,6);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(1000,-125,-400);
	glTexCoord2f(0, 0); glVertex3f(1000,-125,1100);
	glTexCoord2f(0, 1); glVertex3f(-1000,-125,1100);
	glTexCoord2f(1, 1); glVertex3f(-1000,-125,-400);
	glEnd();
 	glDisable(GL_TEXTURE_2D);
}

void drawGround()
{
    //ground
 	Material(1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,7);
	//back
	glPushMatrix();
	glTranslatef(-1000,-70,-400);
	glScalef(2000,20,170);
	MyCube();
	glPopMatrix();
	//front
	glPushMatrix();
	glTranslatef(-1000,-70,230);
	glScalef(2000,20,870);
	MyCube();
	glPopMatrix();
	//left
	glPushMatrix();
	glTranslatef(-1000,-70,-230);
	glScalef(720,20,460);
	MyCube();
	glPopMatrix();
	//right
	glPushMatrix();
	glTranslatef(280,-70,-230);
	glScalef(720,20,460);
	MyCube();
	glPopMatrix();
 	glDisable(GL_TEXTURE_2D);
}

void drawLake()
{
    Material(1,1,1);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix(); //back
    glTranslatef(-280,-70, -230);
    glScalef(560,20,0.5);
    MyCube();
    glPopMatrix();
    glPushMatrix();//left
    glTranslatef(-280,-70, -230);
    glScalef(0.5,20,460);
    MyCube();
    glPopMatrix();
    glPushMatrix();//right
    glTranslatef(280,-70, -230);
    glScalef(-0.5,20,460);
    MyCube();
    glPopMatrix();
    glPushMatrix();//front
    glTranslatef(-280,-70, 230);
    glScalef(560,20,-0.5);
    MyCube();
    glPopMatrix();
    //
    glPushMatrix(); //back
    glTranslatef(-200,-70, -150);
    glScalef(400,20,-0.5);
    MyCube();
    glPopMatrix();//left
    glPushMatrix();
    glTranslatef(-200,-70, -150);
    glScalef(-0.5,20,300);
    MyCube();
    glPopMatrix();
    glPushMatrix();//right
    glTranslatef(200,-70, -150);
    glScalef(0.5,20,300);
    MyCube();
    glPopMatrix();
    glPushMatrix();//front
    glTranslatef(-200,-70, 150);
    glScalef(400,20,0.5);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix(); //back
    glTranslatef(-279.5,-56, -229.5);
    glScalef(559,1,79);
    MyCube();
    glPopMatrix();
    glPushMatrix(); //left
    glTranslatef(-279.5,-56, -150.5);
    glScalef(79,1,301);
    MyCube();
    glPopMatrix();
    glPushMatrix(); //right
    glTranslatef(279.5,-56, -150.5);
    glScalef(-79,1,301);
    MyCube();
    glPopMatrix();
    glPushMatrix(); //front
    glTranslatef(-279.5,-56, 229.5);
    glScalef(559,1,-79);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void tree()
{
    GLUquadricObj *quad;
    quad = gluNewQuadric();

    //body
    glPushMatrix();
    Material(0.2,0.1,0.1);
    glRotatef(90,1,0,0);
    gluCylinder(quad, 2.5, 2.5, 50, 32, 32);
    glPopMatrix();
    //head
    glPushMatrix();
    Material(0,0.25,0);
    glutSolidSphere (2.5, 32, 32);
    glPopMatrix();

    //leaf
    float tx = 0, ty = -10, tz = 0, sx = 1, sy = 1, sz = 1;
    for(int i=0;i<10;i++)
    {
        glPushMatrix();
        Material(0, 0.25,0);
        glTranslatef(tx,ty,tz);
        glRotatef(-90,1,0,0);
        glScalef(sx, sy, sz);
        glutSolidCone(12-i,10,32,32);
        glPopMatrix();
        ty += 5;// sx -= 0.08;
    }
}
void drawTrees()
{
    float tx = -280, ty = 0, tz = -235;
    for(int btree = 1; btree <= 20; btree++)
    {
       glPushMatrix();
       glTranslatef(tx, ty, tz);
       tree();
       glPopMatrix();
       tx += 28;
       if(btree == 20) {tx = -287, ty = 0, tz = -230;}
    }
    for(int ltree = 1; ltree <= 20; ltree++)
    {
       tz += 23;
       glPushMatrix();
       glTranslatef(tx, ty, tz);
       tree();
       glPopMatrix();
       if(ltree == 20) {tx = 287, ty = 0, tz = -230;}
    }

    for(int rtree = 1; rtree <= 20; rtree++)
    {
       tz += 23;
       glPushMatrix();
       glTranslatef(tx, ty, tz);
       tree();
       glPopMatrix();
    }
}
void Gate()
{
    GLUquadricObj *quad;
    quad = gluNewQuadric();

    //Stand
    glPushMatrix();
    Material(0.7,0.45,0.28);
    glRotatef(90,1,0,0);
    gluCylinder(quad, 1.2, 1.2, 70, 32, 32);
    glPopMatrix();
}
void drawGate()
{
    glPushMatrix();
    glTranslatef(-20,20,147);
    Gate();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20,20,147);
    Gate();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    Material(0.53,0.42,0.32);
    glTranslatef(-25,20,145);
    glScalef(50,4,5);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,11);
    glPushMatrix();
    Material(1,1,1);
    glTranslatef(-10,24,150);
    glScalef(20,10,-1);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void bridge()
{
    Material(1,1,1);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,12);
    //back
    glPushMatrix();
    glScalef(1,40,1);
    MyCube();
    glPopMatrix();
    //middle
    glPushMatrix();
    glTranslatef(0,0,25);
    glScalef(1,40,1);
    MyCube();
    glPopMatrix();
    //front
    glPushMatrix();
    glTranslatef(0,0,50);
    glScalef(1,40,1);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //upper
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,13);
    glPushMatrix();
    glTranslatef(0,40,-5);
    glScalef(1,1,60);
    MyCube();
    glPopMatrix();

    for(float ty = 18; ty < 40; ty += 2)
    {
        glPushMatrix();
        glTranslatef(0,ty,0);
        glScalef(1,0.5,50);
        MyCube();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}
void drawBridge()
{
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,14);
    Material(1,1,1);
    //Base
    glPushMatrix();
    glTranslatef(-15,-40,165);
    glScalef(30,1,50);
    MyCube();
    glPopMatrix();
    //front
    glPushMatrix();
    glTranslatef(-15,-40,215);
    glRotatef(20,1,0,0);
    glScalef(30,1,30);  //10/sin(20*PI/180)
    MyCube();
    glPopMatrix();
    //back
    glPushMatrix();
    glTranslatef(-15,-39,165);
    glRotatef(160,1,0,0);
    glScalef(30,1,30);  //10/sin(160*PI/180)
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //railing left
    glPushMatrix();
    glTranslatef(-15,-55,165);
    bridge();
    glPopMatrix();
    //railing right
    glPushMatrix();
    glTranslatef(14,-55,165);
    bridge();
    glPopMatrix();


}
void drawFence()
{
    float tx,ty;
    //horizontal
    Material(0.1,0.1,0.1);
    for(tx=0;tx<=50;tx+=2)
    {
        //back
        glPushMatrix();
        glTranslatef(tx,0,0);
        glScalef(0.1,20,0.5);
        MyCube();
        glPopMatrix();
    }
    //vertical
    for(ty=1;ty<=20;ty+=2)
    {
        //back
        glPushMatrix();
        glTranslatef(0,ty,0);
        glScalef(50.1,0.5,0.5);
        MyCube();
        glPopMatrix();
    }
}
void drawRoad()
{
    Material(1,1,1);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,15);

    //middle horizontal
    glPushMatrix();
    glTranslatef(-7.5,-50,-145);
    glScalef(15,0.5,290);
    MyCube();
    glPopMatrix();
    //middle vertical right
    glPushMatrix();
    glTranslatef(200,-50,-25);
    glScalef(-192.5,0.5,15);
    MyCube();
    glPopMatrix();
    //middle vertical left
    glPushMatrix();
    glTranslatef(-200,-50,0);
    glScalef(192.5,0.5,15);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
void drawDangerStand()
{
    GLUquadricObj *quad;
    quad = gluNewQuadric();

    //1
    glPushMatrix();
    glRotatef(-90,1,0,0);
    gluCylinder(quad, 2, 2, 3, 32, 32);
    glPopMatrix();
    //2
    glPushMatrix();
    glTranslatef(0,3,0);
    glRotatef(90,1,0,0);
    gluDisk(quad,1.5,2,32,32);
    glPopMatrix();
    //3
    glPushMatrix();
    glTranslatef(0,3,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quad, 1.5, 1.5, 5, 32, 32);
    glPopMatrix();
    //4
    glPushMatrix();
    glTranslatef(0,8,0);
    glRotatef(90,1,0,0);
    gluDisk(quad,1,1.5,32,32);
    glPopMatrix();
    //5
    glPushMatrix();
    glTranslatef(0,8,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quad, 1, 1, 17, 32, 32);
    glPopMatrix();
    //6
    glPushMatrix();
    glTranslatef(0,25,0);
    glRotatef(90,1,0,0);
    gluDisk(quad,1,1.5,32,32);
    glPopMatrix();
    //7
    glPushMatrix();
    glTranslatef(0,25,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quad, 1.5, 1.5, 2, 32, 32);
    glPopMatrix();
    //8
    glPushMatrix();
    glTranslatef(0,27,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quad, 1.5, 0, 3, 32, 32);
    glPopMatrix();
}
GLfloat ropeCtrl[L+1][3] =
{
    {0,0,0},{2,-2,0},{4,-4,0},
    {6,-6,0},{8,-8,0},{10,-9,0},
    {12,-8,0},{14,-6,0},{16,-4,0},
    {18,-2,0},{20,0,0}
};

long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;
    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }
    return ans;
}

void BezierCurve(double t,float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;

    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ropeCtrl[i][0];
        y+=coef*ropeCtrl[i][1];
    }
    xy[0] = float(x);
    xy[1] = float(y);
}

void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;
    Ux = x2-x1; Uy = y2-y1; Uz = z2-z1;
    Vx = x3-x1; Vy = y3-y1; Vz = z3-z1;
    Nx = Uy*Vz - Uz*Vy; Ny = Uz*Vx - Ux*Vz; Nz = Ux*Vy - Uy*Vx;
    glNormal3f(-Nx,-Ny,-Nz);
}

void drawCurvedRope()
{
    float x,y,z;			//current coordinates
    float x1,y1,z1;			//next coordinates

    float t = 0;
    float dt = 1.0/nt;
    float xy[2];
    BezierCurve(t,xy);
    x = xy[0];
    y = xy[1];
    for(int i = 0; i<nt; ++i)  			//step through x
    {
        t+=dt;
        BezierCurve(t,xy);
        x1 = xy[0];
        y1 = xy[1];
        z = 0;
        z1 = 0;

        glLineWidth(2);
        glBegin(GL_LINE_STRIP);
        glVertex3f(x, y, z);
        glVertex3f(x1, y1, z1);
        glEnd();

        x = x1;
        y = y1;
    }
}
void showControlPoints(GLfloat ctrlPoints[][3])
{
    glPointSize(5.0);
    Material(1.0,0.0,0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlPoints[i][0]);
    glEnd();
}
void drawPark()
{
    //ground
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    Material(1,1,1);
    glTranslatef(-200,-70,-150);
    glScalef(400,20,300);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawRoad();
    drawLake();
    drawTrees();
    drawGate();
    drawBridge();
    //fence-left
    glPushMatrix();
    glTranslatef(-290,-50,235);
    glScalef(5.44,1.2,1);
    drawFence();
    glPopMatrix();
    //fence-right
    glPushMatrix();
    glTranslatef(18,-50,235);
    glScalef(5.44,1.2,1);
    drawFence();
    glPopMatrix();

    //danger mark //front left
    for(float tx = - 198; tx < -20; tx += 44.5)
    {
        glPushMatrix();
        Material(0.2,0.2,0.2);
        glTranslatef(tx,-50,148);
        drawDangerStand();
        glPopMatrix();
        //chain
        glPushMatrix();
        glTranslatef(tx,-30,148);
        glScalef(2.225,1,1);
        drawCurvedRope();
        glPopMatrix();
        glPushMatrix(); //
        glTranslatef(tx,-35,148);
        glScalef(2.225,1,1);
        drawCurvedRope();
        glPopMatrix();
    }
    //front right
    for(float tx = 64.5; tx < 200; tx += 44.5)
    {
        glPushMatrix();
        glTranslatef(tx,-50,148);
        drawDangerStand();
        glPopMatrix();
        //chain
        glPushMatrix();
        glTranslatef(tx,-30,148);
        glScalef(-2.225,1,1);
        drawCurvedRope();
        glPopMatrix();
        glPushMatrix();//
        glTranslatef(tx,-35,148);
        glScalef(-2.225,1,1);
        drawCurvedRope();
        glPopMatrix();
    }
    //back
    for(float tx = - 198; tx < 200; tx += 49.5)
    {
        glPushMatrix();
        glTranslatef(tx,-50,-148);
        drawDangerStand();
        glPopMatrix();
        //chain
        if(tx < 198)
        {
            glPushMatrix();
            glTranslatef(tx,-30,-148);
            glScalef(2.475,1,1);
            drawCurvedRope();
            glPopMatrix();
            glPushMatrix();//
            glTranslatef(tx,-35,-148);
            glScalef(2.475,1,1);
            drawCurvedRope();
            glPopMatrix();
        }
    }
    //right
    for(float tz = -148; tz < 148; tz += 37)
    {
        glPushMatrix();
        glTranslatef(198,-50,tz);
        drawDangerStand();
        glPopMatrix();
        //chain
        glPushMatrix();
        glTranslatef(198,-30,tz);
        glRotatef(-90,0.0,1.0,0.0);
        glScalef(1.85,1,1);
        drawCurvedRope();
        glPopMatrix();
        glPushMatrix();//
        glTranslatef(198,-35,tz);
        glRotatef(-90,0.0,1.0,0.0);
        glScalef(1.85,1,1);
        drawCurvedRope();
        glPopMatrix();
    }
    //left
   for(float tz = -148; tz < 148; tz += 37)
    {
        glPushMatrix();
        glTranslatef(-198,-50,tz);
        drawDangerStand();
        glPopMatrix();
        //chain
        glPushMatrix();
        glTranslatef(-198,-30,tz);
        glRotatef(-90,0.0,1.0,0.0);
        glScalef(1.85,1,1);
        drawCurvedRope();
        glPopMatrix();
        glPushMatrix();//
        glTranslatef(-198,-35,tz);
        glRotatef(-90,0.0,1.0,0.0);
        glScalef(1.85,1,1);
        drawCurvedRope();
        glPopMatrix();
    }

    /*glPushMatrix();
    Material(0,1,0);
    drawCurvedRope();
    if(shcpt) {showControlPoints(ropeCtrl);}
    glPopMatrix();*/
}
void drawWheel()
{
    //Torus
    glPushMatrix();
    Material(0.604, 0.804, 0.196);
    glutSolidTorus(0.5,radius,10,10);
    glPopMatrix();

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //line
    for (int theta = 0; theta <= 360; theta += 36)
    {
        glPushMatrix();
        Material(0.420, 0.557, 0.137);
        glRotatef(theta,0,0,1);
        glRotatef(90,0,1,0);
        gluCylinder(quadratic, 0.4, 0.4, radius, 32, 32);
        glPopMatrix();
    }
}
void wheelSeat()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(0, 0.5, 0.8);
    glTranslatef(0,-10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0,2,32,32);
    glPopMatrix();
    //basket
    glPushMatrix();
    Material(0, 0.5, 0.5);
    glTranslatef(0,-10,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic, 2,3, 4, 32, 32);
    glPopMatrix();
    //seat head
    glPushMatrix();
    Material(0.416, 0.353, 0.804);
    glTranslatef(0,-1.5,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.15,3, 3, 32, 32);
    glPopMatrix();
    //attached line
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.15,0.15, 10, 32, 32);
    glPopMatrix();
}
void wheelStand()
{
    //left
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,16);
    glPushMatrix();
    Material(1,1,1);
    glRotatef(-18,0,0,1);
    glScalef(1,-55,0.8);
    MyCube();
    glPopMatrix();
    //right
    glPushMatrix();
    Material(1,1,1);
    glRotatef(18,0,0,1);
    glScalef(1,-55,0.8);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void drawFerrisWheel()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //back wheel
    glPushMatrix();
    glRotatef(-angle,0,0,1);
    drawWheel();
    //left wheel
    glPushMatrix();
    glTranslatef(0,0,12);
    drawWheel();
    glPopMatrix();

    for(int th = 0; th <= 360; th += 36)
    {
        //lines between two wheel
        glPushMatrix();
        Material(0.255, 0.412, 0.882);
        glRotatef(th,0,0,1);
        glTranslatef(radius,0,0);
        gluCylinder(quadratic, 0.3, 0.3,12, 32, 32);
        glPopMatrix();
        //seat
        glPushMatrix();
        glRotatef(th, 0,0,1);
        glTranslatef(radius,0,5);
        glRotatef(-th, 0,0, 1);
        glRotatef(angle, 0, 0, 1 );
        wheelSeat();
        glPopMatrix();
    }
    glPopMatrix();

    //lines between two wheel
    glPushMatrix();
    Material(0,0,0.5);
    glTranslatef(0,0,-2);
    gluCylinder(quadratic, 0.3, 0.3,16, 32, 32);
    glPopMatrix();
    //back stand
    glPushMatrix();
    glTranslatef(0,0,-2);
    wheelStand();
    glPopMatrix();
    //left sphere
    glPushMatrix();
    Material(0, 0.5, 0.8);
    glTranslatef(0,0,-2);
    glutSolidSphere(1.5,16,16);
    glPopMatrix();
    //front stand
    glPushMatrix();
    glTranslatef(0,0,13.2);
    wheelStand();
    glPopMatrix();
    //right sphere
    glPushMatrix();
    Material(0, 0.5, 0.8);
    glTranslatef(0,0,14);
    glutSolidSphere(1.5,16,16);
    glPopMatrix();
}
void drawSwingMoving()
{
    Material(1,1,1);
    //seat
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(4.5,-30,-4);
    glScalef(10,1,8);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //left rope
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,17);
    glPushMatrix();
    glTranslatef(4.5,0,0);
    glScalef(0.4,-20,0.4);
    MyCube();
    glPopMatrix();
    //left-front rope
    glPushMatrix();
    glTranslatef(4.5,-20,0);
    glRotatef(-25,1,0,0);
    glScalef(0.4,-10,-0.4);
    MyCube();
    glPopMatrix();
    //left-back rope
    glPushMatrix();
    glTranslatef(4.5,-20,0);
    glRotatef(25,1,0,0);
    glScalef(0.4,-10,0.4);
    MyCube();
    glPopMatrix();

    //right rope
    glPushMatrix();
    glTranslatef(14.5,0,0);
    glScalef(-0.4,-20,0.4);
    MyCube();
    glPopMatrix();
    //right-front rope
    glPushMatrix();
    glTranslatef(14.5,-20,0);
    glRotatef(-25,1,0,0);
    glScalef(-0.4,-10,-0.4);
    MyCube();
    glPopMatrix();
    //right-back rope
    glPushMatrix();
    glTranslatef(14.5,-20,0);
    glRotatef(25,1,0,0);
    glScalef(-0.4,-10,0.4);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void drawSwing()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //upper -stand
    glPushMatrix();
    Material(0.729, 0.333, 0);
    glRotatef(90,0,1,0);
    gluCylinder(quadratic, 0.8, 0.8, 20, 32, 32);
    glPopMatrix();
    //left - front Stand
    glPushMatrix();
    glTranslatef(1,0,0);
    glRotatef(75,1,0,0);
    gluCylinder(quadratic, 0.5, 0.5, 51, 32, 32);
    glPopMatrix();
    //left - back Stand
    glPushMatrix();
    glTranslatef(1,0,0);
    glRotatef(105,1,0,0);
    gluCylinder(quadratic, 0.5, 0.5, 51, 32, 32);
    glPopMatrix();
    //right - front Stand
    glPushMatrix();
    glTranslatef(19,0,0);
    glRotatef(75,1,0,0);
    gluCylinder(quadratic, 0.5, 0.5, 51, 32, 32);
    glPopMatrix();
    //Right - back Stand
    glPushMatrix();
    glTranslatef(19,0,0);
    glRotatef(105,1,0,0);
    gluCylinder(quadratic, 0.5, 0.5, 51, 32, 32);
    glPopMatrix();

    //Moving Part
    glPushMatrix();
    glRotatef(swingAngle, 1,0,0);
    drawSwingMoving();
    glPopMatrix();
}
void rideOneRotaing()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //inner stand
    glPushMatrix();
    Material(1,0.8,0.8);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic,1,1,50,32,32);
    glPopMatrix();

    for(int i = 0; i <= 360; i+=30)
    {
        //attached lines with rope
        glPushMatrix();
        Material(0.933, 0.910, 0.667);
        glRotatef(i,0,1,0);
        glRotatef(90,0,1,0);
        gluCylinder(quadratic,0.4,0.4,20,32,32);
        glPopMatrix();
        //rope attached with seat
        glPushMatrix();
        Material(0.933, 0.910, 0.667);
        glRotatef(i,0,1,0);
        glTranslatef(19.85,0,0);
        glRotatef(90,1,0,0);
        gluCylinder(quadratic,0.15,0.15,25,32,32);
        glPopMatrix();
        //seat
        glPushMatrix();
        glRotatef(i, 0,1,0);
        glTranslatef(19.85,-25,0);
        wheelSeat();
        glPopMatrix();
    }
}
void drawRideOne()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //outer stand
    glPushMatrix();
    Material(0.2, 0.2, 0.2);
    glTranslatef(0,-2,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic,1.5,1.5,48,32,32);
    glPopMatrix();
    //attached cylinder between outer & inner stand
    glPushMatrix();
    Material(0.4,0.4,0.4);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 1,1.5, 2, 32, 32);
    glPopMatrix();
    //base upper disk
    glPushMatrix();
    Material(0.941, 0.902, 0.549);
    glTranslatef(0,-40,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,1.5,15,32,32);
    glPopMatrix();

    //Outer base stand
    glPushMatrix();
    Material(0.502, 0.502, 0.000);
    glTranslatef(0,-40,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic,15,15,10,32,32);
    glPopMatrix();

    //Rotating part
    glPushMatrix();
    glRotatef(-rideOneAng,0,1,0);
    rideOneRotaing();
    glPopMatrix();
}
void sittingSeat()
{
    Material(1,1,1);
    //back
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,18);
    glPushMatrix();
    glScalef(0.5,10,8);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //base
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,19);
    glPushMatrix();
    glTranslatef(0,5,0);
    glScalef(-4,0.5,8);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //right
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,18);
    glPushMatrix();
    glTranslatef(0,5,0);
    glScalef(-4,-5,0.5);
    MyCube();
    glPopMatrix();
    //left
    glPushMatrix();
    glTranslatef(0,5,8);
    glScalef(-4,-5,-0.5);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void teaTable()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //Inner stand
    glPushMatrix();
    Material(0.933, 0.910, 0.667);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,1.2,1.2,65,32,32);
    glPopMatrix();
    //upper cap
    glPushMatrix();
    Material(1.0,0.5,0.25);
    glTranslatef(0,50,0);
    glRotatef(-90,1,0,0);
    glutSolidCone(15, 15, 32, 32);
    glPopMatrix();
    //upper sphere
    glPushMatrix();
    Material(1.0, 0.271, 0.0);
    glTranslatef(0,65,0);
    glutSolidSphere(1.2, 32,32);
    glPopMatrix();
    //lower disk
    glPushMatrix();
    Material(0.0, 0.5, 0.2);
    glTranslatef(0,0.1,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,10,15,32,32);
    glPopMatrix();

    //rotating part
    glPushMatrix();
    glRotatef(rideTwoAng, 0,1,0);
    //Outer stand
    glPushMatrix();
    Material(0.941, 0.902, 0.549);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,10,12,10,32,32);
    glPopMatrix();
    //base
    glPushMatrix();
    Material(0.741, 0.718, 0.420);
    glTranslatef(0,10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,1.2,12,32,32);
    glPopMatrix();

    for(int th = 0; th <=360; th += 120)
    {
       glPushMatrix();
       glRotatef(th,0,1,0);
       glTranslatef(10,10,-4);
       sittingSeat();
       glPopMatrix();
    }
    glPopMatrix();
}
void drawSeat()
{
    Material(1,1,1);
    //seat
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,13);
    glPushMatrix();
    glScalef(30,3,5);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //stand left
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,20);
    glPushMatrix();
    glScalef(2,-15,5);
    MyCube();
    glPopMatrix();
    //stand right
    glPushMatrix();
    glTranslatef(30,0,0);
    glScalef(-2,-15,5);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void drawMoi()
{
    Material(1,1,1);
    //left
    glPushMatrix();
    glScalef(8,-80,1);
    MyCube();
    glPopMatrix();
    //right
    glPushMatrix();
    glTranslatef(0,0,10);
    glScalef(8,-80,1);
    MyCube();
    glPopMatrix();
    //middle
    for(int i = 0; i<=15;i++)
    {
        glPushMatrix();
        glTranslatef(2,-5*i,0.5);
        glScalef(4,-1,10);
        MyCube();
        glPopMatrix();
    }
}
void cableCar()
{
     Material(1,1,1);
     //upper part
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,13);
     //base
     glPushMatrix();
     glTranslatef(-272,15,-180);
     glScalef(67,1,45);
     MyCube();
     glPopMatrix();
     //back
     glPushMatrix();
     glTranslatef(-272,15,-180);
     glScalef(67,10,1);
     MyCube();
     glPopMatrix();
     //left
     glPushMatrix();
     glTranslatef(-272,15,-180);
     glScalef(0.7,10,45);
     MyCube();
     glPopMatrix();
     //front
     glPushMatrix();
     glTranslatef(-272,15,-135);
     glScalef(65,10,-1);
     MyCube();
     glPopMatrix();
     //right
     glPushMatrix();
     glTranslatef(-205,15,-180);
     glScalef(-1,5,30);
     MyCube();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);

     //stand
     glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D,12);
     //left back
     glPushMatrix();
     glTranslatef(-268,15,-178);
     glScalef(2,-70,2);
     MyCube();
     glPopMatrix();
     //right back
     glPushMatrix();
     glTranslatef(-209,15,-178);
     glScalef(2,-70,2);
     MyCube();
     glPopMatrix();
     //left front
     glPushMatrix();
     glTranslatef(-268,15,-139);
     glScalef(2,-70,2);
     MyCube();
     glPopMatrix();
     //right front
     glPushMatrix();
     glTranslatef(-209,15,-139);
     glScalef(2,-70,2);
     MyCube();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);

     //moi
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,10);
     glPushMatrix();
     glTranslatef(-206,16,-145);
     glRotatef(30,0,0,1);
     drawMoi();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);

     glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D,12);
     //cable left stand
     glPushMatrix();
     glTranslatef(-275,60,-175);
     glScalef(2,-115,2);
     MyCube();
     glPopMatrix();
     //cable right stand
     glPushMatrix();
     glTranslatef(275,60,-175);
     glScalef(2,-115,2);
     MyCube();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);

     //cable
     glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D,17);
     glPushMatrix();
     glTranslatef(-275,51.5,-174.5);
     glScalef(550,-1.5,1);
     MyCube();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);

     //cable car
     glPushMatrix();
     glTranslatef(cableTx, 50, -175);
     glScalef(2.5,3,1);
     wheelSeat();
     glPopMatrix();
}
void seesawSeatHandle()
{
    //upper
    glPushMatrix();
	glScalef(1,1,8);
	MyCube();
	glPopMatrix();
    //left
	glPushMatrix();
	glTranslatef(0,0,1);
	glScalef(1,-5,1);
	MyCube();
	glPopMatrix();
    //right
	glPushMatrix();
	glTranslatef(0,0,7);
	glScalef(1,-5,-1);
	MyCube();
	glPopMatrix();
}
void drawSeesaw()
{
    //stand
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,20);
	glPushMatrix();
	Material(1,1,1);
	drawPyramid();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

    //rotating part
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,21);
	Material(1,1,1);
	glPushMatrix();
	glRotatef(seesawAngle, 0,0,1);
    //seat
	glPushMatrix();
	glTranslatef(-20,0,-4);
	glScalef(40,-1,8);
	MyCube();
	glPopMatrix();
    //left handle
    glPushMatrix();
    glTranslatef(-15,5,-4);
    seesawSeatHandle();
    glPopMatrix();
    //right handle
    glPushMatrix();
    glTranslatef(15,5,-4);
    seesawSeatHandle();
    glPopMatrix();

    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void drawSkyDropSeat()
{
    Material(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,22);
    //back
    glPushMatrix();
    glScalef(52,-10,1);
    MyCube();
    glPopMatrix();
    //floor
    glPushMatrix();
    glTranslatef(0,-10,1);
    glScalef(52,1,8);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //belt
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(0.5,-6,6);
    glScalef(51,0.5,1);
    MyCube();
    glPopMatrix();

    for(float tx = 0.5; tx < 52; tx += 5)
    {
        glPushMatrix();
        glTranslatef(tx,-9,1);
        glScalef(1,4,8);
        MyCube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(tx+0.25,-2,1);
        glRotatef(30,1,0,0);
        glScalef(0.5,0.5,7);
        MyCube();
        glPopMatrix();
     }
     //
     glPushMatrix();
     Material(1,1,1);
     glTranslatef(21,-10,0);
     glScalef(11,-3,1);
     MyCube();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);
}
void drawSkyDropRide()
{
    Material(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    //back-left
    glPushMatrix();
    glScalef(1,180,1);
    MyCube();
    glPopMatrix();
    //back-right
    glPushMatrix();
    glTranslatef(10,0,0);
    glScalef(1,180,1);
    MyCube();
    glPopMatrix();
    //front left
    glPushMatrix();
    glTranslatef(0,0,10);
    glScalef(1,180,1);
    MyCube();
    glPopMatrix();
    //front-right
    glPushMatrix();
    glTranslatef(10,0,10);
    glScalef(1,180,1);
    MyCube();
    glPopMatrix();
    float tmp;
    for(float ty = 10; ty < 180; ty += 10)
    {
        glPushMatrix();//back
        glTranslatef(0.3,ty,0.3);
        glScalef(10,0.5,0.4);
        MyCube();
        glPopMatrix();

        glPushMatrix();//left
        glTranslatef(0.3,ty,0.3);
        glScalef(0.4,0.5,10);
        MyCube();
        glPopMatrix();

        glPushMatrix();//front
        glTranslatef(0.3,ty,10.3);
        glScalef(10,0.5,0.4);
        MyCube();
        glPopMatrix();

        glPushMatrix();//right
        glTranslatef(10.3,ty,0.3);
        glScalef(0.4,0.5,10);
        MyCube();
        glPopMatrix();
        tmp = ty;
    }
    for(float tyy = 10; tyy < tmp; tyy += 10)
    {
        glPushMatrix(); //left
        glTranslatef(0.3,tyy,0.3);
        glRotatef(45,1,0,0);
        glScalef(0.4,10/cos(45*PI/180),0.4);
        MyCube();
        glPopMatrix();

        glPushMatrix(); //right
        glTranslatef(10.3,tyy,10.3);
        glRotatef(-45,1,0,0);
        glScalef(0.4,10/cos(-45*PI/180),0.4);
        MyCube();
        glPopMatrix();

        glPushMatrix(); //back
        glTranslatef(10.3,tyy,0.3);
        glRotatef(45,0,0,1);
        glScalef(0.4,10/cos(45*PI/180),0.4);
        MyCube();
        glPopMatrix();

        glPushMatrix(); //front
        glTranslatef(0.3,tyy,10.3);
        glRotatef(-45,0,0,1);
        glScalef(0.4,10/cos(-45*PI/180),0.4);
        MyCube();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);

    //Animated Part
    glPushMatrix();
    glTranslatef(-21,dropTy,10.5);
    drawSkyDropSeat();
    glPopMatrix();
}
void skyDrop()
{
    //base
    glPushMatrix();
    Material(0.439, 0.502, 0.565);
    glScalef(60,10,40);
    MyCube();
    glPopMatrix();
    //ride
    glPushMatrix();
    glTranslatef(25,10,10);
    drawSkyDropRide();
    glPopMatrix();
}
void drawPlane()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //0
    glPushMatrix();
    Material(0.541, 0.169, 0.886);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0.2,0.8,32,32);
    glPopMatrix();
    //1
    glPushMatrix();
    Material(1.0,0.0,1.0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,0.8,0.8,7,32,32);
    glPopMatrix();
    //2
    glPushMatrix();
    Material(0.541, 0.169, 0.886);
    glTranslatef(0,7,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,0.8,0.8,4,32,32);
    glPopMatrix();
    //3
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,11,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,0.8,0.8,5,32,32);
    glPopMatrix();
    //4
    glPushMatrix();
    Material(1.0,0.0,1.0);
    glTranslatef(0,16,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,0.8,0.8,3,32,32);
    glPopMatrix();
    //5
    glPushMatrix();
    Material(0.541, 0.169, 0.886);
    glTranslatef(0,19,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,0.8,0,3,32,32);
    glPopMatrix();
    //pakha
    for(int th =0; th <=360; th += 90)
   {
        glPushMatrix();
        Material(1.0,0.0,1.0);
        glRotatef(th,0,1,0);
        glTranslatef(0,0,0.8);
        glScalef(0.1,5,1);
        MyCube();
        glPopMatrix();
    }
    //upper
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,15,-0.8);
    glScalef(0.4,0.4,-2);
    MyCube();
    glPopMatrix();
    //lower
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,6,-0.8);
    glScalef(0.4,0.4,-2);
    MyCube();
    glPopMatrix();
}
void drawPlaneSeat()
{
    //base
    Material(1,1,1);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,22);
    glPushMatrix();
    glScalef(-10,0.5,10);
    MyCube();
    glPopMatrix();
    //back
    glPushMatrix();
    glScalef(0.5,5,10);
    MyCube();
    glPopMatrix();
    //left
    glBegin(GL_QUADS);
    glVertex3f(0,0.5,0); glTexCoord2d(0,0);
    glVertex3f(-10,0.5,0); glTexCoord2d(1,0);
    glVertex3f(-7,3,0); glTexCoord2d(1,1);
    glVertex3f(0,3,0); glTexCoord2d(0,1);
    glEnd();
    //right
    glBegin(GL_QUADS);
    glVertex3f(0,0.5,10);  glTexCoord2d(0,0);
    glVertex3f(-10,0.5,10); glTexCoord2d(1,0);
    glVertex3f(-7,3,10); glTexCoord2d(1,1);
    glVertex3f(0,3,10); glTexCoord2d(0,1);
    glEnd();
    //front
    glPushMatrix();
    glTranslatef(-10,0.5,0);
    glRotatef(-50,0,0,1);
    glScalef(0.1,5,10);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //torus
    glPushMatrix();
    glTranslatef(-5,0,10);
    glutSolidTorus(0.2,0.5,32,32);
    glPopMatrix();
}
void planeRotating()
{
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,23);
	Material(1,1,1);
	//seat stick
    glPushMatrix();
    glRotatef(-60,0,0,1);
    glScalef(50,1,1);
    MyCube();
    glPopMatrix();
    //seat support stand
    glPushMatrix();
    glTranslatef(25,50*sin(-60*PI/180),0);
    glRotatef(-rotAngle,0,0,1);
    glScalef(10,0.5,1);
    MyCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //seat
    glPushMatrix();
    glTranslatef(26,50*sin(-60*PI/180)+0.5,-5);
    glRotatef(-rotAngle,0,0,1);
    glRotatef(90,0,1,0);
    drawPlaneSeat();
    glPopMatrix();
}
void drawPlaneRide()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //stand base
    glPushMatrix();
    Material(0.467,0.533,0.6);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,40,40,10,32,32); //quad, base, top, height, slices, stacks
    glPopMatrix();
    //stand upper
    glPushMatrix();
    Material(0.4,0.5,0.6);
    glTranslatef(0,10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0.5,40,32,32);
    glPopMatrix();
    //1
    glPushMatrix();
    Material(1.0, 0.078, 0.576);
    glTranslatef(0,10,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,12,7,5,32,32); //quad, base, top, height, slices, stacks
    glPopMatrix();
    //2
    glPushMatrix();
    Material(1.000, 0.078, 0.576);
    glTranslatef(0,15,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,7,7,40,32,32);
    glPopMatrix();
    //3
    glPushMatrix();
    Material(0.282, 0.239, 0.545);
    glTranslatef(0,55,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,7,5,15,32,32);
    glPopMatrix();
    //4
    glPushMatrix();
    Material(1.000, 0.078, 0.576);
    glTranslatef(0,70,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,5,5,30,32,32);
    glPopMatrix();
    //design
    for(int th = 0; th <= 360; th += 90)
    {
        glPushMatrix();
        glRotatef(th,0,1,0);
        glTranslatef(0,73,7);
        drawPlane();
        glPopMatrix();
    }
    for(int theta = 0; theta <= 360; theta += 45)
    {
        glPushMatrix();
        glRotatef(theta,0,1,0);
        for(float ty = 71; ty < 100; ty += 4)
        {
            glPushMatrix();
            Material(0.8,0.8,0.8);
            glTranslatef(0,ty,5);
            glutSolidSphere(0.4,32,32);
            glPopMatrix();
        }
        glPopMatrix();
    }
    //5
    glPushMatrix();
    Material(0.282, 0.239, 0.545);
    glTranslatef(0,100,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,5,3,10,32,32);
    glPopMatrix();
    //6
    glPushMatrix();
    Material(1.0, 0.078, 0.576);
    glTranslatef(0,110,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,3,3,10,32,32);
    glPopMatrix();
    //7
    glPushMatrix();
    Material(0.282, 0.239, 0.545);
    glTranslatef(0,120,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,3,2,8,32,32);
    glPopMatrix();
    //8
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,128,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,2,2,7,32,32);
    glPopMatrix();
    //9
    glPushMatrix();
    Material(1.0, 0.647, 0.0);
    glTranslatef(0,135,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,2,0.5,5,32,32);
    glPopMatrix();
    //10 - inner
    glPushMatrix();
    Material(0.2,0.2,0.2);
    glTranslatef(0,140,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic,0.5,0.5,135,32,32);
    glPopMatrix();
    //11
    glPushMatrix();
    Material(0, 0.5, 0.8);
    glTranslatef(0,140.5,0);
    glutSolidSphere(0.5,16,16);
    glPopMatrix();

    //Rotated part
    glPushMatrix();
    glTranslatef(0,62.5,0);
    glRotatef(-planeAngle,0,1,0);
    for(int th = 0; th <= 360; th += 45)
    {
        glPushMatrix();
        glRotatef(th,0,1,0);
        glRotatef(rotAngle,0,0,1);
        planeRotating();
        glPopMatrix();
   }
   glTranslatef(0,-62.5,0);
   glPopMatrix();
}
GLfloat seatCtrl[cpt+1][3]
{
    {1.45,-1.5,0},{1.125,-1.525,0},
    {0.825, -1.525,0},{0.55, -1.525,0},
    {0.25, -1.5,0},{-0.075, -1.5,0},
    {-0.075, -1.2,0},{-0.175, -0.775,0},
    {-0.2, -0.225,0},{-0.15, -0.025,0}
};
void seatBezierCurve(double t,float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;

    for(int i=0; i<=cpt; i++)
    {
        int ncr=nCr(cpt,i);
        double oneMinusTpow=pow(1-t,double(cpt-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*seatCtrl[i][0];
        y+=coef*seatCtrl[i][1];
    }
    xy[0] = float(x);
    xy[1] = float(y);
}
void drawCurvedSeat()
{
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    const float dtheta = 2*PI / ntheta;		//angular step size
    float t=0;
    float dt=1.0/nt;
    float xy[2];
    seatBezierCurve(t,xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for (int i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        seatBezierCurve(t,xy);
        x1 = xy[0];
        r1 = xy[1];
        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP);
        for (int j=0;j<=10;++j)
        {
            theta += dtheta;
            double cosa = cos(theta);
            double sina = sin(theta);
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1* sina;	//current and next z
            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0) {setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);}
            else
            {
                p1x=x;p1y=y;p1z=z;
                p2x=x1;p2y=y1;p2z=z1;
            }
            glVertex3f (x1, y1, z1);
            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    }
}
void drawdRideThree()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //lower
    glPushMatrix();
    Material(1.0,0.894,0.769);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,5,1.2,8,32,32);
    glPopMatrix();
    //middle
    glPushMatrix();
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic,1.2,1.2,40,32,32);
    glPopMatrix();
    //upper
    glPushMatrix();
    glTranslatef(0,40,0);
    glutSolidSphere(1.2,16,16);
    glPopMatrix();
    //Rotating part
    glPushMatrix();
    glTranslatef(0,8,0);
    glRotatef(unAng,0,1,0);
    glTranslatef(0,-8,0);
    for(int th=0; th<=360; th+= 45)
    {
        glPushMatrix();
        Material(0.847,0.749,0.847);
        glRotatef(th,0,1,0);
        glTranslatef(0,8,0);
        glRotatef(-40,1,0,0);
        gluCylinder(quadratic,0.3,0.3,35,32,32);
        glPopMatrix();
        //seat to stand
        glPushMatrix();
        glRotatef(th,0,1,0);
        glTranslatef(30,8+35*sin(40*PI/180),0);
        glRotatef(-90,0,1,0);
        gluCylinder(quadratic,0.2,0.2,30,32,32);
        glPopMatrix();
        //handle vertical
        glPushMatrix();
        glRotatef(th,0,1,0);
        glTranslatef(24,30.5,0);
        glRotatef(-90,1,0,0);
        gluCylinder(quadratic,0.2,0.2,5,32,32);
        glPopMatrix();
        //handle horizontal
        glPushMatrix();
        glRotatef(th,0,1,0);
        glTranslatef(24,35.5,-4);
        gluCylinder(quadratic,0.2,0.2,8,32,32);
        glPopMatrix();
        //seat
        glPushMatrix();
        Material(0.58,0.0,0.827);
        glRotatef(th,0,1,0);
        glTranslatef(26,31.5,0);
        glRotatef(70, 0.0, 1.0, 0.0);
        glRotatef(90, 0.0, 0.0, 1.0);
        glRotatef(180, 1.0, 0.0, 0.0);
        glScalef(5,5,6);
        drawCurvedSeat();
        glPopMatrix();
    }
    glPopMatrix();
}
void spotLightOne()
{
    glPushMatrix();
    GLfloat light_position[] = {30,200,-95,1};
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glPopMatrix();
}
void LightOne()
{
    glPushMatrix();
    GLfloat light_position[] = {5,500,-100,1.0};  //0,50,0
    glLightfv( GL_LIGHT1, GL_POSITION, light_position);
    glPopMatrix();
}
void LightTwo()
{
    glPushMatrix();
    GLfloat light_position[] = {5,500,100, 1.0};  //0,50,113
    glLightfv( GL_LIGHT2, GL_POSITION, light_position);
    glPopMatrix();
}
void designPark()
{
    drawSkyBox();
    drawGround();
    drawPark();

    //ferris wheel
    glPushMatrix();
    glTranslatef(-70,0,30);
    drawFerrisWheel();
    glPopMatrix();
    //swing
    glPushMatrix();
    glTranslatef(55,0,50);
    drawSwing();
    glPopMatrix();
    //ride-one
    glPushMatrix();
    glTranslatef(-30,0,-90);
    drawRideOne();
    glPopMatrix();
    //tea-table
    glPushMatrix();
    glTranslatef(150,-50,20);
    teaTable();
    glPopMatrix();
    //cable car
    cableCar();
    //seat front right
    glPushMatrix();
    glTranslatef(170,-35,120);
    glRotatef(90,0,1,0);
    drawSeat();
    glPopMatrix();
    //seat front left
    glPushMatrix();
    glTranslatef(-170,-35,120);
    glRotatef(90,0,1,0);
    drawSeat();
    glPopMatrix();
    //seesaw
    glPushMatrix();
    glTranslatef(-90,-30,-90);
    drawSeesaw();
    glPopMatrix();
    //sky drop ride
    glPushMatrix();
    glTranslatef(10,-50,-100);
    skyDrop();
    glPopMatrix();
    //plane ride
    glPushMatrix();
    glTranslatef(135,-50,-80);
    drawPlaneRide();
    glPopMatrix();
    //Ride Three
    glPushMatrix();
    glTranslatef(-140,-50,50);
    drawdRideThree();
    glPopMatrix();

    /*glPushMatrix();
    Material(1,0,1);
    glRotatef(60, 0.0, 1.0, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glRotatef(180, 1.0, 0.0, 0.0);
    glScalef(5,5,6);
    drawCurvedSeat();
    if(shcpt) {showControlPoints(seatCtrl);}
    glPopMatrix();*/
}
void display_Design(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60, 1.5, 1, 200000);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ,  eyeX+dx, eyeY+dy , eyeZ + dz,  0,1,0);

    glEnable(GL_LIGHTING);
    spotLightOne();
    LightOne();
    LightTwo();
    designPark();
    glDisable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}
void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'o':            //zoom out
        eyeZ += 2;
        break;
    case 'i':           //zoom in
        eyeZ -= 2;
        break;
    case 'X':           //Camera Right along X axis
        eyeX += 2;
        break;
    case 'x':           //Camera left along X axis
        eyeX -= 2;
        break;
    case 'Y':           //Camera upwards along Y axis
        eyeY += 2;
        break;
    case 'y':           //Camera downwards along Y axis
        eyeY -= 2;
        break;

    case 'b':          //default eye point and look at/ref. point
        eyeX = 0.0; eyeY = 3.0; eyeZ = 336;                  //290
        dx = 0.0; dy = 0.0; dz = -1.0;
        break;

    case 'u':         //Look at point upwards along Y axis
        dy += 1;
        break;
    case 'v':         //Look at point downwards along Y axis
        dy -= 1;
        break;
    case 'r':         //Look at point right along X axis
        dx += 1;
        break;
    case 'l':         //Look at point left along X axis
        dx-= 1;
        break;
    case 'w':         //Look at point away from screen/ along z axis
        dz += 1;
        break;
    case 't':         //Look at point towards screen/ along z axis
        dz -= 1;
        break;
    case 'c':
        shcpt=!shcpt;
        break;
    case '1':         //wheel on off
        spinFlag =! spinFlag;
        break;
    case '2':        //swing
        swingFlag =! swingFlag;
        break;
    case '3':
        rideOneFlag =! rideOneFlag;
        break;
    case '4':
        rideTwoFlag =! rideTwoFlag;
        break;
    case '5':
        cableFlag =! cableFlag;
        break;
    case '6':
        seesawFlag =! seesawFlag;
        break;
    case '7':
        dropFlag =! dropFlag;
        break;
    case '8':
        planeFlag =! planeFlag;
        break;
    case '9':
        unFlag =! unFlag;
        break;

    case '0':   //spotlight
        if(spotLight == false)
        {
            spotLight = true; ambL0 = true; diffL0 = true; specL0 = true;
            glEnable( GL_LIGHT0);   //Turn on spot light
        }
        else if(spotLight == true)
        {
            spotLight = false; ambL0 = false; diffL0 = false; specL0 = false;
            glDisable( GL_LIGHT0);  //Turn off spot light
        }
        break;
    case 'a':
        ambL0 =! ambL0;
        break;
    case 'd':
        diffL0 =! diffL0;
        break;
    case 's':
        specL0 =! specL0;
        break;
    case 'L':   //light1 white
        if(Light1 == false)
        {
            Light1 = true; ambL1 = true; diffL1 = true; specL1 = true;
            glEnable(GL_LIGHT1);   //Turn on spot light
        }
        else if(Light1 == true)
        {
            Light1 = false; ambL1 = false; diffL1 = false; specL1 = false;
            glDisable(GL_LIGHT1);  //Turn off spot light
        }
        break;
    case 'A':
        ambL1 =! ambL1;
        break;
    case 'D':
        diffL1 =! diffL1;
        break;
    case 'S':
        specL1 =! specL1;
        break;
    case 'K':   //front light white
        if(Light2 == false)
        {
            Light2 = true; ambL2 = true; diffL2 = true; specL2 = true;
            glEnable( GL_LIGHT2);    //Turn on light 2
        }
        else if(Light2 == true)
        {
            Light2 = false; ambL2 = false; diffL2 = false; specL2 = false;
            glDisable( GL_LIGHT2);  //Turn off light 2
        }
        break;
    case 'M':
        ambL2 =! ambL2;
        break;
    case 'E':
        diffL2 =! diffL2;
        break;
    case 'T':
        specL2 =! specL2;
        break;
    case 27:         // Escape key
        exit(1);
    }
    glutPostRedisplay();
}
void animation()
{
    if(spinFlag == true)
    {
        angle += speed;
        if(angle > 360)
            angle -=360;
    }

    if(swingFlag == true) {
        if(thresAngle == 0) {swingFlag = false;}
        if(swingAngle >= thresAngle && direction == 1) { thresAngle--; direction = -1; swingSpeed *= 0.90;}
        if(swingAngle <= -thresAngle && direction == -1) { thresAngle--; direction = 1; swingSpeed *= 0.90;}
        swingAngle += direction * swingSpeed;
    }

    if(rideOneFlag == true)
    {
        rideOneAng += rideOneSpeed;
        if(rideOneAng > 360)
            rideOneAng -=360;
    }

    if(rideTwoFlag == true)
    {
        rideTwoAng += rideTwoSpeed;
        if(rideTwoAng > 360)
            rideTwoAng -=360;
    }

    if(cableFlag == true)
    {
        if(xDir == 1) { cableTx += 2; if(cableTx == thresCableTx) {xDir = -1;} }
        if(xDir == -1) { cableTx -= 2; if(cableTx == -thresCableTx) {xDir = 1; cableFlag = false;} }
    }

    if(seesawFlag == true)
    {
        if(seesawAngle >= seesawThresAngle|| seesawAngle <= -seesawThresAngle ){ seesawDir *= -1;}
        seesawAngle += seesawDir * seesawSpeed;
    }

    if(dropFlag == true)
    {
        if(yDir == 1) { dropTy += 1; if(dropTy == 170) { yDir = -1;} }
        if(yDir == -1) { dropTy -= 2; if(dropTy == 16) {yDir = 1;dropFlag = false;} }
    }

    if(planeFlag == true)
    {
        if(planeDir == 1 && rotAngle <= 60)
        {
            rotAngle += 1;
            if(rotAngle == 60)
                planeFlag1 = true;
        }

        if(planeDir == 1 && planeFlag1 == true)
        {
            planeAngle += planeSpeed;
            if(planeAngle >= 720) //2*360
            {
                planeFlag1 = false;
                planeAngle = 0;
                planeDir = -1;
            }
        }

        if(planeDir == -1 && rotAngle >= 0)
        {
            rotAngle -= 1;
            if(rotAngle == 0)
            {
                planeDir = 1;
                planeFlag = false;
            }
        }
    }
    if(unFlag == true)
    {
        unAng += speed;
        if(unAng > 360)
            unAng -=360;
    }
    //spot light0 white
    if(ambL0 == true) {glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient0);}
    else if(ambL0 == false) {glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);}
    if(diffL0 == true) {glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse0);}
    else if(diffL0 == false) {glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);}
    if(specL0 == true) {glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);}
    else if(specL0 == false) {glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);}

    //Light1 white
    if(ambL1 == true) {glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient1);}
    else if(ambL1 == false) {glLightfv(GL_LIGHT1, GL_AMBIENT, no_light);}
    if(diffL1 == true) {glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse1);}
    else if(diffL1 == false) {glLightfv(GL_LIGHT1, GL_DIFFUSE, no_light);}
    if(specL1 == true) {glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);}
    else if(specL1 == false) {glLightfv(GL_LIGHT1, GL_SPECULAR, no_light);}

     //Light2 white
    if(ambL2 == true) {glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient2);}
    else if(ambL2 == false) {glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);}
    if(diffL2 == true) {glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse2);}
    else if(diffL2 == false) {glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);}
    if(specL2 == true) {glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);}
    else if(specL2 == false) {glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);}
    glutPostRedisplay();
}
void changeSize(int w, int h)
{
    //Prevent a divide by zero, when window is too short;you cant make a window of zero width.
    if (h == 0)
        h = 1;
    float ratio = (GLfloat)w / (GLfloat)h;         //Calculate aspect ratio of the window

    //Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);                   //Use the Projection Matrix
    glLoadIdentity();                              //Reset Matrix

    glViewport(0, 0, w, h);                        //Set the viewport to be the entire window
    gluPerspective(60, ratio, 1, 200);             //Set the correct perspective.
    glMatrixMode(GL_MODELVIEW);                    //Get Back to the Modelview
}
void LoadTexture(const char*filename, int index)
{
    glGenTextures(1, &ID[index]);
    glBindTexture(GL_TEXTURE_2D, ID[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);

    printf("To change camera position:\n");
    printf("1. Zoom In: i\n2. Zoom Out: o\n");
    printf("3. Camera Down: y\n4. Camera Up: Y\n");
    printf("5. Camera Left: x\n6. Camera Right: X");

    printf("\n\nTo change look at point :\n");
    printf("1. To look Right: r\n2. To look Left: l\n");
    printf("3. To look UP: u\n4. To look Down: v\n");
    printf("5. Away from screen: w\n6. Towards screen: t\n");
    printf("Back to Default position: b\n\n");

    printf("***********Light Controls***********\t\n");
    printf("SpotLight on/off: 0\n");
    printf("\t1.SpotLight's Ambient on/off: a\n");
    printf("\t3.SpotLight's Diffuse on/off: d\n");
    printf("\tc.SpotLight's Specular on/off: s\n\n");
    printf("Light1 on/off: L\n");
    printf("\t1.Light1's Ambient on/off: A\n");
    printf("\t2.Light1's  Diffuse on/off: D\n");
    printf("\t3.Light1's Specular on/off: S\n\n");
    printf("Light2 on/off: K\n");
    printf("\t1.Light2's Ambient on/off: M\n");
    printf("\t2.Light2's  Diffuse on/off: E\n");
    printf("\t3.Light2's Specular on/off: T\n\n");

    printf("***********Ride Controls***********\n");
    printf("   Ferris Wheel: 1\n   Swing: 2\n   Ride One: 3\n   Ride Two: 4\n   Cable Car Ride: 5\n");
    printf("   Seesaw: 6\n   Sky Drop Ride: 7\n   Plane Ride: 8\n   Ride Three: 9\n");


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Theme Park - 1607041");
    glutReshapeFunc(changeSize);

    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\skyup.bmp",1);      //top
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\skyfront.bmp",2);   //right
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\skyback.bmp",3);    //left
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\skyright.bmp",4);   //back
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\skyleft.bmp",5);    //front
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\skydown.bmp",6);    //bottom
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\grass.bmp",7);      //grass
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\soil.bmp",8);       //lake par
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\water.bmp",9);      //lake water
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\wood.bmp",10);      //gate seat wood
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\welcome.bmp",11);    //welcome
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\metalPole.bmp",12);  //lake pole
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\cement.bmp",13);     //cable car- seat-bridge railing
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\concrete.bmp",14);   //bridge
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\road.bmp",15);       //road
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\steel.bmp",16);      //wheel stand
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\rope.bmp",17);       //swing rope
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\wheelback.bmp",18);  //wheel-seat back-down
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\wheelseat.bmp",19);  //wheel seat
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\brick.bmp",20);      //brick- seat
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\seesawseat.bmp",21);  //seesaw seat
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\planeSeat.bmp",22);   //plane seat - sky drop seat
    LoadTexture("F:\\4-2\\Graphics Lab\\Project\\1607041\\textureImg\\planeStick.bmp",23);  //plane stick - sky drop tower

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display_Design);
    glutIdleFunc(animation);
    glutMainLoop();

    return 0;
}
