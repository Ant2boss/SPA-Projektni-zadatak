#pragma once
#include "SPA_classes.h"
#include "SPA_Screen.h"
#include<string>
namespace SPA {
	namespace Button {
	
		class ButtonClass {
			SPA::Classes::Point2i ButtonPos;
			SPA::Classes::Point2i ButtonSize;
			std::string ButtonText;

			SPA::Classes::Mouse* Mouse;
			SPA::Screen::ScreenClass* MyScreen;

			int ButtonColor = 7;
			int textPos;
		public:
			ButtonClass(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* M, int xPos, int yPos, int xSize, int ySize);
			ButtonClass(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* M, SPA::Classes::Point2i Pos, SPA::Classes::Point2i Size);

			void SetPos(int x, int y);
			void SetPos(SPA::Classes::Point2i P);

			void SetSize(int x, int y);
			void SetSize(SPA::Classes::Point2i S);

			void SetButtonText(std::string Text);

			void SetButtonColor(int Color);

			SPA::Classes::Point2i GetPos();
			SPA::Classes::Point2i GetSize();

			bool isHovered();
			bool isLeftClick();
			bool isRightClick();

			void draw(int ColorOnHover = 7);

		};

	}
}