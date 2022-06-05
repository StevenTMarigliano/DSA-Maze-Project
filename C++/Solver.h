#pragma once
#include <stack>
#include "Mouse.h"
#include "Maze.h"

class Solver
{
public:
	Solver();
	void Solve_Maze_For_Mouse(Maze& maze, Mouse& mouse);

private:
	stack<Cell> m_intersections;
	//stack to store path
	stack<Cell> m_path;
	//dynamic doble array to mark path in maze
	int** m_path_map;

	int entry_row;
	int entry_column;

	bool Find_Passage_Next_Cell(Maze&, int &, int &, int &, int &);
	bool Go_Back_To_Last_Intersection(int &, int &, int &, int&);
	bool Intersection_Move_Forward(Maze&, int&, int &, int &, int&);
	bool Move_Left(Maze&, int&, int&, int&, int&);
	bool Move_Center(Maze&, int&, int&, int&, int&);
	bool Move_Right(Maze&, int&, int&, int&, int&);
	void Record_Path();
	void Display_Path(Maze&);
};
