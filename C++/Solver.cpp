#include <iostream>
#include "Solver.h"

Solver::Solver()
{
	m_path_map = NULL;
}

//record path in stack as a two dimentional array
void Solver::Record_Path()
{
	while (m_path.empty() == false) {
		Cell cell = m_path.top();
		m_path_map[cell.row][cell.col] = 2;
		m_path.pop();
	}
}

void Solver::Display_Path(Maze& maze)
{
	cout << "\n\n";
	for (int i = 0; i < maze.rows(); ++i) {
		for (int k = 0; k < maze.columns(); ++k) {
			if (m_path_map[i][k] == 2) {
				cout << ' ' << ' ';
			}
			else
			{
				cout << maze.value(i, k) << ' ';
			}
		}

		cout << endl;
	}
	cout << endl;
}

/*
Start from the entrance point and follow the path on passage points....if some intersection
is found then traverse each path of the intersection until reached at exit....if one path of
intersection is found dead then try other remaining paths....following this appraoch itertively
until an exit is found or all options are exhausted...
*/

void Solver::Solve_Maze_For_Mouse(Maze& maze, Mouse& mouse)
{
	if (mouse.Get_Row() >= maze.rows() || mouse.Get_Col() >= maze.columns()) {
		cout << "\nEntry Point Out of Bounds." << endl
			<< "Maze Dimensions: RowxColumn: " << maze.rows() << 'x' << maze.columns() << endl;
		cout << "Enter Row Number Less Than: " << maze.rows() << " and Column Number Less Than: " << maze.columns() << endl;
		return;
	}

	entry_row = mouse.Get_Row();
	entry_column = mouse.Get_Col();

	if (!maze.Check_Entrance(entry_row, entry_column)) {
		cout << "\nCannot Enter Here... Must Enter on the Outside..." << endl
			<< "Maze Dimensions: RowxColumn: " << maze.rows() << 'x' << maze.columns() << endl;
		return;
	}

	if (!maze.Check_Open(entry_row, entry_column)) {
		cout << "\nCannot Enter Through a Wall... Enter an Open Cell as Entry Point." << endl;
		return;
	}

	m_path_map = new int*[maze.rows()];
	for (int i = 0; i < maze.rows(); ++i) {
		m_path_map[i] = new int[maze.columns()];
	}
	for (int i = 0; i < maze.rows(); ++i) {
		for (int k = 0; k < maze.columns(); ++k) {
			m_path_map[i][k] = maze.value(i, k);
		}
	}

	//m_maze[entry_row][entry_column].type = Cell_Type::Entrance;

	int previous_row = entry_row;
	int previous_col = entry_column;

	int current_row = entry_row;
	int current_col = entry_column;

	bool keep_traversing = true;
	while (keep_traversing) {

		//m_path.push(m_maze[current_row][current_col]);
		maze.Evaluate_Cell(previous_row, previous_col, current_row, current_col, entry_row, entry_column);

		if (maze.Get_Cell_Type(current_row, current_col) == Cell_Type::Exit) {
			cout << "\nExit found at following row col:" << current_row << ' ' << current_col << endl << endl;
			m_path.push(maze.Get_Cell(current_row, current_col));
			Record_Path();
			Display_Path(maze);
			//print the path here....
			keep_traversing = false;
		}

		if (maze.Get_Cell_Type(current_row, current_col) == Cell_Type::DeadEnd) {
			//go back to last intersection
			if (Go_Back_To_Last_Intersection(previous_row, previous_col, current_row, current_col) == false)
			{
				cout << "\nNo Exit For This Entry Point. " << endl;
				keep_traversing = false;
			}
		}
		else if (maze.Get_Cell_Type(current_row, current_col) == Cell_Type::Passage) {
			m_path.push(maze.Get_Cell(current_row, current_col));
			if (Find_Passage_Next_Cell(maze, previous_row, previous_col, current_row, current_col) == false)
			{
				keep_traversing = false;
				cout << "\nNo Exit For This Entry Point." << endl;
			}
		}
		else if (maze.Get_Cell_Type(current_row, current_col) == Cell_Type::Intersection) {
			if (Intersection_Move_Forward(maze, previous_row, previous_col, current_row, current_col) == false)
			{
				keep_traversing = false;
				cout << "\nNo Exit For This Entry Point." << endl;
			}
		}
		else if (current_row == entry_row && current_col == entry_column) { //check for loop cases...
																			//go back to last intersection
			if (Go_Back_To_Last_Intersection(previous_row, previous_col, current_row, current_col) == false)
			{
				cout << "\nMaze Has No Solution." << endl;
				keep_traversing = false;
			}
		}
	}

	//clear path and intersection data for new run
	maze.Clear_Evaluations();

	if (m_path_map != NULL) {
		for (int i = 0; i < maze.rows(); ++i) {
			delete[] m_path_map[i];
		}

		delete[] m_path_map;
		m_path_map = NULL;
	}
}

