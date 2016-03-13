#include "tree.h"

//ifndef abs
#define abs(a) ( ((a) > 0) ? (a) : -(a) ) 
//#endif

using namespace std;

int randomInteger(int low, int high)
{
	return low + rand() % (high - low + 1);
}

float randomReal(double low, double high)
{
	return low + static_cast<double>(rand()) / RAND_MAX * (high - low);
}

bool randomBool()
{
	return rand() < RAND_MAX / 2;
}

bool randomChance(double p) 
{
	return rand() < static_cast<int>(p * RAND_MAX);
}

Branch * branch = nullptr;
Leaf * leaf = nullptr;


void initObjects(Branch * pBranch, Leaf * pLeaf)
{
	branch = pBranch;
	leaf = pLeaf;
}


const float stepHeight = 0.1f;
// количество узлов в отрезке [0;1]
const int numberNodes = 5; 

const float minHeight = 0.5;

// ========================================================================================
// =================================== class BranchNode ===================================
// ========================================================================================
	// for usually branch
	// fi in [-PI/2;PI/2] ; pci in [-PI/2;PI/2]
	BranchNode::BranchNode(BranchNode* newParent, glm::mat4x4 & globalMatrix, MyObject * newObject) :
		parent(newParent), modelMatrix(globalMatrix), object(newObject),
		vNodes(vector<Node*>()), height(1.0), fi( randomReal(-90.0, 90.0) ),
		pci( randomReal(10.0, 90.0) ), alpha(parent->alpha)
	{
		// для угла pci от [-90;-10]
		if (randomBool()) pci *= -1;
		//f ( abs(alpha + pci) > 90.0 ) pci += pci > 0.0 ? -90.0 : 90.0;
		//alpha += pci;
		//cout << "alpha = " << alpha << endl;
		glm::mat4x4 tryModelMatrix = glm::rotate(modelMatrix, fi, glm::vec3(0.0f,1.0f,0.0f) );
		tryModelMatrix = glm::rotate(tryModelMatrix, pci, glm::vec3(0.0f,0.0f,1.0f) );

		glm::vec4 vBegin = tryModelMatrix * glm::vec4(0.0f,0.0f,0.0f,1.0f);
		glm::vec4 vEnd = tryModelMatrix * glm::vec4(0.0f,1.0f,0.0f,1.0f);
		//cout << vBegin.y << " " << vEnd.y << endl;
		if (vBegin.y > vEnd.y) 
		{
			pci *= -1;
		}
		// fi
		modelMatrix = glm::rotate(modelMatrix, fi, glm::vec3(0.0f,1.0f,0.0f) );
		// pci
		modelMatrix = glm::rotate(modelMatrix, pci, glm::vec3(0.0f,0.0f,1.0f) );
		//cout << "v1 = " << v1[0] << ' ' << v1[1] << ' ' << v1[2] << ' ' << v1[3] << endl;
		//cout << "v2 = " << v2[0] << ' ' << v2[1] << ' ' << v2[2] << ' ' << v2[3] << endl;
	}

	// for main branch
	BranchNode::BranchNode(MyObject * newObject) :
		parent(nullptr), modelMatrix(glm::mat4()), object(newObject), 
		vNodes(vector<Node*>()), height(1.0), fi(0.0), pci(0.0), alpha(pci)
	{
		// fi
		modelMatrix = glm::rotate(modelMatrix, fi, glm::vec3(0.0f,1.0f,0.0f) );
		// pci
		modelMatrix = glm::rotate(modelMatrix, pci, glm::vec3(0.0f,0.0f,1.0f) );
	}



	void BranchNode::draw(glm::mat4x4 modelGlobalMatrix, glm::mat4x4 & viewMatrix, 
		glm::mat4x4 & projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor,
		int locMV, int locN, int locP, int texLoc, int locFlag, GLuint * texId,
		 int lightPos, int lightCol)
	{

		//modelGlobalMatrix *= modelMatrix;
		glm::mat4x4 drawModelMatrix = glm::scale(modelMatrix, 
			glm::vec3(1.0f + height/500,height,1.0f + height/500) );
		//modelViewMatrix consists of viewMatrix and modelMatrix
		glm::mat4x4 modelViewMatrix = viewMatrix*drawModelMatrix;
		//calculate normal matrix 
		glm::mat4x4 normalMatrix = glm::inverseTranspose(modelViewMatrix);
		//finally calculate modelViewProjectionMatrix
		glm::mat4x4 modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
		//bind texture
		//glBindTexture(GL_TEXTURE_2D,texId[0]);
		
		//pass variables to the shaders
		glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));
		glUniform1ui(texLoc,0);
		glUniform1i(locFlag,false);
		glUniform3fv(lightPos,1, glm::value_ptr(lightPosition));
		glUniform3fv(lightCol,1,glm::value_ptr(lightColor));

		//draw branch
		object->draw();
		//cout << (object == nullptr);
		for (size_t i = 0; i < vNodes.size(); ++i)
			if (vNodes[i]) 
			{
				float step = (i+1) / static_cast<float>(numberNodes);
				glm::mat4x4 tmpMatrix = glm::translate(modelGlobalMatrix, 
					glm::vec3(0.0f,0.5f+step,0.0f) );
				vNodes[i]->draw(tmpMatrix, viewMatrix, projectionMatrix, lightPosition,
					lightColor,
					locMV, locN, locP, texLoc, locFlag, texId, lightPos, lightCol);
			}
	}

	void BranchNode::grow(int & nBranch, int & nLeaf)
	{
		height += stepHeight;
		while ( static_cast<size_t>( (height-0.5) * numberNodes) >= vNodes.size() )
			vNodes.push_back(nullptr);

		for (size_t i = 0; i < vNodes.size(); ++i)
			if (vNodes[i]) vNodes[i]->grow(nBranch, nLeaf);
		if (true) //(randomBool())
		{
			const double chanceBranch = 1.0 / 7;
			// branch is going to grow
			int number = randomInteger(0, vNodes.size()-1);
			if (vNodes[number] != nullptr ) return;
			glm::mat4x4 tmp = glm::translate(modelMatrix, glm::vec3(0.0f, 
					0.5f + static_cast<float>(number)/numberNodes, 0.0f) );
			if (randomChance(chanceBranch))
			{
				vNodes[number] = new BranchNode(this, tmp, branch);
				++nBranch;
			}
			else
			{
				vNodes[number] = new LeafNode(this, tmp, leaf);
				++nLeaf;
			}
		}
	}


