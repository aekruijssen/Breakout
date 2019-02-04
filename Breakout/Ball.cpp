#include "Game.h"
#include "Ball.h"
#include "SpriteComponent.h"
#include "BallMove.h"
#include "Ball.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"

// Laser constructor - initialize SpriteComponent member var to Laser texture, 
	//MoveComponent member var to forward speed of 400, and lifetime to 0
Ball::Ball(Game* game)
	:Actor(game)
{
	sc = new SpriteComponent(this);
	SDL_Texture* texture = game->GetTexture("Assets/Ball.png");
	sc->SetTexture(texture);

	bm = new BallMove(this);
	bm->SetForwardSpeed(100);
	bm->SetAngularSpeed(0);

	cc = new CollisionComponent(this);
	cc->SetSize(20.0f, 20.0f);

	Vector2 initPos;
	initPos.x = 512;
	initPos.y = 384;
	SetPosition(initPos);

	mBallGame = game;
}


Game* Ball::GetGame() {
	return mBallGame;
}