#include "SPA_Button.h"
#include "SPA_functions.h"

SPA::Button::ButtonClass::ButtonClass(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* M, int xPos, int yPos, int xSize, int ySize) : ButtonClass(screen, M, SPA::Classes::Point2i(xPos, yPos), SPA::Classes::Point2i(xSize, ySize)) {}

SPA::Button::ButtonClass::ButtonClass(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* M, SPA::Classes::Point2i Pos, SPA::Classes::Point2i Size) {
	this->MyScreen = screen;
	this->Mouse = M;
	this->ButtonPos = Pos;
	this->ButtonSize = Size;
}

void SPA::Button::ButtonClass::SetPos(int x, int y) {
	this->SetPos(SPA::Classes::Point2i(x, y));
}

void SPA::Button::ButtonClass::SetPos(SPA::Classes::Point2i P) {
	this->ButtonPos = P;
}

void SPA::Button::ButtonClass::SetSize(int x, int y) {
	this->SetSize(SPA::Classes::Point2i(x, y));
}

void SPA::Button::ButtonClass::SetSize(SPA::Classes::Point2i S) {
	this->ButtonSize = S;
	if (this->ButtonSize.y < 3) this->ButtonSize.y = 3;
	this->textPos = std::round(this->ButtonSize.y / 2);
}

void SPA::Button::ButtonClass::SetButtonText(std::string Text) {
	this->ButtonText = Text;
	this->textPos = std::round(this->ButtonSize.y / 2);
}

void SPA::Button::ButtonClass::SetButtonColor(int Color) {
	this->ButtonColor = Color;
}

SPA::Classes::Point2i SPA::Button::ButtonClass::GetPos() {
	return this->ButtonPos;
}

SPA::Classes::Point2i SPA::Button::ButtonClass::GetSize() {
	return this->ButtonSize;
}

bool SPA::Button::ButtonClass::isHovered() {
	return SPA::Functions::isPointInRegion(this->Mouse->GetMousePos(), this->GetPos(), this->GetSize());
}

bool SPA::Button::ButtonClass::isLeftClick() {
	return (this->isHovered() && this->Mouse->isLeftClick());
}

bool SPA::Button::ButtonClass::isRightClick() {
	return (this->isHovered() && this->Mouse->isRightClick());
}

void SPA::Button::ButtonClass::draw(int ColorOnHover) {
	if (this->GetPos().x < 0 || this->GetPos().x >= this->MyScreen->GetSize().x) return;
	if (this->GetPos().y < 0 || this->GetPos().y >= this->MyScreen->GetSize().y) return;

	int xs = this->GetPos().x;
	int ys = this->GetPos().y;

	int xe = this->GetPos().x + this->GetSize().x;
	int ye = this->GetPos().y + this->GetSize().y;

	int t = 0;
	for (int y = ys; y < ye; y++) {
		for (int x = xs; x <xe; x++) {
			if(this->isHovered()) this->MyScreen->SetColorAt(ColorOnHover, x, y);
			
			if (y == ys || y == ye - 1) this->MyScreen->SetCharAt('=', x, y);
			else this->MyScreen->SetCharAt(' ', x, y);
		}
	}
	SPA::Classes::Point2i T = this->GetPos();

	T.alter(0, this->textPos);

	SPA::Functions::PlaceStringToScreen(this->MyScreen, this->ButtonText, this->ButtonColor, T);
}
