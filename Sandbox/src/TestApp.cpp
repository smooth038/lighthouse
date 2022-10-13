#include "TestApp.h"
#include <glm/gtc/type_ptr.hpp>

void TestApp::onAttach()
{
	LH_INFO("TestApp attached!");
	_resetMouse();
	_buildScene();
}

void TestApp::onDetach()
{
	LH_INFO("TestApp detached!");
}

void TestApp::onUpdate()
{
	Lighthouse::RenderCommand::fillCanvas(0.03125f, 0.0546875f, 0.25f, 1.0f);
	_rotateEntity(Lighthouse::Renderer::getScene().getEntityById("king"), 
		-0.2f, 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	_rotateEntity(Lighthouse::Renderer::getScene().getEntityById("lightCube"), 
		0.8f, 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 10.0f)
	);
	Lighthouse::Renderer::setLightPosition(_rotateVector(
		Lighthouse::Renderer::getLightPosition(),
		0.8f,
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f)
	));
	Lighthouse::Renderer::renderScene();

	if (_cameraMove)
	{
		if (_movingForward)
		{
			Lighthouse::Renderer::getCamera().moveForward();
		}
		if (_movingBackward)
		{
			Lighthouse::Renderer::getCamera().moveBackward();
		}
		if (_movingLeft)
		{
			Lighthouse::Renderer::getCamera().moveLeft();
		}
		if (_movingRight)
		{
			Lighthouse::Renderer::getCamera().moveRight();
		}
		if (_movingDownward)
		{
			Lighthouse::Renderer::getCamera().moveDown();
		}
		if (_movingUpward)
		{
			Lighthouse::Renderer::getCamera().moveUp();
		}
		Lighthouse::Renderer::getCamera().look(_mouseX, _mouseY);
		_window->centerMouseCursor();
		_resetMouse();
	}
}

void TestApp::onEvent(Lighthouse::Event& event)
{
		Lighthouse::EventDispatcher dispatcher(event);
		dispatcher.dispatch<Lighthouse::KeyPressedEvent>([this](Lighthouse::KeyPressedEvent& e) { 
			if (e.getKeyCode() == Lighthouse::Key::F6)
			{
				_cameraMove = !_cameraMove;
				_resetMouse();
				if (_cameraMove)
				{
					LH_INFO("Camera move activated");
					_window->centerMouseCursor();
					_window->hideCursor();
				}
				else
				{
					LH_INFO("Camera move de-activated");
					_window->unhideCursor();
				}
				return true;
			}

			if (!_cameraMove) return false;
			switch (e.getKeyCode())
			{
				case Lighthouse::Key::E:
					_movingForward = true;
					_movingBackward = false;
					break;
				case Lighthouse::Key::D:
					_movingForward = false;
					_movingBackward = true;
					break;
				case Lighthouse::Key::S:
					_movingLeft = true;
					_movingRight = false;
					break;
				case Lighthouse::Key::F:
					_movingLeft = false;
					_movingRight = true;
					break;
				case Lighthouse::Key::Q:
					_movingUpward = true;
					_movingDownward = false;
					break;
				case Lighthouse::Key::Z:
					_movingUpward = false;
					_movingDownward = true;
					break;
				default:
					break;
			}
			return true;
		});

		dispatcher.dispatch<Lighthouse::KeyReleasedEvent>([this](Lighthouse::KeyReleasedEvent& e) {
			if (!_cameraMove) return false;
			switch (e.getKeyCode())
			{
			case Lighthouse::Key::E:
				_movingForward = false;
				break;
			case Lighthouse::Key::D:
				_movingBackward = false;
				break;
			case Lighthouse::Key::S:
				_movingLeft = false;
				break;
			case Lighthouse::Key::F:
				_movingRight = false;
				break;
			case Lighthouse::Key::Q:
				_movingUpward = false;
				break;
			case Lighthouse::Key::Z:
				_movingDownward = false;
				break;
			default:
				break;
				return true;
			}
		});

		dispatcher.dispatch<Lighthouse::MouseMovedEvent>([this](Lighthouse::MouseMovedEvent& e) {
				if (!_cameraMove) return false;
				_mouseX = e.getX();
				_mouseY = e.getY();
				return true;
			});
}

