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

	void Camera::moveForward()
	{
		_eye += _speed * _orientation;
	}

	void Camera::moveBackward()
	{
		_eye -= _speed * _orientation;
	}

	void Camera::moveLeft()
	{
		_eye -= _speed * glm::normalize(glm::cross(_orientation, _up));
	}

	void Camera::moveRight()
	{
		_eye += _speed * glm::normalize(glm::cross(_orientation, _up));
	}

	void Camera::moveUp()
	{
		_eye += _speed * _up;
	}

	void Camera::moveDown()
	{
		_eye -= _speed * _up;
	}

	void Camera::look(float mouseX, float mouseY)
	{
		float rotX = _sensitivity * (mouseY - static_cast<float>(_height / 2)) / _height;
		float rotY = _sensitivity * (mouseX - static_cast<float>(_width / 2)) / _width;

		glm::mat4 rotationMat(1);
		rotationMat = glm::rotate(rotationMat, glm::radians(-rotX), glm::normalize(glm::cross(_orientation, _up)));
		rotationMat = glm::rotate(rotationMat, glm::radians(-rotY), _up);
		_orientation = glm::vec3(rotationMat * glm::vec4(_orientation, 1.0f));
	}

	void Camera::setAngle(glm::vec3& eye, glm::vec3& orientation, glm::vec3& up)
	{
		_eye = eye;
		_orientation = orientation;
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
		glm::mat4 view = glm::lookAt(_eye, _eye + _orientation, _up);
		//LH_CORE_INFO("Eye vector is : ({0}, {1}, {2})", _eye.x, _eye.y, _eye.z);
		
		Renderer::getShader()->setUniformMat4f("u_camera", projection * view);
	}

}