
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include <algorithm>

const int nWidth = 1920;
const int nHeight = 1080;
const int nXScl = 1;
const int nYScl = 1;
const int nStars = 100;
const int nDrops = 100;
const int speed = 1;
int nTime, nFrame = 0;

class Switch {
public:
	int x, y, w, h;
	olc::Pixel palette[4];
	bool val;
	int scl = 1;
	void draw(olc::PixelGameEngine* pge) {
		olc::vi2d currMouse = pge->GetMousePos();
		if (currMouse.x >= scl * x && currMouse.y >= y && currMouse.x < scl * (x + w) && currMouse.y < y + h * scl) {
			//if mouse clicks on the slider 
			if (pge->GetMouse(0).bPressed) {
				val = (val) ? 0 : 1;
			}
		}
		olc::Pixel backClr = palette[0];
		olc::Pixel btnClr = (val) ? palette[1] : palette[3];

		pge->FillRect(x + scl*h/4, y + scl*h/4, scl * (w - h/2), scl * (h - h/2), backClr);
		pge->FillRect(x + (w * val), y,scl * (w / 2), scl*h, btnClr);
	}
};

Switch flip = { 8, nHeight - 11 * 32, 50, 25, {olc::DARK_GREY, olc::BLUE, olc::Pixel(0,127,255), olc::WHITE}, 1, 2 };

class Button {
public:
	int x, y, w, h;
	std::string text;
	olc::Pixel palette[4];
	void (*fnc)(olc::PixelGameEngine*);
	bool bLock = false;
	int scl = 1;
	bool bPressed = false;

	void draw(olc::PixelGameEngine* pge) {
		olc::Pixel btnClr = palette[0];
		olc::Pixel txtClr = palette[1];

		olc::vi2d currMouse = pge->GetMousePos();
		olc::vi2d txtSz = pge->GetTextSize(text);
		
		if(!bLock){
			if (currMouse.x >= scl * x && currMouse.y >= y && currMouse.x < scl * (x + w) && currMouse.y < y + h * scl) {
				btnClr = palette[2];
				if (pge->GetMouse(0).bPressed || pge->GetMouse(0).bHeld) {
					txtClr = palette[3];
					bPressed = true;
				}
			}
			else bPressed = false;
		}

		if (bPressed) {
			fnc(pge);
		}
		pge->FillRect(x, y, scl*w, scl*h, btnClr/(1 + bLock));
		pge->DrawString(x + scl*(w-txtSz.x)/2, y + scl*(h-txtSz.y)/2, text, txtClr / (1 + bLock), scl);
	}
};

Button btn = { 8, nHeight - 9 * 32, 50, 25, "Test", { olc::BLUE, olc::WHITE, olc::Pixel(0,127,255), olc::GREY}, 0, flip.val, 2 };

class Slider {
public:
	//position x and y, width and height
	float x, y, w, h;
	float min, value, max;
	olc::Pixel palette[4];
	int index;
	int scl = 1;
	bool activeSlider = false;
	olc::vi2d prevMouse;

	void draw(olc::PixelGameEngine* pge) {
		if (min > max) {
			std::swap(min, max);
		}
		olc::Pixel head = palette[2];
		olc::vi2d currMouse = pge->GetMousePos();
		olc::vi2d dragMouse = currMouse - prevMouse;
		prevMouse = currMouse;
		//if mouse is over the slider
		if (currMouse.x >= scl * x && currMouse.y >= y && currMouse.x < scl * (x + w) && currMouse.y < y + h * scl) {
			head = palette[3];
			//if mouse clicks on the slider 
			if (pge->GetMouse(0).bPressed || pge->GetMouse(0).bHeld) {
				activeSlider = true;
			}
		}
		if (pge->GetMouse(0).bReleased) {
			activeSlider = false;
		}
		if (activeSlider) {
			value = abs(max - min) * currMouse.x / (scl * w);
			//value += abs(max - min) * dragMouse.x / (scl*w);
			value = (value <= min) ? min : value;
			value = (value > max) ? max : value;
		}
		pge->FillRect(x - (h * scl), y - (h * scl), h * scl, 3 * h * scl, palette[0]);//border
		pge->FillRect(x, y, w * scl, h * scl, palette[1]);//line
		pge->FillRect(x + scl * (w * (value - min) / (max - min)), y - 1 * h * scl, h * scl, 3 * h * scl, head);//head
		pge->DrawString(scl* (x + w), y, std::to_string(value), olc::WHITE,scl);//value

	}
};

Slider sldr  = { 8, nHeight - 1 * 32,50,5,1.0f,5.0f,25.0f,{olc::DARK_GREY,olc::VERY_DARK_GREY,olc::BLUE,olc::CYAN},1, 2 };
Slider sldr2 = { 8, nHeight - 2 * 32,50,5,1.0f,5.0f,25.0f,{olc::GREY,olc::DARK_GREY,olc::RED,olc::Pixel(255,127,0)},2, 2 };
Slider sldr3 = { 8, nHeight - 3 * 32,50,5,1.0f,5.0f,25.0f,{olc::WHITE,olc::GREY,olc::GREEN,olc::YELLOW},3, 2 };

