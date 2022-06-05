#pragma once
#include <iostream>
class Mouse
{

public:
	Mouse() { current_row = current_col = 0; }
	Mouse(int x, int y) { current_row = x; current_col = y; }

	int Get_Row() { return current_row;  }
	int Get_Col() { return current_col;  }
	void Set_Row(int x) { current_row = x; }
	void Set_Col(int y) { current_col = y; }

	void Input_Entry_Point()
	{
		std::cout << "\nEnter Entry Row & Column(Row Column): ";
		std::cin >> current_row >> current_col;
	}

private:
	int current_row;
	int current_col;
};
