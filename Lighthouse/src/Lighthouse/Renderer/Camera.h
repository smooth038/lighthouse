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

		glm::vec3 getEyeVector();
		glm::vec3 getOrientationVector();
		glm::vec3 getUpVector();
		glm::mat4 getCameraMatrix();

		float getFov();
		float getZFar();
		float getZNear();

		void setAngle(glm::vec3& eye, glm::vec3& center, glm::vec3& up);

		void setProjection();
	private:
		unsigned int _width;
		unsigned int _height;

		glm::vec3 _eye = glm::vec3(0.0f, 0.0f, 2.0f);
		glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 _cameraMatrix = glm::mat4(1.0f);

		float _fov = 70.0f;
		float _zFar = 100.0f;
		float _zNear = 0.1f;

		float _speed = 0.3f;
		float _sensitivity = 300.0f;
	};
}