// ========================================================================================
// ==================================== class LeafNode ====================================
// ========================================================================================
	LeafNode::LeafNode(BranchNode* newParent, glm::mat4x4 & globalMatrix, MyObject * newObject) :
		parent(newParent), modelMatrix(globalMatrix), object(newObject),
		fi( randomReal(-180.0, 180.0) ), pci( randomReal(10.0, 80.0) )
	{	
		// либо для отрицательных значений pci
		if (randomBool())
			pci *= -1;
		// fi
		modelMatrix = glm::rotate(modelMatrix, fi, glm::vec3(0.0f,1.0f,0.0f) );
		// pci
		modelMatrix = glm::rotate(modelMatrix, pci, glm::vec3(0.0f,0.0f,1.0f) );
	}

	void LeafNode::draw(glm::mat4x4 modelGlobalMatrix, glm::mat4x4 & viewMatrix, 
		glm::mat4x4 & projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor,
		int locMV, int locN, int locP, int texLoc, int locFlag, GLuint * texId,
		int lightPos, int lightCol)
	{
		// повернем лист на 90 градусов вокруг своей оси (чтобы лист был открыт для солнца)
		glm::mat4x4 drawModelMatrix = glm::rotate(modelMatrix, 90.0f,
			 glm::vec3(0.0f, 1.0f, 0.0f));
		// делаем лист маленьким
		drawModelMatrix = glm::scale(drawModelMatrix, 0.3f*glm::vec3(1.0f,1.0f,1.0f));
		//modelViewMatrix consists of viewMatrix and modelMatrix
		glm::mat4x4 modelViewMatrix = viewMatrix*drawModelMatrix;
		//calculate normal matrix 
		glm::mat4x4 normalMatrix = glm::inverseTranspose(modelViewMatrix);
		//finally calculate modelViewProjectionMatrix
		glm::mat4x4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
		
		//pass variables to the shaders
		glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));
		glUniform1ui(texLoc,0);
		glUniform1i(locFlag,true);
		glUniform3fv(lightPos,1, glm::value_ptr(lightPosition));
		glUniform3fv(lightCol,1,glm::value_ptr(lightColor));

		//draw leaf
		object->draw();
	}

	void LeafNode::grow(int & nBranch, int & nLeaf) {}