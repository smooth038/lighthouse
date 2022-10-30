#pragma once

#include "lhpch.h"
#include <glm/glm.hpp>

namespace Lighthouse
{
	class Camera {
	public:
		Camera();
		Camera(unsigned int width, unsigned int height);
		~Camera() = default;
		
		// movements
		void moveForward();
		void moveBackward();
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void look(float mouseX, float mouseY);

		void initMove();

		void setAngle(glm::vec3& eye, glm::vec3& center, glm::vec3& up);

		void setProjection();
	private:
		unsigned int _width;
		unsigned int _height;

		glm::mat4 _projection = glm::mat4(1.0f);
		glm::vec3 _eye = glm::vec3(0.0f, 0.0f, 2.0f);
		glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);

		float _speed = 0.3f;
		float _sensitivity = 100.0f;
	};
}
