#include "SPA_Game.h"
#include <sstream>
#include <algorithm>

#include "SPA_Button.h"
#include "SPA_functions.h"

SPA::Game::StartMenu::StartMenu(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* mouse) {
	this->MyScreen = screen;
	this->MyMouse = mouse;
}

void SPA::Game::StartMenu::initiate() {
	for (int y = 0; y < this->MyScreen->GetSize().y; y++) {
		for (int x = 0; x < this->MyScreen->GetSize().x; x++) this->MyScreen->adminAt(x, y).Char.UnicodeChar = ' ';
	}

	SPA::Button::ButtonClass TitleCard(this->MyScreen, this->MyMouse, 0, 0, 16, 3);
	TitleCard.SetButtonText(" Dungeon Run");

	SPA::Button::ButtonClass NewGame(this->MyScreen, this->MyMouse, 0, 4, 16, 5);
	SPA::Button::ButtonClass Highscores(this->MyScreen, this->MyMouse, 0, 9, 16, 5);
	SPA::Button::ButtonClass Tutorial(this->MyScreen, this->MyMouse, 0, 14, 16, 5);
	SPA::Button::ButtonClass ExitGame(this->MyScreen, this->MyMouse, 0, 19, 16, 5);

	NewGame.SetButtonText(" New Game");
	Highscores.SetButtonText(" Highscores");
	Tutorial.SetButtonText(" Tutorial");
	ExitGame.SetButtonText(" Exit game");
	
	std::string Line("================");

	while (1) {

		TitleCard.draw();
		NewGame.draw(SPA::Classes::Color::GREEN);
		Highscores.draw(SPA::Classes::Color::GREEN);
		Tutorial.draw(SPA::Classes::Color::GREEN);
		ExitGame.draw(SPA::Classes::Color::GREEN);

		SPA::Functions::PlaceStringToScreen(this->MyScreen, Line, 7, { 0, 25 });

		std::stringstream ss;
		if (NewGame.isHovered()) {
			ss << "Start a new game!";
		}
		else if (Highscores.isHovered()) {
			ss << "Check out your scores!";
		}
		else if (ExitGame.isHovered()) {
			ss << "Leave this game for now!";
		}
		else if (Tutorial.isHovered()) {
			ss << "A basic tutorial!";
		}
		else ss << "Hover over one of the options!";

		SPA::Functions::PlaceStringToScreen(this->MyScreen, ss.str(), 7, {0, 26});

		if (ExitGame.isLeftClick()) std::exit(0);
		else if (Tutorial.isLeftClick()) SPA::Game::TutorialMenu(*this->MyScreen, this->MyMouse).initiate();
		else if (Highscores.isLeftClick()) SPA::Game::HighscoreMenu(*this->MyScreen, this->MyMouse, "Highscores.bin").initiate();
		else if (NewGame.isLeftClick()) break;

		this->MyScreen->draw();
	}

}

SPA::Game::TutorialMenu::TutorialMenu(SPA::Screen::ScreenClass screen, SPA::Classes::Mouse* mouse) : MyScreen(screen) {
	this->MyMouse = mouse;
}

void SPA::Game::TutorialMenu::initiate() {
	for (int y = 0; y < this->MyScreen.GetSize().y; y++) {
		for (int x = 0; x < this->MyScreen.GetSize().x; x++) {
			this->MyScreen.adminAt(x, y).Char.UnicodeChar = ' ';
		}
	}

	std::stringstream ss;
	ss << "================" << std::endl << "Tutorial" << std::endl << "================" << std::endl;
	ss << ". ->Floor" << std::endl << "# ->Wall" << std::endl << "% ->Enemy" << std::endl;
	ss << "@ ->You / player" << std::endl << std::endl;
	ss << "Upon entering a room a few" << std::endl;
	ss << "enemies will spawn. It is" << std::endl;
	ss << "your task to defeat them," << std::endl;
	ss << "so you can proceed through" << std::endl;
	ss << "the next room!" << std::endl;
	ss << "The dungeon hides many" << std::endl;
	ss << "traps and secrets and even" << std::endl;
	ss << "some legendary weapons " << std::endl;
	ss << "left behind by the" << std::endl;
	ss << "ancient ones! Or you know," << std::endl;
	ss << "you can find gear left" << std::endl;
	ss << "behind by adventurers" << std::endl;
	ss << "that came before you!" << std::endl;
	ss << "One life, one go, how long" << std::endl;
	ss << "will you last in the Dungeon" << std::endl;

	std::string text(ss.str());

	SPA::Button::ButtonClass Butt(&this->MyScreen, this->MyMouse, 0, 29, 16, 3);
	Butt.SetButtonText("Next page");

	while (1) {
		SPA::Functions::PlaceMultipleLinesOfText(&this->MyScreen, text, 7, SPA::Classes::Point2i(0, 0));

		Butt.draw(SPA::Classes::Color::GREEN);

		if (Butt.isLeftClick()) break;

		this->MyScreen.draw();
	}

	while (Butt.isLeftClick()) {}

	std::stringstream ss2;
	ss2 << "================\n" << "Tutorial\n" << "================\n";
	ss2 << "To move around you must\n" << "press righ-click on the floor!\n";
	ss2 << "Afterward your player\n" << "will automaticaly start\n" << "moving to the selected\n" << "location.\n";
	ss2 << "Your player will automaticaly\n" << "attack any nearby enemeies!\n" << " *Reach of your attack\n" << "  depends on equipped weapon\n";
	ss2 << "You can also boost your\n" << "firepower by using spells!\n" << "Spells are used by pressing\n" << "1, 2 or 3 and then selecting\n";
	ss2 << "a target if that is needed!\n";

	text = ss2.str();

	Butt.SetButtonText("I don't get it");

	while (1) {
		SPA::Functions::PlaceMultipleLinesOfText(&this->MyScreen, text, 7, SPA::Classes::Point2i(0, 0));

		Butt.draw(SPA::Classes::Color::GREEN);

		if (Butt.isLeftClick()) break;

		this->MyScreen.draw();
	}
}

