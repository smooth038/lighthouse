#include "TestApp.h"

void TestApp::onAttach()
{
	LH_INFO("TestApp attached!");

	_buildScene();
}

void TestApp::onDetach()
{
	LH_INFO("TestApp detached!");
}

void TestApp::onUpdate()
{
	Lighthouse::RenderCommand::fillCanvas(0.03125f, 0.0546875f, 0.25f, 1.0f);
	_moveSideways(_entities[0]);
	Lighthouse::Renderer::renderScene();

}

void TestApp::onEvent(Lighthouse::Event& e)
{
}

void TestApp::_buildScene()
{
	//_entities.push_back(Lighthouse::Renderer::addEntity(
	//	"TRIANGLE_1", 
	//	{
	//		-0.5f, -0.5f, 0.0f,
	//		 0.5f, -0.5f, 0.0f,
	//		 0.0f,  0.5f, 0.0f,
	//	},
	//	{
	//		0, 1, 2,
	//	}
	//));

	//_entities.push_back(Lighthouse::Renderer::addEntity(
	//	"SQUARE_1",
	//	{
	//		-0.9f,  0.7f, 0.0f,
	//		-0.7f,  0.7f, 0.0f,
	//		-0.7f,  0.9f, 0.0f,
	//		-0.9f,  0.9f, 0.0f,
	//	},
	//	{
	//		0, 1, 2,
	//		0, 2, 3,
	//	}
	//));
	
	_entities.push_back(Lighthouse::Renderer::addEntity(
		"CUBE",
		{
			-0.4f, -0.4f, -0.4f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.4f, -0.4f, -0.4f, 0.0f, 0.5f, 0.0f, 1.0f,
			 0.4f,  0.4f, -0.4f, 0.0f, 0.0f, 0.5f, 1.0f,
			-0.4f,  0.4f, -0.4f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.4f, -0.4f,  0.4f, 0.5f, 0.0f, 0.5f, 1.0f,
			 0.4f, -0.4f,  0.4f, 0.0f, 0.5f, 0.5f, 1.0f,
			 0.4f,  0.4f,  0.4f, 0.5f, 0.2f, 0.2f, 1.0f,
			-0.4f,  0.4f,  0.4f, 0.2f, 0.2f, 0.5f, 1.0f,
		},
		{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			3, 2, 6,
			3, 6, 7,
			0, 5, 1,
			0, 4, 5,
			0, 3, 7,
			0, 7, 4,
			1, 5, 6,
			1, 6, 2,
		}
	));

}

void TestApp::_moveSideways(Lighthouse::Entity* e)
{
	static std::unordered_map<Lighthouse::Entity*, bool> movingRightMap;

	if (!movingRightMap.count(e)) movingRightMap.insert(std::make_pair(e, true));

	std::vector<float> vertices = e->getVertices();	

	float min = vertices[0], max = vertices[0];
	for (int i = 0; i < vertices.size(); i += 7)
	{
		if (vertices[i] > max) max = vertices[i];
		if (vertices[i] < min) min = vertices[i];
	}

	if (max + 0.01f > 1.0f && movingRightMap[e]) movingRightMap[e] = false;
	if (min - 0.01f < -1.0f && !movingRightMap[e]) movingRightMap[e] = true;

	for (int i = 0; i < vertices.size(); i += 7)
	{
		movingRightMap[e] ? vertices[i] += 0.01f : vertices[i] -= 0.01f;
	}

	e->setVertices(vertices);
}
