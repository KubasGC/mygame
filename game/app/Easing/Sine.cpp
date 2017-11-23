#include "Sine.h"

float Sine::easeIn(float t, float b, float c, float d) {
	return (float)(-c * cos(t / d * (PI / 2)) + c + b);
}
float Sine::easeOut(float t, float b, float c, float d) {
	return (float)(c * sin(t / d * (PI / 2)) + b);
}

float Sine::easeInOut(float t, float b, float c, float d) {
	return (float)(-c / 2 * (cos(PI*t / d) - 1) + b);
}