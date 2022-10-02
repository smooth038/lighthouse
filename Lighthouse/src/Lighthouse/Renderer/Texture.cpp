#include "lhpch.h"

#include "Texture.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <stb_image.h>

namespace Lighthouse
{

	Texture::Texture(const std::string& filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			LH_CORE_ERROR("Unable to load texture!");
		}
		_width = width;
		_height = height;

		GLenum dataFormat;
		GLenum internalFormat;
		if (channels == 4) {
			dataFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else if (channels == 3) {
			dataFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else
		{
			LH_CORE_ERROR("Channel count should be 3 or 4 but was {0}.", channels);
			throw std::invalid_argument("Wrong number of channels");
		}

		_textureStore = Renderer::getShader()->getProgramId();
		glGenTextures(1, &_textureStore);
		glBindTexture(GL_TEXTURE_2D, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			LH_CORE_ERROR("Error detected: {0}", err);
		}

		if (data)
		{
			stbi_image_free(data);
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_textureStore);
	}

	void Texture::bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _textureStore);
	}

	int Texture::getWidth()
	{
		return _width;
	}

	int Texture::getHeight()
	{
		return _height;
	}

}