#pragma comment(lib, "SOIL_ext.lib")

#include "Texture.h"
#include "SOIL/SOIL.h"
#include "IO.h"

float Texture::anisotropyAmount = 1.0f; 
GLenum Texture::magFilterOverride = GL_NEAREST;
GLenum Texture::minFilterOverride = GL_NEAREST;

GLenum filterModes[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};

Texture::Texture(const std::string & file)
{
	this->load(file);
}

Texture::~Texture()
{
	this->unload();
}

bool Texture::load(const std::string & file, bool mipmap)
{
	this->filename = "../assets/textures/" + file;

	unsigned char* textureData = SOIL_load_image((this->filename).c_str(),
		&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		SAT_DEBUG_LOG_ERROR("TEXTURE BROKE: %s", this->filename.c_str());
		return false;
	}
	
	// If the texture is 2D, set it to be a 2D texture;
	_Target = GL_TEXTURE_2D;
	_InternalFormat = GL_RGBA8;	

	int levels = countMipMapLevels(mipmap);
	
	glGenTextures(1, &this->_TexHandle);
	this->bind();
	glTextureStorage2D(this->_TexHandle, levels, this->_InternalFormat, this->sizeX, this->sizeY);
	glTextureSubImage2D(this->_TexHandle, 0, // We are editing the first and only layer in memory
		0, 0, // No offset
		this->sizeX, this->sizeY, // the dimensions of our image loaded
		GL_RGBA, GL_UNSIGNED_BYTE, // Data format and type
		textureData); // Pointer to the texture data
	
	generateMipMaps();

	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_S, this->_Wrap.x);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_T, this->_Wrap.y);

	this->unbind();
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::unload()
{
	if (this->_TexHandle)
	{
		glDeleteTextures(1, &this->_TexHandle);
		return true;
	}
	return false;
}

void Texture::createTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void * newDataPtr)
{
	sizeX = w;
	sizeY = h;
	_Filter.mag = filtering;
	_Wrap.x = edgeBehaviour;
	_Wrap.y = edgeBehaviour;
	_InternalFormat = internalFormat;
	_Target = target;

	GLenum error = 0;
	
	unload();

	glGenTextures(1, &_TexHandle);
	glBindTexture(target, _TexHandle);
	error = glGetError();

	glTexParameteri(_Target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(_Target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_S, edgeBehaviour);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_T, edgeBehaviour);
	error = glGetError();

	glTexImage2D(_Target, 0, internalFormat, w, h, 0, textureFormat, dataType, newDataPtr);
	error = glGetError();

	if (error != 0)
	{
		SAT_DEBUG_LOG_ERROR("[Texture.cpp : createTexture] Error when creating texture. ");
	}

	glBindTexture(_Target, 0);
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_TexHandle);

	// BAD COD
	glTexParameteri(this->_Target, GL_TEXTURE_MIN_FILTER, minFilterOverride);
	glTexParameteri(this->_Target, GL_TEXTURE_MAG_FILTER, magFilterOverride);
	glTexParameterf(this->_Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, Texture::anisotropyAmount);
	// BAD COD

}

void Texture::bind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->bind();
}

void Texture::unbind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unbind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->unbind();
}

GLuint Texture::getID()
{
	return _TexHandle;
}

int Texture::countMipMapLevels(bool mipmap)
{
	int levels = 1;
	if (mipmap)
	{
		float largest = static_cast<float>(max(sizeX, sizeY));
		levels += static_cast<int>(std::log2(largest));
	}
	return levels;
}

void Texture::generateMipMaps()
{
	glGenerateMipmap(_TexHandle);
	glTextureParameterf(this->_TexHandle, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyAmount);
}

TextureWrap::TextureWrap()
{
	x = GL_REPEAT;
	y = GL_REPEAT;
	z = GL_REPEAT;
}
