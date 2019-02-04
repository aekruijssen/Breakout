#include "Block.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"

// Asteroid constructor - initialize sprite component, move component, random position/rotation
Block::Block(Game* game)
	:Actor(game)
{
	cc = new CollisionComponent(this);
	cc->SetSize(64.0f, 32.0f);

	sc = new SpriteComponent(this);

	//add Block actor to game
	mGame->AddBlock(this);
}


// Asteroid Deconstructor - remove Block actor from game
Block::~Block() {
	mGame->RemoveBlock(this);
}

void Block::ChangeTexture(char* texturePath) {
	SDL_Texture* texture = mGame->GetTexture(texturePath);
	sc->SetTexture(texture);
}