#include "Game.h"
#include "Actor.h"
#include "Block.h"
#include "SpriteComponent.h"
#include "Paddle.h"
#include "Random.h"
#include "Ball.h"

#include <cmath>
#include <SDL/SDL_image.h>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <string>


// Game constructor - initialize renderer and window to null, running bool to true
Game::Game() {
	mRenderer = nullptr;
	mWindow = nullptr;
	mIsRunning = true;
}


// Initialize - SDL video/audio, create window/renderer, Random, SDL Image, ticks, and call LoadData()
bool Game::Initialize() {
	//initialize sdl video and audio
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL Video: %s", SDL_GetError());
		return false;
	}
	sdlResult = SDL_Init(SDL_INIT_AUDIO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL Audio: %s", SDL_GetError());
		return false;
	}

	//create window
	mWindow = SDL_CreateWindow(
		"Breakout - Adina Kruijssen",
		100,
		100,
		1024,
		768,
		0
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//create renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, 
		-1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return false;
	}

	Random::Init();

	//initialize SDL Image with IMG_Init
	IMG_Init(IMG_INIT_PNG);

	mTicksCount = 0;

	Game::LoadData();

	return true;
}


// ShutDown  - quit SDL Image, unload data (delete Actors/Textures), destroy renderer/window, quit
void Game::ShutDown() {
	IMG_Quit();
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}


// Run Loop - basic game loop (process input -> update game -> generate output)
void Game::RunLoop() {
	while (mIsRunning) {
		processInput();
		updateGame();
		generateOutput();
	}
}


// Process Input - check if exiting/running, get keyboard state, Process Input for all Actors
void Game::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				mIsRunning = false; 
				break;
		}
	}
	//get keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// make a copy of the actor vector. 
	std::vector<Actor*> mActorsCopy = mActors;
	//loop over the copy of all actors and call ProcessInput on each
	for (auto actor : mActorsCopy) {
		actor->ProcessInput(state);
	}
}


// Generate Output - background, swap, draw sprites
void Game::generateOutput() {
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);
	SDL_RenderClear(mRenderer);
	
	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);
	//draw sprites
	for (auto sprite : mSprites) {
		if (sprite->IsVisible()) {
			sprite->Draw(mRenderer);
		}
	}
	//present 
	SDL_RenderPresent(mRenderer);
}


//Update game - update ticks, deltaTime, and Actors
void Game::updateGame() {
	mTicksCount = SDL_GetTicks();
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Update Actors:
	std::vector<Actor*> mActorsCopy = mActors;
	for (auto actor : mActorsCopy) {
		actor->Update(deltaTime);
	}
	std::vector<Actor*> mActorsDestroy;
	for (auto actor : mActors) {
		if (actor->GetState() == ActorState::Destroy) {
			mActorsDestroy.emplace_back(actor);
		}
	}
	for (auto actor : mActorsDestroy) {
		delete actor;
	}
}


// Load data - create actors (Ship, stars background, 10 Asteroids
void Game::LoadData() {
	Actor* background = new Actor(this);
	const Vector2 bgPos = Vector2(512, 384);
	background->SetPosition(bgPos);
	SpriteComponent* bgSprite = new SpriteComponent(background, 90);
	bgSprite->SetTexture(GetTexture("assets/background.png"));

	char x;
	std::ifstream inFile;

	inFile.open("assets/Level.txt");
	if (!inFile) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
	int xPos = 1;
	int yPos = 1;

	char* color = "";

	while (inFile >> x) {
		if (x == 'D') {
			color = "Assets/BlockD.png";
		}
		else if (x == 'A') {
			color = "Assets/BlockA.png";
		}
		else if (x == 'B') {
			color = "Assets/BlockB.png";
		}
		else if (x == 'C') {
			color = "Assets/BlockC.png";
		}
		else if (x == 'E') {
			color = "Assets/BlockE.png";
		}
		else if (x == '.') {
			color = "Assets/BlockF.png";
		}
		if (x != '.') {
			Block* block = new Block(this);
			Vector2 blockPos = Vector2(xPos * 63, yPos * 29);
			block->SetPosition(blockPos);
			block->ChangeTexture(color);
		}

		if (xPos == 14) {
			yPos++;
			xPos = 0;
		}
		else {
			xPos++;
		}
	}
	inFile.close();

	mPaddle = new Paddle(this);
	const Vector2 padPos = Vector2(512, 740);
	mPaddle->SetPosition(padPos);

	Ball* ball = new Ball(this);
}


// Unload data - delete Actors and Textures
void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}
	mActors.clear();

	for (auto texture : mTextures) {
		SDL_DestroyTexture(std::get<1>(texture));
	}
	mTextures.clear();
}



// Add/Remove functions for Actors, Textures, and Sprite Components:

void Game::AddActor(Actor* actor) {
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	auto search = std::find(std::begin(mActors), std::end(mActors), actor);
	if (search != std::end(mActors)) {
		mActors.erase(search);
	}
}

void Game::AddBlock(Block* block) {
	mBlocks.emplace_back(block);
}

void Game::RemoveBlock(Block* block) {
	auto search = std::find(std::begin(mBlocks), std::end(mBlocks), block);
	if (search != std::end(mBlocks)) {
		mBlocks.erase(search);
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	mSprites.emplace_back(sprite);
	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	auto search = std::find(std::begin(mSprites), std::end(mSprites), sprite);
	if (search != std::end(mSprites)) {
		mSprites.erase(search);
	}
}


// Returns texture from filename
SDL_Texture* Game::GetTexture(std::string filename) {
	auto search = mTextures.find(filename.c_str());
	if (search != mTextures.end()) {
		return mTextures[filename];
	}
	else {
		SDL_Surface *image;
		image = IMG_Load(filename.c_str());
		SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, image);
		SDL_FreeSurface(image);
		mTextures.insert({ filename, tex });
		return mTextures[filename.c_str()];
	}
	return nullptr;
}


// Returns paddle
Paddle* Game::GetPaddle() {
	return mPaddle;
}