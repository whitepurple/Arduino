#pragma once
#include <MsTimer2.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 4
#define Y1 A0
#define X1 A1
#define Y2 A3
#define X2 A4
//Define your screen resolution as per your Touch screen (Max: 1024)
#define Xresolution 256 
#define Yresolution 256
#define m     0.6095
#define row   22
#define col   21
#define F_x   22
#define F_y   31
#define R_x   217
#define R_y   218
#define OutOfRange 230
#define TouchDelay 5
#define ShowDelay 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(col*row, PIN, NEO_GRB + NEO_KHZ800);

bool isPushed = false;
int pushedTime = 0;
const float Xdis = (R_x - F_x) / (2.0*(row - 1.0));
const float Ydis = (R_y - F_y) / (col - 1.0);

unsigned long Tmil = 0;

uint8_t X, Y;	//pushed location(dis)
uint8_t xi = 0, yi = 0;
uint8_t tempX, tempY;
uint8_t xpos, ypos;	//pushed location(Mat)
int ledPos = 0;
uint8_t color = 0;
uint8_t timer = 0;
uint8_t dimm = 0;
uint8_t limR = 255;
uint8_t limG = 255;
uint8_t limB = 255; 
uint8_t DimmDelay = 1;
int8_t DimmSize = -2;
int8_t radLim = 31;
uint8_t isRgbOn = 5;
uint8_t screenSaverTimer = 1;

int menu = 0;


void setLim(uint8_t r, uint8_t g, uint8_t b) {
	limR = r; limG = g; limB = b;
}

void settingFnc() {
	Serial.print("x = ");
	Serial.print(X);
	Serial.print("y = ");
	Serial.println(Y);
	//Serial.println(menu);
}

class pix {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t min;
public:
	pix() { R = 0; G = 0; B = 0;	min = 4; }
	uint8_t getR() {		return R;	}
	uint8_t getG() {		return G;	}
	uint8_t getB() {		return B;	}
	uint8_t getMin() { return min; }

	void whatMin() {
		if (R + G + B == 0) { min = 4; return; }
		else if (R <30 && G <30 && B <30) {
			min = 3;
			return;
		}
		min = R > G ? (R>B?0:2) : (G>B?1:2);
	}

	void setR(uint8_t bright) {
		R = bright;
	}
	void setG(uint8_t bright) { 
		G = bright;
	}
	void setB(uint8_t bright) { 
		B = bright;
	}
	void setAll(uint8_t bright) {
		setR(bright); setG(bright); setB(bright);
	}
	void setRGB(uint8_t r, uint8_t g, uint8_t b) {
		setR(constrain(r,0,255));
		setG(constrain(g,0, 255)),
		setB(constrain(b,0, 255));
	}

	void addR(int bright) {
		R = constrain((int)R + (int)bright,0,limR); 
	}
	void addG(int bright) {
		G = constrain((int)G + (int)bright, 0, limG);
	}
	void addB(int bright) {
		B = constrain((int)B + (int)bright, 0, limB);
	}
	void addAll(int8_t gap) {
		if (!gap) return;
		addR(gap); addG(gap); addB(gap);
		whatMin();
	}
	void addRGB(int r, int g, int b) {
		addR(r); addG(g), addB(b);
	}
};

pix colorTable[21][22];

void resetColor() {
	for (uint8_t i = 0; i < col; i++)
		for (uint8_t j = 0; j < row; j++)
			colorTable[i][j].setAll(0);
}

uint16_t posMat[21][22];		//[ypos][xpos]

void setPosMat() {		//xpos, ypos --> ledPos(0~col*row)
	for (uint8_t i = 0; i < col; i++)
		for (uint8_t j = 0; j < row; j++) {
			posMat[i][j] = ((((i % 2 == 0) ? (j) : (-1 * j - 1)) + row*2 * ((i + 1) / 2))
				+ (i % 2 == 0 ? 1 : -1)*(i / 2)) % row + i * row;
		}
}

uint8_t disMat[21][22][2];	//[ypos][xpos][x,y] 0~255,0~255

void setdisMat() {
	for (uint8_t i = 0; i < col; i++)
		for (uint8_t j = 0; j < row; j++) {
			disMat[i][((row-i/2)+j)%row][0] = F_x + (uint8_t)((2 * j) * Xdis);
			disMat[i][((row-i/2)+j)%row][1] = F_y + (uint8_t)(i*Ydis);
			if (i % 2 == 1) {
				disMat[i][((row-i/2)+j)%row][0] += Xdis;
			}			
		}
}


