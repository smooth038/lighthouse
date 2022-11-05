#pragma once
#include "ChessRenderer.h"

struct PieceInfo;

class ChessRenderer3D : public ChessRenderer
{
public:
	ChessRenderer3D(std::unique_ptr<Lighthouse::Window>& window) : ChessRenderer(window) {}
	~ChessRenderer3D() = default;

	void buildScene() override;
	void onUpdate()   override;

	// Event handling
	bool onWindowResized      (Lighthouse::WindowResizeEvent& e)        override;
	bool onKeyPressed         (Lighthouse::KeyPressedEvent& e)          override;
	bool onKeyReleased        (Lighthouse::KeyReleasedEvent& e)         override;
	bool onMouseMoved         (Lighthouse::MouseMovedEvent& e)          override;
	bool onMouseButtonPressed (Lighthouse::MouseButtonPressedEvent& e)  override;
	bool onMouseButtonReleased(Lighthouse::MouseButtonReleasedEvent& e) override;

private:
	void _loadAllTextures();

	std::unique_ptr<Lighthouse::Entity>& _addEntityFromFile(const std::string& filePath, const std::string& name);
	std::unique_ptr<Lighthouse::Entity>& _addEntityFromFile(PieceInfo pieceInfo);

	void _rotateEntity(std::unique_ptr<Lighthouse::Entity>& e, unsigned int index, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	void _translateEntity(std::unique_ptr<Lighthouse::Entity>& e, unsigned int index, glm::vec3 translationVector);

	glm::mat4 _rotateMatrix   (glm::mat4 m, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	glm::vec3 _rotateVector   (glm::vec3 v, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	glm::mat4 _translateMatrix(glm::mat4 m, glm::vec3 translation);
	glm::vec3 _translateVector(glm::vec3 m, glm::vec3 translation);

	// Camera
	void _updateCamera();
	void _resetMouse();
	bool  _cameraMove     = false, 
		  _movingLeft     = false, 
		  _movingRight    = false, 
		  _movingForward  = false, 
		  _movingBackward = false, 
		  _movingUpward   = false, 
		  _movingDownward = false;
	float _mouseX = 0.0f,
		  _mouseY = 0.0f,
		  _mouseDeltaX = 0.0f,
		  _mouseDeltaY = 0.0f;

	// Piece highlighting and moving
	void _updateMovingPiece();

	void _handlePieceHighlight(Lighthouse::MouseMovedEvent& e);
	void _unhighlightLastPointedPiece();
	void _highlightPointedPiece();

	std::unique_ptr<Lighthouse::Entity>& _getLastPointedPiece();
	unsigned int _getLastPointedPieceIndex();
	glm::vec2 _getLastPointedPiece2dCoordinates();
	Color _getLastPointedPieceColor();

	Square* _movingPieceOriginSquare = nullptr;
	Square* _getSquareFromWorldCoordinates(float x, float z);
	void _translatePieceToSquare(std::unique_ptr<Lighthouse::Entity>& piece, unsigned int index, char file, char rank, bool mirror = false);
	void _translatePieceToPosition(std::unique_ptr<Lighthouse::Entity>& piece, unsigned int index, glm::vec3 position, bool mirror);

	void _updatePickingFrameBuffer();
	bool _shouldUpdatePickingFrameBuffer = false;
	unsigned int _lastPointedObjectIndex = 0;
	bool _isMovingPiece = false;
	glm::vec3 _getOrientationFromPixel(float x, float y);
	glm::vec3 _getPositionOnBoardFromCursorPosition();

	// Chess
	const float _squareSize = 4.63f;
	const float _halfSquareOffset = 2.4f;
	const float _boardZCenter = -25.0f;

	ChessBoard _board;
	void _loadBoard();
	void _updateBoard();
	unsigned int _getPieceIndexByName(const std::string& name);
	Square* _getEntitySquareByName(const std::string& name);
	std::unique_ptr<Lighthouse::Entity>& _getEntityByName(const std::string& name);
	std::unordered_map<std::string, unsigned int> _pieceIndices;
	std::unordered_map<std::string, unsigned int> _pieceObjIndices;
	std::unordered_map<unsigned int, std::string> _objIndicesToPiece;
	std::vector<std::string> _getAllLoadedPieceNames();
	std::string _getPieceTypeFromName(const std::string& pieceName);
	std::string _getPieceStringType(std::shared_ptr<Piece>& piece);
	PieceInfo _generatePieceInfo(std::shared_ptr<Piece>& p);
};

struct PieceInfo
{
	PieceInfo() : name(), meshFilePath(), textureFilePath(), textureSlot(), file(), rank(), type() {}

	PieceInfo(
		const std::string& name,
		const std::string& meshFilePath,
		const std::string& textureFilePath,
		unsigned int textureSlot,
		const char file,
		const char rank,
		const std::string type
		)
		: name(name), meshFilePath(meshFilePath), textureFilePath(textureFilePath), 
		textureSlot(textureSlot), file(file), rank(rank), type(type) {}

	const std::string name;
	const std::string meshFilePath;
	const std::string textureFilePath;
	const unsigned int textureSlot;
	const char file;
	const char rank;
	const std::string type;
};
