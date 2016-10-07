//Author: Yogesh Nitnaware 


#include "../shared/gltools.h"	// OpenGL toolkit
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>

#define RADIUS    0.025
#define P_LENGTH  0.3
using namespace std;
int WIDTH = 1300,
HEIGHT = 950,
lives = 3,
SCORE = 0,
LEVEL = 2,
BRICK_ROW = 10,
BRICK_COL = 9;
double x_brick[10][10],
y_brick[10][10],
P_XPOS = 0,
P_YPOS = -0.8,
x_pos = 0,
y_pos = -0.75,
win_aspect,
SPEED = 0;
bool show[10][10],
phit_center = false,
phit_corner = false,
game_over = false,
RIGHT = true,
LEFT = false,
UP = true,
DOWN = false,
started = false,
hit = false;
char life = '0';

bool lookUp;
bool lookDown;
bool lookLeft;
bool lookRight;
bool walkForward;
bool walkBackward;
bool strafeLeft;
bool strafeRight;
float xTranslation;
float yTranslation;
float zTranslation;
float yRotationAngle;
float zRotationAngle;
float xRotationAngle;
int mouseLastx;
int mouseLasty;

// Light values and coordinates
GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat	 lightPos2[] = { -100.0f, 10.0f, 0.0f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  backdiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  diffref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

#define NUM_TEXTURES 5

GLuint  textureObjects[NUM_TEXTURES];
const char *szTextureFiles[] = { "bricks.tga", "grass.tga", "sun.tga", "roof.tga" };


float sunRotationAngle = 0;
float sunRadius = 150.0;
float day = 0;
float dusk = 1;

void user_input(unsigned char key, int x, int y)
{
	if (key == 13)
		started = true;
}
// To drew  bricks initially 
void drawbricks()
{
	glBegin(GL_QUADS);
	for (int a = 0; a < BRICK_COL; a++)
	{
		for (int b = 0; b < BRICK_ROW; b++)
		{
			switch (b)
			{
			case 0:
				glColor3f(1.0, 0.0, 0.0);
				break;
			case 1:
				glColor3f(1.0, 0.9, 0.1);
				break;
			case 2:
				glColor3f(0.11, 0.6, 0.2);
				break;
			case 3:
				glColor3f(0.11, 0.56, 1.0);
				break;
			case 4:
				glColor3f(0.11, 0.6, 0.2);
				break;
			case 5:
				glColor3f(1.0, 0.9, 0.1);
				break;
			default:
				glColor3f(1.0, 0.0, 0.0);
			}
			if (show[b][a] == true)
			{
				glVertex2f(x_brick[b][a] * win_aspect, y_brick[b][a]);
				glVertex2f(x_brick[b][a] * win_aspect, (y_brick[b][a] - 0.10));
				glVertex2f((x_brick[b][a] + 0.2)*win_aspect, (y_brick[b][a] - 0.10));
				glVertex2f((x_brick[b][a] + 0.2)*win_aspect, y_brick[b][a]);
			}
		}
	}
	glColor3f(0.0, 0.0, 0.0);
	glEnd();
}

// To rearrange the bricks 
void Set_Level(int level)
{
	int R_Limit = 0;
	switch (level)
	{
	case 1:
		for (int a = 0; a < BRICK_ROW; a++)
		{
			for (int b = 0; b < BRICK_COL; b++)
			{
				show[a][b] = 1;
			}
		}
		break;
	case 2:
		for (int a = 0; a < BRICK_ROW ; a++)
		{
			for (int b = 0; b < BRICK_COL; b++)
			{
				if (b >= BRICK_COL / 2 - R_Limit && b <= BRICK_COL / 2 + R_Limit)
					show[a][b] = true;
			}
			R_Limit++;
		}
		R_Limit = BRICK_ROW ;
		for (int a = BRICK_ROW ; a <= BRICK_ROW; a++)
		{
			for (int b = 0; b <= BRICK_COL; b++)
			{
				if (b >= BRICK_COL  - R_Limit && b <= BRICK_COL  + R_Limit)
					show[a][b] = true;
			}
			R_Limit--;
		}
		break;
	case 3:
		for (int a = 0; a <= BRICK_COL / 2; a++)
		{
			show[a][a] = true;
		}
		for (int a = BRICK_COL; a >= BRICK_COL / 2; a--)
		{
			show[BRICK_COL - a - 1][a] = true;
		}
		break;
	}
}
bool all_gone()
{
	int local_flag = 0;
	for (int a = 0; a < BRICK_COL; a++)
	{
		for (int b = 0; b < BRICK_ROW; b++)
		{
			if (show[b][a] == true)
			{
				return false;
				local_flag = 1;
				break;
			}
			if (local_flag)
				break;
		}
	}

	return true;
}
void drawball()
{
	
	/*glBegin(GL_LINES);
	glVertex3f(x_pos, y_pos, 0.0);
	glVertex3f(x_pos, y_pos+0.15, 0);
	glEnd();*/

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x_pos, y_pos);
	for (float angle = 0; angle < (10); angle += 0.01)
	{
		glVertex2f((x_pos + sin(angle) * RADIUS), (y_pos + (cos(angle)) * RADIUS));
	}
	glEnd();


}
bool crashed()
{
	if (y_pos < P_YPOS - 0.1)
		return true;
	else return false;
}
void ballmove() //moves ball+



