#pragma once
#include "LedDef.h"


uint8_t factor = 20; // set 255// add 20



void spread(uint8_t r) {
	if (menu && ypos > 17) return;
	int k = (int)(r / (2 * Xdis))+1;
	double a = (double)factor / (double)(r*r);
	uint8_t bright = 0;
	int xtmp = 0, ytmp = 0;
	for (uint8_t i = 0; i < (2*k+1); i++)
		for (uint8_t j = 0; j < (2 * k + 1); j++) {
			if (i + j < k || i + j>3 * k) continue;
			uint8_t ii = ypos - k + i;
			if (ii >= col) continue;
			uint8_t jj = (xpos + row - k + j) % row;
			xtmp = (int)disMat[ii][jj][0] - (int)X;
			ytmp = (int)disMat[ii][jj][1] - (int)Y;
			if (abs(xtmp) > 180) { continue; }
			bright = constrain(factor - (int)(a*(xtmp*xtmp + 0.7*ytmp*ytmp)), 0, 255);

			switch (isRgbOn) {
			case 5:
				switch (color) {
				case 0:
					colorTable[ii][jj].addR(bright); break;
				case 1:
					colorTable[ii][jj].addG(bright); break;
				case 2:
					colorTable[ii][jj].addB(bright); break;
				case 3:
				case 4:
					colorTable[ii][jj].addB(bright); break;
				}
				break;
			case 6:				
				colorTable[ii][jj].addAll(bright); break;
			case 7:
				colorTable[ii][jj].addRGB((bright*limR)/255,
					(bright*limG) /255, (bright*limB)/255);
				break;
			}
			colorTable[ii][jj].whatMin();
		}
}

void setNextColor() {
	int all[5] = { 0,0,0,0,0 };
	for (uint8_t i = 0; i < col; i++)
		for (uint8_t j = 0; j < row; j++) {
			all[colorTable[i][j].getMin()]++;
		}
	color = 0;
	for (uint8_t i = 1; i<3; i++) {
		if (all[color]>all[i]) color = i;
	}
}