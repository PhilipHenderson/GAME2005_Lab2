#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance().load("../Assets/textures/Bee.png","circle");

	const auto size = TextureManager::Instance().getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
}

Target::~Target()
= default;

void Target::draw()
{
	// draw the target
	TextureManager::Instance().draw("circle",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