SPA::Classes::scorestruct SPA::Game::HighscoreKeeper::createNULLscore() {
	SPA::Classes::scorestruct t;
	t.floorCount = 0;
	t.levelReached = 0;
	t.bestItemFound = "--";

	return t;
}

SPA::Game::HighscoreKeeper::HighscoreKeeper(std::string filename) {
	this->filename = filename;
	for (int i = 0; i < 5; i++) sc[i] = this->createNULLscore();
}

void SPA::Game::HighscoreKeeper::insertScore(SPA::Classes::scorestruct ScoreToAdd) {
	bool insertScore = false;
	int index;
	for (int i = 0; i < 5; i++) {
		if (this->sc[i].floorCount < ScoreToAdd.floorCount) {
			insertScore = true;
			index = i;
			break;
		}
	}

	if (insertScore) {
		for (int i = 3; i >= index; i--) {
			this->sc[i + 1] = this->sc[i];
		}
		this->sc[index] = ScoreToAdd;
	}

}

void SPA::Game::HighscoreKeeper::saveScores() {
	std::ofstream outfile(this->filename, std::ios_base::binary);
	if (!outfile) throw "Failed to save file!";

	for (int i = 0; i < 5; i++) {
		outfile.write((char*)&(this->sc[i].floorCount), sizeof(int));
		outfile.write((char*)&(this->sc[i].levelReached), sizeof(int));
		int s = this->sc[i].bestItemFound.size();
		outfile.write((char*)&(s), sizeof(int));
		outfile.write(this->sc[i].bestItemFound.c_str(), s);
	}

	outfile.close();
}

void SPA::Game::HighscoreKeeper::loadScores() {
	std::ifstream infile(this->filename, std::ios_base::binary);
	if (!infile) throw"Failed to open file!";

	for (int i = 0; i < 5; i++) {
		infile.read((char*)&(this->sc[i].floorCount), sizeof(int));
		infile.read((char*)&(this->sc[i].levelReached), sizeof(int));
		int size;
		infile.read((char*)&(size), sizeof(int));
		char* str = new char[size];

		infile.read(str, size);
		this->sc[i].bestItemFound = std::string(str, size);

		delete[] str;
	}

	infile.close();
}

SPA::Classes::scorestruct SPA::Game::HighscoreKeeper::GetScore(int i) {
	if (i < 0 || i >= 5) throw "Out of bounds!";
	return this->sc[i];
}

SPA::Game::HighscoreMenu::HighscoreMenu(SPA::Screen::ScreenClass MyScreen, SPA::Classes::Mouse* mouse, std::string filename) : MyScreen(MyScreen), HK(filename){
	this->MyMouse = mouse;
}

void SPA::Game::HighscoreMenu::initiate() {
	for (int y = 0; y < this->MyScreen.GetSize().y; y++) {
		for (int x = 0; x < this->MyScreen.GetSize().x; x++) {
			this->MyScreen.adminAt(x, y).Char.UnicodeChar = ' ';
		}
	}

	std::stringstream ss;
	ss << "================" << std::endl;
	ss << " Highscores" << std::endl;
	ss << "================" << std::endl;
	ss << std::endl << "================" << std::endl;

	this->HK.loadScores();
	for (int i = 0; i < 5; i++) {
		ss << "Floor: " << this->HK.GetScore(i).floorCount << std::endl;
		ss << "Level: " << this->HK.GetScore(i).levelReached << std::endl;
		ss << "Best item: " << this->HK.GetScore(i).bestItemFound << std::endl;
		ss << "================" << std::endl;
	}

	std::string line(ss.str());

	SPA::Button::ButtonClass ExitButton(&this->MyScreen, this->MyMouse, 0, 26, 16, 3);
	ExitButton.SetButtonText(" Main menu");

	while (1) {

		SPA::Functions::PlaceMultipleLinesOfText(&this->MyScreen, line, 7, SPA::Classes::Point2i(0, 0));

		ExitButton.draw(SPA::Classes::Color::GREEN);

		if (ExitButton.isLeftClick()) return;

		this->MyScreen.draw();
	}
}

void SPA::Game::GameLoop::DefaultValuesForChangingRooms() {

	this->RoomDisplay.loadRoomBasedOnNode(this->FloorTemp.GetRoomInfo(), &this->GoodGuy, this->vecOfBadGuys, this->CurrentFloor);
	this->MyScreen->GenerateWallMesh(this->Walls);

	SPA::Functions::EmptyPath(this->Path);
	this->GoodGuy.ClearPath();
	this->GoodGuy.RefreshMana();

}

