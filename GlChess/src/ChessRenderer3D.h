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
	bool onWindowResized(Lighthouse::WindowResizeEvent& e) override;
	bool onKeyPressed   (Lighthouse::KeyPressedEvent& e)   override;
	bool onKeyReleased  (Lighthouse::KeyReleasedEvent& e)  override;
	bool onMouseMoved   (Lighthouse::MouseMovedEvent& e)   override;

private:
	void _loadAllTextures();

	std::unique_ptr<Lighthouse::Entity>& _addEntityFromFile(const std::string& filePath, const std::string& name);
	std::unique_ptr<Lighthouse::Entity>& _addEntityFromFile(PieceInfo pieceInfo);

	const glm::vec3 _boardCenter = glm::vec3(0.0f, 0.0f, -25.0f);
	void _translatePieceToSquare(std::unique_ptr<Lighthouse::Entity>& piece, unsigned int index, char file, char rank, bool mirror = false);
	void _rotateEntity(std::unique_ptr<Lighthouse::Entity>& e, unsigned int index, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	void _translateEntity(std::unique_ptr<Lighthouse::Entity>& e, unsigned int index, glm::vec3 translationVector);

	glm::mat4 _rotateMatrix   (glm::mat4 m, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	glm::vec3 _rotateVector   (glm::vec3 v, float deg, glm::vec3 axisDirection, glm::vec3 axisPosition);
	glm::mat4 _translateMatrix(glm::mat4 m, glm::vec3 translation);
	glm::vec3 _translateVector(glm::vec3 m, glm::vec3 translation);

	// Piece highlighting
	void _handlePieceHighlight(Lighthouse::MouseMovedEvent& e);
	void _unhighlightLastPointedPiece();
	void _highlightPointedPiece(unsigned int objIndex);

	void _updatePickingFrameBuffer();
	bool _shouldUpdatePickingFrameBuffer = false;

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
		  _mouseY = 0.0f;

	// Chess
	ChessBoard _board;
	std::unordered_map<std::string, unsigned int> _pieceIndices;
	std::unordered_map<std::string, unsigned int> _pieceObjIndices;
	std::unordered_map<unsigned int, std::string> _objIndicesToPiece;
	std::string _getPieceTypeFromName(std::string& pieceName);
	PieceInfo _generatePieceInfo(std::shared_ptr<Piece>& p);
	unsigned int _lastPointedObjectIndex = 0;
};

struct PieceInfo
{
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

