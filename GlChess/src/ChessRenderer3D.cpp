#include "ChessRenderer3D.h"
#include <glm/gtc/type_ptr.hpp>

void ChessRenderer3D::onUpdate()
{
	Lighthouse::RenderCommand::clearCanvas(0.918f, 0.714f, 0.463f, 1.0f);
	Lighthouse::Renderer::renderScene();
	_updateCamera();
	_updateMovingPiece();
	if (_shouldUpdatePickingFrameBuffer)
	{
		_updatePickingFrameBuffer();
	}
}

void ChessRenderer3D::buildScene()
{
	_resetMouse();
	_loadAllTextures();

	// Board
	std::unique_ptr<Lighthouse::Entity>& board = _addEntityFromFile("res\\meshes\\board.obj", "board");
	Lighthouse::Texture boardTexture("res\\textures\\board.jpg", 0);
	board->setTextureSlot(0, 0);
	_translateEntity(board, 0, glm::vec3(0.0f, 0.0f, _boardZCenter));

	// Pieces
	std::vector<std::shared_ptr<Piece>> pieces = _board.getAllPieces();
	for (auto& piece : pieces)
	{
		PieceInfo pieceInfo = _generatePieceInfo(piece);

		std::unique_ptr<Lighthouse::Entity>& entity = _addEntityFromFile(pieceInfo);

		entity->setTextureSlot(_pieceIndices[pieceInfo.name], pieceInfo.textureSlot);
		entity->setObjectIndex(_pieceIndices[pieceInfo.name], _pieceObjIndices[pieceInfo.name]);
		entity->setHighlightValue(_pieceIndices[pieceInfo.name], piece->getColor() == Color::WHITE ? 1.0f : 1.5f);

		bool mirror = piece->getColor() == Color::WHITE;
		_translatePieceToSquare(entity, _pieceIndices[pieceInfo.name], pieceInfo.file, pieceInfo.rank, mirror);
	}

	// Lighting
	glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, -25.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Lighthouse::Renderer::setLightPosition(lightPosition);
	Lighthouse::Renderer::setLightColor(lightColor);

	// Camera
	glm::vec3 eye = glm::vec3(0.0f, 25.0f, 2.0f);
	glm::vec3 orientation = glm::vec3(0.0f, -0.5f, -0.5f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	Lighthouse::Renderer::getCamera().setAngle(eye, orientation, up);
	_shouldUpdatePickingFrameBuffer = true;
}

std::map<std::pair<PieceType, Color>, std::pair<const std::string, unsigned int>> _pieceTextureMap = {
	{std::make_pair(PieceType::PAWN, Color::WHITE), std::make_pair("pawn_white", 1)},
	{std::make_pair(PieceType::PAWN, Color::BLACK), std::make_pair("pawn_black", 2)},
	{std::make_pair(PieceType::KNIGHT, Color::WHITE), std::make_pair("knight_white", 3)},
	{std::make_pair(PieceType::KNIGHT, Color::BLACK), std::make_pair("knight_black", 4)},
	{std::make_pair(PieceType::BISHOP, Color::WHITE), std::make_pair("bishop_white", 5)},
	{std::make_pair(PieceType::BISHOP, Color::BLACK), std::make_pair("bishop_black", 6)},
	{std::make_pair(PieceType::ROOK, Color::WHITE), std::make_pair("rook_white", 7)},
	{std::make_pair(PieceType::ROOK, Color::BLACK), std::make_pair("rook_black", 8)},
	{std::make_pair(PieceType::QUEEN, Color::WHITE), std::make_pair("queen_white", 9)},
	{std::make_pair(PieceType::QUEEN, Color::BLACK), std::make_pair("queen_black", 10)},
	{std::make_pair(PieceType::KING, Color::WHITE), std::make_pair("king_white", 11)},
	{std::make_pair(PieceType::KING, Color::BLACK), std::make_pair("king_black", 12)},
};

void ChessRenderer3D::_loadAllTextures()
{
	for (auto it = _pieceTextureMap.begin(); it != _pieceTextureMap.end(); it++)
	{
		const std::string fileName = it->second.first;
		unsigned int textureSlot = it->second.second;
		Lighthouse::Texture texture("res\\textures\\" + fileName + ".jpg", textureSlot);
	}
}

void ChessRenderer3D::_rotateEntity(std::unique_ptr<Lighthouse::Entity>& e, unsigned int index, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition)
{
	e->setModelMatrix(index, _rotateMatrix(e->getModelMatrix(index), deg, axisDirection, axisPosition));
}

void ChessRenderer3D::_translateEntity(std::unique_ptr<Lighthouse::Entity>& e, unsigned int index, glm::vec3 translationVector)
{
	e->setModelMatrix(index, _translateMatrix(e->getModelMatrix(index), translationVector));
}

glm::mat4 ChessRenderer3D::_rotateMatrix(glm::mat4 m, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition)
{
	return _translateMatrix(glm::rotate(_translateMatrix(m, -axisPosition), glm::radians(deg), axisDirection), axisPosition);
}

glm::vec3 ChessRenderer3D::_rotateVector(glm::vec3 v, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition)
{
	return glm::vec3(_rotateMatrix(glm::mat4(1.0f), deg, axisDirection, axisPosition) * glm::vec4(v, 1.0f));
}

glm::mat4 ChessRenderer3D::_translateMatrix(glm::mat4 m, glm::vec3 translationVector)
{
	return glm::translate(m, translationVector);
}

glm::vec3 ChessRenderer3D::_translateVector(glm::vec3 m, glm::vec3 translationVector)
{
	return glm::vec3(_translateMatrix(glm::mat4(1.0f), translationVector) * glm::vec4(m, 1.0f));
}

void ChessRenderer3D::_updatePickingFrameBuffer()
{
	Lighthouse::Renderer::updatePickingFrameBuffer();
	_shouldUpdatePickingFrameBuffer = false;
}

std::unique_ptr<Lighthouse::Entity>& ChessRenderer3D::_addEntityFromFile(const std::string& filePath, const std::string& name)
{
	Lighthouse::Renderer::loadObjFile(filePath, name);
	return Lighthouse::Renderer::getScene().getEntityById(name);
}

std::unique_ptr<Lighthouse::Entity>& ChessRenderer3D::_addEntityFromFile(PieceInfo pieceInfo)
{
	bool entityPreexists = Lighthouse::Renderer::getScene().entityExists(pieceInfo.type);
	if (!entityPreexists)
	{
		Lighthouse::Renderer::loadObjFile(pieceInfo.meshFilePath, pieceInfo.type);
	}

	auto& piece = Lighthouse::Renderer::getScene().getEntityById(pieceInfo.type);
	if (entityPreexists)
	{
		piece->addModelMatrix(glm::mat4(1.0f));
		piece->addTextureSlot(0);
		piece->addObjectIndex(0);
		piece->addHightlightValue(1.0f);
		piece->addShaderType(Lighthouse::ShaderType::TEXTURE);
	}

	_pieceIndices.insert(std::make_pair(pieceInfo.name, piece->getEntityCount() - 1));
	_pieceObjIndices.insert(std::make_pair(pieceInfo.name, _pieceObjIndices.size() + 1));
	_objIndicesToPiece.insert(std::make_pair(_objIndicesToPiece.size() + 1, pieceInfo.name));
	return piece;
}

void ChessRenderer3D::_updateCamera()
{
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
		Lighthouse::Renderer::getCamera().look(_mouseDeltaX, _mouseDeltaY);
		_resetMouse();
	}
}

