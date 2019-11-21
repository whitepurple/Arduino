#pragma once
#include "LedDef.h"



int read_x() {
	pinMode(Y1, INPUT);
	pinMode(Y2, INPUT_PULLUP);

	pinMode(X1, OUTPUT);
	digitalWrite(X1, LOW);
	pinMode(X2, OUTPUT);
	digitalWrite(X2, HIGH);
	delay(5);//p
	return (analogRead(Y1)) / (1024.0 / Xresolution);
	//X = analogRead(Y1)/3;
}

int read_y() {
	pinMode(X1, INPUT);
	pinMode(X2, INPUT_PULLUP);

	pinMode(Y1, OUTPUT);
	digitalWrite(Y1, LOW);
	pinMode(Y2, OUTPUT);
	digitalWrite(Y2, HIGH);
	delay(5);//p
	return (analogRead(X1)) / (1024.0 / Yresolution);
	//Y = analogRead(X1)/3;
}

void detectLocation() {
	X = read_x();
	Y = read_y();
	
	if (X > OutOfRange || Y > OutOfRange) { isPushed = false; return; }
	else {
		
		tempX = xi;
		tempY = yi;
		xi = (int)((X - (F_x - Xdis)) / Xdis);
		yi = (int)((Y - (F_y - Ydis)) / Ydis);

		if (((((xi > tempX) ? (xi - tempX) : (tempX - xi)) > 3 )||
			(((yi > tempY) ? (yi - tempY) : (tempY - yi)) > 1)) &&
			isPushed) {
			xi = tempX;
			yi = tempY;
		}
		isPushed = true;

		if ((xi + yi) % 2 == 1) {
			if ((m*(X - (F_x + Xdis * (xi - 0.5))) + (F_y + Ydis * (yi - 0.5)))> Y) {
				xpos = (int)((xi / 2.0) - yi + (int)(yi / 2.0 + 1) + row) % row;
				ypos = yi - 1;
			}
			else {
				xpos = (int)((xi / 2.0) - yi + (int)(yi / 2.0 + 1) + row - 1) % row;
				ypos = yi;
			}
		}
		else {
			xpos = (int)((xi / 2.0) - yi + (int)(yi / 2.0 + 0.5) + row) % row;
			if ((-1 * m*(X - (F_x + Xdis * (xi - 0.5))) + (F_y + Ydis * (yi - 0.5)))> Y) {
				ypos = yi - 1;
			}
			else {
				ypos = yi;
			}
		}
	    //Serial.println(ledPos);
	}
}

