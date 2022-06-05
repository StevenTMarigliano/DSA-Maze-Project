#include "Maze.h"
#include <iostream>
#include <fstream>

using namespace std;

Maze::Maze()
{
	maze_loaded = false;
	m_maze = NULL;
}

Maze::~Maze()
{
	Clear_Allocations();
}

int Maze::value(int row, int col)
{
	if (row < m_rows && col < m_columns)
		return m_maze[row][col].value;

	return -1;
}

Intersection_Direction_Traversed Maze::direction(int row, int col)
{
	if (row < m_rows && col < m_columns)
		return m_maze[row][col].direction;

	return Intersection_Direction_Traversed::NotTraversed;
}

Cell Maze::Get_Cell(int row, int col)
{
	return m_maze[row][col];
}

void Maze::Set_Direction(int row, int col, Intersection_Direction_Traversed dir)
{
	m_maze[row][col].direction = dir;
}

void Maze::Clear_Allocations()
{
	if (m_maze != NULL) {
		for (int i = 0; i < m_rows; ++i) {
			delete[] m_maze[i];
		}

		delete[] m_maze;
		m_maze = NULL;
	}
}

void Maze::Input_Maze_Path()
{
	cout << "           Mazes" << endl;
	cout << "maze1.txt           Dimension: 10x10" << endl;
	cout << "maze2.txt           Dimension: 10x10" << endl;
	cout << "maze3.txt           Dimension: 10x10" << endl;
	cout << "maze4.txt           Dimension: 20x20" << endl;
	cout << "maze5.txt           Dimension: 31x31" << endl;
	cout << "\nEnter a Maze: ";
	cin >> m_maze_path;
}

void Maze::Read_Maze()
{
	try
	{
		//delete previous memory if any allocated
		Clear_Allocations();

		fstream file_input;
		file_input.open(m_maze_path, ios::in);
		file_input >> m_rows >> m_columns;

		m_maze = new Cell*[m_rows];

		for (int i = 0; i < m_rows; ++i) {
			m_maze[i] = new Cell[m_columns];
		}

		int cell_value;
		for (int i = 0; i < m_rows; ++i) {
			for (int k = 0; k < m_columns; ++k) {
				file_input >> cell_value;
				Cell cell;
				cell.row = i;
				cell.col = k;
				cell.value = cell_value;
				cell.type = Cell_Type::Unknown;
				cell.direction = Intersection_Direction_Traversed::NotTraversed;
				m_maze[i][k] = cell;
			}
		}

		maze_loaded = true;
	}
	catch (...)
	{
		cout <<"\nMaze Not Found...Check Maze Path or Verify Maze Format" << endl;
		maze_loaded = false;
	}
}

void Maze::Display_Maze()
{
	if (maze_loaded) {
		cout << "\nMaze Dimensions: " << m_rows << "x"<< m_columns << endl;
		for (int i = 0; i < m_rows; ++i) {
			for (int k = 0; k < m_columns; ++k) {
				cout << m_maze[i][k].value << ' ';
			}

			cout << endl;
		}

		cout << "Row & Column Start At(0,0)" << endl;
	}
	else
	{
		cout << "\nCannot Display... No Maze Loaded..." << endl;
	}
}

//clears all path and intersections data as well as evaluations of cells
void Maze::Clear_Evaluations()
{
	for (int i = 0; i < m_rows; ++i) {
		for (int k = 0; k < m_columns; ++k) {
			m_maze[i][k].type = Cell_Type::Unknown;
			m_maze[i][k].direction = Intersection_Direction_Traversed::NotTraversed;
		}
	}
}

//check if the cell is entrance cell
bool Maze::Check_Entrance(int x, int y)
{
	bool retVal = false;

	if (x == 0 || x == (m_rows - 1) || y == 0 || y == (m_columns - 1) ) {
		retVal = true;
	}

	return retVal;
}
//check if the cell is an open cell
bool Maze::Check_Open(int x, int y)
{
	bool retVal = false;

	if (m_maze[x][y].value == 0)
	{
		retVal = true;
	}

	return retVal;
}

//check if chell is exit, dead, passage, intersection
void Maze::Evaluate_Cell(int previous_row, int previous_col, int current_row, int current_col,
						 int entry_row, int entry_column)
{
	int next_cell_count = 0;
	if ((current_row + 1 < m_rows) && ( (current_row + 1) != previous_row)) {
		if (m_maze[current_row + 1][current_col].value == 0) {
			next_cell_count++;
		}
	}
	if ((current_row - 1 >= 0) && ( (current_row - 1) != previous_row)) {
		if (m_maze[current_row - 1][current_col].value == 0) {
			next_cell_count++;
		}
	}
	if ((current_col + 1 < m_columns) && ( (current_col + 1) != previous_col)) {
		if (m_maze[current_row][current_col + 1].value == 0) {
			next_cell_count++;
		}
	}
	if ((current_col - 1 >= 0) && ((current_col - 1) != previous_col)) {
		if (m_maze[current_row][current_col - 1].value == 0) {
			next_cell_count++;
		}
	}

	if (next_cell_count == 0)
	{
		m_maze[current_row][current_col].type = Cell_Type::DeadEnd;
	}
	else if (next_cell_count == 1) {
		m_maze[current_row][current_col].type = Cell_Type::Passage;
	}
	else {
		m_maze[current_row][current_col].type = Cell_Type::Intersection;
	}

	//check if the cell is an exit....
	if (current_row != entry_row || current_col != entry_column) {
		if (Check_Entrance(current_row, current_col)) {
			m_maze[current_row][current_col].type = Cell_Type::Exit;
		}
	}
}