void ChessRenderer3D::_updateMovingPiece()
{
	if (_isMovingPiece)
	{
		std::string lastPointedPieceName = _objIndicesToPiece[_lastPointedObjectIndex];
		std::string lastPointedPieceType = _getPieceTypeFromName(lastPointedPieceName);
		std::unique_ptr<Lighthouse::Entity>& movingPiece = Lighthouse::Renderer::getScene().getEntityById(lastPointedPieceType);

		bool mirror = lastPointedPieceName.substr(0, 5) == "white";

		glm::vec3 pointedPosition = _getPositionOnBoardFromCursorPosition();

		_translatePieceToPosition(movingPiece, _pieceIndices[lastPointedPieceName], pointedPosition, mirror);
	}
}

glm::vec3 ChessRenderer3D::_getOrientationFromPixel(float pixelX, float pixelY)
{
	float fov = Lighthouse::Renderer::getCamera().getFov();
	float width = _window->getWidth();
	float height = _window->getHeight();
	// There must be a mistake in my calculations for the correct head to screen distance. I can't find it so I
	// added this scaling constant, found by trial and error, to account for the mistake.
	const float MAGIC_SCALING = 0.58f; 
	float distanceHeadToScreen = MAGIC_SCALING * static_cast<float>(_window->getWidth()) / (2 * glm::tan(glm::radians(fov / 2)));

	// unit vectors
	glm::vec3 u = glm::normalize(Lighthouse::Renderer::getCamera().getUpVector());
	glm::vec3 d = glm::normalize(Lighthouse::Renderer::getCamera().getOrientationVector());
	glm::vec3 h = glm::normalize(glm::cross(d, u));
	glm::vec3 v = glm::normalize(glm::cross(d, h));

	// scaled vectors
	glm::vec3 x = (pixelX - (width / 2)) * h;
	glm::vec3 y = (pixelY - (height / 2)) * v;
	glm::vec3 z = distanceHeadToScreen * d;

	return glm::normalize(x + y + z);
}