void SPA::Game::GameLoop::invMenu(SPA::Screen::ScreenClass* MenuScreen) {
	MenuScreen->ClearScreen();
	while (this->MyMouse->isLeftClick()) {}

	SPA::Button::ButtonClass TitleCard(MenuScreen, this->MyMouse, 0, 0, 32, 3);
	SPA::Button::ButtonClass ReturnButton(MenuScreen, this->MyMouse, 0, 29, 32, 3);
	SPA::Button::ButtonClass UpButton(MenuScreen, this->MyMouse, 0, 3, 24, 3);
	SPA::Button::ButtonClass DownButton(MenuScreen, this->MyMouse, 0, 26, 24, 3);

	SPA::Button::ButtonClass SelectedItem(MenuScreen, this->MyMouse, 26, 4, 24, 3);
	SelectedItem.SetButtonText("Selected item:");

	TitleCard.SetButtonText("Inventory");
	UpButton.SetButtonText("Move inventory up");
	DownButton.SetButtonText("Move inventory down");
	ReturnButton.SetButtonText("Return to main menu");

	int itmStart = 0;
	int itmEnd = 3;

	const int ItemCount = 4;

	SPA::Classes::Point2i ItemOrigin(0,7);
	SPA::Classes::Point2i ItemSize(24, (4 * ItemCount) + 1);

	while (1) {
		std::stringstream ssItmList;
		auto sit = this->CollectedItems.begin();
		std::advance(sit, itmStart);

		for (int i = 0; i < ItemCount; i++) {
			if (sit == this->CollectedItems.end()) {
				ssItmList << "========================" << std::endl;
				ssItmList << std::endl;
				ssItmList << "You are yet to find this!" << std::endl;
				ssItmList << std::endl;
				//ssItmList << "========================" << std::endl;
			}
			else {
				ssItmList << "========================" << std::endl;
				ssItmList << this->BasicItemSummary(*sit);
				//ssItmList << "========================" << std::endl;
				++sit;
			}
		}
		ssItmList << "========================" << std::endl;
		ssItmList << "[" << itmStart + 1 << "/" << itmEnd + 1 << "] / " << this->CollectedItems.size();
		SPA::Functions::PlaceMultipleLinesOfText(MenuScreen, ssItmList.str(), 7, ItemOrigin);
		if (SPA::Functions::isPointInRegion(this->MyMouse->GetMousePos(), ItemOrigin, ItemSize)) {
			SPA::Classes::Point2i RelativePos;
			RelativePos.x = this->MyMouse->GetMousePos().x;
			RelativePos.y = this->MyMouse->GetMousePos().y - ItemOrigin.y;

			int selectedBlock = RelativePos.y / 4;
			auto myit = this->CollectedItems.begin();
			if (itmStart + selectedBlock < this->CollectedItems.size() && myit != this->CollectedItems.end()) {
				std::advance(myit, itmStart + selectedBlock);
				SelectedItem.SetButtonText(SPA::Items::ItemSummary(*myit));

				if (this->MyMouse->isLeftClick()) {
					int index;
					switch (myit->itemType) {
					case 1:
						index = 0;
						break;
					case 2:
						index = 0;
						break;
					case 3:
						index = 0;
						break;
					case 4:
						index = 1;
						break;
					case 5:
						index = 2;
						break;
					case 6:
						index = 3;
						break;
					case 7:
						index = 4;
						break;
					case 8:
						index = 5;
						break;
					}
					this->PlayerEquipment[index] = *myit;
					if (index == 0) {
						this->PlayerSpells[0] = this->lib->GetSpellAt(this->PlayerEquipment[0].SpellIndex);
					}

					this->applyEquipmentToPlayer();
					while (this->MyMouse->isLeftClick()) {}
				}
			}
			else SelectedItem.SetButtonText("This item is too /power");
		}

		std::stringstream ssEqpItems;
		for (int i = 0; i < 6; i++) {
			ssEqpItems << SPA::Items::ItemSummary(this->PlayerEquipment[i]) << std::endl;
		}
		ssEqpItems << "========================" << std::endl;
		ssEqpItems << "Click on items in your" << std::endl;
		ssEqpItems << "inventory to equip them!" << std::endl;
		SPA::Functions::PlaceMultipleLinesOfText(MenuScreen, ssEqpItems.str(), SPA::Classes::Color::GRAY, { 26,7 });

		if (UpButton.isLeftClick() && itmStart > 0) {
			itmStart -= ItemCount;
			itmEnd -= ItemCount;

			while (this->MyMouse->isLeftClick()) {}
		}
		if (DownButton.isLeftClick() && itmEnd < this->CollectedItems.size()) {
			itmStart += ItemCount;
			itmEnd += ItemCount;

			while (this->MyMouse->isLeftClick()) {}
		}

		if (ReturnButton.isLeftClick()) {
			while (this->MyMouse->isLeftClick()) {}
			return;
		}

		TitleCard.draw();
		UpButton.draw(SPA::Classes::Color::YEL);
		DownButton.draw(SPA::Classes::Color::YEL);
		ReturnButton.draw(SPA::Classes::Color::GREEN);

		SelectedItem.draw();

		MenuScreen->draw();
	}
}

