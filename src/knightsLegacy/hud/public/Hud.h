#pragma once
#include "AChoiEngine/hudInterface/HudInterface.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <vector>
#include <string>
#include "AChoiEngine/input/public/Keyboard.h"


class ACE_Camera2D;
class ACE_Camera2D_Center;
class Player;
class TTF_Font;

class Hud : public ACE_HudInterface {
public:

	bool ACE_Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, ACE_Camera2D& cam) override;
	bool Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, ACE_Camera2D_Center& cam, Player& p);

	bool Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, ACE_Camera2D& cam, Player& p);
	void ACE_Shutdown() override;
	void ACE_HandleEvent(Keys key, SDL_Renderer* r) override;
	void ACE_UpdateFixed(double dt, const bool* keys) override;
	void ACE_UpdateFrame(uint64_t now_ms) override;
	void UpdateFrame(uint64_t now_ms, Player& p);
	void ACE_Render(SDL_Renderer* r, ACE_Camera2D& cam) override;
	void ACE_Render(SDL_Renderer* r, ACE_Camera2D_Center& cam) override;

	void ACE_SetLineText(int i, const std::string& s) override;

	SDL_Renderer* appR = nullptr;

	//int get_player_health() const;
	//uint64_t get_q_cd() const;
	//uint64_t get_w_cd() const;
	//uint64_t get_e_cd() const;
	//uint64_t get_r_cd() const;

	//void set_player_health(int new_player_health);
	//void set_q_cd(uint64_t new_q_cd);
	//void set_w_cd(uint64_t new_w_cd);
	//void set_e_cd(uint64_t new_e_cd);
	//void set_r_cd(uint64_t new_r_cd);

	float appW_pixels_ = 0;
	float appH_pixels_ = 0;

	TTF_Font* boldFont = nullptr;
	float worldW_pixels = 0;
	float worldH_pixels = 0;

	std::string health_string = "";
	std::string qCD_string = "";
	std::string wCD_string = "";
	std::string eCD_string = "";
	std::string rCD_string = "";


private:

	int playerHealth_ = 0;
	uint64_t qCD_ = 0;
	uint64_t wCD_ = 0;
	uint64_t eCD_ = 0;
	uint64_t rCD_ = 0;

	std::vector<ACE_HudLine> hudLines_;

	float startY_pixels_ = 0.0f;

};