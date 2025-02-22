//PRIMORDIAL STAR IS A HOMEBREW PATHFINDING BASED ON TAUT PATHS
//I LEARNED LATER IT IS EQUIVALENT TO VISIBILITY GRAPHS
//BUT ITS PRETTY OPTIMIZED AND IM SUPER PROUD OF IT 

#pragma once
#include <cfloat>
#include <sc2api/sc2_api.h>
#include <sc2api/sc2_common.h>

#include <map>
#include "constants.h"
#include "debugging.hpp"

#define MAX_CONN_DIST_SQRD 65025

using namespace sc2;
using namespace std;

struct DistanceNode {
	float distancePP;
	float distanceNP;
	float distanceNN;
	float distancePN;
};

struct MinDistanceNode {
	float distance;
};

class StarNode {
public:
	//PathNode this compares to
	int pathNode;

	//distance already travelled
	float g;

	//optimistic distance to travel
	float h;

	StarNode(int pathNode, float g, float h) : pathNode(pathNode), g(g), h(h) {

	}

	//bool operator<(const StarNode& b) const {
	//	return	(g + h) < (b.g + b.h);
	//}

	operator float() {
		return g + h;
	}
};

class AStarCompare {
public:
	bool operator()(StarNode a, StarNode b)
	{
		return (a.g + a.h > b.g + b.h);
	}
};

class DijkStarNode {
public:

	//PathNode this compares to
	int pathNode;

	//distance already travelled
	float g;

	DijkStarNode(int pathNode, float g) : pathNode(pathNode), g(g) {

	}

	//bool operator<(const StarNode& b) const {
	//	return	(g + h) < (b.g + b.h);
	//}

	operator float() {
		return g;
	}
};

class DijkstraCompare {
public:
	bool operator()(DijkStarNode a, DijkStarNode b)
	{
		return (a.g > b.g);
	}
};

namespace PrimordialStar {

	enum Cardinal {
		INVALID,
		UP,
		UP_RT,
		RT,
		DN_RT,
		DN,
		DN_LT,
		LT,
		UP_LT
	};

	Point2D cardinalToDirection(Cardinal &cardinal) {
		switch (cardinal) {
			case(UP): {
				return { 0,1 };
			}
			case(UP_RT): {
				return { 1,1 };
			}
			case(RT): {
				return { 1,0 };
			}
			case(DN_RT): {
				return { 1,-1 };
			}
			case(DN): {
				return { 0,-1 };
			}
			case(DN_LT): {
				return { -1,-1 };
			}
			case(LT): {
				return { -1,0 };
			}
			case(UP_LT): {
				return { -1,1 };
			}
			default: {
				return { 0,0 };
			}
		}
	}

	Point2D cardinalToNormDirection(Cardinal& cardinal) {
		switch (cardinal) {
		case(UP): {
			return { 0,1 };
		}
		case(UP_RT): {
			return { 0.7071067812,0.7071067812 };
		}
		case(RT): {
			return { 1,0 };
		}
		case(DN_RT): {
			return { 0.7071067812,-0.7071067812 };
		}
		case(DN): {
			return { 0,-1 };
		}
		case(DN_LT): {
			return { -0.7071067812,-0.7071067812 };
		}
		case(LT): {
			return { -1,0 };
		}
		case(UP_LT): {
			return { -0.7071067812,0.7071067812 };
		}
		default: {
			return { 0,0 };
		}
		}
	}

	class PathNode {
	private:
		Point2D pos;
	public:

		Cardinal wall;
		int id;
		vector<int> connected;

		PathNode(Point2D pos, Cardinal wall, Agent* agent);

		~PathNode();

		void updatePos(Point2D p) {
			pos = p;
		}

		Point2D rawPos() {
			return pos;
		}

		Point2D position(float radius) {
			if (wall == INVALID) {
				return pos;
			}
			return pos - cardinalToNormDirection(wall) * radius;
		}
	};

	vector<PathNode*> basePathNodes;
	map2d<int8_t>* blobGrid;
	map2d<DistanceNode>* maxDistanceGrid;
	map2d<MinDistanceNode>* minDistanceGrid;

	int maxConnections = 0;
	float maxDistanceConnectionSquared = 0.0F;

