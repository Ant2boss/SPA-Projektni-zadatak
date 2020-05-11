#include "SPA_Rooms.h"
#include "SPA_functions.h"
#include <sstream>
#include <fstream>

void SPA::Room::FloorManager::RelMem(SPA::Room::RoomNode* RoomToCheck) {
	if (RoomToCheck == nullptr) return;

	this->RelMem(RoomToCheck->NextRoom);
	this->RelMem(RoomToCheck->BonusRoom);
	delete RoomToCheck;
}

SPA::Room::FloorManager::FloorManager() {
	this->RootNode = new SPA::Room::RoomNode;

	this->RootNode->RoomType = SPA::Room::Type::Start;
	this->RootNode->Visited = true;

	this->CurrentRoomInfo = *this->RootNode;
}

SPA::Room::FloorManager::~FloorManager() {
	this->RelMem(this->RootNode);
}

void SPA::Room::FloorManager::GenerateRoomNetwork(int RoomCount) {
	SPA::Room::RoomNode* lastNode;
	SPA::Room::RoomNode* newNode = nullptr;
	SPA::Room::RoomNode* hallNode;

	lastNode = this->RootNode;

	for (int i = 1; i < RoomCount; i++) {

		lastNode->NextRoom = new SPA::Room::RoomNode;
		newNode = lastNode->NextRoom;

		if (SPA::Functions::GenRand(1, 100) < 95) newNode->RoomType = SPA::Room::Type::Passage;
		else newNode->RoomType = SPA::Room::Type::PassageWithTreassure;
		newNode->LastRoom = lastNode;

		if (SPA::Functions::GenRand(1, 100) <= 25) {
			newNode->RoomType = SPA::Room::Type::Hallway;
			hallNode = newNode;	//Pohranimo mjesto racvanja za kasnije

			lastNode = newNode;	//Halway nam djeluje kao root
			lastNode->BonusRoom = new SPA::Room::RoomNode;	//Stvorimo novu sobu

			newNode = lastNode->BonusRoom;	//Pozicionramo se i postavimo vrijednosti
			newNode->RoomType = SPA::Room::Type::Passage;
			newNode->orientation = 1;
			newNode->LastRoom = lastNode;

			lastNode = newNode;	//Pozicioniramo se u novu sobu

			for (int j = i + 1; j < RoomCount; j++) {
				lastNode->NextRoom = new SPA::Room::RoomNode;

				newNode = lastNode->NextRoom;
				newNode->RoomType = SPA::Room::Type::Passage;
				newNode->orientation = 1;
				newNode->LastRoom = lastNode;

				lastNode = newNode;
			}

			int val = SPA::Functions::GenRand(1, 100);
			if (val < 50) lastNode->RoomType = SPA::Room::Type::Alchemist;
			else lastNode->RoomType = SPA::Room::Type::Merchant;
			newNode = hallNode;

		}

		lastNode = newNode;
	}

	newNode->RoomType = SPA::Room::Type::NextFloor;

	this->CurrentRoomInfo = *this->RootNode;
}

void SPA::Room::FloorManager::DestroyRoomNetwork() {
	this->RelMem(this->RootNode);
	this->RootNode = new SPA::Room::RoomNode;

	this->RootNode->RoomType = SPA::Room::Type::Start;
	this->RootNode->Visited = true;

	this->CurrentRoomInfo = *this->RootNode;
}

void SPA::Room::FloorManager::MoveToNextRoom() {
	if (this->CurrentRoomInfo.NextRoom == nullptr) return;
	SPA::Room::RoomNode* t = this->CurrentRoomInfo.NextRoom;
	t->LastRoom->Visited = true;
	this->CurrentRoomInfo = *t;
}

void SPA::Room::FloorManager::MoveToBonusRoom() {
	if (this->CurrentRoomInfo.BonusRoom == nullptr) return;
	SPA::Room::RoomNode* t = this->CurrentRoomInfo.BonusRoom;
	t->LastRoom->Visited = true;
	this->CurrentRoomInfo = *t;
}

void SPA::Room::FloorManager::MoveBack() {
	if (this->CurrentRoomInfo.LastRoom == nullptr) return;
	SPA::Room::RoomNode* t = this->CurrentRoomInfo.LastRoom;
	t->NextRoom->Visited = true;
	this->CurrentRoomInfo = *t;
}

SPA::Room::RoomNode SPA::Room::FloorManager::GetRoomInfo() {
	return this->CurrentRoomInfo;
}