Slider sldrr = { 8, nHeight - 4 * 32,50,5,1.0f,1.0f,25.0f,{olc::DARK_RED,olc::RED,olc::GREY,olc::WHITE},4, 2 };
Slider sldrg = { 8, nHeight - 5 * 32,50,5,1.0f,1.0f,25.0f,{olc::DARK_GREEN,olc::GREEN,olc::GREY,olc::WHITE},5, 2 };
Slider sldrb = { 8, nHeight - 6 * 32,50,5,1.0f,1.0f,25.0f,{olc::DARK_BLUE,olc::BLUE,olc::GREY,olc::WHITE},6, 2 };

class Star {
public:
	int value = sldr.value;
	int value2 = sldr2.value;
	int value3 = sldr3.value;
	int valuer = 10*sldrr.value;
	int valueg = 10*sldrg.value;
	int valueb = 10*sldrb.value;
	//positions, frequency, radius, strength, rate of decay, color;
	int index = -1;
	int posx = std::rand() % nWidth;
	int posy = std::rand() % nHeight;
	int velx = std::rand() % value3 * 2 * speed - speed;
	int vely = std::rand() % value3 * 2 * speed - speed;
	int freq = std::rand() % (value2 + 1) + 1;
	int nStep = 1;
	int dispersal = 3 * (std::rand() % (1 + value) + 5);
	float decay = std::rand() % 3;
	olc::Pixel color = { 255 - valuer + std::rand() % valuer, 255 - valueg + std::rand() % valueg , 255 - valueb + std::rand() % valueb };
	//olc::Pixel color = { olc::WHITE };

	void draw(olc::PixelGameEngine* pge) {
		nStep = (nStep < 10000) ? nStep + 1 : 1;

		posx += velx;
		posy += vely;

		posx = (posx < 0) ? nWidth : (posx > nWidth) ? 0 : posx;
		posy = (posy < 0) ? nHeight : (posy > nHeight) ? 0 : posy;

		int radius = dispersal * abs(sin(freq * nStep * 3.14159 / 360));
		for(int i = radius; i > 0; i--)
		{
			color.a = sqrt(i)*decay;
			pge->FillCircle(posx, posy, 1 + i, color);
		}
		if (radius == 0) {
			index = -1;
		}
	}
};

class Raindrop {
public:
	int posx2 = std::rand() % nWidth;
	int posy2 = std::rand() % nHeight;
	float velx = std::rand() % 10 - 5;
	float vely = std::rand() % 5;
	int length = std::rand() % 5;
	
	//olc::Pixel color = { 64,128,255 };
	olc::Pixel color = { olc::WHITE };

	void draw(olc::PixelGameEngine* pge) {
		int posx1 = posx2;
		int posy1 = posy2;

		posx1 = (posx1 < 0) ? nWidth : posx1;
		posy1 = (posy1 < 0) ? nHeight : posy1;

		posx1 = (posx1 > nWidth) ? 0 : posx1;
		posy1 = (posy1 > nHeight) ? 0 : posy1;


		pge->DrawLine(posx1, posy1, posx1 + velx, posy1 + vely, color);

		posx2 += velx;
		posy2 += vely;

		posx2 = (posx2 < 0) ? nWidth : posx2;
		posy2 = (posy2 < 0) ? nHeight : posy2;

		posx2 = (posx2 > nWidth) ? 0 : posx2;
		posy2 = (posy2 > nHeight) ? 0 : posy2;

		velx += (std::rand() % 11 - 1)/10;
		vely += (std::rand() % 11 - 1)/10;

	}
};

Star stars[nStars];
Raindrop drops[nDrops];

void dmyBtn(olc::PixelGameEngine* pge) {
	sldrr.value = 10.0f;
	sldrg.value = 10.0f;
	sldrb.value = 10.0f;
}

class Basics : public olc::PixelGameEngine
{
public:
	Basics()
	{
		sAppName = "Basic()";
	}
public:
	bool OnUserCreate() override
	{
		std::srand(time(NULL));
		
		for (int n = 0; n < nStars; n++)
		{
			stars[n] = Star();
			stars[n].index = n;
		}
		/*
		for (int n = 0; n < nDrops; n++)
		{
			drops[n] = Raindrop();
		}
		*/
		btn.fnc = dmyBtn;
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		SetPixelMode(olc::Pixel::Mode::ALPHA);
		Sleep(5);

		if (GetKey(olc::P).bHeld || GetKey(olc::P).bPressed) {
			Sleep(5);
		}
		else {
			nTime = (nTime < 504000) ? nTime + 1 : 0;
			nFrame = (nFrame + 1 * (nTime % 10 == 0)) % 12;
			Clear(olc::BLACK);
			for (int n = 0; n < nStars; n++)
			{
				if (stars[n].index == -1) {
					stars[n] = Star();
					stars[n].index = n;
				}
				stars[n].draw(this);
			}
			sldr.draw(this);
			sldr2.draw(this);
			sldr3.draw(this);

			sldrr.draw(this);
			sldrg.draw(this);
			sldrb.draw(this);

			btn.draw(this);
			btn.bLock = flip.val;

			flip.draw(this);
		}
		//sldr.draw(this);
		//sldr2.draw(this);
		//sldr3.draw(this);

		/*
		for (int n = 0; n < nDrops; n++)
		{
			drops[n].draw(this);
		}
		*/
		//DrawString(0, 0, std::to_string(nFrame), olc::WHITE);
		return true;
	}
};

int main()
{
	Basics demo;
	if (demo.Construct(nWidth, nHeight, nXScl, nYScl,1))
	{
		demo.Start();
	}
	return 0;
}