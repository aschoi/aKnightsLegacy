#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "knightsLegacy/core/public/App.h"
#include "knightsLegacy/core/public/GameManager.h"
#include "knightsLegacy/debug/DebugConfigGame.h"


// 960 x 544 == 60 x 34 == 1.765
int main(int, char**) {


	App app(960, 544, "A Knight's Legacy");
	GameManager game;
	
	app.Run(game);

	#if DEBUG_GAME
		SDL_Log("main.cpp End");
	#endif

	return 0;

}
