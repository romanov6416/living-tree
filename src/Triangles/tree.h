#ifndef TREE_H
#define TREE_H

//standard libraries
#include <iostream>
#include <vector>
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
#include "MyObject.h"
#include "Branch.h"
#include "Leaf.h"

void initObjects(Branch*, Leaf*);
void deleteObjects();

class Node
{
public:
	virtual void draw(glm::mat4x4 modelGlobalMatrix, glm::mat4x4 & viewMatrix, 
		glm::mat4x4 & projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor, 
		int locMV, int locN, int locP, int texLoc, int locFlag, GLuint * texId,
		int lightPos, int lightCol) = 0;
	virtual void grow(int & nBranch, int & nLeaf) = 0;
};


class BranchNode: public Node
{
	BranchNode* parent;
	glm::mat4x4 modelMatrix;
	MyObject * object;
	std::vector<Node*> vNodes;
	float height;
	float fi;
	float pci;
	float alpha; // angle between axis OY and branch
public:
	// for usually branch
	BranchNode(BranchNode* newParent, glm::mat4x4 & globalMatrix, MyObject * newObject);
	// for main branch
	BranchNode(MyObject * newObject);
	virtual void draw(glm::mat4x4 modelGlobalMatrix, glm::mat4x4 & viewMatrix, 
		glm::mat4x4 & projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor, 
		int locMV, int locN, int locP, int texLoc, int locFlag, GLuint * texId,
		int lightPos, int lightCol);
	virtual void grow(int & nBranch, int & nLeaf);
};

class LeafNode: public Node
{
	BranchNode* parent;
	glm::mat4x4 modelMatrix;
	MyObject * object;
	float fi;
	float pci;
public:

	LeafNode(BranchNode* newParent, glm::mat4x4 & globalMatrix, MyObject * newObject);
	virtual void draw(glm::mat4x4 modelGlobalMatrix, glm::mat4x4 & viewMatrix, 
		glm::mat4x4 & projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor, 
		int locMV, int locN, int locP, int texLoc, int locFlag, GLuint * texId,
		int lightPos, int lightCol);
	virtual void grow(int & nBranch, int & nLeaf);
};
#endif