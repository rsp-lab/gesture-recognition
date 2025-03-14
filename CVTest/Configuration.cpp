#include "Configuration.h"


	
	Configuration::Configuration()
	{
		HAAR_SINGLE_WIDTH = 10; 
		HAAR_WIDTH = 3 * HAAR_SINGLE_WIDTH; // szerokosc
		HAAR_HEIGHT = 15; // wysokosc
		HAAR_STEP_WIDTH = 5; // co ile przesuwac filtr po obrazku co 5 pikseli
		HAAR_STEP_HEIGHT = 10;
		
		similarPosStraight = 5;
		similarPosLeft = 4;
		similarPosRight = 4;

		movementToleranceHorizontal = 5;
		movementToleranceVertical = 5;

		gestureHorizontal = 5;
		gestureVertical = 5;
	}

	Configuration::~Configuration() {}

	void Configuration::set(int hsinglew, int hh, int hstepw, int hsh, int sps, int spl, int spr, int mth, int mtv, int gh, int gv)
	{
		HAAR_SINGLE_WIDTH = hsinglew; 
		HAAR_WIDTH = 3 * hsinglew; // szerokosc
		HAAR_HEIGHT = hh; // wysokosc
		HAAR_STEP_WIDTH = hstepw; // co ile przesuwac filtr po obrazku co 5 pikseli
		HAAR_STEP_HEIGHT = hsh;
		
		similarPosStraight = sps;
		similarPosLeft = spl;
		similarPosRight = spr;

		movementToleranceHorizontal = mth;
		movementToleranceVertical = mtv;

		gestureHorizontal = gh;
		gestureVertical = gv;
	}

	 int Configuration:: getHaarSingleWidth()
	{
		return HAAR_SINGLE_WIDTH;
	}

	 int Configuration::getHaarWidth()
	{
		return HAAR_WIDTH;
	}

	 int Configuration::getHaarHeight()
	{
		return HAAR_HEIGHT;
	}

	 int Configuration::getHaarStepWidth()
	{
		return HAAR_STEP_WIDTH;
	}

	 int Configuration::getHaarStepHeight()
	{
		return HAAR_STEP_HEIGHT;
	}

	 int Configuration::getSimilarPosStraight()
	{
		return similarPosStraight;
	}

	 int Configuration::getSimilarPosLeft()
	{
		return similarPosLeft;
	}

	 int Configuration::getSimilarPosRight()
	{
		return similarPosRight;
	}

	 int Configuration::getMoveToleranceVertical()
	{
		return movementToleranceVertical;
	}

	 int Configuration::getMoveToleranceHorizontal()
	{
		return movementToleranceHorizontal;
	}

	 int Configuration::getGestureHorizontal()
	{
		return gestureHorizontal;
	}

	 int Configuration::getGestureVertical()
	{
		return gestureVertical;
	}