void SPA::Game::GameLoop::spellsMenu(SPA::Screen::ScreenClass* MenuScreen) {
	MenuScreen->ClearScreen();
	while (this->MyMouse->isLeftClick()) {}

	SPA::Button::ButtonClass Title(MenuScreen, this->MyMouse, 0, 0, 32, 3);
	SPA::Button::ButtonClass ReturnButton(MenuScreen, this->MyMouse, 0, 29, 32, 3);
	SPA::Button::ButtonClass UpButton(MenuScreen, this->MyMouse, 0, 3, 24, 3);
	SPA::Button::ButtonClass DownButton(MenuScreen, this->MyMouse, 0, 26, 24, 3);
	Title.SetButtonText("Ability Menu");
	ReturnButton.SetButtonText("Return to main menu");
	UpButton.SetButtonText("Go up the list");
	DownButton.SetButtonText("Go down the list");

	const int spellcount = 8;
	int start = 0;
	int end = start + spellcount;

	while (1) {

		auto myit = this->CollectedSpells.begin();
		std::advance(myit, start);

		SPA::Classes::Point2i SpellOrigin(0, 8);
		SPA::Classes::Point2i SpellSize(64, spellcount * 2);

		std::stringstream ss;
		for (int i = 0; i < spellcount; i++) {
			if (myit != this->CollectedSpells.end()) {
				ss << "[" << myit->SpellName << "]<" << myit->ManaCost << "> " << myit->SpellFlavor << std::endl;
				++myit;
			}
			else {
				ss << "-NA-" << std::endl;
			}
			ss << std::endl;
		}
		SPA::Functions::PlaceMultipleLinesOfText(MenuScreen, ss.str(), 7, { 0,8 });

		if (UpButton.isLeftClick() && start > 0) {
			start -= spellcount;
			end -= spellcount;
			while (this->MyMouse->isLeftClick()) {};
		}
		else if (DownButton.isLeftClick() && end < this->CollectedSpells.size()) {
			start += spellcount;
			end += spellcount;
			while (this->MyMouse->isLeftClick()) {}
		}

		if (SPA::Functions::isPointInRegion(this->MyMouse->GetMousePos(), SpellOrigin, SpellSize)) {
			SPA::Classes::Point2i relMouse;
			relMouse.x = this->MyMouse->GetMousePos().x;
			relMouse.y = this->MyMouse->GetMousePos().y - SpellOrigin.y;

			for (int i = 0; i < 64; i++) MenuScreen->SetColorAt(SPA::Classes::Color::CYAN, i, this->MyMouse->GetMousePos().y);

			int index = ( relMouse.y / 2) + start;

			if (this->MyMouse->isLeftClick() && index < this->CollectedSpells.size()) {
				while (this->MyMouse->isLeftClick()) {}
				SPA::Button::ButtonClass TitleCard(MenuScreen, this->MyMouse, 0, 0, 32, 3);
				SPA::Button::ButtonClass OptionOne(MenuScreen, this->MyMouse, 0, 5, 32, 3);
				SPA::Button::ButtonClass OptionTwo(MenuScreen, this->MyMouse, 0, 9, 32, 3);
				SPA::Button::ButtonClass CancelButt(MenuScreen, this->MyMouse, 0, 29, 32, 3);

				TitleCard.SetButtonText("Choose a spell slot");
				OptionOne.SetButtonText("Spell slot [2]");
				OptionTwo.SetButtonText("Spell slot [3]");
				CancelButt.SetButtonText("Cancel");

				while (1) {
					
					//Odabiranje mjesta za spell

					if (CancelButt.isLeftClick()) {
						while (this->MyMouse->isLeftClick()) {}
						break;
					}
					else if (OptionOne.isLeftClick()) {
						auto tit = this->CollectedSpells.begin();
						std::advance(tit, index);

						this->PlayerSpells[1] = *tit;
						while (this->MyMouse->isLeftClick()) {}
						break;
					}
					else if (OptionTwo.isLeftClick()) {
						auto tit = this->CollectedSpells.begin();
						std::advance(tit, index);

						this->PlayerSpells[2] = *tit;
						while (this->MyMouse->isLeftClick()) {}
						break;
					}

					TitleCard.draw();
					OptionOne.draw(SPA::Classes::Color::GREEN);
					OptionTwo.draw(SPA::Classes::Color::GREEN);
					CancelButt.draw(SPA::Classes::Color::RED);
					MenuScreen->draw();
				}

			}
		}

		Title.draw();
		UpButton.draw(SPA::Classes::Color::YEL);
		DownButton.draw(SPA::Classes::Color::YEL);
		ReturnButton.draw(SPA::Classes::Color::GREEN);

		if (ReturnButton.isLeftClick()){
			while (this->MyMouse->isLeftClick()) {}
			return;
		}


		MenuScreen->draw();
	}
}

void SPA::Game::GameLoop::playerMenu(SPA::Screen::ScreenClass* MenuScreen) {
	MenuScreen->ClearScreen();

	SPA::Button::ButtonClass TitleCard(MenuScreen, this->MyMouse, 0, 0, 32, 3);
	SPA::Button::ButtonClass ReturnButton(MenuScreen, this->MyMouse, 0, 29, 32, 3);

	TitleCard.SetButtonText("Player info");
	ReturnButton.SetButtonText("Return to main menu");

	std::stringstream ss;
	ss << "Stats:" << std::endl << std::endl;
	ss << "Health points: " << this->GoodGuy.GetStats().Health << std::endl;
	ss << "Mana points:   " << this->GoodGuy.GetStats().Mana << std::endl << std::endl;
	ss << "Attack Damage: " << this->GoodGuy.GetStats().Attack << std::endl;
	ss << "Magic Power:   " << this->GoodGuy.GetStats().Magic << std::endl << std::endl;
	ss << "Speed:         " << this->GoodGuy.GetStats().Speed << std::endl << std::endl;
	ss << "Equipped items:" << std::endl;
	for (int i = 0; i < 6; i++) {
		ss << SPA::Items::ItemSummary(this->PlayerEquipment[i]) << std::endl;
	}
	std::string line(ss.str());

	while (1) {
		if (ReturnButton.isLeftClick()) {
			while (this->MyMouse->isLeftClick()) {}
			return;
		}

		TitleCard.draw();
		SPA::Functions::PlaceMultipleLinesOfText(MenuScreen, line, 7, { 0,5 });
		ReturnButton.draw(SPA::Classes::Color::GREEN);

		MenuScreen->draw();
	}
}

