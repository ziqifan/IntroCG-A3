#pragma once
#include "GL/glew.h"
#include "Transform.h"
#include <vector>

class Mesh
{
public:
	void initMeshSphere(const unsigned int xSlices, const unsigned int ySlices, bool invert = false);
	void initMeshPlane(const unsigned int xDivisions, const unsigned int yDivisions);

	void draw() const;
	void Mesh::bind() const;
	static void Mesh::unbind();
private:
	GLuint VAO = 0;
	
	GLuint VBO_Vert   = 0;
	GLuint VBO_UV     = 0;
	GLuint VBO_Normal = 0;

	unsigned int _NumFaces = 0;
	unsigned int _NumVert = 0;

	void uploadToGPU(const std::vector<vec4> *vertex, const std::vector<vec4> *texture, const std::vector<vec4> *normal);
};