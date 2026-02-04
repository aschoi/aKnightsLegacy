#include "knightsLegacy/utilities/public/CooldownComponent.h"



bool CooldownComponent::checkCD() {

	if (onCD) {
		if (timerCD < durationCD) {
			timerCD++;
		}
		else if (timerCD >= durationCD) {
			onCD = false;
			timerCD = 0;
		}
	}
	return onCD;
};


void CooldownComponent::activateCD() {
	onCD = true;
};

void CooldownComponent::resetCD() {
	onCD = false;
	timerCD = 0;
};

