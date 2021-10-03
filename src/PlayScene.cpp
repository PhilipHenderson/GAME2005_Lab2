#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();


	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), startPos.x, startPos.y, startPos.x + vel.x, startPos.y - vel.y);

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 50, 50, 50, 255);


}

void PlayScene::update()
{

	float dt = Game::Instance().getDeltaTime();
	t += dt;



	m_pTarget->getTransform()->position = glm::vec2(startPos);


	while (EventManager::Instance().isKeyUp(SDL_SCANCODE_1))
	{
		vel.y -= accelerationGravity * t;
		startPos.y -= vel.y * dt;
		startPos.x = vel.x * t;

		m_pTarget->getTransform()->position = glm::vec2(startPos);


	}
	updateDisplayList();

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Projectile Sprite
	m_pTarget = new Target();
	addChild(m_pTarget);

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(50.0f, 600.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance().changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(700.0f, 600.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance().changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Goldneo's ImGui Window", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	//static float launchSpeed = {0.0f}; //change velocity // a
	ImGui::SliderFloat("Velocity Vector Y", &vel.y, 5.0f, 250.0f, "%.3f");
	ImGui::SliderFloat("Velocity Vector X", &vel.x, 5.0f, 250.0f, "%.3f");
	ImGui::SliderFloat("Angle", &launchElevationAngle, 5.0f, 10.0f, "%.3f");
	ImGui::SliderFloat("Gravity", &accelerationGravity, 5.0f, 10.0f, "%.3f");

	ImGui::End();
}
