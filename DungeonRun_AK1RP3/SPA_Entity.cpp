#include "SPA_Entity.h"
#include "SPA_functions.h"
#include <string>
#include <sstream>
//Base Entity///////////////////////////////////////////////////
void SPA::Entity::BaseEntity::ModifiyPosition() {
	if (this->LineToFollow.empty()) return;

	auto CurrTime = std::chrono::high_resolution_clock::now();

	auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrTime - this->lastMoved).count();
	if (ElapsedTime >= this->TimeBetweenMoves) {
		this->lastMoved = CurrTime;
		this->Position = this->LineToFollow.top();
		this->LineToFollow.pop();
	}
}
void SPA::Entity::BaseEntity::SetPos(int x, int y) {
	this->SetPos(SPA::Classes::Point2i(x, y));
}
void SPA::Entity::BaseEntity::SetPos(SPA::Classes::Point2i P) {
	this->Position = P;
}
void SPA::Entity::BaseEntity::SetPath(std::stack<SPA::Classes::Point2i>& PathOfPoints) {
	this->LineToFollow = PathOfPoints;
}
void SPA::Entity::BaseEntity::SetDisplayChar(char CharToDisplay) {
	this->DisplayChar = CharToDisplay;
}
void SPA::Entity::BaseEntity::SetTimeBetweenMoves(int TimeInMiliseconds) {
	this->TimeBetweenMoves = TimeInMiliseconds;
}
void SPA::Entity::BaseEntity::ClearPath() {
	SPA::Functions::EmptyPath(this->LineToFollow);
}
SPA::Classes::Point2i SPA::Entity::BaseEntity::GetPos() {
	return this->Position;
}
void SPA::Entity::BaseEntity::GetPath(std::stack<SPA::Classes::Point2i>& PathIFollow) {
	PathIFollow = this->LineToFollow;
}
int SPA::Entity::BaseEntity::GetTimeBetweenMoves() {
	return this->TimeBetweenMoves;
}
char SPA::Entity::BaseEntity::GetDisplayChar() {
	return this->DisplayChar;
}

//Enemy/////////////////////////////////////////////////////////
SPA::Entity::Enemy::Enemy(SPA::Screen::ScreenClass* screen, int MaxHealth) : DamageNums(screen) {
	this->MyScreen = screen;
	this->DamageNums.SetVisibleAmount(200);
	this->SetMaxHealth(MaxHealth);
	this->CurrHealth = this->MaxHealth;
}
SPA::Entity::Enemy::Enemy(SPA::Screen::ScreenClass* screen, int MaxHealth, int TimeBetweenMoves) : Enemy(screen, MaxHealth) {
	this->SetTimeBetweenMoves(TimeBetweenMoves);
}
void SPA::Entity::Enemy::SetMaxHealth(int am) {
	this->MaxHealth = am;
}
void SPA::Entity::Enemy::SetCurrentHealth(int am) {
	this->CurrHealth = am;
}
int SPA::Entity::Enemy::GetMaxHealth() {
	return this->MaxHealth;
}
int SPA::Entity::Enemy::GetCurrentHealth() {
	return this->CurrHealth;
}
void SPA::Entity::Enemy::TakeDamage(int am) {
	this->CurrHealth -= am;
	if (this->CurrHealth < 0) this->CurrHealth = 0;
	if (this->CurrHealth > this->MaxHealth) this->CurrHealth = this->MaxHealth;

	this->AcumulatedDamage += am;

	std::stringstream ss;
	ss << std::abs(this->AcumulatedDamage);
	this->DamageNums.SetText(ss.str());
	if(this->AcumulatedDamage > 0) this->DamageNums.SetColor(SPA::Classes::Color::RED);
	else this->DamageNums.SetColor(SPA::Classes::Color::YEL);

	SPA::Classes::Point2i T = this->GetPos();
	T.alter(2, 0);
	this->DamageNums.SetPosition(T);
}
bool SPA::Entity::Enemy::isAlive() {
	return this->CurrHealth > 0;
}
void SPA::Entity::Enemy::draw() {
	this->ModifiyPosition();
	if (this->DamageNums.isVisible()) this->DamageNums.draw();
	else this->AcumulatedDamage = 0;

	std::stringstream ss;

	int am = this->MaxHealth / 5;
	for (int i = 0; i < 5; i++) {
		if (am * i < this->CurrHealth) ss << "#";
		else ss << "-";
	}
	SPA::Classes::Point2i t = this->GetPos();
	t.alter(-2, -2);
	SPA::Functions::PlaceStringToScreen(this->MyScreen, ss.str(), SPA::Classes::Color::D_GRN, t);
	this->MyScreen->SetCharAt(this->GetDisplayChar(), this->GetPos());
}