void TestApp::_buildScene()
{
	std::unique_ptr<Lighthouse::Entity>& king = _addEntityFromFile("res\\meshes\\king.obj", "king");
	Lighthouse::Texture texture("res\\textures\\wood.jpg", 1);
	king->setTextureSlot(0, 1);
	_translateEntity(king, glm::vec3(0.0f, -5.0f, -10.0f));

	glm::vec3 lightPosition = glm::vec3(5.0f, 0.0f, -10.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	std::unique_ptr<Lighthouse::Entity>& lightCube = _addCube("lightCube", lightPosition, lightColor, 1.0f);
	Lighthouse::Renderer::setLightPosition(lightPosition);
	Lighthouse::Renderer::setLightColor(lightColor);
}

void TestApp::_rotateEntity(std::unique_ptr<Lighthouse::Entity>& e, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition)
{
	e->setModelMatrix(0, _rotateMatrix(e->getModelMatrix(0), deg, axisDirection, axisPosition));
}

void TestApp::_translateEntity(std::unique_ptr<Lighthouse::Entity>& e, glm::vec3 translationVector)
{
	e->setModelMatrix(0, _translateMatrix(e->getModelMatrix(0), translationVector));
}

glm::mat4 TestApp::_rotateMatrix(glm::mat4 m, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition)
{
	return _translateMatrix(glm::rotate(_translateMatrix(m, -axisPosition), glm::radians(deg), axisDirection), axisPosition);
}

glm::vec3 TestApp::_rotateVector(glm::vec3 v, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition)
{
	return glm::vec3(_rotateMatrix(glm::mat4(1.0f), deg, axisDirection, axisPosition) * glm::vec4(v, 1.0f));
}

glm::mat4 TestApp::_translateMatrix(glm::mat4 m, glm::vec3 translationVector)
{
	return glm::translate(m, translationVector);
}

glm::vec3 TestApp::_translateVector(glm::vec3 m, glm::vec3 translationVector)
{
	return glm::vec3(_translateMatrix(glm::mat4(1.0f), translationVector) * glm::vec4(m, 1.0f));
}

std::unique_ptr<Lighthouse::Entity>& TestApp::_addEntityFromFile(const std::string& filepath, const std::string& name)
{
	_entityIds.push_back(name);
	Lighthouse::Renderer::loadObjFile(filepath, name);
	return Lighthouse::Renderer::getScene().getEntityById(_entityIds.back());
}

void TestApp::_resetMouse()
{
	_mouseX = static_cast<float>(_window->getWidth() / 2);
	_mouseY = static_cast<float>(_window->getHeight() / 2);
}

std::unique_ptr<Lighthouse::Entity>& TestApp::_addCube(const std::string& name, glm::vec3 position, glm::vec4 color, float edgeSize)
{
	_entityIds.push_back(name);
	float o = edgeSize / 2;
	float x = position[0];
	float y = position[1];
	float z = position[2];
	float r = color[0];
	float g = color[1];
	float b = color[2];
	float a = color[3];

	std::vector<float> vertices = {
		x-o, y-o, z-o, r, g, b, a,	
		x+o, y-o, z-o, r, g, b, a,	
		x+o, y+o, z-o, r, g, b, a,	
		x-o, y+o, z-o, r, g, b, a,	
		x-o, y-o, z+o, r, g, b, a,	
		x+o, y-o, z+o, r, g, b, a,	
		x+o, y+o, z+o, r, g, b, a,	
		x-o, y+o, z+o, r, g, b, a,	
	};

	std::vector<unsigned int> indices = {
		0, 1, 2, // front face
		0, 2, 3,
		4, 6, 5, // back face
		4, 7, 6,
		0, 5, 1, // bottom face
		0, 4, 5,
		3, 2, 6, // top face
		3, 6, 7,
		0, 3, 7, // left face
		0, 7, 4,
		1, 5, 6, // right face
		1, 6, 2,
	};

	return Lighthouse::Renderer::addEntity(name, vertices, indices, Lighthouse::ShaderType::FLAT_COLOR);
}

