#include <cv.h>             
#include <cxcore.h>            
#include <highgui.h>           
#include <stdio.h>             
#include <math.h>

// Liczba punktów charakterystycznych
#define FEATURES_NUM 300		

// Ustawianie linii przesuniêcia
#define COLOR_RED 0				
#define COLOR_GREEN 0
#define COLOR_BLUE 255
#define LINE_WIDTH 1			

const char* FRAME_PREV = "Poprzednia klatka";
const char* FRAME = "Nastêpna klatka";
const char* FRAME_OPT = "Optical Flow";

inline static double square(int value)
{
	return (double) (value * value);
}

/** rusuje znalezione wektory na obrazku */
void drawFlow(char *opticalFlowFoundFeature, int numberOfFeatures, 
				CvPoint2D32f* firstFrameFeatures, CvPoint2D32f* secondFrameFeatures, IplImage *frame)
{
	const double PI = 3.14159265358979323846;
	const double FACTOR = 1;
	int lineThickness = 1;
	CvScalar line_color = CV_RGB(255, 0, 0);

	//rysujemy wszystkie znalezione wlasciwosci
	for(int i = 0; i < numberOfFeatures; i++) {
		//jezeli algorytm "Lucas Kandel Pyramidial" nie znalazl nic dla i-tego elementu przechodzimy dalej
		if(opticalFlowFoundFeature[i] == 0) {
			continue;
		}

		/* The arrows will be a bit too short for a nice visualization because of the high framerate
		 * (ie: there's not much motion between the frames).  So let's lengthen them by a factor of 3. */
		CvPoint p, q;
		p.x = (int) firstFrameFeatures[i].x;
		p.y = (int) firstFrameFeatures[i].y;
		q.x = (int) secondFrameFeatures[i].x;
		q.y = (int) secondFrameFeatures[i].y;

		double angle;		angle = atan2( (double) p.y - q.y, (double) p.x - q.x );
		double hypotenuse;	hypotenuse = sqrt( square(p.y - q.y) + square(p.x - q.x) );

		/* Here we lengthen the arrow by a factor of three. */
		q.x = (int) (p.x - FACTOR * hypotenuse * cos(angle));
		q.y = (int) (p.y - FACTOR * hypotenuse * sin(angle));

		/* Now we draw the main line of the arrow. */
		/* "frame1" is the frame to draw on.
			* "p" is the point where the line begins.
			* "q" is the point where the line stops.
			* "CV_AA" means antialiased drawing.
			* "0" means no fractional bits in the center cooridinate or radius.
			*/
		cvLine(frame, p, q, line_color, lineThickness, CV_AA, 0 );
		/* Now draw the tips of the arrow.  I do some scaling so that the
			* tips look proportional to the main line of the arrow.
			*/			
		p.x = (int) (q.x + square(FACTOR) * cos(angle + PI / 4));
		p.y = (int) (q.y + square(FACTOR) * sin(angle + PI / 4));
		cvLine(frame, p, q, line_color, lineThickness, CV_AA, 0 );
		p.x = (int) (q.x + square(FACTOR) * cos(angle - PI / 4));
		p.y = (int) (q.y + square(FACTOR) * sin(angle - PI / 4));
		cvLine(frame, p, q, line_color, lineThickness, CV_AA, 0 );
	}
}

