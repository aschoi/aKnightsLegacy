#pragma once


class CooldownComponent {



public:

	CooldownComponent(
		int durationCD)
		:	durationCD(durationCD) { }

	
	bool checkCD();
	void activateCD();
	void resetCD();
	int durationCD;
	int timerCD = 0;
	bool onCD = false;

private:
	
	
};