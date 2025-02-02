#pragma once
#include <cfloat>
#include <sc2api/sc2_api.h>
#include <sc2api/sc2_common.h>

#include <map>
#include "constants.h"
#include "debugging.hpp"

#define MAX_CONN_DIST 50

using namespace sc2;
using namespace std;

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

	class PathNode {
	private:
		Point2D pos;
	public:

		Cardinal wall;
		int id;
		vector<int> connected;

		PathNode(Point2D pos, Cardinal wall, Agent* agent);

		~PathNode();

		Point2D rawPos() {
			return pos;
		}

		Point2D position(float radius) {
			if (wall == INVALID) {
				return pos;
			}
			return pos - normalize(cardinalToDirection(wall)) * radius;
		}
	};

	vector<PathNode*> basePathNodes;
	map2d<int8_t>* blobGrid;

	int maxConnections = 0;
	float maxDistanceConnection = 0.0F;

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

	//void calculateConnection(PathNode* p, Agent* agent) {
	//	static float maxDist = 0;
	//	for (int i = 0; i < basePathNodes.size(); i++) {
	//		if (i == p->id || find(p->connected.begin(), p->connected.end(), i) != p->connected.end()) {
	//			continue;
	//		}
	//		PrimordialStar::PathNode* node = PrimordialStar::basePathNodes[i];
	//		if (checkLinearPath(p->rawPos(), node->rawPos(), agent)) {
	//			p->connected.push_back(i);
	//			node->connected.push_back(p->id);
	//			float d = DistanceSquared2D(node->position(0), p->position(0));
	//		}
	//	}
	//}

	void calculateNewConnection(PathNode* p, Agent* agent) {
		for (int i = 0; i < basePathNodes.size(); i++) {
			if (i == p->id) {
				continue;
			}
			PrimordialStar::PathNode* node = PrimordialStar::basePathNodes[i];
			if (checkLinearPath(p->rawPos(), node->rawPos(), agent)) {
				float dist = Distance2D(p->rawPos(), node->rawPos());
				if (dist > MAX_CONN_DIST) {
					continue;
				}
				if (maxDistanceConnection < dist) {
					maxDistanceConnection = dist;
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
		return up && up_rt && rt;
	}

	bool check_DN_RT(int i, int j) {
		bool rt = Aux::checkPathable(i + 1, j, nullptr);
		bool dn_rt = Aux::checkPathable(i + 1, j - 1, nullptr);
		bool dn = Aux::checkPathable(i, j - 1, nullptr);
		return dn && dn_rt && rt;
	}

	bool check_DN_LT(int i, int j) {
		bool dn = Aux::checkPathable(i, j - 1, nullptr);
		bool dn_lt = Aux::checkPathable(i - 1, j - 1, nullptr);
		bool lt = Aux::checkPathable(i - 1, j, nullptr);
		return dn && dn_lt && lt;
	}

	bool check_UP_LT(int i, int j) {
		bool lt = Aux::checkPathable(i - 1, j, nullptr);
		bool up_lt = Aux::checkPathable(i - 1, j + 1, nullptr);
		bool up = Aux::checkPathable(i, j + 1, nullptr);
		return up && up_lt && lt;
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
		printf("MAX CONNECTIONS OF A NODE: %d\t MAX DISTANCE OF A CONNECTION: %.2f\n", maxConnections, maxDistanceConnection);
	}

	vector<Point2D> getPath(Point2D start, Point2D end, float radius, Agent* agent) {
		PathNode* startNode = new PathNode(start, INVALID, agent);
		
		//calculateNewConnection(startNode, agent);

		PathNode* operatingNode = startNode;
		PathNode* endNode = new PathNode(end, INVALID, agent);
		
		//calculateNewConnection(endNode, agent);

		bool* visited = new bool[basePathNodes.size()];
		memset(visited, 0, basePathNodes.size() * sizeof(bool));

		vector<Point2D> points;
		map<int, int> backpath;

		if (startNode->connected.size() == 0 || endNode->connected.size() == 0) {

		}
		else {
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

			operatingNode = endNode;
			points.push_back(basePathNodes[operatingNode->id]->rawPos());
			for (int i = 0; i < starNodes.size(); i++) {
				operatingNode = basePathNodes[backpath[operatingNode->id]];
				points.insert(points.begin(), basePathNodes[operatingNode->id]->position(radius));
				if (operatingNode->id == startNode->id) {
					break;
				}
			}
		}
		delete[] visited;
		basePathNodes.pop_back();
		basePathNodes.pop_back();

		delete startNode;
		delete endNode;
		
		//printf("SOMETHING WENT WRONG\n");
		return points;
	}

	vector<Point2D> getPathDijkstra(Point2D start, Point2D end, float radius, Agent* agent) {
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

	vector<Point2D> getPathDijkstra2(Point2D start, Point2D end, float radius, Agent* agent) {
		PathNode* startNode = new PathNode(start, INVALID, agent);

		PathNode* endNode = new PathNode(end, INVALID, agent);

		//bool* visited = new bool[basePathNodes.size()];
		//memset(visited, 0, basePathNodes.size() * sizeof(bool));

		vector<Point2D> points;

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
				for (int i = 0; i < operatingNode->connected.size(); i++) {
					PathNode* adjacentNode = basePathNodes[operatingNode->connected[i]];
					float weight = Distance2D(operatingNode->position(radius), adjacentNode->position(radius));

					if (dist[adjacentNode->id] > dist[u.pathNode] + weight) {
						parent[adjacentNode->id] = u.pathNode;
						dist[adjacentNode->id] = dist[u.pathNode] + weight;

						Q.push(DijkStarNode(adjacentNode->id, dist[adjacentNode->id]));
					}
				}
			}

			int node = endNode->id;
			for (int cycles = 0; cycles < 10000; cycles++) {
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