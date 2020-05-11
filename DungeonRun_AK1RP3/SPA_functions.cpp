#include "SPA_functions.h"
#include<cmath>
#include<sstream>

int SPA::Functions::GenRand(int min, int max) {
	return (rand()%(max - min + 1) + min );
}

void SPA::Functions::EmptyPath(SPA::Classes::PathForAI& path) {
	while (!path.empty()) path.pop();
}

std::string SPA::Functions::generateProgressBar(char BarYes, char BarNo, int BarLen, int BarMaxVal, int BarCurrVal) {
	double step = (double)BarMaxVal / (double)BarLen;
	std::stringstream ss;
	for (int i = 0; i < BarLen; i++) {
		if (BarCurrVal > step * i) ss << BarYes;
		else ss << BarNo;
	}
	return ss.str();
}

double SPA::Functions::Distance(SPA::Classes::Point2i P1, SPA::Classes::Point2i P2) {
	return std::sqrt(std::pow(P1.x - P2.x, 2) + std::pow(P1.y - P2.y, 2));
}

bool SPA::Functions::isPointInRegion(SPA::Classes::Point2i P, SPA::Classes::Point2i RegionOrigin, SPA::Classes::Point2i RegionSize) {
	if (P.x < RegionOrigin.x || P.x >= RegionOrigin.x + RegionSize.x) return false;
	if (P.y < RegionOrigin.y || P.y >= RegionOrigin.y + RegionSize.y) return false;
	return true;
}

void SPA::Functions::DrawPathToDestination(SPA::Screen::ScreenClass* MainScreen, std::stack<SPA::Classes::Point2i> Path) {
	while (!Path.empty()) {

		MainScreen->SetCharAt('+', Path.top());
		MainScreen->SetColorAt(SPA::Classes::Color::GREEN, Path.top());

		Path.pop();
	}
}

void SPA::Functions::PlaceStringToScreen(SPA::Screen::ScreenClass* MainScreen, std::string text, int TextColor,SPA::Classes::Point2i Position) {
	if (Position.x < 0 || Position.x >= MainScreen->GetSize().x) return;
	if (Position.y < 0 || Position.y >= MainScreen->GetSize().y) return;

	if (Position.x + text.size() >= MainScreen->GetSize().x) return;

	for (int i = 0; i < text.size(); i++) {
		SPA::Classes::Point2i Poz = Position;
		Poz.alter(i, 0);
		MainScreen->SetCharAt(text[i], Poz);
		MainScreen->SetColorAt(TextColor, Poz);
	}
}

void SPA::Functions::PlaceMultipleLinesOfText(SPA::Screen::ScreenClass* MainScreen, std::string text, int Color, SPA::Classes::Point2i Origin) {

	SPA::Classes::Point2i P = Origin;

	for (int i = 0; i < text.size(); i++) {
		if (!MainScreen->isPointOnScreen(P)) continue;
		if (text[i] == '\n') {
			P.x = Origin.x;
			P.y++;
			continue;
		}
		MainScreen->SetCharAt(text[i], P);
		MainScreen->SetColorAt(Color, P);
		P.x++;
	}
}

int SPA::Functions::RemoveBadGuys(std::vector<SPA::Entity::Enemy>& badGuysVec) {
	int BadGuysRemoved = 0;
	for (auto it = badGuysVec.begin(); it != badGuysVec.end();) {
		if (!it->isAlive()) {
			it = badGuysVec.erase(it);
			BadGuysRemoved++;
		}
		else {
			++it;
		}
	}
	return BadGuysRemoved;
}

