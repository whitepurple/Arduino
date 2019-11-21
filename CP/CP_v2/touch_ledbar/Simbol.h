#pragma once
#include "LedDef.h"
#include "colorSet.h"

class asteroid{
	int mm; //mass
	int x;	//location
	int y;
	int xxpos, yypos;	//pos
	float a;
	int vVector[2]={ 2,1 };	//speed vector
public:
	colorSet cc;
	asteroid() {
		x = 50*random(1, 4);
		y = 50*random(1, 4);
		mm = 1; a = 0;
	}
	int getX() { return x; }
	int getY() { return y; }
	float getA() { return a; }
	int getVX() { return vVector[0]; }
	int getVY() { return vVector[1]; }
	int getM() { return mm; }
	void setVec(int xx, int yy) { vVector[0] = xx; vVector[1] = yy; }

	void setLoc(int xx, int yy) { x = xx; y = yy; }
	void draw() {
		//	x = random(max(F_x, x - 10), min(x + 10, R_x));
		//	y = random(max(F_y, y - 10), min(y + 10, R_y)); 
		setNextLoc();
		locToPos(x, y);
		int k = (int)(20 / (2 * Xdis)) + 1;
		float a = 20/((float)20*20);
		uint8_t bright = 0;
		int xtmp = 0, ytmp = 0;
		for (uint8_t i = 0; i < (2 * k + 1); i++)
			for (uint8_t j = 0; j < (2 * k + 1); j++) {
				if (i + j < k || i + j>3 * k) continue;
				uint8_t ii = yypos - k + i;
				if (ii >= col) continue;
				uint8_t jj = (xxpos + row - k + j) % row;
				xtmp = (int)disMat[ii][jj][0] - (int)x;
				ytmp = (int)disMat[ii][jj][1] - (int)y;
				if (abs(xtmp) > 180) { continue; }
				bright = constrain(20 - (int)(a*(xtmp*xtmp + 0.7*ytmp*ytmp)), 0, 255);
				colorTable[ii][jj].addRGB((bright*cc.getR()) / 255,
					(bright*cc.getG()) / 255, (bright*cc.getB()) / 255);
			}
	}
	void locToPos(int xx, int yy) {
		int xii = (int)((xx - (F_x - Xdis)) / Xdis);
		int yii = (int)((yy - (F_y - Ydis)) / Ydis);

		if ((xii + yii) % 2 == 1) {
			if ((m*(X - (F_x + Xdis * (xii - 0.5))) + (F_y + Ydis * (yii - 0.5)))> Y) {
				xxpos = (int)((xii / 2.0) - yii + (int)(yii / 2.0 + 1) + row) % row;
				yypos = yii - 1;
			}
			else {
				xxpos = (int)((xii / 2.0) - yii + (int)(yii / 2.0 + 1) + row - 1) % row;
				yypos = yii;
			}
		}
		else {
			xxpos = (int)((xii / 2.0) - yii + (int)(yii / 2.0 + 0.5) + row) % row;
			if ((-1 * m*(X - (F_x + Xdis * (xii - 0.5))) + (F_y + Ydis * (yii - 0.5)))> Y) {
				yypos = yii - 1;
			}
			else {
				yypos = yii;
			}
		}
	}
	void setNextLoc() {
		if (x > 240 || x < 20) vVector[0] = -vVector[0];
		if (y > 240 || y < 20) vVector[1] = -vVector[1];
		x = x + vVector[0];
		y = y + vVector[1];	
	}
};

asteroid AA;
//asteroid BB;
//asteroid CC;