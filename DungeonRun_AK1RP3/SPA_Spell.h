#pragma once
#include "SPA_classes.h"
#include "SPA_Entity.h"
#include "SPA_Screen.h"
#include <string>
#include <vector>
namespace SPA {
	
	namespace Spell {

		struct SpellValues {
			int SpellID = -1;
			int CastType = -1;
			int DamageType;

			int DamageAmount;
			int ManaCost = 0;
			int TimeBetweenApply;
			int ExistDuration;

			std::string SpellName = "--";
			std::string SpellFlavor;
		};
		
		class SpellClass {

			SPA::Spell::SpellValues MySpell;

			SPA::Classes::Point2i CastLocation;
		
			std::chrono::time_point<std::chrono::steady_clock> Activated;
			std::chrono::time_point<std::chrono::steady_clock> LastAffect = std::chrono::high_resolution_clock::now();
			SPA::Screen::ScreenClass* MyScreen;

			SPA::Classes::Point2i SpellOrigin;
			SPA::Classes::Point2i SpellSize;

			void CalcSpellProp();
			bool inBounds();

		public:
			SpellClass(SPA::Screen::ScreenClass* screen);
			SpellClass(SPA::Screen::ScreenClass* screen, SPA::Spell::SpellValues ValuesOfSpell);

			void SetActiveSpell(SPA::Spell::SpellValues Val);

			void SetCastLocation(int x, int y);
			void SetCastLocation(SPA::Classes::Point2i P);

			bool NeedsToBeTargeted();

			bool isSpellUp();

			std::string GetSpellName();
			std::string GetSpellFlavor();
			int GetSpellCost();

			void DamageEnemiesInAOE(std::vector<SPA::Entity::Enemy>& EnemiesVec, int PlayerMagicVal = 0);

			void ApplyEffectsToPlayer(SPA::Entity::Player& MyPlayer, int PlayerMagicVal = 0);

			void drawSpell();
		};

		class SpellLibrary {

			std::vector<SPA::Spell::SpellValues> LibraryOfSpells;

		public:
			SpellLibrary(std::string LibraryFileName);

			SPA::Spell::SpellValues GetSpellAt(int index);
			int GetSpellCount();
		};

	}

}