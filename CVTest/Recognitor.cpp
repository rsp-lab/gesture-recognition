#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "ImageProcessor.cpp"
#include "Configuration.h"


/** klasa do rozpoznawania */
class Recognitor {

private:

	/** nazwy okienek */
	char* MRO_WINDOW_CAMERA;
	char* MRO_WINDOW_FIND_SKIN;
	char* MRO_WINDOW_PREVIEW;

	/** uchyt do kamery */
	CvCapture *cam;

	/** uchwyt do obrazka */
	IplImage *img;

	/** klasa do wykrywania skory */
	//FindSkin* findSkin;

	/** klasa do wykrywania gestow */
	//GestureRecognition* gestureRecognition;

	ImageProcessor* imageProcessor;

public:

	Recognitor(Configuration *conf) {
		MRO_WINDOW_CAMERA = "Orygina³";
		MRO_WINDOW_FIND_SKIN = "Maska skóry";
		MRO_WINDOW_PREVIEW = "Podglad wykrywania";

		//findSkin = new FindSkin();
		//gestureRecognition = new GestureRecognition();
		imageProcessor = new ImageProcessor(conf);

		cam = NULL;
		img = NULL;
	}

	~Recognitor() {
		//delete findSkin;
		//delete gestureRecognition;
		delete imageProcessor;
	}

	void configToWorkWithCamera(CvCapture* cam) {
		this->cam = cam;
	}

	void configToWorkWithImage(IplImage *img) {
		this->img = img;
	}

	/** glowna petla porogramu */
	/*
	void run() {
		if(cam == NULL && img == NULL) {
			throw "Recognitor not configured";
		}

		//otwarcie potrzebnych okienek
		cvNamedWindow(MRO_WINDOW_CAMERA, CV_WINDOW_AUTOSIZE);
		cvNamedWindow(MRO_WINDOW_FIND_SKIN, CV_WINDOW_AUTOSIZE);
		cvNamedWindow("T1", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("T2", CV_WINDOW_AUTOSIZE);

		IplImage *currentFrame;
		if(cam != NULL && img == NULL) {
			currentFrame = cvQueryFrame(cam);
		} else {
			currentFrame = img;
		}
		gestureRecognition->configure(currentFrame);
		gestureRecognition->showInfo();

		//petla wlasciwa
		do {
			if(cam != NULL && img == NULL) {
				currentFrame = cvQueryFrame(cam);
			}

			cvShowImage(MRO_WINDOW_CAMERA, currentFrame);

			findSkin->findSkinOnImage(currentFrame);
			if(findSkin->hasNext()) {
				cvShowImage(MRO_WINDOW_FIND_SKIN, findSkin->next());
			}
			gestureRecognition->findFinger(currentFrame);
			if(gestureRecognition->hasNext()) {
				cvShowImage(MRO_WINDOW_FIND_SKIN, gestureRecognition->next());
				cvShowImage("T1", gestureRecognition->getDstImgR());
				cvShowImage("T2", gestureRecognition->getDstImgGB());
			}


			//odczytanie wcisnietego klawisza
			int keyPressed = cvWaitKey(10);
			if(keyPressed == 'q' || keyPressed == 'Q') {
				//wyjscie z programu
				break;
			}
		} while(cam != NULL && img == NULL);

		//poczekaj na wcisniecie w nieskonczonosc
		if(cam == NULL && img != NULL) {
			cvWaitKey(0);
		}

		//zamkniecie okienek
		cvDestroyAllWindows();
	}*/

	/** glowna petla porogramu */
	void run2() {
		if(cam == NULL && img == NULL) {
			throw "Recognitor not configured";
		}

		//otwarcie potrzebnych okienek
		//cvNamedWindow(MRO_WINDOW_CAMERA, CV_WINDOW_AUTOSIZE);
		//cvNamedWindow(MRO_WINDOW_FIND_SKIN, CV_WINDOW_AUTOSIZE);
		cvNamedWindow(MRO_WINDOW_PREVIEW, CV_WINDOW_AUTOSIZE);

		IplImage *currentFrame;
		if(cam != NULL && img == NULL) {
			currentFrame = cvQueryFrame(cam);
		} else {
			currentFrame = img;
		}
		imageProcessor->configure(currentFrame);
		imageProcessor->showInfo();

		//petla wlasciwa
		do {
			if(cam != NULL && img == NULL) {
				currentFrame = cvQueryFrame(cam);
			}

			imageProcessor->doWork(currentFrame);
			//cvShowImage(MRO_WINDOW_CAMERA, imageProcessor->getColorImage());
			//cvShowImage(MRO_WINDOW_FIND_SKIN, imageProcessor->getSkinMask());
			cvShowImage(MRO_WINDOW_PREVIEW, imageProcessor->getGrayscaleImage());

			//odczytanie wcisnietego klawisza
			int keyPressed = cvWaitKey(10);
			//if(keyPressed == 'q' || keyPressed == 'Q') {
			//	//wyjscie z programu
			//	break;
			//}
		} while(cam != NULL && img == NULL);

		//poczekaj na wcisniecie w nieskonczonosc
		if(cam == NULL && img != NULL) {
			cvWaitKey(0);
		}

		//zamkniecie okienek
		cvDestroyAllWindows();
	}

};
