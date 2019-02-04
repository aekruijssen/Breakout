#include "Game.h"
#include "Paddle.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Math.h"

// Ship constructor - initialize SpriteComponent member var to stationary ship texture, 
	//MoveComponent member var to speeds of 0, and laser lifetime to 0
Paddle::Paddle(Game* game)
	:Actor(game)
{
	sc = new SpriteComponent(this);
	SDL_Texture* texture = mGame->GetTexture("Assets/Paddle.png");
	sc->SetTexture(texture);

	mc = new MoveComponent(this);
	mc->SetAngularSpeed(0.0f);
	mc->SetForwardSpeed(0.0f);

	cc = new CollisionComponent(this);
	cc->SetSize(104.0f, 24.0f);

	mPaddleDir = 0;
}


// OnProcessInput - check keystate for paddle control
void Paddle::OnProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_LEFT]) {
		mPaddleDir = -1;
	}
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		mPaddleDir = 1;
	}
	else {
		mPaddleDir = 0;
	}
}


// OnUpdate - update position, ensure paddle cannot move off-screen
void Paddle::OnUpdate(float deltaTime) {
	if (mPaddleDir != 0) {
		mPosition.x += mPaddleDir * 300.0f * deltaTime;
	}

	if (GetPosition().x >= 1024 - 84) {
		Vector2 boundaryPos = GetPosition();
		boundaryPos.x = 1024 - 84;
		SetPosition(boundaryPos);
	}
	else if (GetPosition().x <= 84) {
		Vector2 boundaryPos = GetPosition();
		boundaryPos.x = 84;
		SetPosition(boundaryPos);
	}
}