int main()
{    
	// Utworzenie okienek
	cvNamedWindow(FRAME_PREV, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(FRAME, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(FRAME_OPT, CV_WINDOW_AUTOSIZE);

    // Wczytywanie obrazów
	IplImage* frame_prev = cvLoadImage("frame_prev.jpg", 0); // CV_LOAD_IMAGE_COLOR
	IplImage* frame = cvLoadImage("frame.jpg", 0); 

	// Wyœwietlenie obrazów w oknach
	if(frame != NULL)
	{
		cvShowImage(FRAME, frame);
	}
	if(frame_prev != NULL)
	{
		cvShowImage(FRAME_PREV, frame_prev);
	}

	// Tworzenie buforów na potrzeby algortymu
	IplImage* firstFrame = cvCreateImage(cvGetSize(frame_prev), IPL_DEPTH_8U, 1); // -
	IplImage* secondFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1); // -
	IplImage* pyramid1 = cvCreateImage(cvGetSize(frame_prev), IPL_DEPTH_8U, 1); //cvCreateImage( cvSize( obrazA->width+8, obrazB->height/3) , IPL_DEPTH_32F, 1); 
	IplImage* pyramid2 = cvCreateImage(cvGetSize(frame_prev), IPL_DEPTH_8U, 1); //cvCreateImage( cvSize( obrazA->width+8, obrazB->height/3) , IPL_DEPTH_32F, 1); 
	IplImage* eigImage = NULL; //cvCreateImage( cvGetSize( obrazA ), IPL_DEPTH_32F, 1);
	IplImage* tempImage = NULL; //cvCreateImage( cvGetSize( obrazA ), IPL_DEPTH_32F, 1);

	// Optical Flow
	// Tablica w³aœciwoœci z pierwszej klatki
	//CvPoint2D32f firstFrameFeatures[FEATURES_NUM];
	//CvPoint2D32f secondFrameFeatures[FEATURES_NUM];
	cvConvertImage(frame_prev, firstFrame, CV_CVTIMG_FLIP);
	cvConvertImage(frame, secondFrame, CV_CVTIMG_FLIP);
	CvPoint2D32f* firstFrameFeatures = new CvPoint2D32f[FEATURES_NUM];
	CvPoint2D32f* secondFrameFeatures = new CvPoint2D32f[FEATURES_NUM];

	// Maksymalna liczba wlaœciwoœci
	int numberOfFeatures = FEATURES_NUM;

	/* Shi and Tomasi algorithm
	*
	* firstFram - obrazek wejsciowy
	* eigImage, tempImage - obrazki pomocnicze potrzebne do dzialania algorytmu
	* The first ".01" specifies the minimum quality of the features (based on the eigenvalues).
	* The second ".01" specifies the minimum Euclidean distance between features.
	* "NULL" means use the entire input image.  You could point to a part of the image.
	*
	* RETURNS:
	* firstFrameFeatures - tablica znalezionych wlasciwosci
	* numberOfFeatures - liczba znalezionych wlasciwosci
	*/
	//cvGoodFeaturesToTrack(firstFrame, eigImage, tempImage, firstFrameFeatures, &numberOfFeatures, 0.01, 0.01, NULL); 
	cvGoodFeaturesToTrack(frame_prev, eigImage, tempImage, firstFrameFeatures, &numberOfFeatures, 0.01, 5.0, 0, 3, 0, 0.04);
	/* 
	* "optical_flow_window" is the size of the window to use to avoid the aperture problem.
	* "5" is the maximum number of pyramids to use.  0 would be just one level.
	* "optical_flow_found_feature" is as described above (non-zero iff feature found by the flow).
	* "optical_flow_feature_error" is as described above (error in the flow for this feature).
	* "optical_flow_termination_criteria" is as described above (how long the algorithm should look).
	* "0" means disable enhancements.  (For example, the second array isn't pre-initialized with guesses.)
	*/
	/* This termination criteria tells the algorithm to stop when it has either done 20 iterations or when
	* epsilon is better than .3.  You can play with these parameters for speed vs. accuracy but these values
	* work pretty well in many situations.
	*/
	char opticalFlowFoundFeature[FEATURES_NUM];	// 0
	float opticalFlowFeatureError[FEATURES_NUM]; // 0
	//cvCalcOpticalFlowPyrLK(firstFrame, secondFrame, pyramid1, pyramid2, firstFrameFeatures, secondFrameFeatures,
	//						numberOfFeatures, cvSize(5,5), 5, opticalFlowFoundFeature, opticalFlowFeatureError,
	//						cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 ), 0); // samo ITER 40 zamiast 20
	cvCalcOpticalFlowPyrLK(frame_prev, frame, pyramid1, pyramid2, firstFrameFeatures, secondFrameFeatures, numberOfFeatures, cvSize(10,10), 5, opticalFlowFoundFeature, opticalFlowFeatureError, cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 40, .3 ), 0);
	
	// Rysowanie linii na obrazku wynikowym oraz wyœwietlanie obliczeñ
	//IplImage* resultImg = cvLoadImage("frame.jpg", CV_LOAD_IMAGE_COLOR); 
	//drawFlow(opticalFlowFoundFeature, numberOfFeatures, firstFrameFeatures, secondFrameFeatures, resultImg);
	//cvShowImage("Wynik optical'a flow", resultImg);
	IplImage* frame_opt = cvLoadImage("frame.jpg", CV_LOAD_IMAGE_COLOR);
    
	printf ("WYKONANE OBLICZENIA:\n\n");
    printf ("Liczba znalezionych punktow: %d\n", numberOfFeatures);                  
    printf ("Wspolrzedne sledzonych punktow:\n\n");
    printf ("  NR PUNKTU\tTYP OBRAZU\tWSP. X\t\tWSP. Y\n\n");

    for(int i=0; i<numberOfFeatures; i++)
	{                                                                                                                                                                   
		printf("  %d\t\tpoprzedni\t\t%.1lf\t\t%.1lf \n", i+1, firstFrameFeatures[i].x, firstFrameFeatures[i].y);
		printf("  %d\t\tnastepny\t\t%.1lf\t\t%.1lf \n\n", i+1, secondFrameFeatures[i].x, secondFrameFeatures[i].y);
	
		// cvRound to zamiana float na integer
		CvPoint punkt1 = cvPoint( cvRound(firstFrameFeatures[i].x), cvRound(firstFrameFeatures[i].y) ); 
		CvPoint punkt2 = cvPoint( cvRound(secondFrameFeatures[i].x), cvRound(secondFrameFeatures[i].y) );
		// Tworzenie linii
		cvLine(frame_opt, punkt1, punkt2, CV_RGB(COLOR_RED, COLOR_GREEN, COLOR_BLUE), LINE_WIDTH);              
	} 

	if(frame_opt != NULL)
	{
		cvShowImage(FRAME_OPT, frame_opt);
	}
	
	// Prowizoryczne badanie przesuniêcia
	int x1 = cvRound(firstFrameFeatures[0].x);
	int y1 = cvRound(firstFrameFeatures[0].y);
	int x2 = cvRound(secondFrameFeatures[0].x);
	int y2 = cvRound(secondFrameFeatures[0].y);
	printf("x1: %d y1: %d x2: %d y2: %d\n", x1, y1, x2, y2);

	if(x2 > x1 && y2 < y1)
	{
		printf("PRAWO DO GORY!\n");
	}

	if(x2 < x1 && y2 > y1)
	{
		printf("LEWO NA DOL!\n");
	}


	// Koniec programu
	int keyPressed = cvWaitKey(0);
	cvDestroyAllWindows();
	return 0;
}

