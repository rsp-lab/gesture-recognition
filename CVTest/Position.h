#ifndef POSITION_H
#define POSITION_H

/** klasa do przechowywania pozycji punktow potrzebnychd o wyznaczenia obszaru */
class Position {

public:

	int xTopLeft;
	int yTopLeft;

	int xBottomRight;
	int yBottomRight;

	Position();
	Position(int xTopLeftValue, int yTopLeftValue, int xBottomRightValue, int yBottomRightValue);

};

#endif