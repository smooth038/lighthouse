#include "lhpch.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Lighthouse
{

	Camera::Camera()
		: _width(800), _height(600)
	{
	}

	Camera::Camera(unsigned int width, unsigned int height)
		: _width(width), _height(height)
	{
	}

	void Camera::setAngle(glm::vec3& eye, glm::vec3& center, glm::vec3& up)
	{
		_eye = eye;
		_center = center;
		_up = up;

		setProjection();
	}

	void Camera::setProjection()
	{
		float w = static_cast<float>(_width);
		float h = static_cast<float>(_height);
		float fov = 90.0f;
		float zFar = 100.0f;
		float zNear = 0.1f;
		float aspectRatio = w / h;

		glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
		glm::mat4 view = glm::lookAt(_eye, _center, _up);
		
		Renderer::getShader()->setUniformMat4f("u_camera", projection * view);
	}

}