# include <windows.h>
# include <gl/glut.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
#include <vector>
#include "readfile.h"
#define DOMO 0

RoseFile Rose("rose.txt");


GLint pmode = 0;
GLint pMaterial = 1;
GLint pwenli = 1;

bool rotate = true;

//GLubyte *pixeldata;

vector<Images> image;



//GLubyte *pixel[4];
GLuint texname[5];





GLint px, py;
GLfloat rx, ry;
GLfloat rot[3], depth;
GLsizei width, height;

GLdouble current_rotate_matrix[16] = 
{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

GLfloat ambient[] = {1.0,1.0,0.0,1.0};
GLfloat ambient2[] = {0.0,0.0,0.0,1.0};
GLfloat position[] = {10.0,10.0,10.0,0.0};
GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
GLfloat specular[] = {1.0,1.0,1.0,1.0};
GLfloat shininess[] = {50.0};

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_FLAT);
	position[0] = Rose.xmax;position[1] = Rose.ymax;position[2] = Rose.zmax;

	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_SHININESS,shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient2);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	for (int i=0;i<Rose.textureNum;++i)
	{
		Images im;
		im.name = Rose.vecTexture[i];
		im.loadPixel();
		image.push_back(im);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(i,&texname[i]);

		glBindTexture(GL_TEXTURE_2D,texname[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image[i].ImageWidth, image[i].ImageHeight,
			0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[i].pixelDatas);
	}	
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
GLdouble eyex = 1.0,eyey = 1.0,eyez = 1.0,cx = 0,cy = 0,cz = 0,sx = 1,sy = 1,sz = 0;

void drawtriangle()
{	
	int j = 0;
	int i = 0;
	//gluLookAt(eyex,eyey,eyez,cx,cy,cz,sx,sy,sz);
	for(;j<Rose.ModelNum;++j){
		//if(Rose.vecModel[j].MaterialIdx)
		{
			//if (pwenli)
			{
				glBindTexture(GL_TEXTURE_2D, texname[Rose.vecModel[j].MaterialIdx-1]);
			}
			//if (pMaterial)
			{
				glMaterialfv(GL_FRONT,GL_AMBIENT,Rose.vecMaterial[Rose.vecModel[j].MaterialIdx-1].ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, Rose.vecMaterial[Rose.vecModel[j].MaterialIdx - 1].diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, Rose.vecMaterial[Rose.vecModel[j].MaterialIdx - 1].specular);
				glMaterialfv(GL_FRONT, GL_EMISSION, Rose.vecMaterial[Rose.vecModel[j].MaterialIdx - 1].emission);
				glMaterialfv(GL_FRONT, GL_SHININESS, &(Rose.vecMaterial[Rose.vecModel[j].MaterialIdx - 1].shininess));
			}
		}
		for (i= 0;i<Rose.vecModel[j].TriangleNum;++i)
		{
			switch(pmode){
				case 0:glBegin(GL_TRIANGLES);break;
				case 1:glBegin(GL_POINTS);break;
				case 2:glBegin(GL_LINE_LOOP);break;
				default:glBegin(GL_TRIANGLES);
			}		
				glNormal3f(Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni1-1].x,
					Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni1-1].y,
					Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni1-1].z);
				glTexCoord2f(Rose.vecCoordinate[Rose.vecModel[j].vecTriangle[i].ti1-1].x,
					Rose.vecCoordinate[Rose.vecModel[j].vecTriangle[i].ti1 - 1].y);
				glVertex3f(Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi1-1].x,
					Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi1 - 1].y,
					Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi1 - 1].z);

				glNormal3f(Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni2-1].x,
					Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni2-1].y,
					Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni2-1].z);
				glTexCoord2f(Rose.vecCoordinate[Rose.vecModel[j].vecTriangle[i].ti2-1].x,
					Rose.vecCoordinate[Rose.vecModel[j].vecTriangle[i].ti2 - 1].y);
				glVertex3f(Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi2-1].x,
					Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi2 - 1].y,
					Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi2 - 1].z);

				glNormal3f(Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni3-1].x,
					Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni3-1].y,
					Rose.vecNormal[Rose.vecModel[j].vecTriangle[i].ni3-1].z);
				glTexCoord2f(Rose.vecCoordinate[Rose.vecModel[j].vecTriangle[i].ti3-1].x,
					Rose.vecCoordinate[Rose.vecModel[j].vecTriangle[i].ti3 - 1].y);
				glVertex3f(Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi3-1].x,
					Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi3 - 1].y,
					Rose.vecVertex[Rose.vecModel[j].vecTriangle[i].vi3 - 1].z);
			glEnd();
		}
	}
	
}



