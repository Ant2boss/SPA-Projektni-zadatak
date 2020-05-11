#pragma once
#include "SPA_classes.h"
#include "SPA_Screen.h"
#include "SPA_Items.h"
#include <chrono>
#include <stack>
#include <string>

namespace SPA{

	namespace Entity {

		class TextEntity {
			SPA::Screen::ScreenClass* MyScreen;

			std::string MyText;
			SPA::Classes::Point2i MyPos;
			int VisibleDuration = 1000;
			int textcolor = 7;

			std::chrono::time_point<std::chrono::steady_clock> StartCount;
		public:
			TextEntity(SPA::Screen::ScreenClass* screen);
			TextEntity(SPA::Screen::ScreenClass* screen, std::string text);

			void SetText(std::string text);
			void SetColor(int color);
			void SetVisibleAmount(int timeInMiliseconds);
			void SetPosition(int x, int y);
			void SetPosition(SPA::Classes::Point2i P);
			
			bool isVisible();

			void draw();
		};
	
		class BaseEntity {
		private:
			SPA::Classes::Point2i Position;
			std::stack<SPA::Classes::Point2i> LineToFollow;
			char DisplayChar = '%';
			std::chrono::time_point<std::chrono::steady_clock> lastMoved = std::chrono::high_resolution_clock::now();

			int TimeBetweenMoves = 1000;

		public:
			void ModifiyPosition();
			
			void SetPos(int x, int y);
			void SetPos(SPA::Classes::Point2i P);
			void SetPath(std::stack<SPA::Classes::Point2i>& PathOfPoints);
			void SetDisplayChar(char CharToDisplay);
			void SetTimeBetweenMoves(int TimeInMiliseconds);

			void ClearPath();

			SPA::Classes::Point2i GetPos();
			void GetPath(std::stack<SPA::Classes::Point2i>& PathIFollow);
			int GetTimeBetweenMoves();
			char GetDisplayChar();
		};

		class Enemy : public BaseEntity {
			SPA::Screen::ScreenClass* MyScreen = nullptr;
			int MaxHealth;
			int CurrHealth;

			SPA::Entity::TextEntity DamageNums;

			int AcumulatedDamage = 0;

		public:
			Enemy(SPA::Screen::ScreenClass* screen, int MaxHealth);
			Enemy(SPA::Screen::ScreenClass* screen, int MaxHealth, int TimeBetweenMoves);

			void SetMaxHealth(int am);
			void SetCurrentHealth(int am);

			int GetMaxHealth();
			int GetCurrentHealth();

			void TakeDamage(int am);

			bool isAlive();

			void draw();
		};

		class Player : public BaseEntity {
			SPA::Screen::ScreenClass* MyScreen;

			int MaxHealth;
			int MaxMana;

			int CurrHealth;
			int CurrMana;

			int Level = 1;
			double Exp = 0.0;

			int acumulatedDamage = 0;

			SPA::Classes::Stats MyStats;
			SPA::Classes::Stats BonusStats;

			SPA::Items::ItemStruct MyWeapon;

			SPA::Entity::TextEntity DamageNums;

			std::chrono::time_point<std::chrono::steady_clock> LastAttack = std::chrono::high_resolution_clock::now();

			int TimeBetweenEnemyStrikes = 500;
			std::chrono::time_point<std::chrono::steady_clock> LastTimeHit = std::chrono::high_resolution_clock::now();

			bool inBounds(SPA::Classes::Point2i P);
			void CalcStuffs();
		public:
			Player(SPA::Screen::ScreenClass* screen);

			int GetMaxHealth();
			int GetMaxMana();
			int GetHealth();
			int GetMana();

			int GetLevel();
			double GetExp();
			int GetUntilNextLevel();
			void addExp(double am);
			void levelUpIfPossible();			

			void AddBonusStats(SPA::Classes::Stats StatsToAdd);
			void RemoveAllBonusStats();
			SPA::Classes::Stats GetStats();

			void SetWeapon(SPA::Items::ItemStruct weapon);
			SPA::Items::ItemStruct GetWeapon();
			
			void DamageEnemies(std::vector<SPA::Entity::Enemy>& vecOfEnemy);
			void TakeDamageFromEnemiesInRange(std::vector<SPA::Entity::Enemy>& vecOfEnemy, double range, int DamageAmount);

			void DamagePlayer(int am);
			void UseMana(int am);

			void RefreshHealth();
			void RefreshMana();

			bool canAttack();
			bool isAlive();
			bool hasMana();

			void draw();
		};
	}

}