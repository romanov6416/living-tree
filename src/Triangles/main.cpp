//standard libraries
#include <iostream>
using namespace std;

//opengl headers
#include <GL/glew.h>
#include <GL/freeglut.h>

//opengl mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//functions for shader compilation and linking
#include "shaderhelper.h"
//object for drawing
#include "Branch.h"
#include "Leaf.h"
#include "tree.h"

#include "camera.h"

//model for drawing: a square from two triangles
Branch* pBranch = nullptr;
Leaf* pLeaf = nullptr;
BranchNode * tree = nullptr;

//struct for loading shaders
ShaderProgram shaderProgram;

//window size
int windowWidth = 800;
int windowHeight = 600;

//last mouse coordinates
int mouseX,mouseY;
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f),
			  glm::vec3(0.0f, 7.0f, 0.0f),
			  glm::vec3(0.0f, 1.0f, 0.0f));

//camera position
glm::vec3 eye(0,0,20);
//reference point position
glm::vec3 cen(0,7,0);
//up vector direction (head of observer)
glm::vec3 up(0,1,0);

// освещение
glm::vec3 lightPosition(0.0f,10.0f, 10.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);



//matrices
glm::mat4x4 modelMatrix;
glm::mat4x4 modelViewMatrix;
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelViewProjectionMatrix;
glm::mat4x4 normalMatrix;

///defines drawing mode
bool useTexture = true;

//texture identificator
GLuint texId[1];

//names of shader files. program will search for them during execution
//don't forget place it near executable 
char VertexShaderName[] = "../src/Shaders/Vertex.vert";
char FragmentShaderName[] = "../src/Shaders/Fragment.frag";

int leafNumber = 0;
int branchNumber = 0;

////////////////////////////////////////////////////////
///
void initTexture()
{
		//generate as many textures as you need
	glGenTextures(1,&texId[0]);
	
	//enable texturing and zero slot
	glActiveTexture(GL_TEXTURE0);

	//bind texId to 0 unit
	glBindTexture(GL_TEXTURE_2D,texId[0]);

	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
	// Set nearest filtering mode for texture minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//TODO: load texture from file 
	GLubyte imgDataLeaf[2*2*3] = {
		//row1: yellow,orange
		255,255,0, 255,128,0,
		//row2: green, dark green
		0,255,0, 0,64,0
	};
	
	//set Texture Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2,2, 0, GL_RGB, GL_UNSIGNED_BYTE, &imgDataLeaf[0]);
}

void initLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

}

/////////////////////////////////////////////////////////////////////
///is called when program starts
void init()
{
	//enable depth test
	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	//initialize shader program
	shaderProgram.init(VertexShaderName, FragmentShaderName);
	//use this shader program
	glUseProgram(shaderProgram.programObject);

	//create new branch
	pBranch = new Branch();
	//fill in data
	pBranch->initData();

	//initialize opengl buffers and variables inside of object
	pBranch->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");

	
	//create new leaf
	pLeaf = new Leaf();
	//fill in data
	pLeaf->initData();
	//initialize opengl buffers and variables inside of object
	pLeaf->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");

	//initialize texture
	initTexture();
	// initialize light
	initLight();

	// инициализация объектов для построения дерева (просто копирование указателей)
	initObjects(pBranch, pLeaf);

	tree = new BranchNode(pBranch);
	while (leafNumber < 500 || branchNumber < 100)
		tree->grow(branchNumber, leafNumber);
}


/////////////////////////////////////////////////////////////////////
///called when window size is changed
void reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	//set viewport to match window size
	glViewport(0, 0, width, height);
	
	float fieldOfView = 45.0f;
	float aspectRatio = float(width)/float(height);
	float zNear = 0.1f;
	float zFar = 100.0f;
	//set projection matrix
	projectionMatrix = glm::perspective(fieldOfView,aspectRatio,zNear,zFar);
}