//in case of a dead end we need to go back to last intersection
bool Solver::Go_Back_To_Last_Intersection(int& previous_row, int& previous_col, int& current_row, int& current_col)
{
	bool keep_traversing = true;

	if (m_intersections.empty()) {
		keep_traversing = false;
	}
	else
	{
		//go back to last decision cell
		Cell last_intersection = m_intersections.top();
		current_row = last_intersection.row;
		current_col = last_intersection.col;

		//flush path and reach to last decision cell in path
		bool flush = true;
		while (flush && m_path.empty() == false) {
			Cell last_path_cell = m_path.top();
			if (last_path_cell.row != last_intersection.row || last_path_cell.col != last_intersection.col) {
				m_path.pop();
			}
			else {
				flush = false;
				last_path_cell = m_path.top();
				m_path.pop();
				if (m_path.empty() == false) {
					previous_row = m_path.top().row;
					previous_col = m_path.top().col;
				}
				else
				{
					previous_row = entry_row;
					previous_col = entry_column;
				}
				m_path.push(last_path_cell);
			}
		}
	}

	return keep_traversing;
}

//for a passage cell find the next cell to move
bool Solver::Find_Passage_Next_Cell(Maze& maze, int& previous_row, int& previous_col, int& current_row, int& current_col)
{
	bool retVal = false;

	if (((current_row + 1) < maze.rows()) && ((current_row + 1) != previous_row) &&
		(maze.value(current_row + 1, current_col) == 0))
	{
		previous_row = current_row;
		previous_col = current_col;

		current_row = current_row + 1;
		retVal = true;
	}
	else if ((current_row - 1 >= 0) && ((current_row - 1) != previous_row) &&
		(maze.value(current_row - 1, current_col) == 0))
	{
		previous_row = current_row;
		previous_col = current_col;

		current_row = current_row - 1;
		retVal = true;
	}
	else if (((current_col + 1) < maze.columns()) && ((current_col + 1) != previous_col) &&
		(maze.value(current_row, current_col + 1) == 0))
	{
		previous_row = current_row;
		previous_col = current_col;

		current_col = current_col + 1;
		retVal = true;
	}
	else if ((current_col - 1 >= 0) && ((current_col - 1) != previous_col) &&
		(maze.value(current_row, current_col - 1) == 0))
	{
		previous_row = current_row;
		previous_col = current_col;

		current_col = current_col - 1;
		retVal = true;
	}

	else
	{
		//something wrong
		retVal = false;
	}

	return retVal;
}