	//DDA https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
	bool checkLinearPath(Point2D start, Point2D end, Agent* agent) {
		float dx = end.x - start.x;
		float dy = end.y - start.y;
		float step = 0;

		if (abs(dx) >= abs(dy)) {
			step = abs(dx);
		}
		else {
			step = abs(dy);
		}

		dx /= step;
		dy /= step;

		Point2D operating = start;
		for (int i = 0; i < step; i++) {
			if (!Aux::checkPathable(int(operating.x), int(operating.y), agent)) {
				return false;
			}
			operating += {dx, dy};
		}
		return true;
	}

	float checkWallDistance2(Point2D start, Point2D dir, Agent* agent, int maxSteps = 255){
		float dx = dir.x;
		float dy = dir.y;
		float step = 0;

		if (abs(dx) >= abs(dy)) {
			step = abs(dx);
		}
		else {
			step = abs(dy);
		}

		dx /= step;
		dy /= step;

		Point2D operating = start;
		for (int i = 0; i < maxSteps; i++) {
			if (!Aux::checkPathable(int(operating.x), int(operating.y), agent)) {
				return Distance2D(start, operating);
			}
			operating += {dx, dy};
		}
		return maxSteps;
	}

	float rnd(float in) {
		return (int)(in + 0.1);
	}

	float checkWallDistance(Point2D origin, Point2D dir, Agent* agent, int maxSteps = 255) {
		//var delta = Vector2.Normalize(direction - origin);
		Point2D delta = dir;

		float dxdy = delta.y == 0 ? 0 : delta.x / delta.y;
		float dydx = delta.x == 0 ? 0 : delta.y / delta.x;

		// If delta X is 0, we set the rayLength to a big one so that the Y ray is always chosen (we're moving straight up or straight down)
		float rayLengthWhenMovingInX = FLT_MAX;
		if (delta.x != 0) {
			rayLengthWhenMovingInX = sqrt(1 + dydx * dydx);
		}

		// If delta Y is 0, we set the rayLength to a big one so that the X ray is always chosen (we're moving straight left or straight right)
		float rayLengthWhenMovingInY = FLT_MAX;
		if (delta.y != 0) {
			rayLengthWhenMovingInY = sqrt(1 + dxdy * dxdy);
		}

		// Edge case, if deltaX is 0, stepXDistance can be 0, making the first ray 0, thus it's going to be picked
		// We want to avoid that so we set it to 1
		float stepX = 1;
		float stepXDistance = 1.0F;
		if (delta.x > 0) {
			// Moving right
			stepX = 1;
			stepXDistance = floor(origin.x + 1) - origin.x;
		}
		else if (delta.x < 0) {
			// Moving left
			stepX = -1;
			stepXDistance = origin.x - floor(origin.x);
		}

		// Edge case, if deltaY is 0, stepYDistance can be 0, making the first ray 0, thus it's going to be picked
		// We want to avoid that so we set it to 1
		float stepY = 1;
		float stepYDistance = 1.0F;
		if (delta.y > 0) {
			// Moving up
			stepY = 1;
			stepYDistance = floor(origin.y + 1) - origin.y;
		}
		else if (delta.y < 0) {
			// Moving down
			stepY = -1;
			stepYDistance = origin.y - floor(origin.y);
		}

		Point2D lastIntersection = origin;
		Point2D currentCell = P2D(P2DI(origin));
		float xRayLength = stepXDistance * rayLengthWhenMovingInX;
		float yRayLength = stepYDistance * rayLengthWhenMovingInY;

		while (Aux::getPathable(currentCell.x, currentCell.y, agent) != 127) {
			if (xRayLength < yRayLength) {
				// Step in X, reduce Y ray
				yRayLength -= xRayLength;

				// Move to the cell on the left or right
				currentCell.x += stepX;
				lastIntersection += delta * xRayLength;

				// Due to float imprecision, we get some rounding errors
				// We can round X to the closest int to eliminate the error because we know we stepped in X (int, float)
				lastIntersection.x = rnd(lastIntersection.x);

				// Reset X ray
				xRayLength = rayLengthWhenMovingInX;
			}
			else if (yRayLength < xRayLength) {
				// Step in Y, reduce X ray
				xRayLength -= yRayLength;

				// Move to the cell on the bottom or top
				currentCell.y += stepY;
				lastIntersection += delta * yRayLength;

				// Due to float imprecision, we get some rounding errors
				// We can round Y to the closest int to eliminate the error because we know we stepped in Y (float, int)
				lastIntersection.y = rnd(lastIntersection.y);

				// Reset Y ray
				yRayLength = rayLengthWhenMovingInY;
			}
			else {
				// Both rays are the same, means we landed exactly on a corner
				currentCell.x += stepX; // Move to the left/right
				currentCell.y += stepY; // And up/down
				lastIntersection += delta * yRayLength; // xRayLength and yRayLength are the same, doesn't matter which one we pick

				// Due to float imprecision, we get some rounding errors
				// We can round to the closest int to eliminate the error because we know we are a at corner (int, int)
				lastIntersection.x = rnd(lastIntersection.x);
				lastIntersection.y = rnd(lastIntersection.y);

				// Reset all rays
				xRayLength = rayLengthWhenMovingInX;
				yRayLength = rayLengthWhenMovingInY;
			}
		}
		return Distance2D(origin, lastIntersection);
	}

