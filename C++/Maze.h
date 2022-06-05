#pragma once
#include <string>
#include <stack>

using namespace std;

#include "Cell.h"

class Maze
{
public:
	Maze();
	~Maze();
	void Input_Maze_Path();
	void Read_Maze();
	void Display_Maze();
	bool Is_Maze_Loaded() { return maze_loaded; }
	int rows() { return m_rows; }
	int columns () { return m_columns;  }
	bool Check_Entrance(int x, int y);
	bool Check_Open(int x, int y);
	void Evaluate_Cell(int, int, int, int, int, int);
	int value(int row, int col);
	Intersection_Direction_Traversed direction(int, int);
	void Set_Direction(int, int, Intersection_Direction_Traversed);
	Cell Get_Cell(int, int);
	void Clear_Evaluations();
	void Clear_Allocations();
	Cell_Type Get_Cell_Type(int row, int col) { return m_maze[row][col].type; }

private:
	bool maze_loaded;
	int m_rows;
	int m_columns;
	//dynamic double array for maze
	Cell ** m_maze;
	string m_maze_path;
};
