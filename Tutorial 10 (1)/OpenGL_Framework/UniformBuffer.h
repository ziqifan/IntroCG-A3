#pragma once
#include <GL/glew.h>
#include <MiniMath/Matrix.h>
#include <MiniMath/Vector.h>

class UniformBuffer
{
public:
	UniformBuffer();

	void allocateMemory(unsigned int bytes);
	void sendMatrix(mat4 matrix, int offset);
	void sendVector(vec3 vector, int offset);
	void sendFloat(float scalar, int offset);
	void bind(GLuint slot);

private:
	void bind();
	void unbind();
	bool _IsInit = false;
	GLuint _UBO;
	GLuint _BindLocation;
};