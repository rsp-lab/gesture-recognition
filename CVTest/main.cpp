#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>

#define MRO_FEATURES_NUM 50

/** nazwy okienek */
const char* MRO_CAMERA_WINDOW = "Rozpoznawanie gestow i ruchow dloni";
const char* MRO_CAMERA_WINDOW_PREV = "Poprzednia klatka";
const char* MTO_TMP_WINDOW = "Okno tymczasowe do podgladu";

/** identyfikator kamery w systemie */
const int MRO_CAMERA_ID = 1;

/*
SKIN DETECTION
*/

/** stosuje filtry na obrazku */
IplImage* applyFilters(IplImage *img) {
	IplImage* out = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

	//cvSmooth(img, out, CV_GAUSSIAN, 9, 9);
	//cvSmooth(img, out, CV_BLUR, 9, 9);
	cvSmooth(img, out, CV_MEDIAN, 9, 9);
	
	cvReleaseImage(&img);
	return out;
}

/** wykrywanie skory metoda HSV Lookup */
void findSkin(IplImage *srcImg, IplImage *dstImg) {
	uchar *srcData = (uchar*) srcImg->imageData; 
	uchar *dstData = (uchar*) dstImg->imageData;

	int width     = srcImg->width;
	int height    = srcImg->height;
	int nchannels = srcImg->nChannels;
	int step      = srcImg->widthStep;
	
	int Value, R, G, B, tmp;
	double Saturation, Hue, Cr, Cg, Cb;

	for(int i=0;i<height;i++) {
		for(int j=0;j<width;j++) {
			B = srcData[i*step + j*nchannels + 0];
			G = srcData[i*step + j*nchannels + 1];
			R = srcData[i*step + j*nchannels + 2];
			///////////////////  VALUE
			// find maximum
			if ( (R >= G) && (R >= B))
			   Value = R;
			else if ( (G >= R) && (G >= B))
			   Value = G;
			else if ( (B >= G) && (B >= R))
			   Value = B;
			///////////////////  SATURATION
			// find minimum
			if ( (R <= G) && (R <= B))
			   tmp = R;
			else if ( (G <= R) && (G <= B))
			   tmp = G;
			else if ( (B <= G) && (B <= R))
			   tmp = B;
			if (Value == 0)
			   Saturation = 0;
			else
			   Saturation = ((double)Value - (double)tmp)/(double)Value;
			///////////////////  SATURATION
			if (Saturation == 0)
			   Hue = -1;
			else {
			   Cr = ((double)Value-(double)R)/((double)Value-(double)tmp);
			   Cg = ((double)Value-(double)G)/((double)Value-(double)tmp);
			   Cb = ((double)Value-(double)B)/((double)Value-(double)tmp);
			   if (R == Value)
				  Hue = Cb - Cg;
			   if (G == Value)
				  Hue = 2 + Cr - Cb;
			   if (B == Value)
				  Hue = 4 + Cg - Cr;
			   Hue *= 60;
			   if (Hue < 0)
					Hue +=360;
			}
			///////////////////////
			// DETECT SKIN COLOR //
			///////////////////////
			if ( (Hue >= 0.0) && (Hue <= 50.0) && (Saturation >= 0.23) && (Saturation <= 0.68) ) {
				dstData[i*step + j*nchannels + 0] = B;
				dstData[i*step + j*nchannels + 1] = G;
				dstData[i*step + j*nchannels + 2] = R;
			} else {
				dstData[i*step + j*nchannels + 0] = 255;
				dstData[i*step + j*nchannels + 1] = 255;
				dstData[i*step + j*nchannels + 2] = 255;
			}
		}
	}
}

/*
OPTICAL FLOW
*/

inline static double square(int value) {
	return (double) (value * value);
}

/** zaalokowanie pamieci na obrazek */
inline static void allocateOnDemand(IplImage **img, CvSize size, int depth, int channels) {
	if(*img != NULL) {
		return;
	}

	*img = cvCreateImage(size, depth, channels);
	if (*img == NULL) {
		std::cerr << "Error: Couldn't allocate image. Out of memory?\n";
	}
}

