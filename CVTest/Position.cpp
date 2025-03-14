#include "Position.h"

Position::Position() {

}

Position::Position(int xTopLeftValue, int yTopLeftValue, int xBottomRightValue, int yBottomRightValue) {
	xTopLeft = xTopLeftValue;
	yTopLeft = yTopLeftValue;
	xBottomRight = xBottomRightValue;
	yBottomRight = yBottomRightValue;
}

///** klasa do przechowywania pozycji punktow potrzebnychd o wyznaczenia obszaru */
//class Position {
//
//public:
//
//	int xTopLeft;
//	int yTopLeft;
//
//	int xBottomRight;
//	int yBottomRight;
//
//	Position() {
//
//	}
//
//	Position(int xTopLeftValue, int yTopLeftValue, int xBottomRightValue, int yBottomRightValue) {
//		xTopLeft = xTopLeftValue;
//		yTopLeft = yTopLeftValue;
//		xBottomRight = xBottomRightValue;
//		yBottomRight = yBottomRightValue;
//	}
//
//};