#include<functional>
#include<map>
#include<vector>
#include<queue>
#include<iostream>

#define BOARD_SIZE 3

// predefined types used in the context of this project
struct Comparison;
typedef std::vector<int> intVec;
typedef std::vector<intVec> node;
typedef std::map<node, bool> map_node_bool;
typedef std::map<node, node> map_node_node;
typedef std::vector<node> vector3D;
typedef std::pair<int, int> coords;
typedef std::pair<node, int> state;
typedef std::vector<state> allStates;
typedef std::priority_queue<state, std::vector<state>, Comparison > minPriorityQueue;

// here I store the visited nodes
map_node_bool visited;

// here are stored the parents of each node,used for printing
// the path from the initial node to the final node
map_node_node parent;

// this is the final node
node goalState(BOARD_SIZE, std::vector<int>(BOARD_SIZE));

// checks whether a node has been visited
bool isVisited(const node& toBeChecked)
{
	return visited[toBeChecked];
}

// claculates the Manhattan distance between
// the current node and the final node
int manhattanDist(node& node, int& moves)
{
	int dist = moves;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (node[i][j] != 0)
			{
				for (int k = 0; k < BOARD_SIZE; k++)
				{
					for (int l = 0; l < BOARD_SIZE; l++)
					{
						if (node[i][j] == goalState[k][l])
							dist += std::abs(i - k) + std::abs(j - l);
					}
				}
			}
		}
	}
	return dist;
}

// a functor used for comparing the Manhattan
// distances of two nodes
struct Comparison
{
	bool operator() (state &lhs, state &rhs)
	{
		int leftDist = manhattanDist(lhs.first, lhs.second);
		int rightDist = manhattanDist(rhs.first, rhs.second);
		return leftDist > rightDist;
	}
};

// checks whether moving the cell
// is possible
bool isMoveSafe(int i, int j)
{
	return (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE);
}

// finds the coords of the empty cell
void findEmptyCell(coords& coords, const node& node)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (node[i][j] == 0)
			{
				coords.first = i;
				coords.second = j;
				break;
			}
		}
	}
}

// arrays used for moving X and Y axis
int dx[] = { -1,+1,0,0 };
int dy[] = { 0,0,-1,+1 };

// finds the children of the current node
vector3D children(const node& root)
{
	coords empty;
	findEmptyCell(empty, root);
	vector3D result;

	for (int k = 0; k < 4; k++)
	{
		coords newCoords(empty.first + dx[k], empty.second + dy[k]);
		node newNode = root;
		if (isMoveSafe(newCoords.first, newCoords.second))
		{
			std::swap(newNode[newCoords.first][newCoords.second], newNode[empty.first][empty.second]);
			result.push_back(newNode);
		}
	}
	return result;
}

// prints node
void printNode(const node& node)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			std::cout << node[i][j];
		}
		std::cout << std::endl;
	}
}

// prints the path from the initial node
// to the final node
void printPath(const node& start)
{
	if (parent.count(start))
		printPath(parent[start]);
	std::cout << std::endl;
	printNode(start);
}

// finds solution to the puzzle
// using priority queue
void solvePuzzle(const node& n, int moves)
{

	minPriorityQueue Q;
	Q.push(state(n, moves));
	while (!Q.empty())
	{
		node temp = Q.top().first;
		Q.pop();

		visited[temp] = true;
		if (temp == goalState)
		{
			printPath(temp);
			break;
		}
		vector3D newChilds = children(temp);
		vector3D::iterator it;
		for (it = newChilds.begin(); it != newChilds.end(); it++)
		{
			node nd = *it;
			if (!isVisited(nd))
			{
				parent.insert(std::pair<node, node>(nd, temp));
				Q.push(state(nd, moves + 1));
			}
		}
	}
}

// used for getting an input from the user
void getInitialState(node& start)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			std::cin >> start[i][j];
		}
	}
}

// hardcoded final node
void setGoalState()
{
	goalState[0][0] = 1;
	goalState[0][1] = 2;
	goalState[0][2] = 3;
	goalState[1][0] = 4;
	goalState[1][1] = 5;
	goalState[1][2] = 6;
	goalState[2][0] = 7;
	goalState[2][1] = 8;
	goalState[2][2] = 0;
}

int main()
{
	node start(BOARD_SIZE, intVec(BOARD_SIZE));
	getInitialState(start);
	setGoalState();

	std::cout << "Solution...\n";

	solvePuzzle(start, 0);

	return 0;
}