SPA::Room::RoomDisplayMan::RoomDisplayMan(SPA::Screen::ScreenClass* screen) {
	this->MyScreen = screen;
}

void SPA::Room::RoomDisplayMan::loadRoomBasedOnNode(SPA::Room::RoomNode RoomToDisplay, SPA::Entity::Player* Player, std::vector<SPA::Entity::Enemy>& vecOfEnemy, int CurrLevel) {
	this->MyRoom = RoomToDisplay;
	vecOfEnemy.clear();

	std::string filename;
	std::stringstream ss;

	switch (RoomToDisplay.RoomType) {
	case SPA::Room::Type::Start:
		filename = "S_";
		break;
	case SPA::Room::Type::Passage:
		filename = "P_";
		break;
	case SPA::Room::Type::Hallway:
		filename = "H_";
		break;
	case SPA::Room::Type::Alchemist:
		filename = "A_";
		break;
	case SPA::Room::Type::Merchant:
		filename = "M_";
		break;
	case SPA::Room::Type::PassageWithTreassure:
		filename = "PT_";
		break;
	case SPA::Room::Type::NextFloor:
		filename = "NF_";
		break;
	}
	
	ss << "./RoomLayout/" << filename << this->RoomCounter % 3 << "_" << this->MyRoom.orientation << ".txt";
	this->RoomCounter++;	//Odkomentiraj me plz
	std::ifstream infile(ss.str());
	if (!infile) throw "Failed to open file!";

	SPA::Entity::Enemy T_enemy(this->MyScreen, 35 * CurrLevel);

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			char t;
			infile >> t;
			if (t == '\n') continue;

			this->MyScreen->adminAt(x, y).Char.UnicodeChar = '.';
			this->MyScreen->adminAt(x, y).Attributes = 7;

			if (t == 'P') Player->SetPos(x, y);
			else if (t == 'X') {
				T_enemy.SetPos(x, y);
				T_enemy.SetTimeBetweenMoves(SPA::Functions::GenRand(750, 1250));
				vecOfEnemy.push_back(T_enemy);
			}
			else if (t == 'B') {
				this->MyScreen->adminAt(x, y).Char.UnicodeChar = '+';
				this->MyScreen->adminAt(x, y).Attributes = SPA::Classes::Color::CYAN;
			}
			else if (t == 'T' && RoomToDisplay.Visited == false) {
				this->MyScreen->adminAt(x, y).Char.UnicodeChar = 'T';
				this->MyScreen->adminAt(x, y).Attributes = SPA::Classes::Color::YEL;
			}
			else if (t == 'T') {
				this->MyScreen->adminAt(x, y).Char.UnicodeChar = '.';
			}
			else this->MyScreen->adminAt(x, y).Char.UnicodeChar = t;

		}
	}

	this->DPP = Player->GetPos();

	infile.close();
}

void SPA::Room::RoomDisplayMan::removeRoomBarrier() {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			if (this->MyScreen->adminAt(x, y).Char.UnicodeChar == '+') {
				this->MyScreen->adminAt(x, y).Char.UnicodeChar = '.';
				this->MyScreen->adminAt(x, y).Attributes = 7;
			}
		}
	}
}

SPA::Classes::Point2i SPA::Room::RoomDisplayMan::GetDefaultPlayerPos() {
	return this->DPP;
}

bool SPA::Room::RoomDisplayMan::isNextRoom(SPA::Classes::Point2i PlayerPos) {
	if (PlayerPos.x >= 64 - 3) return true;
	if (PlayerPos.y >= 32 - 2) return true;
	return false;
}

bool SPA::Room::RoomDisplayMan::isBonusRoom(SPA::Classes::Point2i PlayerPos) {
	if (this->MyRoom.RoomType == SPA::Room::Type::Hallway && PlayerPos.y >= 32 - 3) return true;
	return false;
}

bool SPA::Room::RoomDisplayMan::isLastRoom(SPA::Classes::Point2i PlayerPos) {
	if (PlayerPos.x <= 2) return true;
	if (PlayerPos.y <= 1) return true;
	return false;
}

bool SPA::Room::RoomDisplayMan::isNextRoom(int xPos, int yPos) {
	return this->isNextRoom(SPA::Classes::Point2i(xPos, yPos));
}

bool SPA::Room::RoomDisplayMan::isBonusRoom(int xPos, int yPos) {
	return this->isBonusRoom(SPA::Classes::Point2i(xPos, yPos));
}

bool SPA::Room::RoomDisplayMan::isLastRoom(int xPos, int yPos) {
	return this->isLastRoom(xPos, yPos);
}
