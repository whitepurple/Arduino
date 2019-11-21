#pragma once
#include "LedDef.h"
#include "colorSet.h"

int isPushedMenu() {
	if (Y > 196 && X < 49) { menu = 1; return 1; }
	return 0;
}

class modeSet {
private:
	colorSet Set;
	colorSet Dim;
	colorSet Rad;
	colorSet gauge;
	uint8_t dimmlist[6][2] = { {1,0}, {3,-1}, {2,-1},{1,-1},{2,-3},{1,-2} }; //delay,size
	uint8_t dimLv;
	uint8_t radius;
	uint16_t radTimer;
	uint16_t gaugeTimer;
	uint8_t menuLoc[3][9][2] = { { { 17,18 },{ 18,18 },{ 19,18 },
									{ 16,19 },{ 17,19 },{ 18,19 },
									{ 16,20 },{ 17,20 },{ 18,20 } },
									{ { 20,18 },{ 21,18 },{ 0,18 },
									{ 19,19 },{ 20,19 },{ 21,19 },
									{ 19,20 },{ 20,20 },{ 21,20 } },
									{ { 1,18 },{ 2,18 },{ 3,18 },
									{ 2,19 },{ 1,19 },{ 0,19 },
									{ 0,20 },{ 1,20 },{ 2,20 } } };
	uint8_t gaugeLoc[28][2] = { { 4,18 },{ 3,19 },{ 3,20 },
									{ 5,18 },{ 4,19 },{ 4,20 },
									{ 6,18 },{ 5,19 },{ 5,20 },
									{ 7,18 },{ 6,19 },{ 6,20 },
									{ 8,18 },{ 7,19 },{ 7,20 },
									{ 9,18 },{ 8,19 },{ 8,20 },
									{ 10,18 },{ 9,19 },{ 9,20 },
									{ 11,18 },{ 10,19 },{ 10,20 },
									{ 12,18 },{ 11,19 },{ 11,20 },{ 12,19 } };
	uint8_t state;
	int menuState;
	int rgbState;
	int isSelected;
	int fGauge;
	pix dimmTable[9];
	pix gaugeTable[28];
public:		
	modeSet() {
					radTimer = 0; gaugeTimer = 0;
					radius = radLim; state = 1; dimLv = 1;
					menuState = -1;	rgbState = 0;
					isSelected = 1; fGauge = 0;
					Set.setRGB(255, 255, 255);
					for (uint8_t i = 0; i < 9; i++) dimmTable[i].setAll(0);
	}
	void onState() {
		state = 1;
		menuState = -1;
		rgbState = 0;
		isSelected = 1;
	}
	void offState() { state = 0; }
	void displayMode() {
		displayMenu();
			
		offState();
	}
	void resetGauge() { fGauge = 0; }

	int detectButton() {
		if (ypos > 17 && X > (F_x + Xdis * 7)) {
			if (X < (F_x + Xdis * 13)) return 1;
			if (X < (F_x + Xdis * 19)) return 2;
			if (X < (F_x + Xdis * 25)) return 3;
		}
		return 0;
	}
	
	void setMenuPix(int i,int j,int r, int g, int b) {
		strip.setPixelColor(posMat[menuLoc[i][j][1]][menuLoc[i][j][0]],
			r, g, b);	
	}

	void displayMenu() {
		if (isSelected==1){
			menuState = detectButton();
			if (menuState >0) isSelected = 0;
		}
		switch (menuState){
		case 1:
			setRGBDis();
			return;
		case 2:
			dimGauge();
			break;
		case 3:
			radGauge();
			break;
		}
		if (isPushedMenu()) {
			menuState = -1;
			rgbState = 0;
			isSelected = 1;
		}
		// initial state
		rgbDis();
		dimDis();
		radDis();
		gaugeDis();
	}