	float generateMaxDistanceGrid(Agent* agent) {
		int mapWidth = agent->Observation()->GetGameInfo().width;
		int mapHeight = agent->Observation()->GetGameInfo().height;
		float maximum = 0;
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				bool center = Aux::getPathable(i, j, agent) != 127;
				if (center) {
					float max = 0;
					constexpr int angleChecks = 64;
					int steps = 255;
					constexpr int perQuad = angleChecks / 4;

					float maxes[perQuad] = { 0 };
					for (int theta = 0; theta < angleChecks; theta++) {
						float angle = 2 * M_PI * theta / angleChecks;
						Point2D dir = {cos(angle), sin(angle)};
						float dist = checkWallDistance(Point2D{ i + 0.5F, j + 0.5F }, dir, agent, steps);
						if (dist > max) {
							max = dist;
							//steps = dist + 15;
						}
						if (dist > maximum) {
							maximum = dist;
						}

						if (theta % perQuad == (perQuad - 1)) {
							maxes[theta / perQuad] = max;
							max = 0;
						}
					}
					
					imRef(maxDistanceGrid, i, j) = {maxes[0], maxes[1], maxes[2], maxes[3]};
				}
				else {
					imRef(maxDistanceGrid, i, j) = { 0.0 };
				}
			}
		}
		return maximum;
	}

	float generateMinDistanceGrid(Agent* agent) {
		int mapWidth = agent->Observation()->GetGameInfo().width;
		int mapHeight = agent->Observation()->GetGameInfo().height;
		float maximum = 0;
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				bool center = Aux::getPathable(i, j, agent) != 127;
				if (center) {
					float min = 255;
					constexpr int angleChecks = 24;
					int steps = 255;
					for (int theta = 0; theta < angleChecks; theta++) {
						float angle = 2 * M_PI * theta / angleChecks;
						Point2D dir = { cos(angle), sin(angle) };
						float dist = checkWallDistance(Point2D{ i + 0.5F, j + 0.5F }, dir, agent, steps);
						if (dist < min) {
							min = dist;
							//steps = dist + 15;
						}
					}
					if (min > maximum) {
						maximum = min;
					}
					imRef(minDistanceGrid, i, j) = { min };
				}
				else {
					imRef(minDistanceGrid, i, j) = { 255 };
				}
			}
		}
		return maximum;
	}

	void calculateNewConnection(PathNode* p, Agent* agent) {
		Point2D pos = p->rawPos();
		for (int i = 0; i < basePathNodes.size(); i++) {
			if (i == p->id) {
				continue;
			}
			PrimordialStar::PathNode* node = PrimordialStar::basePathNodes[i];
			Point2D testPos = node->rawPos();
			//if ((p->wall == UP_LT && testPos.x > pos.x && testPos.y < pos.y) || 
			//	(p->wall == UP_RT && testPos.x < pos.x && testPos.y < pos.y) ||
			//	(p->wall == DN_LT && testPos.x > pos.x && testPos.y > pos.y) ||
			//	(p->wall == DN_RT && testPos.x < pos.x && testPos.y > pos.y)) {
			//	continue;
			//}
			if (p->wall != INVALID && node->wall != INVALID && 
				(((p->wall == UP_LT || node->wall == DN_RT) && testPos.x > pos.x && testPos.y < pos.y) ||
				((p->wall == UP_RT || node->wall == DN_LT) && testPos.x < pos.x && testPos.y < pos.y) ||
				((p->wall == DN_LT || node->wall == UP_RT) && testPos.x > pos.x && testPos.y > pos.y) ||
				((p->wall == DN_RT || node->wall == UP_LT) && testPos.x < pos.x && testPos.y > pos.y))) {
				continue;
			}
			float distSqrd = DistanceSquared2D(pos, testPos);
			//float box = max(abs(pos.x - testPos.x), abs(pos.y - testPos.y)) + 2;
			float innerMax = 255; //imRef(maxDistanceGrid, int(pos.x), int(pos.y)).distance;
			int intX = (int)(pos.x);
			int intY = (int)(pos.y);
			if (intX < 0 || intX >= maxDistanceGrid->width() || intY < 0 || intY >= maxDistanceGrid->height()) {
				continue;
			}
			if (testPos.x > pos.x && testPos.y >= pos.y) {
				innerMax = imRef(maxDistanceGrid, intX, intY).distancePP;
			}
			else if (testPos.x <= pos.x && testPos.y > pos.y) {
				innerMax = imRef(maxDistanceGrid, intX, intY).distanceNP;
			}
			else if (testPos.x < pos.x && testPos.y <= pos.y) {
				innerMax = imRef(maxDistanceGrid, intX, intY).distanceNN;
			}
			else if (testPos.x >= pos.x && testPos.y < pos.y) {
				innerMax = imRef(maxDistanceGrid, intX, intY).distancePN;
			}
			if (distSqrd > (innerMax * innerMax + 2)) {
				continue;
			}
			if (checkLinearPath(pos, testPos, agent)) {
				
				if (distSqrd > MAX_CONN_DIST_SQRD) {
					continue;
				}
				if (maxDistanceConnectionSquared < distSqrd) {
					maxDistanceConnectionSquared = distSqrd;
				}
				int m = std::max(p->connected.size(), node->connected.size());
				if (maxConnections < m) {
					maxConnections = m;
				}
				p->connected.push_back(i);
				node->connected.push_back(p->id);
			}
		}
	}

	void breakAllConnections(PathNode* p) {
		for (int i = 0; i < p->connected.size(); i++) {
			if (p->connected[i] >= PrimordialStar::basePathNodes.size()) {
				continue;
			}
			PrimordialStar::PathNode* node = PrimordialStar::basePathNodes[p->connected[i]];
			for (int c = 0; c < node->connected.size(); c++) {
				if (node->connected[c] == p->id) {
					node->connected.erase(node->connected.begin() + c);
				}
			}
		}
	}

	PathNode::PathNode(Point2D pos, Cardinal wall, Agent *agent) : pos(pos), wall(wall) {
		id = basePathNodes.size();
		basePathNodes.push_back(this);
		calculateNewConnection(this, agent);
	}

	PathNode::~PathNode(){
		breakAllConnections(this);
	}

	//Point2D findNextWall(Point2D start, Point2D direction) {
	//	Point2D dir = normalize(direction);
	//	if (dir.y == 0) {

	//	}
	//	return { 0,0 };
	//}

	void generateBlobGrid() {

	}

	constexpr float displacementMinute = 0.1F;

