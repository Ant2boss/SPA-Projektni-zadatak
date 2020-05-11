#pragma once

#include"SPA_classes.h"
#include "SPA_Screen.h"
#include "SPA_Entity.h"
#include <stack>
#include <string>
#include <fstream>

namespace SPA{

	namespace Functions {

		int GenRand(int min, int max);
		void EmptyPath(SPA::Classes::PathForAI& path);
		std::string generateProgressBar(char BarYes, char BarNo, int BarLen, int BarMaxVal, int BarCurrVal);
		double Distance(SPA::Classes::Point2i P1, SPA::Classes::Point2i P2);
		bool isPointInRegion(SPA::Classes::Point2i P, SPA::Classes::Point2i RegionOrigin, SPA::Classes::Point2i RegionSize);
		void DrawPathToDestination(SPA::Screen::ScreenClass* MainScreen, std::stack<SPA::Classes::Point2i> Path);
		void PlaceStringToScreen(SPA::Screen::ScreenClass* MainScreen, std::string text, int TextColor, SPA::Classes::Point2i Position);
		void PlaceMultipleLinesOfText(SPA::Screen::ScreenClass* MainScreen, std::string text, int Color, SPA::Classes::Point2i Origin);
		int RemoveBadGuys(std::vector<SPA::Entity::Enemy>& badGuysVec);

	}
}