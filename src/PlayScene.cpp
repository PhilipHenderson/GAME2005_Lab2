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
	
	updateDisplayList();

	m_pTarget->getTransform()->position = glm::vec2(startPos);
	//// a^2 + b^2 = c^2
	//// 400^2 + 100^2 = c^2
	//// 160000  + 10000   = 170000
	//// 170000 = 412
	////  400^2 + 100^2 = 412^2


	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_SPACE))
	{
		float displacementX = pow(startPos.x, 2); // a^2
		float displacementY = pow(startPos.y, 2); // b^2
		float totalDisplacement = sqrt(displacementX + displacementY);
		float velo = totalDisplacement / t;
		std::cout << "Displacement: " << totalDisplacement << ", Velocity: " << velo << std::endl;
		// Created Velocity Veriable
		// now tht we have velocity
		// we can find acceleration
		// Acceleration = V / t
		float acceleration = velo / t;
		// get velocity vector
		// x = speed cos (angle)
		// y = speed sin (angle)
		vel.x = startPos.x + launchSpeed * cos(launchElevationAngle)*t;	// * t ensures it moves accourding to delta time
		vel.y = startPos.y + launchSpeed * sin(launchElevationAngle) * t + acceleration;
		// Created velocity Vector
		// now that we have the velocity vector,
		// WE can find the velocity
		// V = d/t Velocity = Displacement over time
		// so whats the displacement for Vf - Vi
		// so Vf = vel and Vi = 0,0
		// V = vel
		// tf = 5 and ti = 0
		// t = 5
		// So vel/5 = velocity
		// but a^2 + b^2 = displacement
		// a^2 = vel.x
		// b^2 = vel.y


		// t = 






		m_pTarget->getTransform()->position = glm::vec2(vel);
		/*std::cout << "X: " << vel.x << " Y: " << vel.y << std::endl;*/
		//Launch the Projectile
		// Press Space Bar to Launch the Target Sprite by constructing 
		// an initial velocity vector for the sprite 
		// given the veriables:
		//	-launchElevationAngle(angle, in degrees)             Use Pythagorean Thyrum
		//	-launchSpeed(Scalor, in units per second)
		//
		// Starting Position Variables:
		//	-startingX
		//	-startingY
		// The projectile mus be launched using speed and angle variables, not a vector.
		// The sprite should start at position = (startingX, startingY)
		// If the spacebar is pressed again, it resets the sprite
	}
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

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Goldneo's ImGui Window", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	static float launchElevationAngle = 0.0f; // change the angle of launch // b
	if(ImGui::SliderFloat("Launch Angle", &launchElevationAngle, 0.0f, 250.0f))
	{
		std::cout << launchElevationAngle << std::endl;
	}
	
	static float launchSpeed[1] = {0.0f}; //change velocity // a
	if (ImGui::SliderFloat("Launch Speed", launchSpeed, 0.0f, 500.0f))
	{
		std::cout << launchSpeed << std::endl;
	}

	ImGui::Separator();

	static float accelerationGravity[1] = { 0.0f }; // changes the pull of gravity

	if (ImGui::SliderFloat("Gravity", accelerationGravity, 0.0f, 10.0f))
	{
		std::cout << accelerationGravity << std::endl;
	}

	ImGui::End();
}
