#pragma once

#include "lhpch.h"

namespace Lighthouse
{

	class LH_API Texture
	{
	public:
		Texture(const std::string& filepath);
		~Texture();

		void bind(unsigned int slot = 0);

		int getWidth();
		int getHeight();

	private:
		std::string _filepath;
		unsigned int _width, _height;

		unsigned int _textureStore;
	};

}

