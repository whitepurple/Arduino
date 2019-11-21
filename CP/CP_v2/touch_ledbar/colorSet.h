#pragma once
#include "LedDef.h"

class colorSet {
	uint8_t R;
	uint8_t G;
	uint8_t B;
public:
	color() { R = 255; G = 255; B = 255; }
	color(uint8_t r, uint8_t b, uint8_t g){ R = r; G = g; B = b; };
	uint8_t getR() { return R; }
	uint8_t getG() { return G; }
	uint8_t getB() { return B; }
	void addR(int bright) {
		R = constrain((int)R + (int)bright, 0, 254);
	}
	void addG(int bright) {
		G = constrain((int)G + (int)bright, 0, 254);
	}
	void addB(int bright) {
		B = constrain((int)B + (int)bright, 0, 254);
	}
	uint8_t getColor(int i) {
		switch (i) {
		case 0:
			return R;
		case 1:
			return G;
		}
		return B;
	}

	void setR(uint8_t bright) { R = bright; }
	void setG(uint8_t bright) { G = bright; }
	void setB(uint8_t bright) { B = bright; }
	void setRGB(uint8_t r, uint8_t g, uint8_t b) { R = r; G = g; B = b; }
};