	//rgb
	void rgbDis() {
		for (uint8_t i = 0; i < 9; i++) {
			setMenuPix(0,i,
				(i < 3) ? (Set.getR() / ((i + 1) * 2 - 1)) : 0,
				(i / 3 == 1) ? (Set.getG() / ((i - 2) * 2 - 1)) : 0,
				(i > 5) ? (Set.getB() / ((i - 5) * 2 - 1)) : 0);
			if (state) strip.show();
		}
	}
	void setRGB() {
		int a = detectButton();
		if (a > 0) rgbState = a;


		a = detectGauge();
		if (fGauge < 10) fGauge = a;
		int aa = (a - fGauge)/2;
		if (a > 0)
			switch (rgbState) {
			case 1:
				setLim(constrain(Set.getR() + aa,0,250), Set.getG(), Set.getB());
				Set.addR(aa);
				break;
			case 2:
				setLim(Set.getR(), constrain(Set.getG(),0,250) + aa, Set.getB());
				Set.addG(aa);
				break;
			case 3:
				setLim(Set.getR(), Set.getG(), constrain(Set.getB() + aa,0,250));
				Set.addB(aa);
				break;
			}
		
		isRgbOn = 7;
	}
	void setRGBDis() {
		setRGB();

		for(uint8_t i=0;i<3;i++)
			for (uint8_t j = 0; j < 9; j++) {
				setMenuPix(i,j,
					(i == 0) ? (Set.getR() / (j / 3 + 1)) : 0,
					(i == 1) ? (Set.getG() / (j / 3 + 1)) : 0,
					(i == 2) ? (Set.getB() / (j / 3 + 1)) : 0);			
			}
		for (uint8_t k = 0; k < 28; k++) {
			uint8_t xx = gaugeLoc[k][0];
			uint8_t yy = gaugeLoc[k][1];
			strip.setPixelColor(posMat[yy][xx],
				Set.getR() / (k % 3 + 1), Set.getG() / (k % 3 + 1), Set.getB() / (k % 3 + 1));
		}
		returnRGB();
	}
	void returnRGB() {
		uint8_t  mm[6][2] = { { 14,18 },{ 15,18 },{ 13,19 },
										{ 14,19 },{ 13,20 },{ 14,20 } };
		for (uint8_t i = 0; i < 6; i++) {
			strip.setPixelColor(posMat[mm[i][1]][mm[i][0]],
				(i / 2 == 0) ? 255 * (i % 2 + 1) : 0,
				(i / 2 == 1) ? 255 * (i % 2 + 1) : 0,
				(i / 2 == 2) ? 255 * (i % 2 + 1) : 0);
		}
		if (isPushedMenu()) {
			setLim(255, 255, 255);
			isRgbOn = 5;
			onState();
			Set.setRGB(255, 255, 255);
		}
	}
	//rgb end

	//dimm
	void dimDis() {
		if(dimm%(256/(max(dimLv-1,2)))==0)setdimDis();
		for (uint8_t i = 0; i < 9; i++) {
			uint8_t xx = menuLoc[1][i][0];
			uint8_t yy = menuLoc[1][i][1];
			if (!(dimm%DimmDelay)) { dimmTable[i].addAll(DimmSize); }
			strip.setPixelColor(posMat[yy][xx], dimmTable[i].getR(),
				dimmTable[i].getG(), dimmTable[i].getB());
			if (state) strip.show();
		}
	}
	void setdimDis() { 
		setRandDimColor();
		for (int i = 0; i < 9; i++) {
			uint8_t xx = menuLoc[1][i][0];
			uint8_t yy = menuLoc[1][i][1];
			int xtmp = (int)disMat[yy][xx][0] - (int)disMat[18][19][0];
			int ytmp = (int)disMat[yy][xx][1] - (int)disMat[18][19][1];
			int bright = constrain(255 - (int)(0.18*(xtmp*xtmp + ytmp*ytmp)), 0, 255);

			dimmTable[i].setRGB((bright / (float)255)*Dim.getR(),
				(bright / (float)255)*Dim.getG(), (bright / (float)255)*Dim.getB());
		}
	}
	void setRandDimColor() {
		Dim.setRGB(random(30, 250), random(30, 250), random(30, 250));
	}
	void setDimLv(uint8_t Lv) {
		dimLv = constrain(Lv,0,5);
		DimmDelay = dimmlist[dimLv][0];
		DimmSize = dimmlist[dimLv][1];
	}
	void dimGauge() {
		int g = dimLv;
		if (detectGauge()!=0) {
			g = detectGauge();
			 //10~79
			g = (g - 10) / 10;
			setDimLv(g);
		}	
		for (uint8_t k = 0; k < 28; k++) {
			if (k > g*5+4)return;
			uint8_t xx = gaugeLoc[k][0];
			uint8_t yy = gaugeLoc[k][1];
			strip.setPixelColor(posMat[yy][xx],
				Dim.getR(), Dim.getG(), Dim.getB());
		}
	}
	//dimm end

