#pragma once
#include "SPA_classes.h"
#include <vector>
#include <stack>

namespace SPA {

	namespace Pathfinding {

		//Nije najbolji, ni najoptimalniji, ali nade put, ako postoji
		//Stoga je savrsen :)

		//Pathfinding je neka kombinacija "probaj sve sto psotoji" i "prioritizitaj one koji su blizi odredistu"

		struct Path_Point {
			SPA::Classes::Point2i MyLocation;
			SPA::Classes::Point2i MyParent;
		};
		
		class PathfindUtility {
			SPA::Pathfinding::Path_Point Start;
			SPA::Classes::Point2i End;

			SPA::Pathfinding::Path_Point CurrentPoint;

			std::vector<bool> WallMesh;
			std::vector<Path_Point> PathInfo;

			SPA::Classes::Point2i AreaSize;

			SPA::Classes::Point2i WhatIsNextBestOption(std::vector<SPA::Classes::Point2i>& PointCol, SPA::Classes::Point2i Goal);

			int indexOf(int x, int y);
			void MakeEmptyPath();
			bool RecursiveFunc(std::stack<SPA::Classes::Point2i>& Path);

		public:
			PathfindUtility(int x, int y);
			PathfindUtility(SPA::Classes::Point2i Size);

			void SetStartingPoint(SPA::Classes::Point2i Start);
			void SetEndingPoint(SPA::Classes::Point2i End);
			void SetWallMesh(std::vector<bool>& WallMesh);

			void FindPath(std::stack<SPA::Classes::Point2i>& PathToFollow);
		};

	}

}