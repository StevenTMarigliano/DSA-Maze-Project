#pragma once
enum class Cell_Type { Unknown, Entrance, Passage, Intersection, DeadEnd, Exit };
enum class Intersection_Direction_Traversed { NotTraversed, Left, Center, Right };

class Cell
{
public:
	int row;
	int col;
	int value;
	Cell_Type type;
	Intersection_Direction_Traversed direction;
};