void drawLine(){
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(100,0,0);
		glVertex3f(0,0,0);
		glVertex3f(0,100,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,100);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texname[0]);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);
	glTranslatef(0.0, 0.0, depth);
	if(Rose.Scale[0]> 10) 
		glScalef(10,10,10);
	else
		glScalef(Rose.Scale[0],Rose.Scale[1],Rose.Scale[2]);
	glMultMatrixd(current_rotate_matrix);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_POINTS);
	drawtriangle() ;
	//Triangles();
	//drawLine();
	
	glFlush();
}

void mouse(int button, int state, int x, int y)  //捕捉鼠标按下的消息
{ 
	bool rotate = true;
	if(GLUT_LEFT_BUTTON == button)
	{
		if(GLUT_DOWN == state)
		{
			px = x;
			py = y;
			rotate = true;  //随鼠标旋转
		}
		else
			rotate = false;  //停止旋转
	}
}

void crossprod(double v1[3], double v2[3])  //计算两个向量的叉乘
{
	rot[0] = v1[1]*v2[2] - v1[2]*v2[1];
	rot[1] = v1[2]*v2[0] - v1[0]*v2[2];
	rot[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

void motion(int x, int y)  //捕捉鼠标移动的消息，实现旋转
{
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLdouble w1[3], w2[3], w3[3];
	bool rotate = true;
	if(rotate == true)
	{
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
		gluUnProject((GLdouble)x, (GLdouble)(viewport[3] - y - 1), 0.0,
			mvmatrix, projmatrix, viewport, &w1[0], &w1[1], &w1[2]);

		gluUnProject((GLdouble)px, (GLdouble)(viewport[3] - py - 1), 0.0,
			mvmatrix, projmatrix, viewport, &w2[0], &w2[1], &w2[2]);
		
		gluUnProject((GLdouble)width/2, (GLdouble)height/2, 0.0,
			mvmatrix, projmatrix, viewport, &w3[0], &w3[1], &w3[2]);

		w1[0] = w2[0] - w1[0];
		w1[1] = w2[1] - w1[1];
		w1[2] = w2[2] - w1[2];
		crossprod(w1, w3);
		px = x;
		py = y;

		glPushMatrix();
			glLoadMatrixd(current_rotate_matrix);
			glRotatef(4.5, rot[0], rot[1], rot[2]);   //旋转叠加
			glGetDoublev(GL_MODELVIEW_MATRIX, current_rotate_matrix);  //保存旋转矩阵
		glPopMatrix();

		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y)  //捕捉键盘按下消息，实现远近观察
{
	switch(key)
	{
		case 'f':
			depth -= 0.1;   //远离
			glutPostRedisplay();
			break;
		case 'j':
			depth += 0.1;  //靠近
			glutPostRedisplay();
			break;
		case ' ':
			if(pmode<3) {
				pmode=3;
				glEnable(GL_LIGHTING);
				//glEnable(GL_LIGHT0);
				
			}
			else if (pmode == 3)
			{
				++pmode;
				glDisable(GL_LIGHTING);
			}
			else {
				pmode = 0;
				pMaterial = 0;
				pwenli = 0;
			}
			/*if(pmode == 3) {
				pMaterial = 1;
				pwenli = 0;
			}
			else if (pmode == 6)
			{
				pwenli = 1;
				pMaterial = 0;
			}
			else if (pmode == 9)
			{
				pwenli = 1;
				pMaterial = 1;
			}*/
			glutPostRedisplay();
			break;
			
		default:
			break;
	}
}
void idle()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutSetCursor(GLUT_CURSOR_INFO);
	init();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}