	//radius
	void radDis() {
		radius = radLim;
		setRadColor();
		for (uint8_t i = 0; i < 9; i++) {
			int br = getRadBright(i);
			setMenuPix(2,i,
				(int)((Rad.getR()*br)/(float)255),
				(int)((Rad.getG()*br) / (float)255),
				(int)((Rad.getB()*br) / (float)255));
			if (state) strip.show();
		}
	}
	int getRadBright(int i) {
		uint8_t factor = 255;
		int xtmp = Xdis * 2 * (i+1);
		float a = (float)factor / (float)(radius*radius+800);
		int bright = constrain(factor - (int)(a*(xtmp*xtmp)), 0, 255);
		return bright;
	}
	void Wheel(byte WheelPos) {
		WheelPos = 255 - WheelPos;
		if (WheelPos < 85) {
			Rad.setRGB(255 - WheelPos * 3, 0, WheelPos * 3);
			return;
		}
		if (WheelPos < 170) {
			WheelPos -= 85;
			Rad.setRGB(0, WheelPos * 3, 255 - WheelPos * 3);
			return;
		}
		WheelPos -= 170;
		Rad.setRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
		return;
	}
	void setRadColor() {
		radTimer++;
		Wheel(((256 / 9) + radTimer) & 255);
	}
	void radGauge() {
		int g = radius;
		if (detectGauge() != 0) {
			g = detectGauge();
			//10~79
			radius = radLim = constrain(83-g,8,80);
		}
		Serial.println(radius);
		double a = (double)255 / (double)(radius*radius);
		int xtmp =0, ytmp = 0;
		for (uint8_t k = 0; k < 28; k++) {
			uint8_t xx = gaugeLoc[k][0];
			uint8_t yy = gaugeLoc[k][1];
			xtmp = (int)disMat[yy][xx][0] - (int)disMat[19][12][0];
			ytmp = (int)disMat[yy][xx][1] - (int)disMat[19][12][1];
			int bright = constrain(255 - (int)(a*(xtmp*xtmp + 0.7*ytmp*ytmp)), 0, 255);
			strip.setPixelColor(posMat[yy][xx],
				max(0,(Rad.getR()*bright)/256), 
				max(0,(Rad.getG()*bright) / 256), 
				max(0,(Rad.getB()*bright) / 256));
		}
	}
	
	//radius end

	//gauge
	int detectGauge() { // 10~79
		if (ypos > 17 && X > (R_x - 17 * Xdis) && X < OutOfRange) {
			return constrain(X - (R_x - 17 * Xdis),10,80);
		}
		else return 0;
	}
	void gaugeDis() {
		if (menuState > 1) return;
		setWaveGauge();
		int a;
		for (uint8_t i = 0; i < 28; i++) {
			a = i % 3 + 1;
			uint8_t xx = gaugeLoc[i][0];
			uint8_t yy = gaugeLoc[i][1];
			strip.setPixelColor(posMat[yy][xx], gaugeTable[i].getR()/a,
				gaugeTable[i].getG()/a, gaugeTable[i].getB()/a);
		}
	}
	void setWaveGauge() {
		uint8_t i = gaugeTimer++ % 28;
		gaugeTable[i].setRGB(255 - Rad.getR(), 255 - Rad.getG(), 255 - Rad.getB());
	}
	//gauge end
};

modeSet ms;

void exportTable() {
	for (uint8_t i = 0; i < col; i++) {
		for (uint8_t j = 0; j < row; j++) {
			//if (colorTable[i][j].getMin() == 4 && i<18)continue;
			if (!(dimm%DimmDelay)) { colorTable[i][j].addAll(DimmSize); }
			strip.setPixelColor(posMat[i][j], colorTable[i][j].getR(),
				colorTable[i][j].getG(), colorTable[i][j].getB());
		}
	}
	if (menu) ms.displayMode();
}

void rShow() {
	exportTable();
	strip.show();
}
