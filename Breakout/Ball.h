#pragma once
#include "Actor.h"

class Ball : public Actor
{
public:
	Ball(class Game* game);

	Game* mBallGame;
	Game* GetGame();

	class CollisionComponent* cc;

private:
	class BallMove* bm;
	class SpriteComponent* sc;
};