void SPA::Game::GameLoop::Mercahnt(SPA::Screen::ScreenClass* MenuScreen, SPA::Items::ItemStruct* Offering) {
	this->MyScreen->draw();
	MenuScreen->ClearScreen();

	SPA::Button::ButtonClass TitleCard(MenuScreen, this->MyMouse, 0, 0, 32, 3);
	SPA::Button::ButtonClass ReturnCard(MenuScreen, this->MyMouse, 0, 29, 32, 3);

	SPA::Button::ButtonClass Offering1(MenuScreen, this->MyMouse, 0, 5, 24, 5);
	SPA::Button::ButtonClass Offering2(MenuScreen, this->MyMouse, 0, 11, 24, 5);
	SPA::Button::ButtonClass Offering3(MenuScreen, this->MyMouse, 0, 17, 24, 5);

	SPA::Entity::TextEntity PurchaseYes(MenuScreen);
	PurchaseYes.SetVisibleAmount(2000);

	//SPA::Button::ButtonClass SellSpells(MenuScreen, this->MyMouse, 0, 23, 24, 3);

	TitleCard.SetButtonText("Merchant");
	ReturnCard.SetButtonText("Go back to adventruing");

	Offering1.SetButtonText(Offering[0].Name + "(35)");
	Offering2.SetButtonText(Offering[1].Name + "(35)");
	Offering3.SetButtonText(Offering[2].Name + "(35)");

	const int price = 35;

	while (1) {

		if (ReturnCard.isLeftClick()) {
			while (this->MyMouse->isLeftClick()) {}
			return;
		}

		TitleCard.draw();

		if (this->PlayerGoldAmount >= price) Offering1.draw(SPA::Classes::Color::CYAN);
		else Offering1.draw(SPA::Classes::Color::D_RED);
		if (this->PlayerGoldAmount >= price) Offering2.draw(SPA::Classes::Color::CYAN);
		else Offering2.draw(SPA::Classes::Color::D_RED);
		if (this->PlayerGoldAmount >= price) Offering3.draw(SPA::Classes::Color::CYAN);
		else Offering3.draw(SPA::Classes::Color::D_RED);

		if (Offering1.isLeftClick() && this->PlayerGoldAmount >= price) {
			this->CollectedItems.push_back(Offering[0]);
			this->PlayerGoldAmount -= price;
			PurchaseYes.SetText(Offering[0].Name);
			PurchaseYes.SetPosition(0, 26);
			while (this->MyMouse->isLeftClick()) {}
		}
		else if (Offering2.isLeftClick() && this->PlayerGoldAmount >= price) {
			this->CollectedItems.push_back(Offering[1]);
			this->PlayerGoldAmount -= price;
			PurchaseYes.SetText(Offering[1].Name);
			PurchaseYes.SetPosition(0, 26);
			while (this->MyMouse->isLeftClick()) {}
		}
		else if (Offering3.isLeftClick() && this->PlayerGoldAmount >= price) {
			this->CollectedItems.push_back(Offering[2]);
			this->PlayerGoldAmount -= price;
			PurchaseYes.SetText(Offering[2].Name);
			PurchaseYes.SetPosition(0, 26);
			while (this->MyMouse->isLeftClick()) {}
		}

		std::stringstream ss;
		ss << "Your gold: " << this->PlayerGoldAmount;
		SPA::Functions::PlaceStringToScreen(MenuScreen, ss.str(), SPA::Classes::Color::YEL, { 0, 25 });

		std::stringstream ss2;
		ss2 << "Greetings, traveler!";
		SPA::Functions::PlaceStringToScreen(MenuScreen, ss2.str(), SPA::Classes::Color::GRAY, { 0,3 });

		if (PurchaseYes.isVisible()) PurchaseYes.draw();
		ReturnCard.draw(SPA::Classes::Color::RED);

		MenuScreen->draw();
	}
}

void SPA::Game::GameLoop::Alchemist(SPA::Screen::ScreenClass* MenuScreen, SPA::Spell::SpellValues* Offer) {
	this->MyScreen->draw();
	MenuScreen->ClearScreen();

	SPA::Button::ButtonClass TitleCard(MenuScreen, this->MyMouse, 0, 0, 32, 3);
	SPA::Button::ButtonClass ReturnCard(MenuScreen, this->MyMouse, 0, 29, 32, 3);

	SPA::Button::ButtonClass Offering1(MenuScreen, this->MyMouse, 0, 5, 24, 5);
	SPA::Button::ButtonClass Offering2(MenuScreen, this->MyMouse, 0, 11, 24, 5);
	SPA::Button::ButtonClass Offering3(MenuScreen, this->MyMouse, 0, 17, 24, 5);

	SPA::Entity::TextEntity PurchaseYes(MenuScreen);
	PurchaseYes.SetVisibleAmount(2000);

	//SPA::Button::ButtonClass SellSpells(MenuScreen, this->MyMouse, 0, 23, 24, 3);

	TitleCard.SetButtonText("Alchemist");
	ReturnCard.SetButtonText("Go back to adventruing");

	Offering1.SetButtonText(Offer[0].SpellName + "(10)");
	Offering2.SetButtonText(Offer[1].SpellName + "(10)");
	Offering3.SetButtonText(Offer[2].SpellName + "(10)");

	//SellSpells.SetButtonText("Sell spells");

	while (1) {

		if (ReturnCard.isLeftClick()) {
			while (this->MyMouse->isLeftClick()) {}
			return;
		}


		TitleCard.draw();

		if (this->PlayerGoldAmount >= 10) Offering1.draw(SPA::Classes::Color::CYAN);
		else Offering1.draw(SPA::Classes::Color::D_RED);
		if (this->PlayerGoldAmount >= 10) Offering2.draw(SPA::Classes::Color::CYAN);
		else Offering2.draw(SPA::Classes::Color::D_RED);
		if (this->PlayerGoldAmount >= 10) Offering3.draw(SPA::Classes::Color::CYAN);
		else Offering3.draw(SPA::Classes::Color::D_RED);

		if (Offering1.isLeftClick() && this->PlayerGoldAmount >= 10) {
			this->CollectedSpells.push_back(Offer[0]);
			this->PlayerGoldAmount -= 10;
			PurchaseYes.SetText(Offer[0].SpellName);
			PurchaseYes.SetPosition(0, 26);
			while (this->MyMouse->isLeftClick()) {}
		}
		else if (Offering2.isLeftClick() && this->PlayerGoldAmount >= 10) {
			this->CollectedSpells.push_back(Offer[1]);
			this->PlayerGoldAmount -= 10;
			PurchaseYes.SetText(Offer[1].SpellName);
			PurchaseYes.SetPosition(0, 26);
			while (this->MyMouse->isLeftClick()) {}
		}
		else if (Offering3.isLeftClick() && this->PlayerGoldAmount >= 10) {
			this->CollectedSpells.push_back(Offer[2]);
			this->PlayerGoldAmount -= 10;
			PurchaseYes.SetText(Offer[2].SpellName);
			PurchaseYes.SetPosition(0, 26);
			while (this->MyMouse->isLeftClick()) {}
		}

		//SellSpells.draw(SPA::Classes::Color::RED);
		std::stringstream ss;
		ss << "Your gold: " << this->PlayerGoldAmount;
		SPA::Functions::PlaceStringToScreen(MenuScreen, ss.str(), SPA::Classes::Color::YEL, {0, 25});

		std::stringstream ss2;
		ss2 << "What will it be today?";
		SPA::Functions::PlaceStringToScreen(MenuScreen, ss2.str(), SPA::Classes::Color::GRAY, {0,3});

		if (PurchaseYes.isVisible()) PurchaseYes.draw();
		ReturnCard.draw(SPA::Classes::Color::RED);

		MenuScreen->draw();
	}
}

