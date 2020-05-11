#include "SPA_Spell.h"
#include "SPA_functions.h"
#include <fstream>
#include <sstream>

void SPA::Spell::SpellClass::CalcSpellProp() {
	switch (this->MySpell.CastType) {
	case 1:
		//1x1
		this->SpellOrigin = this->CastLocation;
		this->SpellSize.init(1, 1);
		break;
	case 2:
		//5x3
		this->SpellOrigin = this->CastLocation;
		this->SpellOrigin.alter(-2, -1);
		this->SpellSize.init(5, 3);
		break;
	case 3:
		//7x5
		this->SpellOrigin = this->CastLocation;
		this->SpellOrigin.alter(-3, -2);
		this->SpellSize.init(7, 5);
		break;
	case 4:
		//11x7
		this->SpellOrigin = this->CastLocation;
		this->SpellOrigin.alter(-5, -3);
		this->SpellSize.init(11, 7);
		break;
	}
}

bool SPA::Spell::SpellClass::inBounds() {
	this->CalcSpellProp();
	if (this->SpellOrigin.x < 0 || this->SpellOrigin.x + this->SpellSize.x > this->MyScreen->GetSize().x) return false;
	if (this->SpellOrigin.y < 0 || this->SpellOrigin.y + this->SpellSize.y > this->MyScreen->GetSize().y) return false;
	return true;
}

SPA::Spell::SpellClass::SpellClass(SPA::Screen::ScreenClass* screen) {
	this->MyScreen = screen;
}

SPA::Spell::SpellClass::SpellClass(SPA::Screen::ScreenClass* screen, SPA::Spell::SpellValues ValuesOfSpell) : SpellClass(screen) {
	this->SetActiveSpell(ValuesOfSpell);
}

void SPA::Spell::SpellClass::SetActiveSpell(SPA::Spell::SpellValues Val) {
	this->MySpell = Val;
}

void SPA::Spell::SpellClass::SetCastLocation(int x, int y) {
	this->SetCastLocation(SPA::Classes::Point2i(x, y));
}

void SPA::Spell::SpellClass::SetCastLocation(SPA::Classes::Point2i P) {
	this->CastLocation = P;
	if (!this->inBounds()) return;
	this->LastAffect = std::chrono::high_resolution_clock::now();
	this->Activated = std::chrono::high_resolution_clock::now();
}

bool SPA::Spell::SpellClass::NeedsToBeTargeted() {
	return this->MySpell.CastType != 1;
}

bool SPA::Spell::SpellClass::isSpellUp() {
	auto now = std::chrono::high_resolution_clock::now();
	auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->Activated).count();

	return ElapsedTime < this->MySpell.ExistDuration;
}

std::string SPA::Spell::SpellClass::GetSpellName() {
	return this->MySpell.SpellName;
}

std::string SPA::Spell::SpellClass::GetSpellFlavor() {
	return this->MySpell.SpellFlavor;
}

int SPA::Spell::SpellClass::GetSpellCost() {
	return this->MySpell.ManaCost;
}

void SPA::Spell::SpellClass::DamageEnemiesInAOE(std::vector<SPA::Entity::Enemy>& EnemiesVec, int PlayerMagicVal) {
	auto now = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->LastAffect).count();
	if (elapsedTime > this->MySpell.TimeBetweenApply) {

		this->CalcSpellProp();
		for (int i = 0; i < EnemiesVec.size(); i++) {

			if (SPA::Functions::isPointInRegion(EnemiesVec[i].GetPos(), SpellOrigin, SpellSize)) {
				switch (this->MySpell.DamageType) {
				case 1:
					EnemiesVec[i].TakeDamage(this->MySpell.DamageAmount + PlayerMagicVal);
					this->LastAffect = now;
					break;
				case 2:
					EnemiesVec[i].SetTimeBetweenMoves(this->MySpell.DamageAmount + PlayerMagicVal * 10);
					this->LastAffect = now;
					break;
				}
			}
		}

		
	}
}

void SPA::Spell::SpellClass::ApplyEffectsToPlayer(SPA::Entity::Player& MyPlayer, int PlayerMagicVal) {
	auto now = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->LastAffect).count();
	if (elapsedTime > this->MySpell.TimeBetweenApply) {

		this->CalcSpellProp();

		if (SPA::Functions::isPointInRegion(MyPlayer.GetPos(), this->SpellOrigin, this->SpellSize) || this->MySpell.CastType == 1) {
			switch (this->MySpell.DamageType) {
			case 3:
				MyPlayer.DamagePlayer(-(this->MySpell.DamageAmount + PlayerMagicVal));
				this->LastAffect = now;
				break;
			}
		}

		
	}
}

void SPA::Spell::SpellClass::drawSpell() {

	if (this->isSpellUp() && this->inBounds()) {
		this->CalcSpellProp();

		int xs = SpellOrigin.x;
		int xe = SpellOrigin.x + SpellSize.x;

		int ys = SpellOrigin.y;
		int ye = SpellOrigin.y + SpellSize.y;

		int color = 7;
		switch (this->MySpell.DamageType) {
		case 1:
			color = SPA::Classes::Color::RED;
			break;
		case 2:
			color = SPA::Classes::Color::CYAN;
			break;
		case 3:
			color = SPA::Classes::Color::YEL;
			break;
		case 4:
			color = SPA::Classes::Color::BLACK;
			break;
		}

		for (int y = ys; y < ye; y++) {
			for (int x = xs; x < xe; x++) {
				if (this->MyScreen->GetValuesAt(x, y).Char.UnicodeChar == '#' && color != 0) continue;
				this->MyScreen->SetColorAt(color, x, y);
				if (rand() % 100 <= 1) this->MyScreen->SetCharAt('*', x, y);
				else this->MyScreen->SetCharAt(':', x, y);
			}
		}
	}
}

SPA::Spell::SpellLibrary::SpellLibrary(std::string LibraryFileName) {
	std::ifstream infile(LibraryFileName);
	if (!infile) throw "Failed to open file!";

	std::string line;
	while (std::getline(infile, line)) {
		SPA::Spell::SpellValues temp;

		std::stringstream ss(line);

		ss >> temp.CastType;
		ss >> temp.DamageType;
		ss >> temp.DamageAmount;
		ss >> temp.ManaCost;
		ss >> temp.TimeBetweenApply;
		ss >> temp.ExistDuration;

		std::getline(infile, temp.SpellName);
		std::getline(infile, temp.SpellFlavor);

		this->LibraryOfSpells.push_back(temp);
	}

	infile.close();
}

SPA::Spell::SpellValues SPA::Spell::SpellLibrary::GetSpellAt(int index) {
	return this->LibraryOfSpells.at(index);
}

int SPA::Spell::SpellLibrary::GetSpellCount() {
	return this->LibraryOfSpells.size();
}
