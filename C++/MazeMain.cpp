// Author: Steven Marigliano //
// Date : 5/15/2017         //
// Project: Maze Final     //



// Maze.cpp : Defines the entry point for the console application.

#include <iostream>
#include "Maze.h"
#include "Solver.h"

using namespace std;

int main()
{
	int input = 1;

	Mouse mouse;
	Maze maze;
	Solver solver;

	while (input) {
		try
		{
			cout <<"\nMaze Traversal" << endl <<"Use following commands to operate" << endl;
			cout <<"0 - End Program" << endl 
				 <<"1 - Load Maze" << endl 
				 <<"2 - Enter Entry Point" << endl
				 <<"3 - Display Loaded Maze" << endl;

			cout << "Enter Command(0-3): ";
			cin >> input;

			if (input == 1)
			{
				maze.Input_Maze_Path();
				maze.Read_Maze();
				maze.Display_Maze();
			}
			else if (input == 2) {
				if (maze.Is_Maze_Loaded())
				{
					mouse.Input_Entry_Point();
					solver.Solve_Maze_For_Mouse(maze, mouse);
				}
				else
				{
					cout << "\nMaze Not Found." << endl;
				}
			}
			else if (input == 3)
			{
				maze.Display_Maze();
			}
			else if (input != 0)
			{
				cout << "Unknown Maze Command" << endl;
			}
		}
		catch (std::exception& exp) {
			cout << "Error: " << exp.what() << endl;
		}
	}

    return 0;
}

