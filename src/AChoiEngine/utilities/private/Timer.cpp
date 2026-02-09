#pragma once


class ACE_Timer {
	float length;
	float time;
	bool timeout;
public:
	ACE_Timer(float length) : length(length), time(0), timeout(false) {}
	void step(float deltaTime) {
		time += deltaTime;
		if (time >= length) {
			time -= length;
			timeout = true;
		}
	}
	bool isTimeout() const { return timeout; }
	float getTime() const { return time; }
	float getLength() const { return length; }
	void reset() { time = 0; }
};