glm::vec3 ChessRenderer3D::_getPositionOnBoardFromCursorPosition()
{
	glm::vec3 position = Lighthouse::Renderer::getCamera().getEyeVector();
	glm::vec3 direction = _getOrientationFromPixel(_mouseX, _mouseY);

	float t = -position.y / direction.y;
	float x = direction.x * t + position.x;
	float z = direction.z * t + position.z;

	return glm::vec3(x, 0, z);
}

void ChessRenderer3D::_resetMouse()
{
	_window->centerMouseCursor();
	_mouseX = static_cast<float>(_window->getWidth() / 2);
	_mouseY = static_cast<float>(_window->getHeight() / 2);
	_mouseDeltaX = 0.0f;
	_mouseDeltaY = 0.0f;
}

bool ChessRenderer3D::onWindowResized(Lighthouse::WindowResizeEvent& e)
{
	_shouldUpdatePickingFrameBuffer = true;
	return true;
}

bool ChessRenderer3D::onKeyPressed(Lighthouse::KeyPressedEvent& e)
{
	if (e.getKeyCode() == Lighthouse::Key::F6 && !_isMovingPiece)
	{
		_cameraMove = !_cameraMove;
		_resetMouse();
		if (_cameraMove)
		{
			LH_INFO("Camera move activated");
			_window->hideCursor();
			_unhighlightLastPointedPiece();
		}
		else
		{
			LH_INFO("Camera move de-activated");
			_window->unhideCursor();
			_shouldUpdatePickingFrameBuffer = true;
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

}

bool ChessRenderer3D::onKeyReleased(Lighthouse::KeyReleasedEvent& e)
{
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
}

bool ChessRenderer3D::onMouseMoved(Lighthouse::MouseMovedEvent& e)
{
	if (_cameraMove || _isMovingPiece)
	{
		float deltaX = e.getX() - _mouseX;
		float deltaY = e.getY() - _mouseY;

		_mouseX = e.getX();
		_mouseY = e.getY();

		if ((_mouseDeltaX > 0 && deltaX > _mouseDeltaX) || (_mouseDeltaX < 0 && deltaX < _mouseDeltaX) || _mouseDeltaX == 0) _mouseDeltaX = deltaX;
		if ((_mouseDeltaY > 0 && deltaY > _mouseDeltaY) || (_mouseDeltaY < 0 && deltaY < _mouseDeltaY) || _mouseDeltaY == 0) _mouseDeltaY = deltaY;	

	}
	else
	{
		_handlePieceHighlight(e);
	}

	return true;
}

bool ChessRenderer3D::onMouseButtonPressed(Lighthouse::MouseButtonPressedEvent& e)
{
	if (_cameraMove) return false;

	if (_lastPointedObjectIndex > 0)
	{
		_unhighlightLastPointedPiece();
		_isMovingPiece = true;
		glm::vec2 piecePosition = _getLastPointedPiece2dCoordinates();
		_window->setMouseCursorPosition(static_cast<double>(piecePosition.x), static_cast<double>(piecePosition.y));
		_mouseX = piecePosition.x;
		_mouseY = piecePosition.y;
	}

	return true;
}

bool ChessRenderer3D::onMouseButtonReleased(Lighthouse::MouseButtonReleasedEvent& e)
{
	if (_isMovingPiece)
	{
		_isMovingPiece = false;
		_shouldUpdatePickingFrameBuffer = true;
		_highlightPointedPiece();

		std::unique_ptr<Lighthouse::Entity>& movingPiece = _getLastPointedPiece();
		glm::vec4 position = movingPiece->getModelMatrix(_getLastPointedPieceIndex())[3];

		bool mirror = _getLastPointedPieceColor() == Color::WHITE;
		Square destination = _getSquareFromWorldCoordinates(position.x, position.z);
		_translatePieceToSquare(movingPiece, _getLastPointedPieceIndex(), destination.getFile(), destination.getRank(), mirror);
	}

	return true;
}

void ChessRenderer3D::_handlePieceHighlight(Lighthouse::MouseMovedEvent& e)
{
	unsigned int objIndex = Lighthouse::Renderer::getObjectIndexFromPixel(e.getX(), e.getY());
	if (objIndex != _lastPointedObjectIndex)
	{
		if (_lastPointedObjectIndex > 0)
		{
			_unhighlightLastPointedPiece();
			_lastPointedObjectIndex = 0;
		}
		if (objIndex > 0)
		{
			_lastPointedObjectIndex = objIndex;
			_highlightPointedPiece();
		}
	}
}

void ChessRenderer3D::_unhighlightLastPointedPiece()
{
	std::string lastPointedPieceName = _objIndicesToPiece[_lastPointedObjectIndex];
	std::string lastPointedPieceType = _getPieceTypeFromName(lastPointedPieceName);
	std::unique_ptr<Lighthouse::Entity>& lastPointedEntity = Lighthouse::Renderer::getScene().getEntityById(lastPointedPieceType);
	lastPointedEntity->setShaderType(_pieceIndices[lastPointedPieceName], Lighthouse::ShaderType::TEXTURE);
}

void ChessRenderer3D::_highlightPointedPiece()
{
	std::unique_ptr<Lighthouse::Entity>& lastPointedPiece = _getLastPointedPiece();	
	lastPointedPiece->setShaderType(_getLastPointedPieceIndex(), Lighthouse::ShaderType::HIGHLIGHT);
}

glm::vec2 ChessRenderer3D::_getLastPointedPiece2dCoordinates()
{
	std::unique_ptr<Lighthouse::Entity>& lastPointedPiece = _getLastPointedPiece();

	glm::mat4 model = lastPointedPiece->getModelMatrix(_getLastPointedPieceIndex());
	glm::mat4 camera = Lighthouse::Renderer::getCamera().getCameraMatrix();

	glm::vec4 cameraSpace = camera * model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float halfWidth  = _window->getWidth()  * 0.5f;
	float halfHeight = _window->getHeight() * 0.5f;

	// The coordinates are a little bit off for some reason so we need to add this empirical scaling
	float constexpr kScale = 1.027f;

	glm::vec2 screenSpace = glm::vec2(
		kScale * halfWidth * (cameraSpace.x / cameraSpace.z) + halfWidth, 
		kScale * halfHeight * (-cameraSpace.y / cameraSpace.z) + halfHeight
	);

	return screenSpace;
}

unsigned int ChessRenderer3D::_getLastPointedPieceIndex()
{
	std::string lastPointedPieceName = _objIndicesToPiece[_lastPointedObjectIndex];
	return _pieceIndices[lastPointedPieceName];
}

std::unique_ptr<Lighthouse::Entity>& ChessRenderer3D::_getLastPointedPiece()
{
	std::string lastPointedPieceName = _objIndicesToPiece[_lastPointedObjectIndex];
	std::string lastPointedPieceType = _getPieceTypeFromName(lastPointedPieceName);
	std::unique_ptr<Lighthouse::Entity>& lastPointedPiece = Lighthouse::Renderer::getScene().getEntityById(lastPointedPieceType);
	return lastPointedPiece;
}

Color ChessRenderer3D::_getLastPointedPieceColor()
{	
	std::string lastPointedPieceName = _objIndicesToPiece[_lastPointedObjectIndex];
	Color color = lastPointedPieceName.substr(0, 5) == "white" ? Color::WHITE : Color::BLACK;
	return color;
}

std::string ChessRenderer3D::_getPieceTypeFromName(std::string& pieceName)
{
	size_t begin = pieceName.find("_") + 1;
	size_t end = pieceName.find("_", begin);
	if (end == -1) end = pieceName.length();
	return std::string(&pieceName[begin], &pieceName[end]);
}

PieceInfo ChessRenderer3D::_generatePieceInfo(std::shared_ptr<Piece>& p)
{
	std::string type;
	switch (p->getType())
	{
	case PieceType::PAWN:
		type = "pawn";
		break;
	case PieceType::KNIGHT:
		type = "knight";
		break;
	case PieceType::BISHOP:
		type = "bishop";
		break;
	case PieceType::ROOK:
		type = "rook";
		break;
	case PieceType::QUEEN:
		type = "queen";
		break;
	case PieceType::KING:
		type = "king";
		break;
	}

	std::string color = p->getColor() == Color::WHITE ? "white" : "black";
	std::string meshPath = "res\\meshes\\" + type + ".obj";
	std::string texturePath = "res\\textures\\" + type + "_" + color + ".jpg";

	static std::unordered_map<std::string, unsigned int> pieceCount;
	std::string name = color + "_" + type;

	if (p->getType() != PieceType::KING)
	{
		if (!pieceCount.count(name))
		{
			pieceCount.insert(std::make_pair(name, 1));
		}
		else
		{
			pieceCount[name] += 1;
		}
		name += "_" + std::to_string(pieceCount[name]);
	}

	unsigned int textureSlot = _pieceTextureMap[std::make_pair<PieceType, Color>(p->getType(), p->getColor())].second;

	const char file = p->getSquare()->getFile();
	const char rank = p->getSquare()->getRank();

	PieceInfo info(name, meshPath, texturePath, textureSlot, file, rank, type);
	return info;
}

Square ChessRenderer3D::_getSquareFromWorldCoordinates(float x, float z)
{
	char file = static_cast<char>(glm::round(( x - _halfSquareOffset + 0.1f)    / _squareSize) + 4);
	char rank = static_cast<char>(glm::round((-z - _halfSquareOffset + _boardZCenter) / _squareSize) + 4);

	return Square(file, rank);
}

void ChessRenderer3D::_translatePieceToSquare(std::unique_ptr<Lighthouse::Entity>& piece, unsigned int index, char file, char rank, bool mirror)
{
	glm::mat4 model = glm::mat4(1.0f);
	if (mirror)
	{
		model = _rotateMatrix(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}

	const float x = (file - 4) * _squareSize + _halfSquareOffset - 0.1f;
	const float z = (4 - rank) * _squareSize - _halfSquareOffset + _boardZCenter;

	model = _translateMatrix(model, glm::vec3(mirror ? -x : x, 0.0f, mirror ? -z : z));
	piece->setModelMatrix(index, model);
}

void ChessRenderer3D::_translatePieceToPosition(std::unique_ptr<Lighthouse::Entity>& piece, unsigned int index, glm::vec3 position, bool mirror)
{
	float constexpr LEFT_EDGE  = -16.22f;
	float constexpr RIGHT_EDGE =  16.22f;
	float constexpr FRONT_EDGE =  -8.88f;
	float constexpr BACK_EDGE =  -41.29f;

	glm::mat4 model = glm::mat4(1.0f);
	if (mirror)
	{
		model = _rotateMatrix(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}

	float x = position.x < LEFT_EDGE ? LEFT_EDGE : position.x > RIGHT_EDGE ? RIGHT_EDGE : position.x;
	float z = position.z < BACK_EDGE ? BACK_EDGE : position.z > FRONT_EDGE ? FRONT_EDGE : position.z;
	
	model = _translateMatrix(model, glm::vec3(mirror ? -x : x, 0, mirror ? -z : z));

	piece->setModelMatrix(index, model);
}