#pragma once
#include "SPA_Screen.h"
#include "SPA_Entity.h"

namespace SPA {

	namespace Room {

		static struct Type {
			static const int Non = -1;
			static const int Start = 0;
			static const int Passage = 1;
			static const int PassageWithTreassure = 2;
			static const int Hallway = 3;
			static const int Merchant = 4;
			static const int Alchemist = 5;
			static const int NextFloor = 6;
		};
	
		struct RoomNode {
			int RoomType = -1;
			int orientation = 0;
			bool Visited = false;

			RoomNode* NextRoom = nullptr;
			RoomNode* LastRoom = nullptr;

			RoomNode* BonusRoom = nullptr;
		};

		class FloorManager {
			SPA::Room::RoomNode* RootNode;

			SPA::Room::RoomNode CurrentRoomInfo;

			void RelMem(SPA::Room::RoomNode* RoomToCheck);
		public:
			FloorManager();

			~FloorManager();

			void GenerateRoomNetwork(int RoomCount);
			void DestroyRoomNetwork();

			void MoveToNextRoom();
			void MoveToBonusRoom();
			void MoveBack();

			SPA::Room::RoomNode GetRoomInfo();

		};

		class RoomDisplayMan {
			SPA::Screen::ScreenClass* MyScreen;
			SPA::Room::RoomNode MyRoom;

			SPA::Classes::Point2i DPP;

			int RoomCounter = 3;

		public:
			RoomDisplayMan(SPA::Screen::ScreenClass* screen);

			void loadRoomBasedOnNode(SPA::Room::RoomNode RoomToDisplay, SPA::Entity::Player* Player, std::vector<SPA::Entity::Enemy>& vecOfEnemy, int CurrLevel = 1);
			void removeRoomBarrier();
			SPA::Classes::Point2i GetDefaultPlayerPos();

			bool isNextRoom(SPA::Classes::Point2i PlayerPos);
			bool isBonusRoom(SPA::Classes::Point2i PlayerPos);
			bool isLastRoom(SPA::Classes::Point2i PlayerPos);

			bool isNextRoom(int xPos, int yPos);
			bool isBonusRoom(int xPos, int yPos);
			bool isLastRoom(int xPos, int yPos);
		};

	}

}