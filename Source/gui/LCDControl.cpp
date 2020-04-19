#include "LCDControl.h"

#include <lcdgui/LayeredScreen.hpp>
#include "Constants.h"

#include <Logger.hpp>
#include <gui/BasicStructs.hpp>

LCDControl::LCDControl(const String& componentName, std::weak_ptr<mpc::lcdgui::LayeredScreen> ls)
	: VmpcComponent(componentName)
{
	this->ls = ls;
	lcd = Image(Image::ARGB, 496, 120, true);
}

void LCDControl::drawPixelsToImg() {
	auto pixels = ls.lock()->getPixels();
	Colour c;
	const auto rectX = dirtyRect.getX();
	const auto rectY = dirtyRect.getY();
	const auto rectRight = dirtyRect.getRight();
	const auto rectBottom = dirtyRect.getBottom();

	for (int x = rectX; x < rectRight; x++) {
		for (int y = rectY; y < rectBottom; y++) {
			const auto x_x2 = x * 2;
			const auto y_x2 = y * 2;
			if ((*pixels)[x][y] == true) {
				c = Constants::LCD_HALF_ON;
				lcd.setPixelAt(x_x2, y_x2, Constants::LCD_ON);
			}
			else {
				c = Constants::LCD_OFF;
				lcd.setPixelAt(x_x2, y_x2, c);
			}
					
			lcd.setPixelAt(x_x2 + 1, y_x2, c);
			lcd.setPixelAt(x_x2 + 1, y_x2 + 1, c);
			lcd.setPixelAt(x_x2, y_x2 + 1, c);
		}
	}
	dirtyRect = Rectangle<int>();
}

void LCDControl::checkLsDirty() {
	if (ls.lock()->IsDirty()) {
		ls.lock()->Draw();
		auto da = ls.lock()->getDirtyArea();
		da->L -= 1;
		da->B += 1;
		if (da->L < 0) da->L = 0;
		if (da->R > 248) da->R = 248;
		if (da->T < 0) da->T = 0;
		if (da->B > 60) da->B = 60;
		dirtyRect = Rectangle<int>(da->L, da->T, da->W(), da->H());
		ls.lock()->getDirtyArea()->Clear();
		drawPixelsToImg();
		repaint();
	}
}

void LCDControl::timerCallback() {
	checkLsDirty();
}

void LCDControl::paint(Graphics& g)
{
	g.drawImageAt(lcd, 0, 0);
}

LCDControl::~LCDControl() {
	MLOG("LCDControl dtor");
}