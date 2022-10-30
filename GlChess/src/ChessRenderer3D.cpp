#include "ChessRenderer3D.h"
#include <glm/gtc/type_ptr.hpp>

void ChessRenderer3D::onUpdate()
{
	Lighthouse::RenderCommand::clearCanvas(0.918f, 0.714f, 0.463f, 1.0f);
	Lighthouse::Renderer::renderScene();
	_updateCamera();
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
	_translateEntity(board, 0, _boardCenter);

	// Pieces
	std::vector<std::shared_ptr<Piece>> pieces = _board.getAllPieces();
	for (auto& piece : pieces)
	{
		PieceInfo pieceInfo = _generatePieceInfo(piece);

		std::unique_ptr<Lighthouse::Entity>& entity = _addEntityFromFile(pieceInfo);

		entity->setTextureSlot(_pieceIndices[pieceInfo.name], pieceInfo.textureSlot);
		entity->setObjectIndex(_pieceIndices[pieceInfo.name], _pieceObjIndices[pieceInfo.name]);
		entity->setHighlightValue(_pieceIndices[pieceInfo.name], piece->getColor() == Color::WHITE ? 1.0f : 1.5f);

		if (piece->getColor() == Color::WHITE)
		{
			_rotateEntity(entity, _pieceIndices[pieceInfo.name], 180.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			_translatePieceToSquare(entity, _pieceIndices[pieceInfo.name], pieceInfo.file, pieceInfo.rank, true);
		}
		else
		{
			_translatePieceToSquare(entity, _pieceIndices[pieceInfo.name], pieceInfo.file, pieceInfo.rank);
		}
	}

	// Lighting
	glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, -25.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Lighthouse::Renderer::setLightPosition(lightPosition);
	Lighthouse::Renderer::setLightColor(lightColor);

	// Camera
	glm::vec3 eye = glm::vec3(0.0f, 25.0f, 2.0f);
	glm::vec3 orientation = glm::vec3(0.0f, -1.0f, -1.0f);
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
		Lighthouse::Renderer::getCamera().look(_mouseX, _mouseY);
		_window->centerMouseCursor();
		_resetMouse();
	}
}

void ChessRenderer3D::_resetMouse()
{
	_mouseX = static_cast<float>(_window->getWidth() / 2);
	_mouseY = static_cast<float>(_window->getHeight() / 2);
}

bool ChessRenderer3D::onWindowResized(Lighthouse::WindowResizeEvent& e)
{
	_shouldUpdatePickingFrameBuffer = true;
	return true;
}

bool ChessRenderer3D::onKeyPressed(Lighthouse::KeyPressedEvent& e)
{
	if (e.getKeyCode() == Lighthouse::Key::F6)
	{
		_cameraMove = !_cameraMove;
		_resetMouse();
		if (_cameraMove)
		{
			LH_INFO("Camera move activated");
			_window->centerMouseCursor();
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
	if (_cameraMove)
	{
		_mouseX = e.getX();
		_mouseY = e.getY();
	}
	else
	{
		_handlePieceHighlight(e);
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
		}
		if (objIndex > 0)
		{
			_highlightPointedPiece(objIndex);
		}
	}
}

void ChessRenderer3D::_unhighlightLastPointedPiece()
{
	std::string lastPointedPieceName = _objIndicesToPiece[_lastPointedObjectIndex];
	std::string lastPointedPieceType = _getPieceTypeFromName(lastPointedPieceName);
	std::unique_ptr<Lighthouse::Entity>& lastPointedEntity = Lighthouse::Renderer::getScene().getEntityById(lastPointedPieceType);
	lastPointedEntity->setShaderType(_pieceIndices[lastPointedPieceName], Lighthouse::ShaderType::TEXTURE);
	_lastPointedObjectIndex = 0;
}

void ChessRenderer3D::_highlightPointedPiece(unsigned int objIndex)
{
	std::string pointedPieceName = _objIndicesToPiece[objIndex];
	std::string pointedPieceType = _getPieceTypeFromName(pointedPieceName);
	std::unique_ptr<Lighthouse::Entity>& pointedEntity = Lighthouse::Renderer::getScene().getEntityById(pointedPieceType);
	pointedEntity->setShaderType(_pieceIndices[pointedPieceName], Lighthouse::ShaderType::HIGHLIGHT);
	_lastPointedObjectIndex = objIndex;
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

void ChessRenderer3D::_translatePieceToSquare(std::unique_ptr<Lighthouse::Entity>& piece, unsigned int index, char file, char rank, bool mirror)
{
	const float scale = 4.63f;
	const float halfSquareOffset = 2.4f;
	const float zCenter = _boardCenter.z;

	const float x = (file - 4) * scale + halfSquareOffset - 0.1f;
	const float z = (4 - rank) * scale - halfSquareOffset + zCenter;

	_translateEntity(piece, index, glm::vec3(x, 0.0f, mirror ? -z : z));
}