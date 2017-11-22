#pragma once
#include <string>

namespace Textures
{
	struct Texture
	{
		int textureId;
		int left;
		int top;
		int width;
		int height;
		sf::Texture sfmlTexture;
	};
}