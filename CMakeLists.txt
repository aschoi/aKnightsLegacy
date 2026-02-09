cmake_minimum_required(VERSION 3.24)
project(aKnightsLegacy LANGUAGES CXX)

if (MSVC)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(USE_SYSTEM_SDL3_MIXER ON)
if (MSVC)
  set(USE_SYSTEM_SDL3_MIXER OFF)
endif()
if (USE_SYSTEM_SDL3_MIXER)
  find_package(SDL3_mixer CONFIG QUIET)
endif()

add_executable(aKnightsLegacy
	src/main.cpp
	"src/AChoiEngine/gameObject/private/GameObject.cpp"
	"src/AChoiEngine/gfx/private/AnimationSys.cpp" 
	"src/AChoiEngine/physics/private/OverlappingHitboxDetectionSys.cpp" 
	 "src/AChoiEngine/physics/private/CollisionResponseSys.cpp"
	"src/AChoiEngine/utilities/private/Timer.cpp" 
	"src/AChoiEngine/AI/private/aiSys.cpp" 
	"src/AChoiEngine/worldLayer/private/LdtkMapLoader.cpp" 
	"src/AChoiEngine/worldLayer/private/MapObject.cpp" 
	"src/AChoiEngine/worldLayer/private/TxtMapLoader.cpp"
	"src/AChoiEngine/render/private/RenderSys.cpp"
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
	"src/knightsLegacy/audioManager/private/AudioManager.cpp"
	"src/assets.cpp")


target_compile_definitions(aKnightsLegacy PRIVATE SHOW_VECTOR_MAP=0)

target_include_directories(aKnightsLegacy PRIVATE src)

target_link_libraries(aKnightsLegacy PRIVATE 
	SDL3::SDL3 
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
	nlohmann_json::nlohmann_json
)

if (SDL3_mixer_FOUND)
  target_link_libraries(aKnightsLegacy PRIVATE SDL3_mixer::SDL3_mixer)
else()
  include(FetchContent)
  FetchContent_Declare(SDL3_mixer_src
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG prerelease-3.1.2
  )
  FetchContent_MakeAvailable(SDL3_mixer_src)

  target_link_libraries(aKnightsLegacy PRIVATE SDL3_mixer::SDL3_mixer)
endif()

if (WIN32)
  add_custom_command(TARGET aKnightsLegacy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:aKnightsLegacy>
            $<TARGET_RUNTIME_DLLS:aKnightsLegacy>
    COMMAND_EXPAND_LISTS
  )
endif()
