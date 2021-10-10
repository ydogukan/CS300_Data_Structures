#include <iostream>
#include <vector>
#include "DynamicStack.h"
#include <random>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Dogukan Yildirim

struct Cell { // Struct to represent the maze as a collection of cells
	int x, y, l, r, u, d;
	bool visited;

	Cell(int xPos, int yPos) : x(xPos), y(yPos), l(1), r(1), u(1), d(1), visited(false)
	{}

	Cell(int xPos, int yPos, int lWall, int rWall, int uWall, int dWall) 
		: x(xPos), y(yPos), l(lWall), r(rWall), u(uWall), d(dWall), visited(false)
	{}
};

int random_range(const int& min, const int& max) { // Returns a random integer within a given range
	static mt19937 generator(time(0));
	uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

vector<vector<Cell>> createMatrixOfCells(int M, int N) { // Creates a 'default' maze matrix with unvisited cells that have walls between them.
	vector<vector<Cell>> maze;
	for (int i = 0; i < N; i++) {
		vector<Cell> column;
		for (int j = 0; j < M; j++) {
			column.push_back(Cell(i, j));
		}
		maze.push_back(column);
	}
	return maze;
}

/*
*	Input:	An MxN maze matrix, a pointer to the current cell, a reference variable named direction to store the direction
*			for later, and an integer variable named target which can be 0 or 1.
*	Output: A pointer to an unvisited neighbour cell of the current cell. Returns NULL if such neighbour does not exist.
*
*	target variable determines whether the function will target cells with a wall between them or not.
*	if target = 1, then the function will target neighbour cells that have a wall between them and the current cell.
*/
Cell* getUnvisitedNeighbour(vector<vector<Cell>>& maze, Cell*& currentCell, char& direction, int target = 1) {
	/*
	*	How this function works:
	*	Try all four directions RANDOMLY until no direction left to try. IF the next (neighbour) cell with the given direction
	*	meets the target (i.e. if we are looking for a wall to break and there exists such a wall between the cells) AND the
	*	next cell is not out of the range of the maze matrix AND the next cell is unvisited, then return a pointer to that
	*	neighbour cell. Else, remove that direction from the directions vector and try with the remaining directions randomly.
	*	If all directions are exhausted, which would mean there is no unvisited neighbour cell with the given target, return NULL.
	*/

	vector<char> directions{ 'l', 'r', 'u', 'd' };

	while (directions.size() != 0) {
		int randomNumber = random_range(0, directions.size() - 1), changePositionX = 0, changePositionY = 0;
		direction = directions[randomNumber];

		if (direction == 'l' && currentCell->l == target) {
			changePositionX = -1;
		}

		else if (direction == 'r' && currentCell->r == target) {
			changePositionX = 1;
		}

		else if (direction == 'u' && currentCell->u == target) {
			changePositionY = 1;
		}

		else if (direction == 'd' && currentCell->d == target) {
			changePositionY = -1;
		}

		directions.erase(directions.begin() + randomNumber);

		if ((changePositionX != 0 || changePositionY != 0) &&
			currentCell->x + changePositionX >= 0 && currentCell->x + changePositionX < maze.size() &&
			currentCell->y + changePositionY >= 0 && currentCell->y + changePositionY < maze[0].size() &&
			maze[currentCell->x + changePositionX][currentCell->y + changePositionY].visited == false) {

			return &maze[currentCell->x + changePositionX][currentCell->y + changePositionY];
		}
	}

	return NULL;
}

void breakWallBetweenCells(Cell*& currentCell, char direction, Cell*& unvisitedNeighbourCell) {
	/*
	*	How this function works:
	*	Given the current cell, the direction, and the neighbour cell obtained by going through that direction, sets the walls
	*	between those two cells to 0, in other words, breaks the wall between them.
	*/

	if (direction == 'l') {
		currentCell->l = 0;
		unvisitedNeighbourCell->r = 0;
	}

	else if (direction == 'r') {
		currentCell->r = 0;
		unvisitedNeighbourCell->l = 0;
	}

	else if (direction == 'u') {
		currentCell->u = 0;
		unvisitedNeighbourCell->d = 0;
	}

	else {
		currentCell->d = 0;
		unvisitedNeighbourCell->u = 0;
	}
}

vector<vector<Cell>> generateMaze(int M, int N) { // Generates and returns a maze with M rows, N columns.
	vector<vector<Cell>> maze = createMatrixOfCells(M, N); // Creating a maze matrix
	Stack<Cell*> generatorStack; // Starting with an empty stack.
	maze[0][0].visited = true; // The initial cell is visited and pushed to the stack.
	generatorStack.push(&maze[0][0]);

	while (!generatorStack.isEmpty()) {
		char direction;
		Cell* currentCell = generatorStack.top(), * newCell = getUnvisitedNeighbour(maze, currentCell, direction); // Note here that the target is 1.

		if (newCell != NULL) { // If there exists an unvisited neighbour cell
			breakWallBetweenCells(currentCell, direction, newCell); // Break the wall between them.
			newCell->visited = true; // Make the new cell visited, then push it to the stack.
			generatorStack.push(newCell);
		}

		else {
			generatorStack.pop(); // Backtrack.
		}
	}

	return maze;
}

void writeMaze(const vector<vector<Cell>>& maze, int M, int N, int ID) { // Writes a given maze to a .txt file.
	string outputFileName = "maze_" + to_string(ID) + ".txt";
	ofstream output;
	output.open(outputFileName.c_str());

	if (output.fail()) {
		cout << "ERROR: File named " << outputFileName << " cannot be opened." << endl;
	}

	else {
		output << M << " " << N << endl;
		for (int y = 0; y < M; y++) {
			for (int x = 0; x < N; x++) {
				output << "x=" << maze[x][y].x << " y=" << maze[x][y].y << " l=" << maze[x][y].l <<
					" r=" << maze[x][y].r << " u=" << maze[x][y].u << " d=" << maze[x][y].d << endl;
			}
		}
	}

	output.close();
}

vector<vector<Cell>> readMazeFromFile(int M, int N, int mazeID) { // Reads the maze from the file that has the given mazeID
	string inputFileName = "maze_" + to_string(mazeID) + ".txt";
	ifstream input;
	input.open(inputFileName.c_str());

	if (input.fail()) {
		cout << "ERROR: File named " << inputFileName << " cannot be opened." << endl;
	}

	else {
		vector<vector<Cell>> maze = createMatrixOfCells(M, N); // Creating a maze matrix
		string line;
		getline(input, line); // Skipping the line where we have M and N, as we already have them.

		while (getline(input, line)) {
			istringstream properties(line);
			string temp;
			vector<int> elements;

			while (properties >> temp) {
				elements.push_back(stoi(temp.substr(temp.find("=") + 1)));
			}
			// elements[0] => x, elements[1] => y, elements[2] => l, elements[3] => r, elements[4] => u, elements[5] => d
			maze[elements[0]][elements[1]] = Cell(elements[0], elements[1], elements[2], elements[3], elements[4], elements[5]);
		}

		input.close();
		return maze;
	}
}

template <class type>
void reverseStack(Stack<type> & currentStack) { // Reverses a given stack.
	Stack<type> reversedStack;
	while (!currentStack.isEmpty()) {
		reversedStack.push(currentStack.topAndPop());
	}
	currentStack = reversedStack;
}

/*
*	Input:	An MxN maze matrix and the coordinates of the entry and exit points.
*	Output: A stack containing a path (i.e. all the points that must be traversed) from the entry point
*           to the exit point.
*/
Stack<Cell*> findPath(vector<vector<Cell>>& maze, int entryX, int entryY, int exitX, int exitY) {
	Stack<Cell*> pathStack; // Starting with an empty stack.
	maze[entryX][entryY].visited = true; // The entry cell is visited and pushed to the stack.
	pathStack.push(&maze[entryX][entryY]);

	while (!pathStack.isEmpty()) {
		char direction;
		Cell* currentCell = pathStack.top(), * newCell = getUnvisitedNeighbour(maze, currentCell, direction, 0); // Note here that the target is 0.

		if (newCell != NULL) { // If there exists an unvisited neighbour cell
			newCell->visited = true; // Make the new cell visited, then push it to the stack.
			pathStack.push(newCell);

			if (newCell == &maze[exitX][exitY]) { // If the neighbour cell is the exit cell
				reverseStack(pathStack);
				return pathStack; // Return the stack that contains the path from the entry point to the exit point.
			}
		}

		else {
			pathStack.pop(); // Backtrack.
		}
	}
}

void writePath(Stack<Cell*> pathStack, int ID, int entryX, int entryY, int exitX, int exitY) { // Writes a given path to a maze from an entry point to the exit point to a .txt file.
	string outputFileName = "maze_" + to_string(ID) + "_path_" + to_string(entryX) + "_"
		+ to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
	ofstream output;
	output.open(outputFileName.c_str());

	if (output.fail()) {
		cout << "ERROR: File named " << outputFileName << " cannot be opened." << endl;
	}

	else {
		while (!pathStack.isEmpty()) {
			Cell* pathCell = pathStack.topAndPop();
			output << pathCell->x << " " << pathCell->y << endl;
		}
	}

	output.close();
}

int main() {
	int K, M, N;
	cout << "Enter the number of mazes: ";
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> M >> N;

	for (int i = 0; i < K; i++) {
		vector<vector<Cell>> maze = generateMaze(M, N);
		writeMaze(maze, M, N, i + 1);
	}
	cout << "All mazes are generated." << endl << endl;

	int mazeID, entryX, entryY, exitX, exitY;
	cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
	cin >> mazeID;
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryX >> entryY;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitX >> exitY;
	
	vector<vector<Cell>> maze = readMazeFromFile(M, N, mazeID);
	Stack<Cell*> path = findPath(maze, entryX, entryY, exitX, exitY);
	writePath(path, mazeID, entryX, entryY, exitX, exitY);

	return 0;
}