//#define BASIC(i, j) pathables[i + 2][j + 2]
//#define BASIC_UP(i, j) BASIC(i,j+1)
//#define BASIC_UP_RT(i, j) BASIC(i+1,j+1)
//#define BASIC_RT(i, j) BASIC(i+1,j)
//#define BASIC_DN_RT(i, j) BASIC(i+1,j-1)
//#define BASIC_DN(i, j) BASIC(i,j-1)
//#define BASIC_DN_LT(i, j) BASIC(i-1,j-1)
//#define BASIC_LT(i, j) BASIC(i-1,j)
//#define BASIC_UP_LT(i, j) BASIC(i-1,j+1)

	bool check_UP_RT(int i, int j) {
		bool up = Aux::checkPathable(i, j + 1, nullptr);
		bool up_rt = Aux::checkPathable(i + 1, j + 1, nullptr);
		bool rt = Aux::checkPathable(i + 1, j, nullptr);

		//extra trimming conditions
		bool up_lt = Aux::checkPathable(i - 1, j + 1, nullptr);
		bool dn_rt = Aux::checkPathable(i + 1, j - 1, nullptr);

		return up && up_rt && rt && (up_lt || dn_rt);
	}

	bool check_DN_RT(int i, int j) {
		bool rt = Aux::checkPathable(i + 1, j, nullptr);
		bool dn_rt = Aux::checkPathable(i + 1, j - 1, nullptr);
		bool dn = Aux::checkPathable(i, j - 1, nullptr);

		//extra trimming conditions
		bool up_rt = Aux::checkPathable(i + 1, j + 1, nullptr);
		bool dn_lt = Aux::checkPathable(i - 1, j - 1, nullptr);

		return dn && dn_rt && rt && (up_rt || dn_lt);
	}

	bool check_DN_LT(int i, int j) {
		bool dn = Aux::checkPathable(i, j - 1, nullptr);
		bool dn_lt = Aux::checkPathable(i - 1, j - 1, nullptr);
		bool lt = Aux::checkPathable(i - 1, j, nullptr);

		//extra trimming conditions
		bool dn_rt = Aux::checkPathable(i + 1, j - 1, nullptr);
		bool up_lt = Aux::checkPathable(i - 1, j + 1, nullptr);

		return dn && dn_lt && lt && (dn_rt || up_lt);
	}

	bool check_UP_LT(int i, int j) {
		bool lt = Aux::checkPathable(i - 1, j, nullptr);
		bool up_lt = Aux::checkPathable(i - 1, j + 1, nullptr);
		bool up = Aux::checkPathable(i, j + 1, nullptr);

		//extra trimming conditions
		bool dn_lt = Aux::checkPathable(i - 1, j - 1, nullptr);
		bool up_rt = Aux::checkPathable(i + 1, j + 1, nullptr);

		return up && up_lt && lt && (dn_lt || up_rt);
	}

	void generatePathNodes(Agent *agent) {
		int mapWidth = agent->Observation()->GetGameInfo().width;
		int mapHeight = agent->Observation()->GetGameInfo().height;
		for (int i = 1; i < mapWidth - 1; i++) {
			for (int j = 1; j < mapHeight - 1; j++) {
				bool center = Aux::checkPathable(i, j, agent);
				if (!center) {
					//bool up =		Aux::checkPathable(i	, j + 1	, agent);
					//bool up_rt =	Aux::checkPathable(i + 1, j + 1	, agent);
					//bool rt =		Aux::checkPathable(i + 1, j		, agent);
					//bool dn_rt =	Aux::checkPathable(i + 1, j - 1	, agent);
					//bool dn =		Aux::checkPathable(i	, j - 1	, agent);
					//bool dn_lt =	Aux::checkPathable(i - 1, j - 1	, agent);
					//bool lt =		Aux::checkPathable(i - 1, j		, agent);
					//bool up_lt =	Aux::checkPathable(i - 1, j + 1	, agent);

					//bool basic_up_rt = up && up_rt && rt;
					//bool basic_up_lt = dn && dn_rt && rt;
					//bool basic_dn_rt = dn && dn_lt && lt;
					//bool basic_dn_lt = up && up_lt && lt;

					//if (basic_up_rt) {
					//	new PathNode(Point2D{ (float)(i + 1 + displacementMinute), (float)(j + 1 + displacementMinute) }, DN_LT);
					//}
					//if (basic_up_lt) {
					//	new PathNode(Point2D{ (float)(i + 1 + displacementMinute), (float)(j - displacementMinute) }, UP_LT);
					//}
					//if (basic_dn_rt) {
					//	new PathNode(Point2D{ (float)(i - displacementMinute), (float)(j - displacementMinute) }, UP_RT);
					//}
					//if (basic_dn_lt) {
					//	new PathNode(Point2D{ (float)(i - displacementMinute), (float)(j + 1 + displacementMinute) }, DN_RT);
					//}


					//bool basic_up_rt = up && up_rt && rt;
					//bool basic_dn_rt = dn && dn_rt && rt;
					//bool basic_dn_lt = dn && dn_lt && lt;
					//bool basic_up_lt = up && up_lt && lt;

					//bool basic_up_rt = up && up_rt && rt;
					//bool basic_dn_rt = dn && dn_rt && rt;
					//bool basic_dn_lt = dn && dn_lt && lt;
					//bool basic_up_lt = up && up_lt && lt;

					if (check_UP_RT(i, j) && ((Aux::checkPathable(i - 1, j + 1, agent) || !check_UP_RT(i - 1, j + 1)) || (Aux::checkPathable(i + 1, j - 1, agent) || !check_UP_RT(i + 1, j - 1)))) {
						new PathNode(Point2D{ (float)(i + 1 + displacementMinute), (float)(j + 1 + displacementMinute) }, DN_LT, agent);
					}
					if (check_DN_RT(i, j) && ((Aux::checkPathable(i - 1, j - 1, agent) || !check_DN_RT(i - 1, j - 1)) || (Aux::checkPathable(i + 1, j + 1, agent) || !check_DN_RT(i + 1, j + 1)))) {
						new PathNode(Point2D{ (float)(i + 1 + displacementMinute), (float)(j - displacementMinute) }, UP_LT, agent);
					}
					if (check_DN_LT(i, j) && ((Aux::checkPathable(i - 1, j + 1, agent) || !check_DN_LT(i - 1, j + 1)) || (Aux::checkPathable(i + 1, j - 1, agent) || !check_DN_LT(i + 1, j - 1)))) {
						new PathNode(Point2D{ (float)(i - displacementMinute), (float)(j - displacementMinute) }, UP_RT, agent);
					}
					if (check_UP_LT(i, j) && ((Aux::checkPathable(i - 1, j - 1, agent) || !check_UP_LT(i - 1, j - 1)) || (Aux::checkPathable(i + 1, j + 1, agent) || !check_UP_LT(i + 1, j + 1)))) {
						new PathNode(Point2D{ (float)(i - displacementMinute), (float)(j + 1 + displacementMinute) }, DN_RT, agent);
					}
				}
			}
		}
		//for (int i = 0; i < basePathNodes.size(); i++) {
		//	calculateConnection(basePathNodes[i], agent);
		//}
		printf("MAX CONNECTIONS OF A NODE: %d\t MAX DISTANCE OF A CONNECTION: %.2f\n", maxConnections, sqrt(maxDistanceConnectionSquared));
	}

	vector<Point2D> getPath(Point2D start, Point2D end, float radius, Agent* agent) {
		Profiler profiler("getPath");
		PathNode* startNode = new PathNode(start, INVALID, agent);
		profiler.midLog("getPath.startN");

		PathNode* operatingNode = startNode;
		PathNode* endNode = new PathNode(end, INVALID, agent);
		profiler.midLog("getPath.endN");

		bool* visited = new bool[basePathNodes.size()];
		memset(visited, 0, basePathNodes.size() * sizeof(bool));

		vector<Point2D> points;
		map<int, int> backpath;
		profiler.midLog("getPath.init");

		if (startNode->connected.size() == 0) {
			Point2D loc = basePathNodes[0]->rawPos();
			float mindist = DistanceSquared2D(loc, start);
			for (int i = 1; i < basePathNodes.size() - 2; i++) {
				float dist = DistanceSquared2D(basePathNodes[i]->rawPos(), start);
				if (dist < mindist || (dist == mindist && (DistanceSquared2D(loc, end) > DistanceSquared2D(basePathNodes[i]->rawPos(), end)))) {
					mindist = dist;
					loc = basePathNodes[i]->rawPos();
				}
			}
			startNode->updatePos(loc);
			calculateNewConnection(startNode, agent);
		}
		if (endNode->connected.size() == 0) {
			Point2D loc = basePathNodes[0]->rawPos();
			float mindist = DistanceSquared2D(loc, end);
			for (int i = 1; i < basePathNodes.size() - 2; i++) {
				float dist = DistanceSquared2D(basePathNodes[i]->rawPos(), end);
				if (dist < mindist || (dist == mindist && (DistanceSquared2D(loc, start) > DistanceSquared2D(basePathNodes[i]->rawPos(), start)))) {
					mindist = dist;
					loc = basePathNodes[i]->rawPos();
				}
			}
			endNode->updatePos(loc);
			calculateNewConnection(endNode, agent);
		}
		profiler.midLog("getPath.correction");

		if (std::find(startNode->connected.begin(), startNode->connected.end(), endNode->id) != startNode->connected.end()) {
			profiler.subScope();
			points.push_back(startNode->rawPos());
			points.push_back(endNode->rawPos());
			profiler.midLog("getPath.skip");
		}
		else {
			profiler.subScope();
			priority_queue<StarNode, vector<StarNode>, AStarCompare> starNodes;
			starNodes.push(StarNode(operatingNode->id, 0, Distance2D(start, end)));
			visited[operatingNode->id] = true;
			bool found = false;
			for (int cycles = 0; cycles < 10000; cycles++) {
				if (starNodes.size() == 0) {
					break;
				}
				StarNode star = starNodes.top();
				starNodes.pop();
				operatingNode = basePathNodes[star.pathNode];
				Point2D currentPos = operatingNode->position(radius);
				for (int i = 0; i < operatingNode->connected.size(); i++) {
					int subNodeID = operatingNode->connected[i];
					if (visited[subNodeID]) {
						continue;
					}
					Point2D nextPos = basePathNodes[subNodeID]->position(radius);
					backpath[subNodeID] = operatingNode->id;
					starNodes.push(StarNode(subNodeID, star.g + Distance2D(currentPos, nextPos), Distance2D(nextPos, end)));
					visited[subNodeID] = true;

					//DebugText(agent, strprintf("%.1f,%.1f", star.g + Distance2D(currentPos, nextPos), Distance2D(nextPos, end)), AP3D(nextPos));

					if (operatingNode->connected[i] == endNode->id) {
						found = true;
						break;
					}
				}
				if (found) {
					break;
				}
			}
			profiler.midLog("getPath.source");
			operatingNode = endNode;
			points.push_back(basePathNodes[operatingNode->id]->rawPos());
			for (int i = 0; i < starNodes.size(); i++) {
				operatingNode = basePathNodes[backpath[operatingNode->id]];
				points.insert(points.begin(), basePathNodes[operatingNode->id]->position(radius));
				if (operatingNode->id == startNode->id) {
					break;
				}
			}
			profiler.midLog("getPath.backtrack");
		}

		delete[] visited;
		basePathNodes.pop_back();
		basePathNodes.pop_back();

		delete startNode;
		delete endNode;
		
		//printf("SOMETHING WENT WRONG\n");
		profiler.midLog("getPath.end");
		return points;
	}

	vector<Point2D> getPathDijkstra3(Point2D start, Point2D end, float radius, Agent* agent) {
		PathNode* startNode = new PathNode(start, INVALID, agent);

		PathNode* endNode = new PathNode(end, INVALID, agent);

		//bool* visited = new bool[basePathNodes.size()];
		//memset(visited, 0, basePathNodes.size() * sizeof(bool));

		vector<Point2D> points;

		if (startNode->connected.size() == 0 || endNode->connected.size() == 0) {

		}
		else {
			vector<StarNode> unvisited;
			unvisited.reserve(basePathNodes.size());

			for (PathNode* p : basePathNodes) {
				unvisited.push_back(StarNode(p->id, FLT_MAX, 0));
			}

			int opId = basePathNodes.size() - 2;
			unvisited[opId].g = 0;

			for (int cycles = 0; cycles < 10000; cycles++) {
				
				//visited[opId] = true;
				for (int i = 0; i < basePathNodes[opId]->connected.size(); i++) {
					int subNodeID = basePathNodes[opId]->connected[i];
					float dist = Distance2D(basePathNodes[opId]->position(radius), basePathNodes[subNodeID]->position(radius)) + unvisited[opId].g;
					
					if (unvisited[subNodeID].g > dist) {
						unvisited[subNodeID].g = dist;
					}
				}

				int minId = -1;
				for (int i = 0; i < unvisited.size(); i++) {
					//if (visited[i]) {
					//	continue;
					//}
					if (minId == -1 || unvisited[minId] > unvisited[i]) {
						minId = i;
					}
				}
				opId = minId;
			}
		}




		//delete[] visited;
		basePathNodes.pop_back();
		basePathNodes.pop_back();

		delete startNode;
		delete endNode;

		//printf("SOMETHING WENT WRONG\n");
		return points;
	}

	vector<Point2D> getPathDijkstra(Point2D start, Point2D end, float radius, Agent* agent) {
		Profiler profiler("getDijkstra");
		PathNode* startNode = new PathNode(start, INVALID, agent);

		PathNode* endNode = new PathNode(end, INVALID, agent);

		//bool* visited = new bool[basePathNodes.size()];
		//memset(visited, 0, basePathNodes.size() * sizeof(bool));

		vector<Point2D> points;
		profiler.midLog("getDijkstra.init");
		if (startNode->connected.size() == 0 || endNode->connected.size() == 0) {

		}
		else {
			priority_queue<DijkStarNode, vector<DijkStarNode>, DijkstraCompare> Q;

			vector<float> dist;
			vector<int> parent;
			dist.reserve(basePathNodes.size());
			parent.reserve(basePathNodes.size());
			for (int i = 0; i < basePathNodes.size(); i++) {
				dist.push_back(FLT_MAX);
				parent.push_back(-1);
			}

			Q.push(DijkStarNode(startNode->id, 0));
			dist[startNode->id] = 0;

			while (Q.size() > 0) {
				DijkStarNode u = Q.top(); Q.pop();

				PathNode* operatingNode = basePathNodes[u.pathNode];
				Point2D rawPosOp = operatingNode->rawPos();
				for (int i = 0; i < operatingNode->connected.size(); i++) {
					PathNode* adjacentNode = basePathNodes[operatingNode->connected[i]];
					Point2D rawPosAdj = adjacentNode->rawPos();
					float weight = Distance2D(rawPosOp, rawPosAdj);

					if (dist[adjacentNode->id] > dist[u.pathNode] + weight) {
						parent[adjacentNode->id] = u.pathNode;
						dist[adjacentNode->id] = dist[u.pathNode] + weight;

						Q.push(DijkStarNode(adjacentNode->id, dist[adjacentNode->id]));
					}
				}
			}

			int node = endNode->id;
			for (int cycles = 0; cycles < 10000; cycles++) {
				if (node == -1) {
					points.clear();
					break;
				}
				points.insert(points.begin(), basePathNodes[node]->position(radius));
				if (node == startNode->id) {
					break;
				}
				node = parent[node];
			}
		}




		//delete[] visited;
		basePathNodes.pop_back();
		basePathNodes.pop_back();

		delete startNode;
		delete endNode;

		//printf("SOMETHING WENT WRONG\n");
		return points;
	}

	float getPathLength(vector<Point2D> path) {
		if (path.size() == 0) return 0.0F;
		float travelled = 0;
		for (int i = 0; i < path.size() - 1; i++) {
			travelled += Distance2D(path[i], path[i + 1]);
		}
		return travelled;
	}

	float getPathLength(Point2D start, Point2D end, float radius, Agent* agent) {
		return getPathLength(getPath(start, end, radius, agent));

		//PathNode* startNode = new PathNode(start, INVALID);
		//PathNode* operatingNode = startNode;
		//PathNode* endNode = new PathNode(end, INVALID);

		//calculateConnection(startNode, agent);
		//calculateConnection(endNode, agent);

		//bool* visited = new bool[basePathNodes.size()];
		//memset(visited, 0, basePathNodes.size() * sizeof(bool));

		//float length = FLT_MAX;

		//if (startNode->connected.size() == 0 || endNode->connected.size() == 0) {
		//	//length = -1;
		//}
		//else {
		//	std::priority_queue<StarNode, vector<StarNode>, greater<float>> starNodes;
		//	starNodes.push(StarNode(operatingNode->id, 0, Distance2D(start, end)));
		//	visited[operatingNode->id] = true;
		//	bool found = false;
		//	for (int cycles = 0; cycles < 10000; cycles++) {
		//		if (starNodes.size() == 0) {
		//			break;
		//		}
		//		StarNode star = starNodes.top();
		//		starNodes.pop();
		//		operatingNode = basePathNodes[star.pathNode];
		//		Point2D currentPos = operatingNode->position(radius);
		//		for (int i = 0; i < operatingNode->connected.size(); i++) {
		//			int subNodeID = operatingNode->connected[i];
		//			if (visited[subNodeID]) {
		//				continue;
		//			}
		//			Point2D nextPos = basePathNodes[subNodeID]->position(radius);
		//			starNodes.push(StarNode(subNodeID, star.g + Distance2D(currentPos, nextPos), Distance2D(nextPos, end)));
		//			visited[subNodeID] = true;

		//			//DebugText(agent, strprintf("%.1f,%.1f", star.g + Distance2D(currentPos, nextPos), Distance2D(nextPos, end)), AP3D(nextPos));

		//			if (operatingNode->connected[i] == endNode->id) {
		//				length = star.g + Distance2D(currentPos, nextPos);
		//				found = true;
		//				break;
		//			}
		//		}
		//		if (found) {
		//			break;
		//		}
		//	}
		//}
		//delete[] visited;
		//basePathNodes.pop_back();
		//basePathNodes.pop_back();

		//return length;
	}

	Point2D distanceAlongPath(vector<Point2D> path, float distance) {
		if (path.size() == 0) return { 0,0 };
		float travelled = 0;
		for (int i = 0; i < path.size() - 1; i++) {
			float dist = Distance2D(path[i], path[i + 1]);
			if ((travelled + dist) < distance) {
				travelled += dist;
			}
			else {
				Point2D dir = normalize((path[i + 1] - path[i]));
				return path[i] + (dir * (distance - travelled));
			}
		}
		return { 0,0 };
	}

	vector<Point2D> stepPointsAlongPath(vector<Point2D> path, float distance) {
		if (path.size() == 0) return vector<Point2D>();
		int siz = int(400 / distance);
		vector<Point2D> midpath;
		for (int i = 1; i < siz; i++) {
			Point2D ne = distanceAlongPath(path, distance * i);
			
			if (ne != Point2D{0, 0}) {
				midpath.push_back(ne);
			}
			else {
				break;
			}
		}
		return midpath;
	}
}