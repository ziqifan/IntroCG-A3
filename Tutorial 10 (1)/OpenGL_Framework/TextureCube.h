#pragma once
#include "Texture.h"
#include <vector>

class TextureCube : public Texture
{
public:
	TextureCube() = default;
	TextureCube(const std::string &file) = delete;
	TextureCube(const std::vector<std::string> &file);
	bool load(const std::vector<std::string> &file, bool mipmap = true);
};
