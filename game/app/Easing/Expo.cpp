#include "Expo.h"

float Expo::easeIn(float t, float b, float c, float d) {
	return (t == 0) ? (float)b : (float)(c * pow(2, 10 * (t / d - 1)) + b);
}
float Expo::easeOut(float t, float b, float c, float d) {
	return (t == d) ? (float)(b + c) : (float)(c * (-pow(2, -10 * t / d) + 1) + b);
}

float Expo::easeInOut(float t, float b, float c, float d) {
	if (t == 0) return (float)b;
	if (t == d) return (float)(b + c);
	if ((t /= d / 2) < 1) return (float)(c / 2 * pow(2, 10 * (t - 1)) + b);
	return (float)(c / 2 * (-pow(2, -10 * --t) + 2) + b);
}