#pragma once

#include "lhpch.h"

namespace Lighthouse
{

	class Texture
	{
	public:
		Texture(const std::string& filepath, unsigned int slot);
		~Texture();

		void bind(unsigned int slot = 0);

		int getWidth();
		int getHeight();

	private:
		std::string _filepath;
		unsigned int _width, _height;

		unsigned int _textureId;
	};

}

