#include "assets.h"
#include <vector>
#include <string>
#include <unordered_map>

/* 
*  Artist Credits:
*  CRAFTPIX https://free-game-assets.itch.io/free-2d-top-down-pixel-dungeon-asset-pack
*  crusenho https://crusenho.itch.io
*/


std::unordered_map<std::string, std::string> hmapPathAsset = {
    {"playerIdlePath", "assets/textures/player/Colour2/Outline/120x80_gifs/__Idle.gif"},
    {"playerRunPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__Run.gif"},
    {"playerAttackPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__Attack.gif"},
    {"playerCrouchPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__CrouchAll.gif"},
    {"playerDeathPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__Death.gif"},
    {"playerTakesHitPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__Hit.gif"},
    {"playerRollPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__Roll.gif"},
    {"playerSlidePath", "assets/textures/player/Colour2/Outline/120x80_gifs/__SlideAll.gif"},
    {"playerTripleSwingGifPath", "assets/textures/player/Colour2/Outline/120x80_gifs/__SwordTripleSwing.gif"},
    {"playerComboAttackTexPath", "assets/textures/player/Colour2/Outline/120x80_PNGSheets/___AttackCombo.png"},
    {"playerProjectilePath", "assets/textures/player/Projectile1.gif"},
    {"playerStunHammerPath", "assets/textures/player/HammerStun.gif"},
    {"wallsfloorPath", "assets/textures/environment/walls_floor.png"},
    {"objectsPath", "assets/textures/environment/objects.png"},
    {"doorChestAnimPath", "assets/textures/environment/doors_lever_chest_animation.png"},
    {"skeletonIdlePath", "assets/textures/enemies/enemies-skeleton1_idle.png"},
    {"skeletonMovePath", "assets/textures/enemies/enemies-skeleton1_movement.png"},
    {"skeletonAttack1Path", "assets/textures/enemies/enemies-skeleton1_attack.png"},
    {"skeletonAttack2Path", "assets/textures/enemies/enemies-skeleton2_attack.png"},
    {"skeletonTakesDmgPath", "assets/textures/enemies/enemies-skeleton1_take_damage.png"},
    {"skeletonDeathPath", "assets/textures/enemies/enemies-skeleton1_death.png"},
    {"dungeon0Base", "assets/levels/level0Base.txt"},
    {"dungeon0Objects", "assets/levels/level0Objects.txt"},
    {"ui1Path", "assets/textures/ui/Spritesheet_UI_Flat.png"},
    {"fontBold1Path", "assets/fonts/BoldPixels.ttf"},
};
//const std::string startMenuBase = "assets/levels/startMenuBase.txt";
//const std::string startMenuText = "assets/levels/startMenuBase.txt";
//const std::string startMenuSelection = "assets/levels/startMenuBase.txt";


std::vector<std::string> environmentTilesheets = {
    "assets/textures/environment/walls_floor.png",  // sheetID 0        // Credit: CRAFTPIX 
    "assets/textures/environment/objects.png",      // sheetID 1        // Credit: CRAFTPIX 
    "assets/textures/ui/Spritesheet_UI_Flat.png",   // sheetID 2        // Credit: crusenho 
    "assets/textures/ui/banners.png",               // sheetID 3 
    "assets/textures/ui/RectangleBox_96x96.png",    // sheetID 4
    "assets/textures/ui/FreeHorrorUi.png",          // sheetID 5
    "assets/textures/environment/trap_animation.png"    // sheetID 6    // Credit: CRAFTPIX 
};