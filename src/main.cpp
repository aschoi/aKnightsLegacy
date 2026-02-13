#include "knightsLegacy/configs/configs.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "knightsLegacy/core/public/App.h"
#include "knightsLegacy/core/public/GameManager.h"


// 960 x 544 == 60 x 34 == 1.765
int main(int, char**) {

#ifdef PERF_TEST_ON
	App app(1700, 1000, "A Knight's Legacy");
#else
	App app(960, 544, "A Knight's Legacy");
#endif
	GameManager game;
	
	app.Run(game);

#if DEBUG_GAME
	SDL_Log("main.cpp End");
#endif

	return 0;

}