std::string SPA::Game::GameLoop::BasicItemSummary(SPA::Items::ItemStruct DisplayMe) {
	std::stringstream ss;
	switch (DisplayMe.itemType) {
	case 1:
		ss << "[Sword Weapon]" << std::endl;
		break;
	case 2:
		ss << "[Staff Weapon]" << std::endl;
		break;
	case 3:
		ss << "[Bow Weapon]" << std::endl;
		break;
	case 4:
		ss << "[Helmet]" << std::endl;
		break;
	case 5:
		ss << "[Chestplate]" << std::endl;
		break;
	case 6:
		ss << "[Leggings]" << std::endl;
		break;
	case 7:
		ss << "[Boots]" << std::endl;
		break;
	case 8:
		ss << "[Accessory]" << std::endl;
		break;
	}

	ss << DisplayMe.Name << std::endl;

	ss << "H[" << DisplayMe.ItemStats.Health << "]M[" << DisplayMe.ItemStats.Mana << "]";
	ss << "A[" << DisplayMe.ItemStats.Attack << "]M[" << DisplayMe.ItemStats.Magic << "]";
	ss << "S[" << DisplayMe.ItemStats.Speed << "]" << std::endl;
	
	return ss.str();
}

void SPA::Game::GameLoop::applyEquipmentToPlayer() {
	this->GoodGuy.RemoveAllBonusStats();
	this->GoodGuy.SetWeapon(this->PlayerEquipment[0]);
	for (int i = 1; i < 6; i++) {
		if (this->PlayerEquipment[i].itemType != -1) {
			this->GoodGuy.AddBonusStats(this->PlayerEquipment[i].ItemStats);
		}
	}
}

SPA::Game::GameLoop::GameLoop(SPA::Screen::ScreenClass* screen, SPA::Classes::Mouse* mouse) : GoodGuy(screen),
PFU(screen->GetSize()), RoomDisplay(screen)
{
	this->MyMouse = mouse;
	this->MyScreen = screen;
}

