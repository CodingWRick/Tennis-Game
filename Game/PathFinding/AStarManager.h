#pragma once
#include <windows.h>
#include <cfloat>
#include <vector>
#include <algorithm>
#include "../../Engine/base.h"
#include "node.h"

using namespace std;

extern vector<Node*> nodes;
extern float step;
extern float nodeStep;

vector<Node*> GeneratePath(Node* startNode, Node* targetNode);
Node* FindClosestNode(float x, float y);
Node* FindFurthestNode(float x, float y);
void AssignNeighbors(Node* currentNode, Node* targetNode, vector<Node*>& openSet);
int GetLowestF(const vector<Node*>& openSet);