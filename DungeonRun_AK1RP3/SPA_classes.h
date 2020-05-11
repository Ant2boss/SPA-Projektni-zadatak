#pragma once
#include <Windows.h>
#include <stack>
#include <vector>
#include <string>

namespace SPA {

	namespace Classes {

		class Point2i {
		public:
			int x;
			int y;

			Point2i();
			Point2i(int x, int y);
			void init(int x, int y);
			void alter(int xalt, int yalt);

			bool operator== (Point2i P);
			bool operator!= (Point2i P);

			bool operator== (int v);
			bool operator!= (int v);
		};

		class Mouse {

			SPA::Classes::Point2i GetRealPos();

		public:

			Mouse(HANDLE& ConsoleInputHandle);

			SPA::Classes::Point2i GetMousePos();

			bool isLeftClick();
			bool isRightClick();
		};

		static struct Color {
			static const int BLACK = 0;
			static const int D_BLU = 1;
			static const int D_GRN = 2;
			static const int D_CYN = 3;
			static const int D_RED = 4;
			static const int D_MAG = 5;
			static const int D_YEL = 6;
			static const int WHITE = 7;
			static const int GRAY = 8;
			static const int BLUE = 9;
			static const int GREEN = 10;
			static const int CYAN = 11;
			static const int RED = 12;
			static const int MAG = 13;
			static const int YEL = 14;
			static const int WHITEWHITE = 15;
		};

		static struct Direction {
			static const int UP = 0;
			static const int RIGHT = 1;
			static const int DOWN = 2;
			static const int LEFT = 3;
		};

		struct Stats {
			int Health;
			int Mana;

			int Attack;
			int Magic;

			int Speed;

			void SetAll(int am);
		};

		struct scorestruct {
			int floorCount;
			int levelReached;
			std::string bestItemFound;

		};

		typedef std::stack<SPA::Classes::Point2i> PathForAI;

		typedef std::vector<bool> GenericWallMesh;
	}

}