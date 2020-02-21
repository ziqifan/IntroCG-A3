#include "Mesh.h"
#include <vector>
#include "Math.h"

void Mesh::initMeshSphere(const unsigned int xSlices, const unsigned int ySlices, bool invert)
{
	std::vector<vec4> vertexData;
	std::vector<vec4> textureData;
	std::vector<vec4> normalData;
	
	float x = (float)xSlices;
	float y = (float)ySlices;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			float s0 = sin(((j + 0) / y)* PI);
			float s1 = sin(((j + 1) / y)* PI);
		
			float c0 = cos((2.0f * (j + 0) / y - 1.0f)* PI * 0.5f + PI * 0.5f);
			float c1 = cos((2.0f * (j + 1) / y - 1.0f)* PI * 0.5f + PI * 0.5f);

			vec4 blPos = vec4(s0 * sin((i + 0) * PI*2.0f / x), c0, s0 * cos((i + 0) * PI*2.0f / x), 0.0f);
			vec4 brPos = vec4(s0 * sin((i + 1) * PI*2.0f / x), c0, s0 * cos((i + 1) * PI*2.0f / x), 0.0f);
			vec4 tlPos = vec4(s1 * sin((i + 0) * PI*2.0f / x), c1, s1 * cos((i + 0) * PI*2.0f / x), 0.0f);
			vec4 trPos = vec4(s1 * sin((i + 1) * PI*2.0f / x), c1, s1 * cos((i + 1) * PI*2.0f / x), 0.0f);

			//s0 = 1.0f;
			//s1 = 1.0f;
			if (!invert)
			{
				vertexData.push_back(blPos);
				vertexData.push_back(tlPos);
				vertexData.push_back(brPos);
				vertexData.push_back(tlPos);
				vertexData.push_back(trPos);
				vertexData.push_back(brPos);
			}
			else
			{
				vertexData.push_back(blPos);
				vertexData.push_back(brPos);
				vertexData.push_back(tlPos);
				vertexData.push_back(tlPos);
				vertexData.push_back(brPos);
				vertexData.push_back(trPos);
			}

			vec4 bl = vec4((i    ) / x, 1.f - (j    ) / y, 0.f, 0.f);
			vec4 br = vec4((i + 1) / x, 1.f - (j    ) / y, 0.f, 0.f);	
			vec4 tl = vec4((i    ) / x, 1.f - (j + 1) / y, 0.f, 0.f);
			vec4 tr = vec4((i + 1) / x, 1.f - (j + 1) / y, 0.f, 0.f);

			if (!invert)
			{
				textureData.push_back(bl);
				textureData.push_back(tl);
				textureData.push_back(br);
				textureData.push_back(tl);
				textureData.push_back(tr);
				textureData.push_back(br);
			}
			else
			{
				textureData.push_back(bl);
				textureData.push_back(br);
				textureData.push_back(tl);
				textureData.push_back(tl);
				textureData.push_back(br);
				textureData.push_back(tr);
			}
			
			if (!invert)
			{
				normalData.push_back(vertexData[vertexData.size() - 6].GetNormalized());
				normalData.push_back(vertexData[vertexData.size() - 5].GetNormalized());
				normalData.push_back(vertexData[vertexData.size() - 4].GetNormalized());
				normalData.push_back(vertexData[vertexData.size() - 3].GetNormalized());
				normalData.push_back(vertexData[vertexData.size() - 2].GetNormalized());
				normalData.push_back(vertexData[vertexData.size() - 1].GetNormalized());
			}
			else
			{
				normalData.push_back(-vertexData[vertexData.size() - 6].GetNormalized());
				normalData.push_back(-vertexData[vertexData.size() - 5].GetNormalized());
				normalData.push_back(-vertexData[vertexData.size() - 4].GetNormalized());
				normalData.push_back(-vertexData[vertexData.size() - 3].GetNormalized());
				normalData.push_back(-vertexData[vertexData.size() - 2].GetNormalized());
				normalData.push_back(-vertexData[vertexData.size() - 1].GetNormalized());
			}
			
		}
	}

	uploadToGPU(&vertexData, &textureData, &normalData);
}

void Mesh::initMeshPlane(const unsigned int xDivisions, const unsigned int yDivisions)
{
	std::vector<vec4> vertexData;
	std::vector<vec4> textureData;
	std::vector<vec4> normalData;

	float x = (float)xDivisions;
	float y = (float)yDivisions;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			vertexData.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));
			vertexData.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			vertexData.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));
			vertexData.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			vertexData.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			vertexData.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));

			vec4 bl = vec4((i) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 br = vec4((i + 1) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 tl = vec4((i) / x, 1.f - (j + 1) / y, 0.f, 0.f);
			vec4 tr = vec4((i + 1) / x, 1.f - (j + 1) / y, 0.f, 0.f);

			textureData.push_back(bl);
			textureData.push_back(tl);
			textureData.push_back(br);
			textureData.push_back(tl);
			textureData.push_back(tr);
			textureData.push_back(br);

			normalData.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			normalData.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			normalData.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			normalData.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			normalData.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			normalData.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
		}
	}

	uploadToGPU(&vertexData, &textureData, &normalData);
}



void Mesh::bind() const
{
	glBindVertexArray(VAO);
}

void Mesh::unbind()
{
	glBindVertexArray(GL_NONE);
}

void Mesh::draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, _NumVert);
}

void Mesh::uploadToGPU(const std::vector<vec4>* vertex, const std::vector<vec4>* texture, const std::vector<vec4>* normal)
{
	_NumVert = vertex->size();
	_NumFaces = _NumVert / 3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Vert);
	glGenBuffers(1, &VBO_UV);
	glGenBuffers(1, &VBO_Normal);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vertex->size(), &vertex[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), reinterpret_cast<void*>(0));

	//Uvs
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * texture->size(), &texture[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), reinterpret_cast<void*>(0));

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * normal->size(), &normal[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), reinterpret_cast<void*>(0));
}
