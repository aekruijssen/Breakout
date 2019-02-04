#pragma once
#include "Actor.h"

class Paddle : public Actor
{
public:
	Paddle(class Game* game);
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;


	class CollisionComponent* cc;

private:
	class MoveComponent* mc;
	class SpriteComponent* sc;

	int mPaddleDir;
};