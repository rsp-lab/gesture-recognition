#ifndef CONFIG
#define CONFIG

class Configuration {

public:

	 int HAAR_SINGLE_WIDTH;
	 int HAAR_WIDTH;// szerokosc
	 int HAAR_HEIGHT;// wysokosc
	 int HAAR_STEP_WIDTH;// co ile przesuwac filtr po obrazku co 5 pikseli
	 int HAAR_STEP_HEIGHT;

	 int similarPosStraight;
	 int similarPosLeft;
	 int similarPosRight;

	 int movementToleranceHorizontal;
	 int movementToleranceVertical;

	 int gestureHorizontal;
	 int gestureVertical;

	Configuration();
	~Configuration();

	 void set(int hsinglew, int hh, int hstepw, int hsh, int sps, int spl, int spr, int mth, int mtv, int gh, int gv);
	 int getHaarSingleWidth();
	 int getHaarWidth();
	 int getHaarHeight();

	 int getHaarStepWidth();

	 int getHaarStepHeight();

	 int getSimilarPosStraight();

	 int getSimilarPosLeft();

	 int getSimilarPosRight();

	 int getMoveToleranceVertical();

	 int getMoveToleranceHorizontal();

	 int getGestureHorizontal();

	 int getGestureVertical();

};

#endif