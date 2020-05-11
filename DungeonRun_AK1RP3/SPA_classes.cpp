#include "SPA_classes.h"

SPA::Classes::Point2i::Point2i() : x(-1), y(-1) {}

SPA::Classes::Point2i::Point2i(int x, int y) {
	this->init(x, y);
}

void SPA::Classes::Point2i::init(int x, int y) {
	this->x = x;
	this->y = y;
}

void SPA::Classes::Point2i::alter(int xalt, int yalt) {
	this->x += xalt;
	this->y += yalt;
}

bool SPA::Classes::Point2i::operator==(Point2i P) {
	if (this->x != P.x) return false;
	if (this->y != P.y) return false;
	return true;
}

bool SPA::Classes::Point2i::operator!=(Point2i P) {
	if (this->x == P.x && this->y == P.y) return false;
	return true;
}

bool SPA::Classes::Point2i::operator==(int v) {
	return (this->x == v && this->y == v);
}

bool SPA::Classes::Point2i::operator!=(int v) {
	if (this->x == v && this->y == v) return false;
	return true;
}

SPA::Classes::Point2i SPA::Classes::Mouse::GetRealPos() {
	POINT FullMousePos;
	GetCursorPos(&FullMousePos);

	RECT ConsoleRect;
	GetWindowRect(GetConsoleWindow(), &ConsoleRect);

	SPA::Classes::Point2i MousePos;
	MousePos.x = FullMousePos.x - ConsoleRect.left - 8;
	MousePos.y = FullMousePos.y - ConsoleRect.top - 32;

	return MousePos;
}

SPA::Classes::Mouse::Mouse(HANDLE& ConsoleInputHandle) {
	SetConsoleMode(ConsoleInputHandle, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);
}

SPA::Classes::Point2i SPA::Classes::Mouse::GetMousePos() {
	SPA::Classes::Point2i MosePos = this->GetRealPos();
	MosePos.x /= 8;
	MosePos.y /= 16;
	
	return MosePos;
}

bool SPA::Classes::Mouse::isLeftClick() {
	return (GetKeyState(VK_LBUTTON) < 0);
}

bool SPA::Classes::Mouse::isRightClick() {
	return (GetKeyState(VK_RBUTTON) < 0);
}

void SPA::Classes::Stats::SetAll(int am) {
	this->Health = am;
	this->Mana = am;
	this->Attack = am;
	this->Magic = am;
	this->Speed = am;
}