//TextEntitiy////////////////////////////////////////////////////
SPA::Entity::TextEntity::TextEntity(SPA::Screen::ScreenClass* screen) {
	this->MyScreen = screen;
}
SPA::Entity::TextEntity::TextEntity(SPA::Screen::ScreenClass* screen, std::string text) : TextEntity(screen) {
	this->SetText(text);
}
void SPA::Entity::TextEntity::SetText(std::string text) {
	this->MyText = text;
}
void SPA::Entity::TextEntity::SetColor(int color) {
	this->textcolor = color;
}
void SPA::Entity::TextEntity::SetVisibleAmount(int timeInMiliseconds) {
	this->VisibleDuration = timeInMiliseconds;
}
void SPA::Entity::TextEntity::SetPosition(int x, int y) {
	this->SetPosition(SPA::Classes::Point2i(x, y));
}
void SPA::Entity::TextEntity::SetPosition(SPA::Classes::Point2i P) {
	this->StartCount = std::chrono::high_resolution_clock::now();
	this->MyPos = P;
}
bool SPA::Entity::TextEntity::isVisible() {
	auto now = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (now - this->StartCount).count();

	return elapsedTime < this->VisibleDuration;
}
void SPA::Entity::TextEntity::draw() {
	if (this->isVisible()) {
		
		if (this->MyPos.x < 0 || this->MyPos.x + this->MyText.size() >= this->MyScreen->GetSize().x) return;
		if (this->MyPos.y < 0 || this->MyPos.y >= this->MyScreen->GetSize().y) return;

		SPA::Classes::Point2i T = this->MyPos;

		for (int i = 0; i < this->MyText.size(); i++) {
			this->MyScreen->SetCharAt(this->MyText[i], T);
			this->MyScreen->SetColorAt(this->textcolor, T);

			T.alter(1, 0);
		}

	}
}