//at intersection make a decision to move left, forward or right.....cannot go back because from there we came
bool Solver::Intersection_Move_Forward(Maze& maze, int& previous_row, int& previous_col, int& current_row, int& current_col)
{
	bool keep_traversing = true;
	//if first entry to this intersection....
	if (maze.direction(current_row, current_col) == Intersection_Direction_Traversed::NotTraversed) {

		//push path
		m_path.push(maze.Get_Cell(current_row, current_col));
		m_intersections.push(maze.Get_Cell(current_row, current_col));

		if (Move_Left(maze, previous_row, previous_col, current_row, current_col) == true)
		{
			maze.Set_Direction(previous_row, previous_col, Intersection_Direction_Traversed::Left);
		}
		else if (Move_Center(maze, previous_row, previous_col, current_row, current_col) == true)
		{
			maze.Set_Direction(previous_row, previous_col, Intersection_Direction_Traversed::Center);
		}
		else if (Move_Right(maze, previous_row, previous_col, current_row, current_col) == true)
		{
			maze.Set_Direction(previous_row, previous_col, Intersection_Direction_Traversed::Right);
		}
		else
		{
			maze.Set_Direction(current_row, current_col, Intersection_Direction_Traversed::Right);
		}
	}//if left side has been traversed then move center or right
	else if (maze.direction(current_row, current_col) == Intersection_Direction_Traversed::Left) {
		if (Move_Center(maze, previous_row, previous_col, current_row, current_col) == true)
		{
			maze.Set_Direction(previous_row, previous_col, Intersection_Direction_Traversed::Center);
		}
		else if (Move_Right(maze, previous_row, previous_col, current_row, current_col) == true)
		{
			maze.Set_Direction(previous_row, previous_col, Intersection_Direction_Traversed::Right);
		}
		else
		{
			maze.Set_Direction(current_row, current_col, Intersection_Direction_Traversed::Right);
		}
	}//if left, center have been traversed then move right
	else if (maze.direction(current_row, current_col) == Intersection_Direction_Traversed::Center) {
		if (Move_Right(maze, previous_row, previous_col, current_row, current_col) == true)
		{
			maze.Set_Direction(previous_row, previous_col, Intersection_Direction_Traversed::Right);
		}
		else
		{
			maze.Set_Direction(current_row, current_col, Intersection_Direction_Traversed::Right);
		}
	}//in case we do not find any exit ahead of this intersection then go back to last intersection
	else if (maze.direction(current_row, current_col) == Intersection_Direction_Traversed::Right) {
		//all directions have been traveresed and failed. Pop this intersection and go back to 
		//intersection before this intersection
		m_intersections.pop();

		//go back to intersection before this intersection...in case we do not have any intersection left
		//there is no exit so stop
		if (Go_Back_To_Last_Intersection(previous_row, previous_col, current_row, current_col) == false)
		{
			keep_traversing = false;
		}
	}

	return keep_traversing;
}
//move left from intersection
bool Solver::Move_Left(Maze& maze, int& previous_row, int& previous_col, int& current_row, int& current_col)
{
	bool retVal = false;

	if (previous_row == current_row) {
		if (previous_col < current_col) {
			if ((current_row - 1 >= 0) && (maze.value(current_row - 1, current_col) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_row = current_row - 1;
				retVal = true;
			}
		}
		else {
			if ((current_row + 1 < maze.rows()) && (maze.value(current_row + 1, current_col) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_row = current_row + 1;
				retVal = true;
			}
		}
	}
	else if (previous_col == current_col) {
		if (previous_row < current_row) {
			if ((current_col + 1 < maze.columns()) && (maze.value(current_row, current_col + 1) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_col = current_col + 1;
				retVal = true;
			}
		}
		else {
			if ((current_col - 1 >= 0) && (maze.value(current_row, current_col - 1) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_col = current_col - 1;
				retVal = true;
			}
		}
	}

	return retVal;
}
//move forward from intersection
bool Solver::Move_Center(Maze& maze, int& previous_row, int& previous_col, int& current_row, int& current_col)
{
	bool retVal = false;

	if (previous_row == current_row) {
		if (previous_col < current_col) {
			if ((current_col + 1 < maze.columns()) && (maze.value(current_row, current_col + 1) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_col = current_col + 1;
				retVal = true;
			}

		}
		else {
			if ((current_col - 1 >= 0) && (maze.value(current_row, current_col - 1) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_col = current_col - 1;
				retVal = true;
			}
		}
	}
	else if (previous_col == current_col) {
		if (previous_row < current_row) {
			if ((current_row + 1 < maze.rows()) && (maze.value(current_row + 1, current_col) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_row = current_row + 1;
				retVal = true;
			}
		}
		else {
			if ((current_row - 1 >= 0) && (maze.value(current_row - 1, current_col) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_row = current_row - 1;
				retVal = true;
			}
		}
	}

	return retVal;
}
//move right from intersection
bool Solver::Move_Right(Maze& maze, int& previous_row, int& previous_col, int& current_row, int& current_col)
{
	bool retVal = false;

	if (previous_row == current_row) {
		if (previous_col < current_col) {
			if ((current_row + 1 < maze.rows()) && (maze.value(current_row + 1, current_col) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_row = current_row + 1;
				retVal = true;
			}

		}
		else {
			if ((current_row - 1 >= 0) && (maze.value(current_row - 1, current_col) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_row = current_row - 1;
				retVal = true;
			}
		}
	}
	else if (previous_col == current_col) {
		if (previous_row < current_row) {
			if ((current_col - 1 >= 0) && (maze.value(current_row, current_col - 1) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_col = current_col - 1;
				retVal = true;
			}
		}
		else {
			if ((current_col + 1 < maze.columns()) && (maze.value(current_row, current_col + 1) == 0)) {

				previous_row = current_row;
				previous_col = current_col;
				current_col = current_col + 1;
				retVal = true;
			}
		}
	}

	return retVal;
}
