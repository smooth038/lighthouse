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

	void Camera::look(float mouseDeltaX, float mouseDeltaY)
	{
		float maximumYAngle = 88.0f;
		float maxY = glm::tan(glm::radians(maximumYAngle)) / glm::sqrt(1 + glm::pow(glm::tan(glm::radians(maximumYAngle)), 2.0f));
		float maxHorizontalSquared = 1 - glm::pow(maxY, 2.0f);

		// rotations according to X axis (vertical movements) and Y axis (horizontal movements)
		float rotX = _sensitivity * mouseDeltaY / _height;
		float rotY = _sensitivity * mouseDeltaX / _width;

		glm::mat4 rotationMat(1.0f);
		rotationMat = glm::rotate(rotationMat, glm::radians(-rotX), glm::normalize(glm::cross(_orientation, _up)));
		rotationMat = glm::rotate(rotationMat, glm::radians(-rotY), _up);

		glm::vec3 newOrientation = glm::normalize(glm::vec3(rotationMat * glm::vec4(_orientation, 1.0f)));
		float xSquared = glm::pow(newOrientation.x, 2);
		float zSquared = glm::pow(newOrientation.z, 2);

		bool upsideDown = glm::sign(newOrientation.x) != glm::sign(_orientation.x) && glm::sign(newOrientation.z) != glm::sign(_orientation.z);
		float angleBefore = glm::degrees(glm::atan(_orientation.y / glm::sqrt(1.0f - glm::pow(_orientation.y, 2.0))));
		bool tooVertical = glm::abs(angleBefore - rotX) > maximumYAngle || glm::abs(newOrientation.y) > maxY;
		if (tooVertical || upsideDown) 
		{
			float k = (upsideDown ? -1 : 1) * glm::sqrt(maxHorizontalSquared / (xSquared + zSquared));
			_orientation = glm::vec3(k * newOrientation.x, maxY * (_orientation.y > 0 ? 1 : -1), k * newOrientation.z);
		}
		else
		{
			_orientation = newOrientation;
		}
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
		float aspectRatio = w / h;

		glm::mat4 projection = glm::perspective(glm::radians(_fov), aspectRatio, _zNear, _zFar);
		glm::mat4 view = glm::lookAt(_eye, _eye + _orientation, _up);

		_cameraMatrix = projection * view;
		
		Renderer::getShader()->setUniformMat4f("u_camera", _cameraMatrix);
	}

	glm::vec3 Camera::getEyeVector()
	{
		return _eye;
	}

	glm::vec3 Camera::getOrientationVector()
	{
		return _orientation;
	}

	glm::vec3 Camera::getUpVector()
	{
		return _up;
	}

	glm::mat4 Camera::getCameraMatrix()
	{
		return _cameraMatrix;
	}

	float Camera::getFov()
	{
		return _fov;
	}

	float Camera::getZFar()
	{
		return _zFar;
	}

	float Camera::getZNear()
	{
		return _zNear;
	}

	void Camera::setDimensions(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
	}
}