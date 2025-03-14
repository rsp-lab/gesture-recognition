#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "Recognitor.cpp"
#include "Configuration.h"


#pragma HAVE_JPEG //aby moc w VS wczytac plik JPEG

/** czy ma uzywac kamery czy pliku */
const bool USE_CAMERA = true;

/** plik do otwarcia - mozna uzyc sciezki wzglednej lub bezwzglednej */
//const char* MRO_FILE_NAME = "C:\\Temp\\reka06.jpg";
//const char* MRO_FILE_NAME = "C:\\Temp\\reka93.jpg";
//const char* MRO_FILE_NAME = "C:\\Temp\\reka95.jpg";
//const char* MRO_FILE_NAME = "C:\\Temp\\skora1.jpg";
//const char* MRO_FILE_NAME = "C:\\Temp\\skora2.jpg";
//const char* MRO_FILE_NAME = "C:\\Temp\\image008.jpg";
const char* MRO_FILE_NAME = "C:\\full_color.jpg";

/** identyfikator kamery w systemie */
const int MRO_CAMERA_ID = 1;

int main(int argc, char* argv[]) {



	Configuration *conf = new Configuration();
	conf->set(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), atoi(argv[10]), atoi(argv[11]));

	//otwarcie handlera do kamery
	CvCapture* cvCapture = NULL;
	IplImage* iplImage = NULL;
	if(USE_CAMERA) {
		cvCapture = cvCaptureFromCAM(MRO_CAMERA_ID);
	} else {
		iplImage = cvLoadImage(MRO_FILE_NAME);
	}

	//sprawdzenie poprawnosci otwarcia handlerow
	if (USE_CAMERA && cvCapture == NULL) {
		std::cout << "ERROR: Nie mozna uruchomic kamery\n";
		getchar();
		return -10;
	} else if(USE_CAMERA) {
		std::cout << "Kamera uruchomiona prawidlowo\n";
	} else if(!USE_CAMERA && iplImage == NULL) {
		std::cout << "Nie mozna wczytac obrazka: " << MRO_FILE_NAME << "\n";
		getchar();
		return -20;
	} else if(!USE_CAMERA) {
		std::cout << "Wczytano obrazek: " << MRO_FILE_NAME << "\n";
	}
	
	if(cvCapture != NULL || iplImage != NULL){
		//uruchomienie rozpoznawania
		Recognitor *recognitor = new Recognitor(conf);
		if(USE_CAMERA) {
			recognitor->configToWorkWithCamera(cvCapture);
		} else {
			recognitor->configToWorkWithImage(iplImage);
		}
		//recognitor->run();
		recognitor->run2();
		delete recognitor;
	}

	if(USE_CAMERA && cvCapture != NULL) {
		//zwolnienie kamery
		cvReleaseCapture(&cvCapture);
	}

	return 0;
}
