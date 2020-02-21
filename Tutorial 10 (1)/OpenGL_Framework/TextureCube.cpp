#include "TextureCube.h"
#include <assert.h>
#include "IO.h"
#include "SOIL/SOIL.h"

TextureCube::TextureCube(const std::vector<std::string>& file)
{
	load(file);
}


bool TextureCube::load(const std::vector<std::string>& file, bool mipmap)
{
	assert(file.size() == 6);

	_Target = GL_TEXTURE_CUBE_MAP;
	_InternalFormat = GL_RG8;

	glGenTextures(1, &this->_TexHandle);
	bind();

	std::vector<unsigned char*> textureData(6);

	for (int i = 0; i < 6; i++)
	{
		textureData[i] = SOIL_load_image(("../assets/textures/cubemap/" + file[i]).c_str(),
			&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGB);

		if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
		{
			SAT_DEBUG_LOG_ERROR("TEXTURE BROKE: %s", file[i].c_str());
			return false;
		}

		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y * 2 < sizeY; y++)
			{
				for (int c = 0; c < 3; c++)
				{
					std::swap(textureData[i][(x + (y*sizeX)) * 3 + c],
						textureData[i][(x + ((sizeY - y - 1)*sizeX)) * 3 + c]);
				}
			}
		}
	}

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData[i]
		);
		SOIL_free_image_data(textureData[i]);
	}
	if (mipmap)
	{
		generateMipMaps();
	}

	_Wrap.x = GL_CLAMP_TO_EDGE;
	_Wrap.y = GL_CLAMP_TO_EDGE;


	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_S, this->_Wrap.x);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_T, this->_Wrap.y);

	this->unbind();
	return true;
}
