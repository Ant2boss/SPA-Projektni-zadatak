#include "SPA_Items.h"
#include <sstream>
#include <fstream>

SPA::Items::ItemLibrary::ItemLibrary(std::string ItemLibraryFile) {
	
	std::ifstream infile(ItemLibraryFile);
	if (!infile) throw "Error opening file!";

	std::string line;
	while (std::getline(infile, line)) {
		
		std::stringstream ss(line);
		SPA::Items::ItemStruct T;
		ss >> T.itemType;
		ss >> T.SpellIndex;
		ss >> T.TimeBetweenAttack;

		std::getline(infile, line);
		std::stringstream ss2(line);

		ss2 >> T.ItemStats.Health;
		ss2 >> T.ItemStats.Mana;
		ss2 >> T.ItemStats.Attack;
		ss2 >> T.ItemStats.Magic;
		ss2 >> T.ItemStats.Speed;

		std::getline(infile, T.Name);
		std::getline(infile, T.Lore);

		this->vecOfItems.push_back(T);
	}


	infile.close();
}

SPA::Items::ItemStruct SPA::Items::ItemLibrary::GetItem(int index) {
	return this->vecOfItems.at(index);
}

int SPA::Items::ItemLibrary::GetItemCount() {
	return this->vecOfItems.size();
}

std::string SPA::Items::ItemSummary(SPA::Items::ItemStruct Summarize) {
	std::stringstream ss;

	if (Summarize.itemType == -1) return "-NA-";

	ss << Summarize.Name << "[" << Summarize.ItemStats.Health << ", " << Summarize.ItemStats.Mana << ", "
		<< Summarize.ItemStats.Attack << ", " << Summarize.ItemStats.Magic << ", " << Summarize.ItemStats.Speed << "]";
	return ss.str();
}
