#pragma once
#include"SPA_classes.h"
#include<vector>

namespace SPA {

	namespace Screen {

		class ScreenClass {
			HANDLE hConOut = NULL;

			SPA::Classes::Point2i Size;

			std::vector<CHAR_INFO> CIarray;
			std::vector<CHAR_INFO> OutArray;

			int indexOf(int x, int y);

			void MakeEmptyArray();

		public:
			ScreenClass(HANDLE& ConsoleOutputHandle, int xSize, int ySize);
			ScreenClass(HANDLE& ConsoleOutputHandle, SPA::Classes::Point2i ScreenSize);

			SPA::Classes::Point2i GetSize();

			void SetCharAt(char C, int x, int y);
			void SetCharAt(char C, SPA::Classes::Point2i P);

			void SetColorAt(int C, int x, int y);
			void SetColorAt(int C, SPA::Classes::Point2i P);

			CHAR_INFO GetValuesAt(int x, int y);
			CHAR_INFO GetValuesAt(SPA::Classes::Point2i P);

			CHAR_INFO& adminAt(int x, int y);
			CHAR_INFO& adminAt(SPA::Classes::Point2i P);

			void GenerateWallMesh(std::vector<bool>& WallMesh);

			bool isPointOnScreen(SPA::Classes::Point2i P);

			void ClearScreen();
			void ClearScreen(int x, int y);

			void draw();
		};

	}

}