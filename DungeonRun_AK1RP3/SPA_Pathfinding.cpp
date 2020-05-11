#include "SPA_Pathfinding.h"
#include "SPA_functions.h"

SPA::Classes::Point2i SPA::Pathfinding::PathfindUtility::WhatIsNextBestOption(std::vector<SPA::Classes::Point2i>& PointCol, SPA::Classes::Point2i Goal) {

	double min = SPA::Functions::Distance(PointCol[0], Goal);
	int indexmin = 0;

	for (int i = 1; i < PointCol.size(); i++) {
		double t = SPA::Functions::Distance(PointCol[i], Goal);
		if (t < min) {
			min = t;
			indexmin = i;
		}
	}

	SPA::Classes::Point2i ShortestRoute = PointCol[indexmin];

	PointCol.erase(PointCol.begin() + indexmin);

	return ShortestRoute;
}

int SPA::Pathfinding::PathfindUtility::indexOf(int x, int y) {
	return y * this->AreaSize.x + x;
}

void SPA::Pathfinding::PathfindUtility::MakeEmptyPath() {
	this->PathInfo.clear();
	this->PathInfo.resize(this->WallMesh.size());
}

bool SPA::Pathfinding::PathfindUtility::RecursiveFunc(std::stack<SPA::Classes::Point2i>& Path) {
	if (this->CurrentPoint.MyLocation.x < 0 || this->CurrentPoint.MyLocation.x >= this->AreaSize.x) return false;
	if (this->CurrentPoint.MyLocation.y < 0 || this->CurrentPoint.MyLocation.y >= this->AreaSize.y) return false;

	if (this->CurrentPoint.MyLocation == this->End) {
		SPA::Pathfinding::Path_Point Locator = this->CurrentPoint;
		while (Locator.MyLocation != this->Start.MyLocation) {
			Path.push(Locator.MyLocation);

			int ind = this->indexOf(Locator.MyParent.x, Locator.MyParent.y);
			Locator = this->PathInfo[ind];
		}
		return true;
	}

	int index = this->indexOf(this->CurrentPoint.MyLocation.x, this->CurrentPoint.MyLocation.y);

	if (this->WallMesh[index]) return false;
	this->WallMesh[index] = true;
	this->PathInfo[index] = this->CurrentPoint;

	SPA::Classes::Point2i tP;
	std::vector<SPA::Classes::Point2i> PointVec;
	//Desna tocke
	tP = this->CurrentPoint.MyLocation;
	tP.alter(1, 0);
	PointVec.push_back(tP);
	//Ljeva tocka
	tP = this->CurrentPoint.MyLocation;
	tP.alter(-1, 0);
	PointVec.push_back(tP);
	//Tocka ispod
	tP = this->CurrentPoint.MyLocation;
	tP.alter(0, 1);
	PointVec.push_back(tP);
	//Gornja tocka
	tP = this->CurrentPoint.MyLocation;
	tP.alter(0, -1);
	PointVec.push_back(tP);

	SPA::Pathfinding::Path_Point NextPoint;
	NextPoint.MyParent = this->CurrentPoint.MyLocation;
	while (!PointVec.empty()) {
		NextPoint.MyLocation = this->WhatIsNextBestOption(PointVec, this->End);

		this->CurrentPoint = NextPoint;

		if (this->RecursiveFunc(Path)) return true;
	}

	return false;
}

SPA::Pathfinding::PathfindUtility::PathfindUtility(int x, int y) : PathfindUtility(SPA::Classes::Point2i(x, y)) {}

SPA::Pathfinding::PathfindUtility::PathfindUtility(SPA::Classes::Point2i Size) {
	this->AreaSize = Size;
}

void SPA::Pathfinding::PathfindUtility::SetStartingPoint(SPA::Classes::Point2i Start) {
	this->Start.MyLocation = Start;
	this->Start.MyParent.init(-1, -1);
}

void SPA::Pathfinding::PathfindUtility::SetEndingPoint(SPA::Classes::Point2i End) {
	this->End = End;
}

void SPA::Pathfinding::PathfindUtility::SetWallMesh(std::vector<bool>& WallMesh) {
	if (this->AreaSize.x * this->AreaSize.y != WallMesh.size()) throw "Incorrect area size!";
	this->WallMesh = WallMesh;
	this->MakeEmptyPath();
}

void SPA::Pathfinding::PathfindUtility::FindPath(std::stack<SPA::Classes::Point2i>& PathToFollow) {
	if (this->Start.MyLocation == -1) throw "Invalid Start Location!";
	if (this->End == -1) throw "Invalid End location!";

	this->CurrentPoint = this->Start;

	this->RecursiveFunc(PathToFollow);
}