////////////////////////////////////////////////////////////////////
///actions for single frame
void display()
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Draw triangle with shaders (in screen coordinates)
	//need to set uniform in modelViewMatrix
	
	glUseProgram(shaderProgram.programObject);

	//we will need this uniform locations to connect them to our variables
	int locMV = glGetUniformLocation(shaderProgram.programObject,"modelViewMatrix");
	int locN = glGetUniformLocation(shaderProgram.programObject,"normalMatrix");
	int locP = glGetUniformLocation(shaderProgram.programObject,"modelViewProjectionMatrix");
	int texLoc = glGetUniformLocation(shaderProgram.programObject,"textureSampler");
	int locFlag = glGetUniformLocation(shaderProgram.programObject,"useTexture");

	int lightPos = glGetUniformLocation(shaderProgram.programObject, "lightPos");
	int lightCol = glGetUniformLocation(shaderProgram.programObject, "lightCol");

	//if there is some problem to exit
	if (locMV<0 || locN<0 || locP<0 || texLoc <0 || locFlag<0 || lightPos<0 || lightCol<0)
	{
		//not all uniforms were allocated - show blue screen.
		//check your variables properly. May be there is unused?
		glClearColor(0,0,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		//end frame visualization
		glutSwapBuffers();
		return;
	}

	//camera matrix. camera is placed in point "eye" and looks at point "cen".
//	glm::mat4x4 viewMatrix = glm::lookAt(eye,cen,up);

	// lightning
	GLfloat material_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

	GLfloat light1_diffuse[] = {0.4, 0.7, 0.2};
	GLfloat light1_position[] = {0.0, 0.0, 1.0, 1.0};
	//glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	// источник света в координатах камеры
	glm::vec4 tmp(lightPosition, 1.0f);
	tmp = camera.get_view_matrix() * tmp;
	glm::vec3 lightPos_camera;
	for (int i = 0; i < 3; ++i)
        lightPos_camera[i] = tmp[i];
	glm::mat4x4 view_matrix = camera.get_view_matrix();
	tree->draw(glm::mat4(), view_matrix, projectionMatrix, lightPos_camera, lightColor,
               locMV, locN, locP, texLoc, locFlag, texId, lightPos, lightCol);



	glDisable(GL_LIGHT1);
	//glBindTexture(GL_TEXTURE_2D,texId[0]);
	//end frame visualization
	glutSwapBuffers();
	
}

//////////////////////////////////////////////////////////////////////////
///IdleFunction
void update()
{
	//make animation
	glutPostRedisplay();
}


//inline float module(const glm::vec3 & v)
//{
//	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
//}

int n = 0;
void keyboard(unsigned char key, int mx, int my)
{
	switch(key)
	{
		case ' ':
			useTexture = !useTexture;
			break;
		case 'q': case 'Q':
			camera.zoom_out(1.0f);
			break;
		case 'e': case 'E':
			camera.zoom_in(1.0f);
			break;
		case 'a': case 'A':
			camera.rotate_left(M_PI/8);
			break;
		case 'd': case 'D':
			camera.rotate_right(M_PI/8);
			break;
		case 'w': case 'W':
			camera.rotate_up(M_PI/8);
			break;
		case 's': case 'S':
			camera.rotate_down(M_PI/8);
			break;
		case 't': case 'T':
			camera.shift_up(3);
			break;
		case 'g': case 'G':
			camera.shift_down(3);
			break;
		case 'f': case 'F':
			camera.shift_left(3);
			break;
		case 'h': case 'H':
			camera.shift_right(3);
			break;
		case '*':
			tree->grow(branchNumber, leafNumber);
			cout << "Branchs = " << branchNumber << endl;
			cout << "Leafs = " << leafNumber << endl;
			break;
		default:
			break;
	}
}

/////////////////////////////////////////////////////////////////////////
///is called when mouse button is pressed
///TODO: place camera rotations in this function
void mouse(int button, int mode, int posx, int posy) {
	cout << button << " " << mode << " " << posx << " " << posy << " " << endl;
	if (button == GLUT_LEFT_BUTTON) {
		if (mode == GLUT_DOWN) {
			mouseX = posx;
			mouseY = posy;
		}
		else {
			mouseX = -1;
			mouseY = -1;
		}
	}
}

void active_mouse(int posx, int posy) {
	cout << posx << " " << posy << endl;
	return;
}


////////////////////////////////////////////////////////////////////////
///this function is used in case of InitializationError
void emptydisplay()
{
}


// сделать источник света структурой и подать этот в фрагментный шейдер
// 


////////////////////////////////////////////////////////////////////////
///entry point
int main (int argc, char* argv[])
{
	glutInit(&argc, argv);
#ifdef __APPLE__
	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE| GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitContextVersion (3, 2);  
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags (GLUT_FORWARD_COMPATIBLE);
	glewExperimental = GL_TRUE;
#endif

	glutCreateWindow("Test OpenGL application");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutReshapeWindow(windowWidth,windowHeight);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(active_mouse);



	glewInit();
	const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );
	cout << "GLSL Version: " << slVer << endl;

	try
	{
	init();
	}
	catch (const char *str)
	{
		cout << "Error During Initialiation: " << str << endl;
		delete pBranch;
		delete pLeaf;
		glDeleteTextures(1,texId);
		//start main loop with empty screen
		glutDisplayFunc(emptydisplay);
		glutMainLoop();
		return -1;
	}


	try
	{
	glutMainLoop();
	}
	catch (const char *str)
	{
		cout << "Error During Main Loop: " << str << endl;
	}
	//release memory
	delete pBranch;
	delete pLeaf;

	glDeleteTextures(1,texId);
	return 0;
}