void SPA::Game::GameLoop::initiate(SPA::Spell::SpellLibrary& MySpellLibrary, SPA::Items::ItemLibrary& MyItemLibrary) {

	this->lib = &MySpellLibrary;

	this->FloorTemp.GenerateRoomNetwork(this->CurrentFloor + 2);
	this->RoomDisplay.loadRoomBasedOnNode(this->FloorTemp.GetRoomInfo(), &this->GoodGuy, this->vecOfBadGuys);
	this->MyScreen->GenerateWallMesh(this->Walls);

	int TimeBetweenAIUpdates = 10000;
	auto LastMoves = std::chrono::high_resolution_clock::now();

	SPA::Classes::PathForAI LineToDraw;

	SPA::Spell::SpellClass CastingASpell(this->MyScreen);

	SPA::Spell::SpellValues AlchemistOfferings[3];
	SPA::Items::ItemStruct MerchantOfferings[3];
	for (int i = 0; i < 3; i++) {
		AlchemistOfferings[i] = MySpellLibrary.GetSpellAt(SPA::Functions::GenRand(0, MySpellLibrary.GetSpellCount() - 1));
		MerchantOfferings[i] = MyItemLibrary.GetItem(SPA::Functions::GenRand(1, MyItemLibrary.GetItemCount() - 1));
	}

	PlayerEquipment[0] = MyItemLibrary.GetItem(0);

	this->GoodGuy.SetWeapon(PlayerEquipment[0]);
	this->GoodGuy.RefreshHealth();
	this->GoodGuy.RefreshMana();
	this->CollectedItems.push_back(PlayerEquipment[0]);
	
	this->PlayerSpells[0] = MySpellLibrary.GetSpellAt(0);

	SPA::Classes::Point2i LastPos = this->GoodGuy.GetPos();

	while (1) {
		SPA::Classes::Point2i MousePos = this->MyMouse->GetMousePos();

		GoodGuy.GetPath(LineToDraw);
		SPA::Functions::DrawPathToDestination(this->MyScreen, LineToDraw);

		if (this->MyScreen->isPointOnScreen(MousePos) && this->MyScreen->GetValuesAt(MousePos).Char.UnicodeChar != '#') {

			if (this->MyMouse->isRightClick()) {
				SPA::Functions::EmptyPath(this->Path);

				this->PFU.SetStartingPoint(this->GoodGuy.GetPos());
				this->PFU.SetEndingPoint(MousePos);
				this->PFU.SetWallMesh(this->Walls);

				this->PFU.FindPath(this->Path);

				this->GoodGuy.SetPath(this->Path);
			}
			//Provjeri i kalkuliraj spelove
			for (int i = 0; i < 3; i++) {
				if (GetKeyState('1' + i) < 0) {	//Ako je stisnut spell
					if (PlayerSpells[i].CastType != -1 && this->GoodGuy.GetMana() >= PlayerSpells[i].ManaCost) {	//Ako tu postoji neki spell i igrac ima mane
						if (!CastingASpell.isSpellUp()) {	//I trenutno nije pokrenut neki spell
							CastingASpell.SetActiveSpell(PlayerSpells[i]);

							if (PlayerSpells[i].CastType != 1) CastingASpell.SetCastLocation(MousePos);	//Spell na mis
							else if (PlayerSpells[i].DamageType == 3) CastingASpell.SetCastLocation(this->GoodGuy.GetPos());	//Spell na playera
							else {	//Spell na najblizeg neprijatelja
								if (this->vecOfBadGuys.empty()) break;
								int closestIndex = 0;
								double shortestDistance = SPA::Functions::Distance(this->GoodGuy.GetPos(), this->vecOfBadGuys[0].GetPos());

								for (int i = 1; i < vecOfBadGuys.size(); i++) {
									double thisDistance = SPA::Functions::Distance(this->GoodGuy.GetPos(), this->vecOfBadGuys[i].GetPos());
									if (thisDistance < shortestDistance) {
										shortestDistance = thisDistance;
										closestIndex = i;
									}
								}
								CastingASpell.SetCastLocation(this->vecOfBadGuys[closestIndex].GetPos());
							}

							this->GoodGuy.UseMana(PlayerSpells[i].ManaCost);
						}
					}
				}
			}

		}

		if (RoomDisplay.isBonusRoom(this->GoodGuy.GetPos())) {
			this->FloorTemp.MoveToBonusRoom();
			this->DefaultValuesForChangingRooms();
		}
		else if (RoomDisplay.isNextRoom(this->GoodGuy.GetPos())) {
			this->FloorTemp.MoveToNextRoom();
			this->DefaultValuesForChangingRooms();
		}
		else if (RoomDisplay.isLastRoom(this->GoodGuy.GetPos())) {
			this->FloorTemp.MoveBack();
			this->DefaultValuesForChangingRooms();
		}
		
		//Player pos managing
		if (this->MyScreen->GetValuesAt(this->GoodGuy.GetPos()).Char.UnicodeChar == 'N') {
			this->CurrentFloor++;
			this->FloorTemp.DestroyRoomNetwork();
	
			this->FloorTemp.GenerateRoomNetwork(this->CurrentFloor + 2);
			//Namjestimo ponude ducana
			for (int i = 0; i < 3; i++) {
				AlchemistOfferings[i] = MySpellLibrary.GetSpellAt(SPA::Functions::GenRand(0, MySpellLibrary.GetSpellCount() - 1));
				MerchantOfferings[i] = MyItemLibrary.GetItem(SPA::Functions::GenRand(1, MyItemLibrary.GetItemCount() - 1));
			}
			
			this->DefaultValuesForChangingRooms();
			this->GoodGuy.DamagePlayer(-450);
		}
		else if (this->MyScreen->GetValuesAt(this->GoodGuy.GetPos()).Char.UnicodeChar == 'T') {
			this->CollectedItems.push_back(MyItemLibrary.GetItem(SPA::Functions::GenRand(0, MyItemLibrary.GetItemCount() - 1)));	//TODO MAKE TREASSUERS GOOD

			this->MyScreen->adminAt(this->GoodGuy.GetPos()).Char.UnicodeChar = '.';
		}
		else if (this->MyScreen->GetValuesAt(this->GoodGuy.GetPos()).Char.UnicodeChar == 'A') {
			SPA::Classes::Point2i t = this->GoodGuy.GetPos();
			t.alter(0, -1);
			this->GoodGuy.SetPos(t);

			HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
			SPA::Screen::ScreenClass tscreen(hCon, 64, 32);
			this->Alchemist(&tscreen, AlchemistOfferings);
		}
		else if (this->MyScreen->GetValuesAt(this->GoodGuy.GetPos()).Char.UnicodeChar == 'M') {
			SPA::Classes::Point2i t = this->GoodGuy.GetPos();
			t.alter(0, -1);
			this->GoodGuy.SetPos(t);

			HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
			SPA::Screen::ScreenClass tscreen(hCon, 64, 32);
			this->Mercahnt(&tscreen, MerchantOfferings);
		}
		else if (this->MyScreen->adminAt(this->GoodGuy.GetPos()).Char.UnicodeChar == '+') {
			this->GoodGuy.ClearPath();
			this->GoodGuy.SetPos(this->RoomDisplay.GetDefaultPlayerPos());
		}


		if (CastingASpell.isSpellUp()) {
			CastingASpell.DamageEnemiesInAOE(this->vecOfBadGuys, this->GoodGuy.GetStats().Magic);
			CastingASpell.ApplyEffectsToPlayer(this->GoodGuy);
			CastingASpell.drawSpell();
		}
		if (vecOfBadGuys.empty()) {
			this->RoomDisplay.removeRoomBarrier();
		}

		int c = SPA::Functions::RemoveBadGuys(this->vecOfBadGuys);
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (now - LastMoves).count();
		for (int i = 0; i < this->vecOfBadGuys.size(); i++) {
			if (vecOfBadGuys[i].isAlive()) vecOfBadGuys[i].draw();
			
			if (elapsedTime >= TimeBetweenAIUpdates) {
				this->PFU.SetStartingPoint(vecOfBadGuys[i].GetPos());
				this->PFU.SetEndingPoint(this->GoodGuy.GetPos());
				this->PFU.SetWallMesh(this->Walls);

				SPA::Classes::PathForAI TP;
				this->PFU.FindPath(TP);
				vecOfBadGuys[i].ClearPath();
				vecOfBadGuys[i].SetPath(TP);
			}
		}

		if (this->GoodGuy.isAlive()) {
			this->GoodGuy.draw();
			this->GoodGuy.DamageEnemies(this->vecOfBadGuys);
			this->GoodGuy.TakeDamageFromEnemiesInRange(this->vecOfBadGuys, 4, 3 + (4 * (this->CurrentFloor - 1)));
			int RoomT = this->FloorTemp.GetRoomInfo().RoomType;
			if (c != 0) {
				this->GoodGuy.addExp(c + (((this->CurrentFloor) - 1) / 2));
				this->PlayerGoldAmount += c;
				this->GoodGuy.levelUpIfPossible();
			}
		}
		else {
			SPA::Game::HighscoreKeeper HGH("Highscores.bin");
			SPA::Classes::scorestruct Score;
			Score.floorCount = this->CurrentFloor;
			Score.levelReached = this->GoodGuy.GetLevel();
			Score.bestItemFound = this->PlayerEquipment[0].Name;

			HGH.loadScores();
			HGH.insertScore(Score);
			HGH.saveScores();

			return;
		}

		if (GetKeyState('M') < 0) {
			this->MyScreen->draw();
			
			HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

			SPA::Screen::ScreenClass MenuScreen(hConOut, 64, 32);

			for (int y = 0; y < MenuScreen.GetSize().y; y++) {
				for (int x = 0; x < MenuScreen.GetSize().x; x++) {
					MenuScreen.adminAt(x, y).Char.UnicodeChar = ' ';
				}
			}

			SPA::Button::ButtonClass TitleCard(&MenuScreen, this->MyMouse, 0, 0, 32, 3);
			SPA::Button::ButtonClass InventoryButton(&MenuScreen, this->MyMouse, 0, 5, 24, 5);
			SPA::Button::ButtonClass SpellsButton(&MenuScreen, this->MyMouse, 0, 12, 24, 5);
			SPA::Button::ButtonClass InformationButton(&MenuScreen, this->MyMouse, 0, 19, 24, 5);
			SPA::Button::ButtonClass ReturnButton(&MenuScreen, this->MyMouse, 0, 29, 32, 3);
			SPA::Button::ButtonClass ExitGame(&MenuScreen, this->MyMouse, 0, 24, 16, 3);
			
			TitleCard.SetButtonText("Mini menu");
			InventoryButton.SetButtonText("Item inventory");
			SpellsButton.SetButtonText("Spell inventory");
			InformationButton.SetButtonText("Player info");
			ReturnButton.SetButtonText("Return");
			ExitGame.SetButtonText("Exit game");
			
			while (1) {

				if (ReturnButton.isLeftClick()) break;
				else if (InventoryButton.isLeftClick()) this->invMenu(&MenuScreen);
				else if (SpellsButton.isLeftClick()) this->spellsMenu(&MenuScreen);
				else if (InformationButton.isLeftClick()) this->playerMenu(&MenuScreen);
				else if (ExitGame.isLeftClick()) {
					this->GoodGuy.DamagePlayer(100000000000);
					break;
				}

				TitleCard.draw();
				InventoryButton.draw(SPA::Classes::Color::GREEN);
				SpellsButton.draw(SPA::Classes::Color::GREEN);
				InformationButton.draw(SPA::Classes::Color::GREEN);
				ReturnButton.draw(SPA::Classes::Color::GREEN);
				ExitGame.draw(SPA::Classes::Color::D_RED);
				MenuScreen.draw();
			}
		}

		//HUD
		std::stringstream ssHUD;
		ssHUD << " Lvl: " << this->GoodGuy.GetLevel() << " / Exp: " << this->GoodGuy.GetExp() << "/" << this->GoodGuy.GetUntilNextLevel() 
			<< "/ Gold: " << this->PlayerGoldAmount << std::endl;
		ssHUD << " HP: " << SPA::Functions::generateProgressBar('#', '-', 16, this->GoodGuy.GetMaxHealth(), this->GoodGuy.GetHealth())
			<< " " << this->GoodGuy.GetHealth() << " / " << this->GoodGuy.GetMaxHealth() << std::endl;
		ssHUD << " MP: " << SPA::Functions::generateProgressBar('*', '-', 16, this->GoodGuy.GetMaxMana(), this->GoodGuy.GetMana())
			<< " " << this->GoodGuy.GetMana() << " / " << this->GoodGuy.GetMaxMana() << std::endl;
		ssHUD << " EQ: " << this->GoodGuy.GetWeapon().Name << std::endl;
		ssHUD << "[1] " << PlayerSpells[0].SpellName << " (" << PlayerSpells[0].ManaCost << ")" << std::endl;
		ssHUD << "[2] " << PlayerSpells[1].SpellName << " (" << PlayerSpells[1].ManaCost << ")" << std::endl;
		ssHUD << "[3] " << PlayerSpells[2].SpellName << " (" << PlayerSpells[2].ManaCost << ")" << std::endl;
		ssHUD << "[M] - Open Mini Menu!" << std::endl;

		SPA::Functions::PlaceMultipleLinesOfText(this->MyScreen, ssHUD.str(), 7, {0, 32});

		this->MyScreen->draw();

		LastPos = this->GoodGuy.GetPos();
	}

}

SPA::Game::LoadingLoop::LoadingLoop(SPA::Screen::ScreenClass* screen, int duration) {
	this->MyScreen = screen;
	this->Dur = duration;
}

void SPA::Game::LoadingLoop::initiate() {
	for (int y = 0; y < this->MyScreen->GetSize().y; y++) {
		for (int x = 0; x < this->MyScreen->GetSize().x; x++) {
			if (y == 0 || y == this->MyScreen->GetSize().y - 1) this->MyScreen->adminAt(x, y).Char.UnicodeChar = '#';
			else if(x==0 || x== this->MyScreen->GetSize().x - 1) this->MyScreen->adminAt(x, y).Char.UnicodeChar = '#';
			else this->MyScreen->adminAt(x, y).Char.UnicodeChar = ' ';
		}
	}

	std::string Text = "The end is loading...";

	auto begin = std::chrono::high_resolution_clock::now();
	while (1) {
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (now - begin).count();
		if (elapsedTime > this->Dur) return;

		SPA::Functions::PlaceStringToScreen(this->MyScreen, Text, SPA::Classes::Color::GRAY, { 5, 5 });
		this->MyScreen->draw();
	}
}