//Player ///////////////////////////////////////////////////////
bool SPA::Entity::Player::inBounds(SPA::Classes::Point2i P) {
	return this->MyScreen->isPointOnScreen(P);
}
void SPA::Entity::Player::CalcStuffs() {
	this->MaxHealth = ( this->MyStats.Health + this->BonusStats.Health ) * 10;
	this->MaxMana = this->MyStats.Mana + this->BonusStats.Mana;

	//this->CurrHealth = this->MaxHealth;
	//this->CurrMana = this->MaxMana;

	int Speed = this->MyStats.Speed + this->BonusStats.Speed;
	this->SetTimeBetweenMoves(150 - Speed);
}
SPA::Entity::Player::Player(SPA::Screen::ScreenClass* screen) : DamageNums(screen) {
	this->MyScreen = screen;
	
	this->MyStats.SetAll(1);
	this->MyStats.Health = 10;

	this->BonusStats.SetAll(0);

	this->CalcStuffs();
	this->SetDisplayChar('@');

	this->CurrHealth = this->MaxHealth;
	this->CurrMana = this->MaxMana;

	this->DamageNums.SetVisibleAmount(250);
}
int SPA::Entity::Player::GetMaxHealth() {
	return this->MaxHealth;
}
int SPA::Entity::Player::GetMaxMana() {
	return this->MaxMana;
}
int SPA::Entity::Player::GetHealth() {
	return this->CurrHealth;
}
int SPA::Entity::Player::GetMana() {
	return this->CurrMana;
}
int SPA::Entity::Player::GetLevel() {
	return this->Level;
}
double SPA::Entity::Player::GetExp() {
	return this->Exp;
}
int SPA::Entity::Player::GetUntilNextLevel() {
	return 10 + 15 * (this->Level - 1);
}
void SPA::Entity::Player::addExp(double am) {
	if (this->Level == 20) return;
	this->Exp += am;
	this->levelUpIfPossible();
}
void SPA::Entity::Player::levelUpIfPossible() {
	
	if (this->Level == 20) return;

	double amountToLevelUp = this->GetUntilNextLevel();
	
	if (this->Exp >= amountToLevelUp) {
		this->Exp -= amountToLevelUp;
		this->Level++;

		this->MyStats.SetAll(this->Level);
		if (this->Level < 10) this->MyStats.Health = 10;
		
		if (this->Level == 20) {
			this->Exp = -1;
			this->MyStats.SetAll(25);
		}

		this->CalcStuffs();
	}
}
void SPA::Entity::Player::AddBonusStats(SPA::Classes::Stats StatsToAdd) {
	this->BonusStats.Health += StatsToAdd.Health;
	this->BonusStats.Mana += StatsToAdd.Mana;
	this->BonusStats.Attack += StatsToAdd.Attack;
	this->BonusStats.Magic += StatsToAdd.Magic;
	this->BonusStats.Speed += StatsToAdd.Speed;

	this->CalcStuffs();
}
void SPA::Entity::Player::RemoveAllBonusStats() {
	this->BonusStats.SetAll(0);
}
SPA::Classes::Stats SPA::Entity::Player::GetStats() {
	SPA::Classes::Stats T;
	T.Health = this->MyStats.Health + this->BonusStats.Health;
	T.Mana = this->MyStats.Mana + this->BonusStats.Mana;
	T.Attack = this->MyStats.Attack + this->BonusStats.Attack;
	T.Magic = this->MyStats.Magic + this->BonusStats.Magic;
	T.Speed = this->MyStats.Speed + this->BonusStats.Speed;

	return T;
}
void SPA::Entity::Player::SetWeapon(SPA::Items::ItemStruct weapon) {
	this->MyWeapon = weapon;
	this->RemoveAllBonusStats();
	this->AddBonusStats(weapon.ItemStats);
}
SPA::Items::ItemStruct SPA::Entity::Player::GetWeapon() {
	return this->MyWeapon;
}
void SPA::Entity::Player::DamageEnemies(std::vector<SPA::Entity::Enemy>& vecOfEnemy) {
	if (!this->canAttack()) return;
	if (vecOfEnemy.empty()) return;
	this->LastAttack = std::chrono::high_resolution_clock::now();

	//Find closest enemy
	double minDist = SPA::Functions::Distance(vecOfEnemy[0].GetPos(), this->GetPos());
	int minIndex = 0;
	for (int i = 1; i < vecOfEnemy.size(); i++) {
		double d = SPA::Functions::Distance(vecOfEnemy[i].GetPos(), this->GetPos());
		if (d < minDist) {
			minDist = d;
			minIndex = i;
		}
	}

	SPA::Classes::Point2i MeleOrigin;
	SPA::Classes::Point2i MeleSize(7, 5);
	switch (this->GetWeapon().itemType) {
	case 1:
		if (this->GetPos().x - vecOfEnemy[minIndex].GetPos().x > 0) MeleOrigin.init(this->GetPos().x - 6, this->GetPos().y - 2);
		else if (this->GetPos().x - vecOfEnemy[minIndex].GetPos().x < 0) MeleOrigin.init(this->GetPos().x, this->GetPos().y - 2);
		else if (this->GetPos().y - vecOfEnemy[minIndex].GetPos().y < 0) MeleOrigin.init(this->GetPos().x - 3, this->GetPos().y);
		else MeleOrigin.init(this->GetPos().x - 3, this->GetPos().y - 4);

		for (int i = 0; i < vecOfEnemy.size(); i++) {
			if (SPA::Functions::isPointInRegion(vecOfEnemy[i].GetPos(), MeleOrigin, MeleSize)) {
				vecOfEnemy[i].TakeDamage(this->GetStats().Attack);
			}
		}

		break;
	case 2:
		if (minDist < 16) {
			vecOfEnemy[minIndex].TakeDamage(this->GetStats().Attack);
		}
		break;
	case 3:
		vecOfEnemy[minIndex].TakeDamage(this->GetStats().Attack);
		break;	
	}
}
void SPA::Entity::Player::TakeDamageFromEnemiesInRange(std::vector<SPA::Entity::Enemy>& vecOfEnemy, double range, int DamageAm) {
	auto now = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (now - this->LastTimeHit).count();

	if (elapsedTime >= this->TimeBetweenEnemyStrikes) {
		this->LastTimeHit = now;
		for (int i = 0; i < vecOfEnemy.size(); i++) {
			if (SPA::Functions::Distance(this->GetPos(), vecOfEnemy[i].GetPos()) <= range) {
				this->DamagePlayer(DamageAm);
			}
		}
	}
}
void SPA::Entity::Player::DamagePlayer(int am) {
	this->CurrHealth -= am;

	if (this->CurrHealth < 0) this->CurrHealth = 0;
	if (this->CurrHealth > this->MaxHealth) this->CurrHealth = this->MaxHealth;

	this->acumulatedDamage += am;

	std::stringstream ss;
	ss << std::abs(this->acumulatedDamage);
	this->DamageNums.SetText(ss.str());
	if (this->acumulatedDamage > 0) this->DamageNums.SetColor(SPA::Classes::Color::RED);
	else this->DamageNums.SetColor(SPA::Classes::Color::YEL);

	SPA::Classes::Point2i T = this->GetPos();
	T.alter(2, 0);
	this->DamageNums.SetPosition(T);
}
void SPA::Entity::Player::UseMana(int am) {
	this->CurrMana -= am;

	if (this->CurrMana < 0) this->CurrMana = 0;
	if (this->CurrMana > this->MaxMana) this->CurrMana = this->MaxMana;
}
void SPA::Entity::Player::RefreshHealth() {
	this->CurrHealth = this->MaxHealth;
}
void SPA::Entity::Player::RefreshMana() {
	this->CurrMana = this->MaxMana;
}
bool SPA::Entity::Player::canAttack() {
	auto now = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (now - this->LastAttack).count();

	return elapsedTime >= this->GetWeapon().TimeBetweenAttack;
}
bool SPA::Entity::Player::isAlive() {
	return this->CurrHealth > 0;
}
bool SPA::Entity::Player::hasMana() {
	return this->CurrMana > 0;
}
void SPA::Entity::Player::draw() {
	if (this->DamageNums.isVisible()) this->DamageNums.draw();
	else this->acumulatedDamage = 0;

	if (this->isAlive()) {
		this->ModifiyPosition();

		SPA::Classes::Point2i T = this->GetPos();
		T.alter(-3, -2);

		int am = this->MaxHealth / 7;
		for (int i = 0; i < 7; i++) {
			if (this->inBounds(T)) {
				if (am * i < this->CurrHealth) {
					this->MyScreen->SetCharAt('#', T);
					this->MyScreen->SetColorAt(SPA::Classes::Color::GREEN, T);
				}
				else {
					this->MyScreen->SetCharAt('-', T);
					this->MyScreen->SetColorAt(SPA::Classes::Color::RED, T);
				}
			}
			T.alter(1, 0);
		}

		this->MyScreen->SetCharAt(this->GetDisplayChar(), this->GetPos());
		T = this->GetPos();
		T.alter(1, 0);

		switch (this->GetWeapon().itemType) {
		case 1:
			this->MyScreen->SetCharAt('/', T);
			if(!this->canAttack()) this->MyScreen->SetCharAt('-', T);
			break;
		case 2:
			this->MyScreen->SetCharAt('I', T);
			if (!this->canAttack()) this->MyScreen->SetCharAt('i', T);
			break;
		case 3:
			this->MyScreen->SetCharAt(')', T);
			if (!this->canAttack()) this->MyScreen->SetCharAt('>', T);
			break;
		}
	}

}
