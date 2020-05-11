#include "SPA_Screen.h"

int SPA::Screen::ScreenClass::indexOf(int x, int y) {
	return y * this->GetSize().x + x;
}

void SPA::Screen::ScreenClass::MakeEmptyArray() {
	this->CIarray.empty();
	this->CIarray.reserve(this->Size.x * this->Size.y);

	CHAR_INFO t;
	t.Attributes = 7;
	t.Char.UnicodeChar = '#';

	for (int i = 0; i < this->GetSize().x * this->GetSize().y; i++) {
		this->CIarray.push_back(t);
	}
}

SPA::Screen::ScreenClass::ScreenClass(HANDLE& hConOut, int xSize, int ySize) : ScreenClass(hConOut, SPA::Classes::Point2i(xSize, ySize) ) {}

SPA::Screen::ScreenClass::ScreenClass(HANDLE& hConOut, SPA::Classes::Point2i ScreenSize) {
	this->hConOut = hConOut;
	this->Size = ScreenSize;

	this->MakeEmptyArray();
	this->OutArray = CIarray;
}

SPA::Classes::Point2i SPA::Screen::ScreenClass::GetSize() {
	return this->Size;
}

void SPA::Screen::ScreenClass::SetCharAt(char C, int x, int y) {
	int ind = this->indexOf(x, y);
	this->OutArray[ind].Char.UnicodeChar = C;
}

void SPA::Screen::ScreenClass::SetCharAt(char C, SPA::Classes::Point2i P) {
	this->SetCharAt(C, P.x, P.y);
}

void SPA::Screen::ScreenClass::SetColorAt(int C, int x, int y) {
	int ind = this->indexOf(x, y);
	this->OutArray[ind].Attributes = C;
}

void SPA::Screen::ScreenClass::SetColorAt(int C, SPA::Classes::Point2i P) {
	this->SetColorAt(C, P.x, P.y);
}

CHAR_INFO SPA::Screen::ScreenClass::GetValuesAt(int x, int y) {
	int ind = this->indexOf(x, y);
	return this->OutArray[ind];
}

CHAR_INFO SPA::Screen::ScreenClass::GetValuesAt(SPA::Classes::Point2i P) {
	return this->GetValuesAt(P.x , P.y);
}

CHAR_INFO& SPA::Screen::ScreenClass::adminAt(int x, int y) {
	int ind = this->indexOf(x, y);
	return this->CIarray[ind];
}

CHAR_INFO& SPA::Screen::ScreenClass::adminAt(SPA::Classes::Point2i P) {
	return this->adminAt(P.x , P.y);
}

void SPA::Screen::ScreenClass::GenerateWallMesh(std::vector<bool>& WallMesh) {
	WallMesh.clear();
	for (int y = 0; y < this->GetSize().y; y++) {
		for (int x = 0; x < this->GetSize().x; x++) {
			int ind = this->indexOf(x, y);
			if (this->CIarray[ind].Char.UnicodeChar == '#') WallMesh.push_back(true);
			else WallMesh.push_back(false);
		}
	}
}

bool SPA::Screen::ScreenClass::isPointOnScreen(SPA::Classes::Point2i P) {
	if (P.x < 0 || P.x >= this->GetSize().x) return false;
	if (P.y < 0 || P.y >= this->GetSize().y) return false;
	return true;
}

void SPA::Screen::ScreenClass::ClearScreen() {
	for (int y = 0; y < this->GetSize().y; y++) {
		for (int x = 0; x < this->GetSize().x; x++) {
			this->adminAt(x, y).Char.UnicodeChar = ' ';
			this->adminAt(x, y).Attributes = 7;
		}
	}
}

void SPA::Screen::ScreenClass::ClearScreen(int _x, int _y) {
	for (int y = 0; y < _y; y++) {
		for (int x = 0; x < _x; x++) {
			this->adminAt(x, y).Char.UnicodeChar = ' ';
			this->adminAt(x, y).Attributes = 7;
		}
	}
}

void SPA::Screen::ScreenClass::draw() {
	COORD BuffSIze;
	BuffSIze.X = this->GetSize().x;
	BuffSIze.Y = this->GetSize().y;

	COORD BuffCoord = { 0,0 };

	SMALL_RECT SR;
	SR.Top = 0;
	SR.Left = 0;
	SR.Bottom = this->GetSize().y;
	SR.Right = this->GetSize().x;

	WriteConsoleOutput(this->hConOut, &this->OutArray[0], BuffSIze, BuffCoord, &SR);
	this->OutArray = this->CIarray;
}
