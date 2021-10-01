#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Target.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;


private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;
	Target* m_pTarget;

	glm::vec2 vel = glm::vec2(400.0f, 100.0f); // (vel.x, vel.y)
	float startingX = 0.0f;
	float startingY = 300.0f;
	glm::vec2 startPos = glm::vec2(startingX, startingY);

	float t;
	float d = t;

	float launchSpeed = 10.0f;
	float a = launchSpeed;

	float launchElevationAngle = 250; //Angle In Degrees
	float b = launchElevationAngle;

	float accelerationGravity = 1.0f;
	float c = accelerationGravity;


	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
};

#endif /* defined (__PLAY_SCENE__) */