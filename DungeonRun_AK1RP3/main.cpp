#include<iostream>
#include<sstream>
#include<queue>
using namespace std;

#include "SPA_Game.h"

int main() {

	HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConIn = GetStdHandle(STD_INPUT_HANDLE);

	SPA::Spell::SpellLibrary Slib("./SpellFolder/GameSpells.txt");
	SPA::Items::ItemLibrary Ilib("./ItemsFolder/GameItems.txt");

	srand(time(nullptr));

	SPA::Screen::ScreenClass DisplayScreen(hConOut, 64, 48);
	SPA::Classes::Mouse MyMouse(hConIn);

	SPA::Game::LoadingLoop(&DisplayScreen, 5000).initiate();

	while (1) {
		SPA::Game::StartMenu(&DisplayScreen, &MyMouse).initiate();

		SPA::Game::GameLoop(&DisplayScreen, &MyMouse).initiate(Slib, Ilib);
	}
	return 0;
}

/*
int main() {

	HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConIn = GetStdHandle(STD_INPUT_HANDLE);

	SPA::Screen::ScreenClass MyScreen(hConOut, 64 , 32);
	SPA::Classes::Mouse MyMouse(hConIn);

	for (int y = 1; y < MyScreen.GetSize().y - 1; y++) {
		for (int x = 1; x < MyScreen.GetSize().x - 1; x++) {
			MyScreen.adminAt(x, y).Char.UnicodeChar = '.';
		}
	}

	SPA::Entity::Player TestDummy(&MyScreen);
	TestDummy.SetPos(20, 20);

	SPA::Classes::PathForAI P;
	SPA::Classes::GenericWallMesh Walls;

	MyScreen.GenerateWallMesh(Walls);

	SPA::Spell::SpellLibrary SPL("./SpellFolder/ExampleSpells.txt");
	SPA::Spell::SpellClass MySpell(&MyScreen, SPL.GetSpellAt(1));

	SPA::Items::ItemLibrary ITL("./ItemsFolder/ExampleItems.txt");

	TestDummy.SetWeapon(ITL.GetItem(0));

	vector<SPA::Entity::Enemy> BadGuys;
	SPA::Entity::Enemy Tbad(&MyScreen, 1000);
	Tbad.SetPos(5, 5);

	BadGuys.push_back(Tbad);

	SPA::Button::ButtonClass B1(&MyScreen, &MyMouse, 5, 5, 20, 7);
	B1.SetButtonText("ExampleText");
	B1.SetPos(10, 5);

	while (1) {
		SPA::Classes::Point2i Muuse = MyMouse.GetMousePos();

		if (MyScreen.isPointOnScreen(Muuse)) {
			
			if (MyMouse.isLeftClick() && MyScreen.adminAt(Muuse).Char.UnicodeChar != '#') {
				while (!P.empty()) P.pop();
				SPA::Pathfinding::PathfindUtility PFU(MyScreen.GetSize());
				PFU.SetStartingPoint(TestDummy.GetPos());
				PFU.SetEndingPoint(Muuse);
				PFU.SetWallMesh(Walls);

				PFU.FindPath(P);

				TestDummy.SetPath(P);

				while (MyMouse.isLeftClick()) {}
			}

			if (MyMouse.isRightClick()) {
				while (!P.empty()) P.pop();
				TestDummy.GetPath(P);
				SPA::Functions::DrawPathToDestination(&MyScreen, P);
			}

			if (GetKeyState(' ') < 0) {
				
				TestDummy.DamageEnemies(BadGuys);
			}

			if (GetKeyState('R') < 0) {
				if(!TestDummy.isAlive()) TestDummy.DamagePlayer(-1000);
			}

			if (GetKeyState('W') < 0) {
				MySpell.SetCastLocation(TestDummy.GetPos());
			}
		}

		if (BadGuys[0].isAlive()) BadGuys[0].draw();

		if (MySpell.isSpellUp()) {
			MySpell.ApplyEffectsToPlayer(TestDummy);
			MySpell.DamageEnemiesInAOE(BadGuys, TestDummy.GetStats().Magic);
			MySpell.drawSpell();
		}
		if (TestDummy.isAlive()) TestDummy.draw();

		B1.draw(SPA::Classes::Color::RED);

		MyScreen.draw();
	}


	/*
	SPA::Items::ItemLibrary ExampleItemLib("./ItemsFolder/ExampleItems.txt");
	SPA::Entity::TextEntity TextExample(&MyScreen, "Apple");
	TextExample.SetPosition(10, 10);
	TextExample.SetVisibleAmount(5000);

	SPA::Spell::SpellLibrary ExampleSpellLib("./SpellFolder/ExampleSpells.txt");
	
	SPA::Spell::SpellClass ExampleSpell(&MyScreen);
	ExampleSpell.SetActiveSpell(ExampleSpellLib.GetSpellAt(0));

	SPA::Entity::Enemy BadGuy(&MyScreen, 200);
	BadGuy.SetPos(10, 10);

	vector<SPA::Entity::Enemy> vecOfBadGuys;
	vecOfBadGuys.push_back(BadGuy);

	BadGuy.SetPos(30, 10);
	vecOfBadGuys.push_back(BadGuy);

	SPA::Pathfinding::PathfindUtility PFU(64, 32);

	PFU.SetStartingPoint(SPA::Classes::Point2i(50, 2));
	PFU.SetEndingPoint(SPA::Classes::Point2i(50, 30));

	vector<bool> Walls;
	MyScreen.GenerateWallMesh(Walls);
	PFU.SetWallMesh(Walls);

	stack<SPA::Classes::Point2i> Path;
	PFU.FindPath(Path);

	BadGuy.SetPath(Path);
	
	BadGuy.SetPos(50, 2);
	BadGuy.SetTimeBetweenMoves(500);
	vecOfBadGuys.push_back(BadGuy);

	SPA::Items::ItemClass Item("./ItemsFolder/ExampleItems.txt", "./SpellFolder/ExampleSpells.txt", 2);

	while (1) {

		if (SPA::Functions::isPointInRegion(MyMouse.GetMousePos(), { 0,0 }, MyScreen.GetSize())) {

			if (GetKeyState(' ') < 0) {
				ExampleSpell.SetCastLocation(MyMouse.GetMousePos());
			}

			if (MyMouse.isRightClick()) {
				MyScreen.adminAt(MyMouse.GetMousePos()).Char.UnicodeChar = '#';
			}

			if (GetKeyState('R') < 0) {
					for (int i = 0; i < vecOfBadGuys.size(); i++) {
						if (!vecOfBadGuys[i].isAlive()) vecOfBadGuys[i].TakeDamage(-1000);
					}
				}
		}

		if (ExampleSpell.isSpellUp()) {
			ExampleSpell.DamageEnemiesInAOE(vecOfBadGuys);
			ExampleSpell.drawSpell();
		}
		for (int i = 0; i < vecOfBadGuys.size(); i++) {
			if (vecOfBadGuys[i].isAlive()) vecOfBadGuys[i].draw();
		}
		
		if(TextExample.isVisible()) TextExample.draw();
		MyScreen.draw();
	}


	/*
	SPA::Pathfinding::PathfindUtility PFU( MyScreen.GetSize() );

	PFU.SetStartingPoint(SPA::Classes::Point2i(5, 5));
	PFU.SetEndingPoint(SPA::Classes::Point2i(20, 10));

	std::vector<bool> Walls;
	MyScreen.GenerateWallMesh(Walls);

	stack<SPA::Classes::Point2i> Putanja;

	SPA::Entity::Enemy BadGuy1(&MyScreen, 50);
	BadGuy1.SetPos(7, 7);
	BadGuy1.SetEnemyChar('0');
	BadGuy1.SetTimeBetweenMoves(50);

	while (1) {

		if (SPA::Functions::isPointInRegion(MyMouse.GetMousePos(), { 0,0 }, MyScreen.GetSize())) {
			//MyScreen.SetColorAt(SPA::Classes::Color::GREEN, MyMouse.GetMousePos());
			//MyScreen.SetCharAt('0', MyMouse.GetMousePos());
			//SPA::Functions::PlaceStringToScreen(&MyScreen, "Proba", MyMouse.GetMousePos());
			if (MyMouse.isLeftClick()) {
				PFU.SetEndingPoint(MyMouse.GetMousePos());
				PFU.SetWallMesh(Walls);
				PFU.FindPath(Putanja);

				SPA::Functions::DrawPathToDestination(&MyScreen, Putanja);
				while (!Putanja.empty()) Putanja.pop();

			}
			if (MyMouse.isRightClick()) {
				MyScreen.adminAt(MyMouse.GetMousePos()).Char.UnicodeChar = '#';
				//MyScreen.adminAt(MyMouse.GetMousePos()).Attributes = 0;
				MyScreen.GenerateWallMesh(Walls);
			}
			if (GetKeyState(' ') < 0) {
				PFU.SetStartingPoint(MyMouse.GetMousePos());
			}

			MyScreen.SetCharAt('8', MyMouse.GetMousePos());
			MyScreen.SetColorAt(SPA::Classes::Color::CYAN, MyMouse.GetMousePos());

			SPA::Classes::Point2i Pos = MyMouse.GetMousePos();

			if (MyMouse.isLeftClick() && MyScreen.adminAt(Pos).Char.UnicodeChar != '#' && BadGuy1.isAlive()) {
				while (!Putanja.empty()) Putanja.pop();

				PFU.SetStartingPoint(BadGuy1.GetPos());
				PFU.SetEndingPoint(Pos);
				PFU.SetWallMesh(Walls);

				PFU.FindPath(Putanja);
				BadGuy1.SetPath(Putanja);

				while (MyMouse.isLeftClick()) {}
			}

			if (MyMouse.isRightClick()) {
				BadGuy1.GetPath(Putanja);
				SPA::Functions::DrawPathToDestination(&MyScreen, Putanja);
			}

			if (GetKeyState(' ') < 0) {

				BadGuy1.TakeDamage(10);

				while (GetKeyState(' ') < 0) {}
			}

			if (GetKeyState('Q') < 0) {
				MyScreen.adminAt(Pos).Char.UnicodeChar = '#';
				MyScreen.adminAt(Pos).Attributes = 7;
				MyScreen.GenerateWallMesh(Walls);
			}

			if (GetKeyState('H') < 0) {
				MyScreen.adminAt(Pos).Char.UnicodeChar = '#';
				MyScreen.adminAt(Pos).Attributes = 0;
				MyScreen.GenerateWallMesh(Walls);
			}
			if (GetKeyState('R') < 0) {
				MyScreen.adminAt(Pos).Char.UnicodeChar = '.';
				MyScreen.adminAt(Pos).Attributes = 7;
				MyScreen.GenerateWallMesh(Walls);
			}

		}

		if(BadGuy1.isAlive()) BadGuy1.draw();

		MyScreen.draw();
	}

	return 0;
}
*/