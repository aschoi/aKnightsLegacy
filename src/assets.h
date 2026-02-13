#pragma once
#include <string>
#include <unordered_map>
#include <vector>

extern std::unordered_map<std::string, std::string> hmapPathAsset;
extern std::vector<std::string> environmentTilesheets;

// PLAYER VISUALS
// Credit: aamatniekss https://aamatniekss.itch.io/fantasy-knight-free-pixelart-animated-character
const std::string playerIdlePath = "assets/textures/player/Colour2/Outline/120x80_gifs/__Idle.gif";
const std::string playerRunPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__Run.gif";
const std::string playerAttackPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__Attack.gif";
const std::string playerCrouchPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__CrouchAll.gif";
const std::string playerDeathPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__Death.gif";
const std::string playerTakesHitPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__Hit.gif";
const std::string playerRollPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__Roll.gif";
const std::string playerSlidePath = "assets/textures/player/Colour2/Outline/120x80_gifs/__SlideAll.gif";
const std::string playerTripleSwingGifPath = "assets/textures/player/Colour2/Outline/120x80_gifs/__SwordTripleSwing.gif";
const std::string playerComboAttackTexPath = "assets/textures/player/Colour2/Outline/120x80_PNGSheets/___AttackCombo.png";

// WEAPON VISUALS
// Credit: frostwindz https://frostwindz.itch.io/
const std::string playerProjectilePath = "assets/textures/player/Projectile1.gif";
const std::string playerStunHammerPath = "assets/textures/player/HammerStun.gif";

// SKELETON VISUALS
// Credit: Pixel_Poem https://pixel-poem.itch.io/dungeon-assetpuck
const std::string skeletonIdlePath = "assets/textures/enemies/enemies-skeleton1_idle.png";
const std::string skeletonMovePath = "assets/textures/enemies/enemies-skeleton1_movement.png";
const std::string skeletonAttack1Path = "assets/textures/enemies/enemies-skeleton1_attack.png";
const std::string skeletonAttack2Path = "assets/textures/enemies/enemies-skeleton2_attack.png";
const std::string skeletonTakesDmgPath = "assets/textures/enemies/enemies-skeleton1_take_damage.png";
const std::string skeletonDeathPath = "assets/textures/enemies/enemies-skeleton1_death.png";

// MISC VFX
const std::string ouch1Path = "assets/textures/effects/ouch1Gif.gif";

// WORLD TILES
// Credit: CRAFTPIX https://free-game-assets.itch.io/free-2d-top-down-pixel-dungeon-asset-pack
const std::string wallsfloorPath = "assets/textures/environment/walls_floor.png";
const std::string objectsPath = "assets/textures/environment/objects.png";
const std::string doorChestAnimPath = "assets/textures/environment/doors_lever_chest_animation.png";
const std::string trapsPath = "assets/textures/environment/trap_animation.png";

// UI VISUALS
// Credit: crusenho https://crusenho.itch.io
const std::string ui1Path = "assets/textures/ui/Spritesheet_UI_Flat.png";
const std::string ui2Path = "assets/textures/ui/Spritesheet_UI_Flat_Animated.png";
const std::string fontBold1Path = "assets/fonts/BoldPixels.ttf";
// Credit: bdragon1727 https://bdragon1727.itch.io/
const std::string ui3Path = "assets/textures/ui/banners.png";
const std::string boxPath = "assets/textures/ui/RectangleBox_96x96.png";
const std::string ui4Path = "assets/textures/ui/FreeHorrorUi.png";

// GAMESTATE LAYOUTS
const std::string startMenuBase = "assets/levels/startMenuBase.txt";
const std::string startMenuText = "assets/levels/startMenuText.txt";
const std::string startMenuTextV2 = "assets/levels/startMenuTextV2.txt";
const std::string startMenuTextV3 = "assets/levels/startMenuTextV3.txt";
const std::string startMenuTextV4 = "assets/levels/startMenuTextV4.txt";
const std::string startMenuTextV5 = "assets/levels/startMenuTextV5.txt";
const std::string startMenuTextV6 = "assets/levels/startMenuTextV6.txt";

// START MENU
const std::string backgroundCastle = "assets/textures/ui/interior_castle_2.png";
const std::string jsonStartMenu = "assets/levels/start_menu/title_screen.ldtk";

// DUNGEON LAYOUTS
const std::string dungeon0Base = "assets/levels/level0Base.txt";
const std::string dungeon0Objects = "assets/levels/level0Objects.txt";
const std::string dungeon1Base = "assets/levels/level_1/level1Base.txt";
const std::string dungeon1Objects = "assets/levels/level_1/level1Objects.txt";
const std::string map1 = "assets/levels/level_1/map1.txt";
const std::string jsonLevel1 = "assets/levels/level_1/Level1.ldtk";
const std::string jsonLevel1v2 = "assets/levels/level_1/level1_v2.ldtk";

// PERF TEST MAPS
const std::string emptyMap1824x1824 = "assets/levels/perf_tests/1824x1824_empty_map.ldtk";
const std::string emptyMap3008x3008 = "assets/levels/perf_tests/3008x3008_empty_map.ldtk";
const std::string wallsMap3008x3008 = "assets/levels/perf_tests/3008x3008_walls_map.ldtk";
const std::string emptyMap4096x4096 = "assets/levels/perf_tests/4096x4096_empty_map.ldtk";


// SFX
const std::string sfxSwipe = "assets/audio/swipe.wav";
const std::string uiConfirm = "assets/audio/sci_fi_confirm.wav";
const std::string uiSelect = "assets/audio/sci_fi_hover_high.wav";
const std::string sfxHammerPath = "assets/audio/sfxHammer.wav";
const std::string sfxUltPath = "assets/audio/ult.wav";
const std::string sfxSplatPath = "assets/audio/crunch_splat.wav";

// MUSIC
const std::string music1 = "assets/audio/music/Action_1_(Loop).wav";
const std::string music2 = "assets/audio/music/Action_3_(Loop).wav";





const std::vector<std::string> arrowPaths{
	"assets/textures/ui/arrows/arrow_n.png",
	"assets/textures/ui/arrows/arrow_ne.png",
	"assets/textures/ui/arrows/arrow_e.png",
	"assets/textures/ui/arrows/arrow_se.png",
	"assets/textures/ui/arrows/arrow_s.png",
	"assets/textures/ui/arrows/arrow_sw.png",
	"assets/textures/ui/arrows/arrow_w.png",
	"assets/textures/ui/arrows/arrow_nw.png",
};

