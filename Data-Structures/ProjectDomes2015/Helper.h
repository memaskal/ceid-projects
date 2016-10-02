#pragma once

#include <cstdlib>
#include <ctime>

namespace Helper {

	time_t currTime;

	/**
	*  Seeds the rand with clock
	**/
	void seedRand() {
		srand(time(NULL));
	}


	/**
	*	Returns a random number beetween min, max
	**/
	int getRandNum(int min, int max) {
		return rand() % (max - min) + min;
	}


	/**
	*  Saves the current time
	**/
	void startTimer() {
		currTime = clock();
	}


	/**
	*  Returns the elapsed time
	**/
	double stopTimer() {
		return (double)1000 *(clock() - currTime) / CLOCKS_PER_SEC;
	}
};