#pragma once
#include "SPA_Screen.h"
#include "SPA_Entity.h"
#include "SPA_Pathfinding.h"
#include "SPA_Rooms.h"
#include "SPA_Spell.h"

#include <list>
namespace SPA {

	namespace Game {
	
		class StartMenu {

			SPA::Screen::ScreenClass* MyScreen;
			SPA::Classes::Mouse* MyMouse;

		public:
			StartMenu(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* mouse);

			void initiate();

		};

		class TutorialMenu {
			SPA::Screen::ScreenClass MyScreen;
			SPA::Classes::Mouse* MyMouse;

		public:
			TutorialMenu(SPA::Screen::ScreenClass screen, SPA::Classes::Mouse* mouse);

			void initiate();
		};

		class HighscoreKeeper {
			SPA::Classes::scorestruct sc[5];
			std::string filename;

			SPA::Classes::scorestruct createNULLscore();

		public:
			HighscoreKeeper(std::string filename);

			void insertScore(SPA::Classes::scorestruct);

			void saveScores();
			void loadScores();

			SPA::Classes::scorestruct GetScore(int);

		};

		class HighscoreMenu {
			SPA::Game::HighscoreKeeper HK;

			SPA::Screen::ScreenClass MyScreen;
			SPA::Classes::Mouse* MyMouse;

		public:
			HighscoreMenu(SPA::Screen::ScreenClass MyScreen, SPA::Classes::Mouse* mouse, std::string filename);

			void initiate();
		};

		class GameLoop {
			SPA::Screen::ScreenClass* MyScreen;
			SPA::Classes::Mouse* MyMouse;

			SPA::Entity::Player GoodGuy;
			std::vector<SPA::Entity::Enemy> vecOfBadGuys;

			SPA::Classes::PathForAI Path;
			SPA::Classes::GenericWallMesh Walls;

			SPA::Pathfinding::PathfindUtility PFU;

			SPA::Room::FloorManager FloorTemp;
			SPA::Room::RoomDisplayMan RoomDisplay;

			std::list<SPA::Items::ItemStruct> CollectedItems;
			std::list<SPA::Spell::SpellValues> CollectedSpells;

			SPA::Items::ItemStruct PlayerEquipment[6];
			SPA::Spell::SpellValues PlayerSpells[3];

			SPA::Spell::SpellLibrary* lib;

			int CurrentFloor = 1;
			int PlayerGoldAmount = 5;

			void DefaultValuesForChangingRooms();

			void invMenu(SPA::Screen::ScreenClass* MenuScreen);
			void spellsMenu(SPA::Screen::ScreenClass* MenuScreen);
			void playerMenu(SPA::Screen::ScreenClass* MenuScreen);

			void Mercahnt(SPA::Screen::ScreenClass* MenuScreen, SPA::Items::ItemStruct* Offering);
			void Alchemist(SPA::Screen::ScreenClass* MenuScreen, SPA::Spell::SpellValues* Offer);

			std::string BasicItemSummary(SPA::Items::ItemStruct DisplayMe);

			void applyEquipmentToPlayer();

		public:
			GameLoop(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* mouse);

			void initiate(SPA::Spell::SpellLibrary& MySpellLibrary, SPA::Items::ItemLibrary& MyItemLibrary);
		};

		class LoadingLoop {
			SPA::Screen::ScreenClass* MyScreen;
			int Dur;

		public:
			LoadingLoop(SPA::Screen::ScreenClass* screen, int duration);

			void initiate();
		};

	}

}