/** rusuje znalezione wektory na obrazku */
void drawFlow(char *opticalFlowFoundFeature, int numberOfFeatures, 
				CvPoint2D32f* firstFrameFeatures, CvPoint2D32f* secondFrameFeatures, 
				IplImage *frame) {

	const double PI = 3.14159265358979323846;
	const int FACTOR = 1;
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
		p.x = (int) (q.x + square(FACTOR) * cos(angle + PI / 4.0));
		p.y = (int) (q.y + square(FACTOR) * sin(angle + PI / 4.0));
		cvLine(frame, p, q, line_color, lineThickness, CV_AA, 0 );
		p.x = (int) (q.x + square(FACTOR) * cos(angle - PI / 4.0));
		p.y = (int) (q.y + square(FACTOR) * sin(angle - PI / 4.0));
		cvLine(frame, p, q, line_color, lineThickness, CV_AA, 0 );
	}
}

int main(void) {

	/* przygotowanie pracy programu */

	//otwarcie handlera do kamery
	CvCapture *inputVideo = cvCaptureFromCAM(MRO_CAMERA_ID);
	if (inputVideo == NULL) {
		std::cerr << "ERROR: Nie mozna uruchomic kamery\n";
		return -10;
	} else {
		std::cout << "Kamera uruchomiona prawidlowo\n";
	}

	//otworzenie okienka
	cvNamedWindow(MRO_CAMERA_WINDOW, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(MRO_CAMERA_WINDOW_PREV, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(MTO_TMP_WINDOW, CV_WINDOW_AUTOSIZE);

	/* rozpoczecie przetwarzania w petli */

	IplImage* currentImg = NULL;
	IplImage* prevImg = NULL;
	CvSize frameSize;
	while(true) {

		currentImg = cvQueryFrame(inputVideo);
		currentImg = applyFilters(currentImg);
		IplImage *dstImg = cvCreateImage(cvGetSize(currentImg), currentImg->depth, currentImg->nChannels);
		findSkin(currentImg, dstImg);
		cvReleaseImage(&currentImg);
		currentImg = dstImg;
		
		if(prevImg == NULL) {
			//odczytanie wymiarow z pierwszej klatki
			frameSize.width		=	(int) cvGetCaptureProperty(inputVideo, CV_CAP_PROP_FRAME_WIDTH);
			frameSize.height	=	(int) cvGetCaptureProperty(inputVideo, CV_CAP_PROP_FRAME_HEIGHT);
			std::cout << "Rozmiar obrazu: " << frameSize.width << "x" << frameSize.height << " px\n";
		}

		//wyswietlenie obrazkow w okienkach
		cvShowImage(MRO_CAMERA_WINDOW, currentImg);
		if(prevImg != NULL) {
			cvShowImage(MRO_CAMERA_WINDOW_PREV, prevImg);
		}

		/* OPTICAL FLOW	- START */
		if(prevImg != NULL) {
			IplImage *firstFrame = NULL, *secondFrame = NULL;
			IplImage *eigImage = NULL, *tempImage = NULL, *pyramid1 = NULL, *pyramid2 = NULL;

			allocateOnDemand(&firstFrame, frameSize, IPL_DEPTH_8U, 1);
			allocateOnDemand(&secondFrame, frameSize, IPL_DEPTH_8U, 1);
			cvConvertImage(currentImg, firstFrame, CV_CVTIMG_FLIP);
			cvConvertImage(prevImg, secondFrame, CV_CVTIMG_FLIP);

			/* Shi and Tomasi Feature Tracking! */

			//wlasciwosci z pierwszej klatki
			CvPoint2D32f firstFrameFeatures[MRO_FEATURES_NUM];

			//maksymalna liczba wlasciwosci - po wywyolanu zawiera aktulana liczbe wlasciwosci
			int numberOfFeatures = MRO_FEATURES_NUM;

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
			cvGoodFeaturesToTrack(firstFrame, eigImage, tempImage, firstFrameFeatures, &numberOfFeatures, 0.01, 0.01, NULL);

			/* Pyramidal Lucas Kanade Optical Flow */

			/* This array will contain the locations of the points from frame 1 in frame 2. */
			CvPoint2D32f secondFrameFeatures[MRO_FEATURES_NUM];

			/* The i-th element of this array will be non-zero if and only if the i-th feature of
			 * firstFrame was found in secondFrame. */
			char opticalFlowFoundFeature[MRO_FEATURES_NUM];

			/* The i-th element of this array is the error in the optical flow for the i-th feature
			 * of frame1 as found in frame 2.  If the i-th feature was not found (see the array above)
			 * I think the i-th entry in this array is undefined.
			 */
			float opticalFlowFeatureError[MRO_FEATURES_NUM];

			/* This is the window size to use to avoid the aperture problem (see slide "Optical Flow: Overview"). */
			CvSize opticalFlowWindow = cvSize(5, 5);
		
			/* This termination criteria tells the algorithm to stop when it has either done 20 iterations or when
			 * epsilon is better than .3.  You can play with these parameters for speed vs. accuracy but these values
			 * work pretty well in many situations.
			 */
			CvTermCriteria opticalFlowTerminationCriteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3);

			/* This is some workspace for the algorithm.
			 * (The algorithm actually carves the image into pyramids of different resolutions.)
			 */
			allocateOnDemand(&pyramid1, frameSize, IPL_DEPTH_8U, 1);
			allocateOnDemand(&pyramid2, frameSize, IPL_DEPTH_8U, 1);

			/* Actually run Pyramidal Lucas Kanade Optical Flow!!
			 * "frame1_1C" is the first frame with the known features.
			 * "frame2_1C" is the second frame where we want to find the first frame's features.
			 * "pyramid1" and "pyramid2" are workspace for the algorithm.
			 * "frame1_features" are the features from the first frame.
			 * "frame2_features" is the (outputted) locations of those features in the second frame.
			 * "number_of_features" is the number of features in the frame1_features array.
			 * "optical_flow_window" is the size of the window to use to avoid the aperture problem.
			 * "5" is the maximum number of pyramids to use.  0 would be just one level.
			 * "optical_flow_found_feature" is as described above (non-zero iff feature found by the flow).
			 * "optical_flow_feature_error" is as described above (error in the flow for this feature).
			 * "optical_flow_termination_criteria" is as described above (how long the algorithm should look).
			 * "0" means disable enhancements.  (For example, the second array isn't pre-initialized with guesses.)
			 */
			cvCalcOpticalFlowPyrLK(firstFrame, secondFrame, pyramid1, pyramid2, firstFrameFeatures, secondFrameFeatures, numberOfFeatures, opticalFlowWindow, 5, opticalFlowFoundFeature, opticalFlowFeatureError, opticalFlowTerminationCriteria, 0);

			IplImage* resultImg = cvCloneImage(currentImg);
			drawFlow(opticalFlowFoundFeature, numberOfFeatures, firstFrameFeatures, secondFrameFeatures, resultImg);
			cvShowImage(MTO_TMP_WINDOW, resultImg);
		}
		/* OPTICAL FLOW	- KONIEC */

		//zapamietanie aktualnej klatki (utworzenie kopii)
		if(prevImg != NULL) {
			cvReleaseImage(&prevImg);
		}
		prevImg = cvCloneImage(currentImg);

		//odczytanie wcisnietego klawisza (czekanie w nieskonczonosc na klawisz)
		int keyPressed = cvWaitKey(0);
		if(keyPressed == 'q' || keyPressed == 'Q') {
			//wyjscie z programu
			break;
		} else {
			//pobierz nastepna klatke
			continue;
		}

	}

	/* zakonczenie pracy programu */

	//zwolnienie kamery
	cvReleaseCapture(&inputVideo);
	//zamkniecie okienek
	cvDestroyAllWindows();

	return 0;
}