#pragma once
#include "SPA_classes.h"
#include <string>
#include <vector>

namespace SPA {
	
	namespace Items {
	
		struct ItemStruct {
			int itemType = -1;
			int SpellIndex;
			int TimeBetweenAttack;
			SPA::Classes::Stats ItemStats;

			std::string Name;
			std::string Lore;
		};

		class ItemLibrary {
			std::vector<SPA::Items::ItemStruct> vecOfItems;

		public:
			ItemLibrary(std::string ItemLibraryFile);

			SPA::Items::ItemStruct GetItem(int index);
			int GetItemCount();
		};

		std::string ItemSummary(SPA::Items::ItemStruct Summarize);
	}

}