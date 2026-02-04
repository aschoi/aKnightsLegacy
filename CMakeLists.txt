cmake_minimum_required(VERSION 3.20)
project(aKnightsLegacy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)


add_executable(aKnightsLegacy
	src/main.cpp
	"src/AChoiEngine/gameObject/private/GameObject.cpp"
    	"src/AChoiEngine/gfx/private/AnimationComponent.cpp" 
	"src/AChoiEngine/physics/private/CollisionDetectionComponent.cpp" 
	"src/AChoiEngine/utilities/private/Timer.cpp" 
	"src/AChoiEngine/AI/private/aiComponent.cpp" 
	"src/AChoiEngine/worldLayer/private/LdtkMapLoader.cpp" 
	"src/AChoiEngine/worldLayer/private/Map.cpp" 
	"src/AChoiEngine/worldLayer/private/TxtMapLoader.cpp"
    	"src/knightsLegacy/core/private/App.cpp"
	"src/knightsLegacy/core/private/GameManager.cpp"
        "src/knightsLegacy/gameStates/private/CreditsGameState.cpp"
        "src/knightsLegacy/gameStates/private/MainGameplayGameState.cpp"
        "src/knightsLegacy/gameStates/private/StartMenuGameState.cpp"
	"src/knightsLegacy/entities/player/private/Player.cpp"
    	"src/knightsLegacy/entities/enemies/private/Skeleton.cpp" 
    	"src/knightsLegacy/world/private/TileCatalog.cpp" 
	"src/knightsLegacy/utilities/private/CooldownComponent.cpp" 
	"src/knightsLegacy/weapons/private/Projectile.cpp" 
	"src/knightsLegacy/weapons/private/Stun.cpp" 
	"src/knightsLegacy/hud/private/Hud.cpp" 
	"src/knightsLegacy/gameStates/private/PauseGameState.cpp"
	"src/knightsLegacy/audioManager/private/AudioManager.cpp"
	"src/assets.cpp" )


target_compile_definitions(aKnightsLegacy PRIVATE SHOW_VECTOR_MAP=0)

target_include_directories(aKnightsLegacy PRIVATE src)

target_link_libraries(aKnightsLegacy PRIVATE 
	SDL3::SDL3 
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
	nlohmann_json::nlohmann_json
)

# ---- SDL3_mixer (manual imported target, MSVC-safe) ----
set(SDL_MIXER_PREFIX "C:/dev/third_party/SDL3_mixer_vcpkg")

add_library(SDL3_mixer_manual SHARED IMPORTED GLOBAL)
set_target_properties(SDL3_mixer_manual PROPERTIES
  IMPORTED_IMPLIB "${SDL_MIXER_PREFIX}/lib/SDL3_mixer.lib"   # link-time
  IMPORTED_LOCATION "${SDL_MIXER_PREFIX}/bin/SDL3_mixer.dll" # runtime
  INTERFACE_INCLUDE_DIRECTORIES "${SDL_MIXER_PREFIX}/include"
)

target_link_libraries(aKnightsLegacy PRIVATE SDL3_mixer_manual)

if (WIN32)
  add_custom_command(TARGET aKnightsLegacy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${SDL_MIXER_PREFIX}/bin"
      "$<TARGET_FILE_DIR:aKnightsLegacy>"
    COMMENT "Copying SDL3_mixer runtime DLLs"
  )
endif()