{
	if (UP && RIGHT)
	{
		//x_pos += SPEED;
		y_pos += SPEED;
	}
	if (UP && LEFT)
	{
		x_pos -= SPEED;
		y_pos += SPEED;
	}
	if (DOWN && RIGHT)
	{
		x_pos += SPEED;
		y_pos -= SPEED;
	}
	if (DOWN && LEFT)
	{
		x_pos -= SPEED;
		y_pos -= SPEED;
	}
}
//to draw paddle 
void peddle_draw()
{
	glColor3f(1.5, 1.1, 0.2);
	glBegin(GL_QUADS);
	glVertex2f(P_XPOS - P_LENGTH, P_YPOS);
	glVertex2f(P_XPOS + P_LENGTH, P_YPOS);
	glVertex2f(P_XPOS + P_LENGTH, P_YPOS - 0.05);
	glVertex2f(P_XPOS - P_LENGTH, P_YPOS - 0.05);
	glEnd();

	
}

//to set bricks position 
void SET_BRICKS(int level)
{
	switch (level)
	{
	case 1:
		BRICK_ROW = 4;
		break;
	case 2:
		BRICK_ROW = 7;
		break;
	case 3:
		BRICK_ROW = 7;
		break;
	}
}
void gamesetup() //first screen
{
	const int   win_width = glutGet(GLUT_WINDOW_WIDTH),
		win_height = glutGet(GLUT_WINDOW_HEIGHT);
	win_aspect = (float)win_width / (float)win_height;
	glClearColor(0.5, 0.5, 0.5, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, win_width, win_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-win_aspect, win_aspect, -1, 1, -1, 1);
	drawball();
	drawbricks();
	peddle_draw();
	life = '0' + lives;
	if (started && lives > 0)
		ballmove();
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void GameLogic()
{
	if (all_gone())
	{
		x_pos = 0;
		y_pos = -0.7;
		LEVEL++;
		for (int a = 0; a < BRICK_ROW; a++)
		{
			for (int b = 0; b <BRICK_COL; b++)
			{
				show[a][b] = true;
			}
		}
		phit_center = false,
			phit_corner = false,
			game_over = false,
			RIGHT = true,
			LEFT = false,
			UP = true,
			DOWN = false,
			started = false,
			hit = false;
		SET_BRICKS(LEVEL);
		Set_Level(LEVEL);
	}
	if (x_pos >= win_aspect)
	{
		RIGHT = 0;
		LEFT = 1;
	}
	else if (x_pos <= -win_aspect)
	{
		RIGHT = 1;
		LEFT = 0;
	}
	if (y_pos >= 1 - RADIUS || hit)
	{
		UP = 0;
		DOWN = 1;
	}
	else if (y_pos <= -1 + RADIUS || hit)
	{
		UP = 1;
		DOWN = 0;
	}
	hit = false;
	int flag = 1;
	for (int a = 0; a < BRICK_COL; a++)
	{
		for (int b = 0; b < BRICK_ROW; b++)
		{
			if (x_pos >= x_brick[b][a] * win_aspect && x_pos <= (x_brick[b][a] + 0.2)*win_aspect)
			{
				if (y_pos <= y_brick[b][a] && y_pos >= y_brick[b][a] - 0.1)
				{
					if (show[b][a] == 1)
					{
						show[b][a] = 0;
						flag = 0;
						hit = true;
						break;
					}
					if (hit == true)
					{
						drawball();
					}
				}
			}
		}
		if (flag == 0)
			break;
	}
	phit_corner = false;
	phit_center = false;
	if (x_pos <= (P_XPOS + P_LENGTH - 0.05) && x_pos >= (P_XPOS - P_LENGTH - 0.05))
	{
		if (y_pos <= P_YPOS)
		{
			phit_center = true;
		}
	}
	else if ((x_pos >= (P_XPOS + P_LENGTH - 0.05) && x_pos <= (P_XPOS + P_LENGTH)) || (x_pos <= (P_XPOS - P_LENGTH + 0.05) && x_pos >= P_XPOS - P_LENGTH))
	{
		if (y_pos <= P_YPOS)
		{
			phit_corner = true;
		}
	}
	if (phit_center)
	{
		DOWN = false;
		UP = true;
	}
	if (phit_corner)
	{
		if (LEFT)
		{
			LEFT = false;
			RIGHT = true;
		}
		else
		{
			RIGHT = false;
			LEFT = true;
		}
		UP = false;
		DOWN = true;
	}
	if (crashed()) //reset everything
	{
		x_pos = 0;
		y_pos = -0.7;
		started = false;
		UP = true;
		RIGHT = true;
		DOWN = false;
		LEFT = false;
		P_XPOS = 0;
			P_YPOS = -0.8;
	}
	gamesetup();
}
void ArrowKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT && P_XPOS >= -0.9*win_aspect)
	{
		for (float a = 0; a < 0.05; a += 0.001)
		{
			P_XPOS -= 0.002;
			GameLogic();
			
		}
		if (!started)
			started = true;
	}
	if (key == GLUT_KEY_RIGHT && P_XPOS <= 0.9*win_aspect)
	{
		for (float a = 0; a < 0.05; a += 0.001)
		{
			P_XPOS += 0.002;
			GameLogic();
		}
		if (!started)
			started = true;
	}
}
void Init_Game()
{

	int c = 0;
	for (float a = -0.94; c < BRICK_COL; a += 0.21)
	{

		for (int b = 0; b <= BRICK_ROW; b++)
		{
			x_brick[b][c] = a;

		}
		c++;
	}
	c = 0;
	for (float a = 0.99; c < BRICK_ROW; a -= 0.11)
	{
		for (int b = 0; b < BRICK_COL; b++)
		{
			y_brick[c][b] = a;
		}
		c++;
	}
}
void ball_speed()
{
	clock_t start = clock();
	for (int a = 1; a < 99999999LLU; a += 1)
	{
		;
	}
	clock_t end = clock();
	SPEED = (double)(end - start) / CLOCKS_PER_SEC;
	SPEED /= 800;
}



int main(int argc, char **argv)
{
	
	ball_speed();
	Init_Game();
	SET_BRICKS(LEVEL);
	Set_Level(LEVEL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Brick Break");
	glutKeyboardFunc(user_input);
	glutSpecialFunc(ArrowKeys);
	
	glutIdleFunc(GameLogic);
	glutDisplayFunc(gamesetup);
	glClearColor(1,1,1,1);
	glutMainLoop();
	
	return 0;
}