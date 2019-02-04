#pragma once

#include "SDL/SDL.h"

#include <vector>
#include <unordered_map>
#include <SDL/SDL_stdinc.h>
#include <string>

struct vector2 {
	float x;
	float y;
};

class Game {
	public:
		Game();
		bool Initialize();
		void RunLoop();
		void ShutDown();

		class Actor* mActor;
		void AddActor(Actor*);
		void RemoveActor(Actor*);

		std::vector<class Block*> mBlocks;
		void AddBlock(Block*);
		void RemoveBlock(Block*);

		class SpriteComponent* mSprite;
		void AddSprite(SpriteComponent*);
		void RemoveSprite(SpriteComponent*);
		SDL_Texture* GetTexture(std::string);

		class Paddle* GetPaddle();
		Paddle* mPaddle;

	private:
		void processInput();
		void updateGame();
		void generateOutput();
		void LoadData();
		void UnloadData();

		SDL_Renderer* mRenderer;
		SDL_Window* mWindow;
		bool mIsRunning;
		const int thickness = 15;
		Uint32 mTicksCount;

		std::vector<Actor*> mActors;
		std::unordered_map<std::string, SDL_Texture*> mTextures;
		std::vector<SpriteComponent*> mSprites;
};
