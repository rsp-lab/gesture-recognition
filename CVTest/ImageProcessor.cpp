#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <list>
#include <set>
#include "Position.h"
#include "Configuration.h"

// zwykly Haar - obszar policz srednia jasnosc wez srodkowy pasek srednia wartosc pomnoz przez 3 i trzeba znalezc prog treshold (nie mogl dopasowac)
// jest pseudoHaar

/** pobieranie i obrabianie obrazka */
class ImageProcessor {

public:

	/** szerokosc, do jakiej zostanie przeskalowany obrazek*/
	static const int ALGORITHM_WIDTH = 256; //px szerokosc obrazka przeskalowanego kazdy obrazek na wejsciu zeby zachowac propr

	/* typy filtra */
	static const int HAAR_TYPE_DARK_ON_LIGHT = 1;
	static const int HAAR_TYPE_LIGHT_ON_DARK = 2;
	static const int HAAR_TYPE_CHANGE_THRESHOLD = 30;

	/* poszczegolne wymiary filtra */
	/*static const int config->getHaarSingleWidth() = 10; 
	static const int config->getHaarWidth() = 3 * config->getHaarSingleWidth(); // szerokosc
	static const int config->getHaarHeight() = 15; // wysokosc
	static const int config->getHaarStepWidth() = 5; // co ile przesuwac filtr po obrazku co 5 pikseli
	static const int config->getHaarStepHeight() = 10;*/

private:

	Configuration * config;
	
	/* rozmiary obrazka oryginalnego */
	int imgOrginalWidth;
	int imgOrginalHeight;

	/* rozmiary obrazka po zmniejszeniu */
	int imgWidth; // wszystkie obrazki oprocz oryginalnego
	int imgHeight;

	/** obrazek kolorowy */
	IplImage* srcImg;
	int scrImgWidthStep; // o ile trzeba przesunac wskaznik aby przejsc do nast linii obrazka (tablica jednowymiarowa obrazek jest trzymany)

	/** obrazek w skali szarosci */
	IplImage* dstImg;
	int dstImgWidthStep; // inna szerokosc przejscia niz w kolorowym

	/** maska skory */
	IplImage* skinMask;
	int skinMaskWidthStep; // w skali szarosci trzymana maska

	/** integral image */
	unsigned int* integral; // tablica unsigned int, szerokosc i wysokosc jest o 1 piksel wieksza niz dla wejsciowego obrazka (dstImage) nie mozna policzyc dla kolorowego (trzy integral dla kazdego kanalu)
	int integralWidth; // obraz sumowany (tab jednowymiarowa), piksel o wsp 10, 20 ma jak¹s liczbe która mówi œrednia jasnoœæ w prostok¹cie 0 0 i 10 20 wynosi tyle ile wartoœæ w punkcie 10 20, nie trzeba iœæ piksel po pikselu ¿eby sumowaæ 
	int integralHeight; // œredni¹ jasnoœæ bierze siê -> prawy dolny róg tego prostok¹ta dodajemy lewy górny i odejmujemy z dwóch pozosta³ych i mamy œredni¹ wartoœæ jasnoœci w tym obszarze, którym sprawdzamy

	/** lista obszarow pasujacych do zadanego klasyfikatora Haara */
	std::list<Position*> positionsListTypeDarkOnLight;
	std::list<Position*> positionsListTypeLightOnDark;

	/** lista obszarow z palcami */
	std::list<Position*> fingerPositions; // lista tych bia³ych

	/** zmienne pomocnicze */
	int currentHaarType;
	int haarTypeMeasurment;
	bool isConfigured;

	/** zmienne do przechowywania poprzednich wspó³rzêdnych, dla ruchu **/
	int X;
	int Y;
	int X2;
	int Y2;
	int X3;
	int Y3;

	/** flaga odpowiedzialna za pojedyñcze wykonanie kodu (inicjowanie poprzednich wspó³rzêdnych) **/
	bool jeden;

	/** liczniki wykorzystywane do wykrywania gestów **/
	int left;
	int right;
	int top;
	int bottom;
	//int fingerStraight;
	//int fingerLeft;
	//int fingerRight;

public:

	ImageProcessor(Configuration *config) {
		
		this->config = config;
		currentHaarType = HAAR_TYPE_DARK_ON_LIGHT;
		haarTypeMeasurment = 0;
		isConfigured = false;
		X = 0;
		Y = 0;
		X2 = 0;
		Y2 = 0;
		X3 = 0;
		Y3 = 0;
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		jeden = true;
	}

	~ImageProcessor() {
		if(isConfigured) {
			cvReleaseImage(&srcImg);
			cvReleaseImage(&dstImg);
			cvReleaseImage(&skinMask);
			delete integral;
			delete config;
		}
	}

	/** konfiguruje obiekt na przykladzie wzorcowej klatki */ // obliczana wysokosc, tworzenie obrazków i tworzenie tablicy integral image +1
	void configure(IplImage* sampleImg) {
		//odczytanie parametrow obrazka zrodlowego
		CvSize size = cvGetSize(sampleImg);
		imgOrginalWidth = size.width;
		imgOrginalHeight = size.height;

		//obliczenie rozmiaru obrazka wynikowego
		imgWidth = ALGORITHM_WIDTH;
		//wysokosc oblicz, aby zachowac oryginalne proporcje
		imgHeight = (int)((double)imgOrginalHeight / ((double)imgOrginalWidth / (double)ALGORITHM_WIDTH));
		size = cvSize(imgWidth, imgHeight);

		srcImg = cvCreateImage(size, sampleImg->depth, sampleImg->nChannels);
		scrImgWidthStep = srcImg->widthStep;

		dstImg = cvCreateImage(size, IPL_DEPTH_8U, 1);
		dstImgWidthStep = dstImg->widthStep;

		skinMask = cvCreateImage(size, IPL_DEPTH_8U, 1);
		skinMaskWidthStep = skinMask->widthStep;

		//utworzenie integral image
		integralWidth = imgWidth + 1;
		integralHeight = imgHeight + 1;
		integral = new unsigned int[integralWidth * integralHeight];

		isConfigured = true;
	}

	/** @return wskaznik do aktualnej listy z pozycjami */
	std::list<Position*>* getCurrentPositionsList() {
		if(currentHaarType == HAAR_TYPE_DARK_ON_LIGHT) {
			return &positionsListTypeDarkOnLight;
		} else if(currentHaarType == HAAR_TYPE_LIGHT_ON_DARK) {
			return &positionsListTypeLightOnDark;
		}

		return NULL;
	}

	/** wypluwa troszke wiadomosci na ekran */
	void showInfo() {
		std::cout << "Rozmiar oryginalny: " << imgOrginalWidth << "x" << imgOrginalHeight << std::endl;
		std::cout << "Rozmiar przeskalowany: " << imgWidth << "x" << imgHeight << std::endl;
	}

	void fingerAmount(std::list<Position*> *positionsList)
	{
		// uwaga na ilosc kwadratów!!
		int fingerAmount = (int)fingerPositions.size();
		//std::cout << "ilosc palcow: " << fingerAmount << std::endl;
		if(fingerAmount >= 3)
		{
			//std::cout << "Wykryto dlon -\\|/-" << std::endl;
		}
		else if(fingerAmount == 0) 
		{
			if(positionsList->size() > 0)
			{
				//std::cout << "Wykryto piesc *" << std::endl;
			}
			left = 0;
			right = 0;
			top = 0;
			bottom = 0;
			jeden = true;
		}
	}

	/** @param iplImage obrazek do obrobki */
	void doWork(IplImage* iplImage)
	{
		std::cout << "Parametry wykrycia:" << std::endl;

		if(!isConfigured)
		{
			throw "ImageProcessor not configured";
		}
		cleanBeforeWork(); // czyœci tablice
		
		changeSize(iplImage); // przeskalowuje obrazek z kamerki do szerokosci
		toGrayScale(); // kopia grayscale
		calculateIntegralImage(); // tworzy integral
		
		applyHaar();
		findFingersAndManage();
		
		findSkin(); // szuka skóry 
		improveSkinMask(); // improve maski

		//std::cout << "..." << std::endl;
		system("cls");
	}

	/** @return oryginalny obrazek tylko zmieniejszony */
	IplImage* getColorImage()
	{
		return srcImg;
	}

	/** @return obrazek zmniejszony i przeksztalcony na skale szarosci */
	IplImage* getGrayscaleImage()
	{
		return dstImg;
	}

	/** @return obraz sumowany dla obrazka w skali szarosci o ktorym mowa wyzej.
	    UWAGA: obraz sumowany jest o 1 piksel szerszy i wyzszy! */
	unsigned int* getIntegralImage()
	{
		return integral;
	}

	/** @return maske skory */
	IplImage* getSkinMask()
	{
		return skinMask;
	}

	/** @return liste obszarow, ktore moga zawierac palce */
	std::list<Position*>* getProbablyFingerPosition()
	{
		return &fingerPositions;
	}

	int getImgWidth() 
	{
		return imgWidth;
	}

	int getImgHeight()
	{
		return imgHeight;
	}

	int getIntegralWidth()
	{
		return integralWidth;
	}

	int getIntegralHeight() 
	{
		return integralHeight;
	}

private:

	/** robi porzadki w klasie */
	void cleanBeforeWork() {
		clearPositionsListAndFingersPositions();
	}

	void clearPositionsListAndFingersPositions() {
		clearPositionsList(&positionsListTypeDarkOnLight);
		clearPositionsList(&positionsListTypeLightOnDark);

		/* czyszczenie pozycji rozpoznanych palcow */
		Position* pos;
		for(std::list<Position*>::iterator it = fingerPositions.begin() ; it != fingerPositions.end() ; ++it) {
			pos = *it;
			delete pos;
		}
		fingerPositions.clear();
	}

	/** wczysci wskazana liste z obszarami pasujacymi do klasyfikatora */
	void clearPositionsList(std::list<Position*> *positionsList) {
		Position* pos;
		for(std::list<Position*>::iterator it = positionsList->begin() ; it != positionsList->end() ; ++it) {
			pos = *it;
			delete pos;
		}
		positionsList->clear();
	}

	/** zmiana oryginalnego rozmiaru */
	void changeSize(IplImage* iplImage) {
		cvResize(iplImage, srcImg, CV_INTER_AREA); //CV_INTER_ARE daje najlepsze efekty, jesli chodzi usuwanie szumu z obrazka
	}

	/** utworzenie obrazka w skali szarosci */
	void toGrayScale() {
		cvCvtColor(srcImg, dstImg, CV_RGB2GRAY);
	}

	/** obliczenie integral image dla obrazka w skali szarosci */
	void calculateIntegralImage() { 
		// przejscie po tablicy integral i jednoczesnie po obrazku czarnobialym, trzeba dostac sie dostac do danych obrazka
		uchar *orgData = (uchar*) dstImg->imageData;
		int i;

		//pierwszy wiersz wypelniamy zerami
		for(i = 0 ; i < integralWidth ; i++) {
			integral[i] = 0;
		}

		int integralPrevLine = 0;
		for(i = 1 ; i < integralHeight ; i++) {
			int integralLine = i * integralWidth;
			int orgLine = (i - 1) * dstImgWidthStep;
			register unsigned int lineSum = 0;
			integral[integralLine] = 0; //pierwsza kolumna ma zawsze zero
			for(int j = 1 ; j < integralWidth ; j++) {
				lineSum += (unsigned int) orgData[orgLine + (j - 1)];
				integral[integralLine + j] =  integral[integralPrevLine + j] + lineSum;
			}
			integralPrevLine = integralLine;
		}
	}

	/** stosuje na obrazku klasyfikator Haara */
	void applyHaar() { 
		// chodzi po obrazku od lewej do prawej od gory do dolu i liczy jak filtr ma 30 pikseli szerokosci to dzieli na 3 czesci po 10 pikseli
		// oblicza w trzech obszarrach avg 1 2 3, za pomoca integral liczy srednia jasnosc w tym obsarze
		register unsigned int avg1 = 0;
		register unsigned int avg2 = 0;
		register unsigned int avg3 = 0;
		float HAAR_FACTOR = 1.2;

		for(int i = 0 ; i <= (imgHeight - config->getHaarStepWidth()) ; i += config->getHaarStepHeight()) {
			for(int j = 0 ; j <= (imgWidth - config->getHaarWidth()) ; j += config->getHaarStepWidth()) {
				int base = i * (imgWidth + 1) + j;
				
				avg1 =  integral[base + integralWidth + config->getHaarSingleWidth()];
				avg1 += integral[base];
				avg1 -= integral[base + integralWidth];
				avg1 -= integral[base + config->getHaarSingleWidth()];

				base += config->getHaarSingleWidth();
				avg2 =  integral[base + integralWidth + config->getHaarSingleWidth()];
				avg2 += integral[base];
				avg2 -= integral[base + integralWidth];
				avg2 -= integral[base + config->getHaarSingleWidth()];

				base += config->getHaarSingleWidth();
				avg3 =  integral[base + integralWidth + config->getHaarSingleWidth()];
				avg3 += integral[base];
				avg3 -= integral[base + integralWidth];
				avg3 -= integral[base + config->getHaarSingleWidth()];

				// probelm palec ma byc ciemny tlo jasne
				// avg to srodek filtra, jesli srodek pomnozony przez 1.2 (jasnosc na srodku jest mniejsza od jasnosci po lewej itd.) jasnosc palca jest mniejsza niz tla za nim
				//pseudo klasyfikator haara
				if(avg2 * HAAR_FACTOR < avg1 && avg2 * HAAR_FACTOR < avg3) {
					positionsListTypeDarkOnLight.push_back(new Position(j, i, j+config->getHaarWidth(), i+config->getHaarHeight()));
					if(currentHaarType == HAAR_TYPE_DARK_ON_LIGHT) {
						cvRectangle(dstImg, cvPoint(j, i), cvPoint(j+config->getHaarWidth(), i+config->getHaarHeight()), cvScalar(0,0,0,0));
					}
				} else if(avg2 > avg1 * HAAR_FACTOR && avg2 > avg3 * HAAR_FACTOR) {
					positionsListTypeLightOnDark.push_back(new Position(j, i, j+config->getHaarWidth(), i+config->getHaarHeight()));
					if(currentHaarType == HAAR_TYPE_LIGHT_ON_DARK) {
						cvRectangle(dstImg, cvPoint(j, i), cvPoint(j+config->getHaarWidth(), i+config->getHaarHeight()), cvScalar(0,0,0,0));
					}
				}
			}
		}
	}

		/** odnajduje palce i ew. decyduje kiedy przelaczyc typ filtra */
	void findFingersAndManage() {
		int resultDarkOnLight = findFingersOnList(&positionsListTypeDarkOnLight, currentHaarType == HAAR_TYPE_DARK_ON_LIGHT ? true : false);
		int resultLightOnDark = findFingersOnList(&positionsListTypeLightOnDark, currentHaarType == HAAR_TYPE_LIGHT_ON_DARK ? true : false);
		
		if(resultDarkOnLight > 0 && resultLightOnDark == 0) {
			haarTypeMeasurment += resultDarkOnLight;
		} else if(resultLightOnDark > 0 && resultDarkOnLight == 0) {
			haarTypeMeasurment -= resultLightOnDark;
		}

		if(haarTypeMeasurment > HAAR_TYPE_CHANGE_THRESHOLD) {
			changeHaarType(HAAR_TYPE_DARK_ON_LIGHT);
			haarTypeMeasurment = 0;
		} else if(haarTypeMeasurment < -(HAAR_TYPE_CHANGE_THRESHOLD)) {
			changeHaarType(HAAR_TYPE_LIGHT_ON_DARK);
			haarTypeMeasurment = 0;
		}
	}

	/** zmienia typ klasyfikatora */
	void changeHaarType(int haarType) {
		if(currentHaarType != haarType) {
			currentHaarType = haarType;
			std::cout << "Haar type changed to: " << currentHaarType << "\n";
			std::cout << (char) 7;
			if(currentHaarType == HAAR_TYPE_LIGHT_ON_DARK) {
				std::cout << (char) 7;
			}
		}
	}

	/** na podstawie obszarow zaklasyfikowanych przez filtr Haara wskazuje obszary z potencjalnym palcem */
	int findFingersOnList(std::list<Position*> *positionsList, bool isCurrentHaarType) { 
		int fingersNumber = 0;
		const int maxWidthDiff = config->getHaarStepWidth();
		const int maxHeightDiff = config->getHaarStepHeight();
		std::set<int> showedSet;
		int PREV = 0;
		int PREV2 = 0;
		int PREV3 = 0;

		for(std::list<Position*>::iterator iter=positionsList->begin() ; iter != positionsList->end() ; ++iter)
		{
			Position* currentPos = *iter;
			int xPos = currentPos->xTopLeft;
			int yPosLast = currentPos->yTopLeft;

			int xPos2 = currentPos->xTopLeft;
			int yPosLast2 = currentPos->yTopLeft;

			int xPos3 = currentPos->xTopLeft;
			int yPosLast3 = currentPos->yTopLeft;

			std::list<Position*> similarPos;
			std::list<Position*> similarPos2;
			std::list<Position*> similarPos3;

			Position* pos = NULL;
			Position* pos2 = NULL;
			Position* pos3 = NULL;
			
			int temp = 0;
			int temp2 =0;
			
			for(std::list<Position*>::iterator subIter = iter ; subIter != positionsList->end() ; ++subIter)
			{
				pos = *subIter;
				
				if(pos->xTopLeft <= (xPos+4) && pos->xTopLeft >= (xPos-4))
				{
					if(pos->yTopLeft - yPosLast <= maxHeightDiff) // maxHeightDiff -> 10
					{
						//xPos = pos->xTopLeft;
						yPosLast = pos->yTopLeft;
						similarPos.push_back(pos);
					}
				}
			}
			
			for(std::list<Position*>::iterator subIter = iter ; subIter != positionsList->end() ; ++subIter)
			{
				pos2 = *subIter;

				//if(pos2->xTopLeft <= (xPos2+4) && pos2->xTopLeft >= (xPos2-4))
				if(pos2->xTopLeft==xPos2)
				{
					if(pos2->yTopLeft - yPosLast2 <= 5) // maxHeightDiff -> 10
					{
						temp++;
						xPos2 = pos2->xTopLeft;
						yPosLast2 = pos2->yTopLeft;
						//if(temp%3 == 0)
							similarPos2.push_back(pos2);
					}
				}

				else if(((pos2->xTopLeft) < (xPos2+20)) && (pos2->xTopLeft > (xPos2+4)))
				{
					if(pos2->yTopLeft - yPosLast2 <= maxHeightDiff+10) // maxHeightDiff -> 10
					{
						xPos2 = pos2->xTopLeft;
						yPosLast2 = pos2->yTopLeft;
						similarPos2.push_back(pos2);
					}
				}	
			}
			
			for(std::list<Position*>::iterator subIter = iter ; subIter != positionsList->end() ; ++subIter)
			{
				pos3 = *subIter;
		
				//if(pos3->xTopLeft <= (xPos3+4) && pos3->xTopLeft >= (xPos3-4))
				if(pos3->xTopLeft==xPos3)
				{
					if(pos3->yTopLeft - yPosLast3 <= maxHeightDiff-5) 
					{
						temp2++;
						xPos3 = pos3->xTopLeft;
						yPosLast3 = pos3->yTopLeft;
						//if(temp2%3==0)
							similarPos3.push_back(pos3);
					}
				}

				if((pos3->xTopLeft > (xPos3-20)) && (pos3->xTopLeft < (xPos3-4)))
				{
					if(pos3->yTopLeft - yPosLast3 <= maxHeightDiff+10) // maxHeightDiff -> 10
					{
						xPos3 = pos3->xTopLeft;
						yPosLast3 = pos3->yTopLeft;
						similarPos3.push_back(pos3);
					}
				}		
			}
			
			if(similarPos.size() >= config->getSimilarPosStraight())
			{ 
				Position* p1 = *(similarPos.begin());
				std::set<int>::iterator it = showedSet.find(p1->xTopLeft+(PREV-p1->xTopLeft));
				if(it == showedSet.end())
				{
					PREV = p1->xTopLeft;
					Position* p2 = *(--(similarPos.end()));
					Position* fingerPos = new Position(p1->xTopLeft, p1->yTopLeft, p2->xBottomRight, p2->yBottomRight);
					showedSet.insert(p1->xTopLeft);
					fingerPositions.push_back(fingerPos);
					if(isCurrentHaarType) {
						cvRectangle(dstImg, cvPoint(fingerPos->xTopLeft, fingerPos->yTopLeft), cvPoint(fingerPos->xBottomRight, fingerPos->yBottomRight), cvScalar(255,0,0,0)); 
						std::cout << "Wykryto palec |\n";
					}
					fingersNumber++;
				}
			}
			
			else if(similarPos2.size() >= config->getSimilarPosLeft())
			{ 
				Position* p1 = *(similarPos2.begin());

				std::set<int>::iterator it = showedSet.find(p1->xTopLeft+(PREV2-p1->xTopLeft));
				if(it == showedSet.end())
				{
					PREV2 = p1->xTopLeft;
					Position* p2 = *(--(similarPos2.end()));
					Position* fingerPos = new Position(p1->xTopLeft, p1->yTopLeft, p2->xBottomRight, p2->yBottomRight);
					
					showedSet.insert(p1->xTopLeft);
					fingerPositions.push_back(fingerPos);
					if(isCurrentHaarType) {
						std::cout << "Wykryto palec \\" << std::endl;
						cvRectangle(dstImg, cvPoint(fingerPos->xTopLeft, fingerPos->yTopLeft), cvPoint(fingerPos->xBottomRight, fingerPos->yBottomRight), cvScalar(255,0,0,0));
					}
					fingersNumber++;
				}
			}
						
			else if(similarPos3.size() >= config->getSimilarPosRight())
			{ 
				Position* p1 = *(similarPos3.begin());

				std::set<int>::iterator it = showedSet.find(p1->xTopLeft+(PREV3-p1->xTopLeft));
				if(it == showedSet.end())
				{
					PREV3 = p1->xTopLeft;
					Position* p2 = *(--(similarPos3.end()));
					Position* fingerPos = new Position(p2->xTopLeft, p1->yTopLeft, p1->xBottomRight, p2->yBottomRight);
					
					showedSet.insert(p1->xTopLeft);
					fingerPositions.push_back(fingerPos);
					if(isCurrentHaarType) {
						cvRectangle(dstImg, cvPoint(fingerPos->xTopLeft, fingerPos->yTopLeft), cvPoint(fingerPos->xBottomRight, fingerPos->yBottomRight), cvScalar(255,0,0,0));
						std::cout << "Wykryto palec /" << std::endl;
					}
					fingersNumber++;
				}
			}
		}

		if(isCurrentHaarType)
		{
			detectMovement();
			fingerAmount(positionsList);
		}
		return fingersNumber;
	}

	/** Wykrywanie ruchu na podstawie wspó³rzêdnych **/
	void detectMovement()
	{
		for(std::list<Position*>::iterator iter=fingerPositions.begin() ; iter != fingerPositions.end() ; ++iter)
		{
				Position *currentPos = *iter;

				if(jeden)
				{ 
					// if n == 1 to zapisz do X i Y, n ==2 zapisz do X2 Y2 itd. ewentualnie lista intów X i Y
					// a co jeœli kwadrat zniknie z kamierki? Zmienna jeden dalej bêdzie na false! W fingeramount()==0 zmienna ustawic na true!
					X = currentPos->xTopLeft;
					Y = currentPos->yTopLeft;
					jeden = false;
				}
				else
				{
					// Myœle ¿e ró¿nicê tu trzeba przechowywaæ tylko a wypisywanie daæ pod forem!
					//std::cout << "ABS X: " << abs(currentPos->xTopLeft-X) << std::endl;
					//std::cout << "ABS Y: " << abs(currentPos->yTopLeft-Y) << std::endl;
					
					if((currentPos->xTopLeft-X) > 0 && abs(currentPos->xTopLeft-X) > config->getMoveToleranceHorizontal())
					{
						left++;
						right = 0;
						std::cout << "Ruch w lewo o " << abs(currentPos->xTopLeft-X) << " pikseli" << std::endl;
					}
					if((currentPos->xTopLeft-X) < 0 && abs(currentPos->xTopLeft-X) > config->getMoveToleranceHorizontal())
					{
						right++;
						left = 0;
						std::cout << "Ruch w prawo o " << abs(currentPos->xTopLeft-X) << " pikseli" << std::endl;
					}
					if((currentPos->yTopLeft-Y) > 0 && abs(currentPos->yTopLeft-Y) > config->getMoveToleranceVertical()) // mniejsza tolerancja dla pionu (czyli wiêksza wartoœæ)
					{
						bottom++;
						top = 0;
						std::cout << "Ruch na dol o " << abs(currentPos->yTopLeft-Y) << " pikseli" << std::endl;
					}
					if((currentPos->yTopLeft-Y) < 0 && abs(currentPos->yTopLeft-Y) > config->getMoveToleranceVertical()) // mniejsza tolerancja dla pionu (czyli wiêksza wartoœæ)
					{
						top++;
						bottom = 0;
						std::cout << "Ruch do gory o " << abs(currentPos->yTopLeft-Y) << " pikseli" << std::endl;
					}

					X = currentPos->xTopLeft;
					Y = currentPos->yTopLeft;
				}

				if((left == config->getGestureHorizontal()) || (abs(currentPos->xTopLeft-X) > 30))
				{
					std::cout << "Wykonano gest w lewo" << std::endl;
					left = 0;
				}
				if(right == config->getGestureHorizontal() || (abs(currentPos->xTopLeft-X) > 30))
				{
					std::cout << "Wykonano gest w prawo" << std::endl;
					right = 0;
				}
				if(top == config->getGestureVertical() || (abs(currentPos->xTopLeft-X) > 20))
				{
					std::cout << "Wykonano gest w gora" << std::endl;
					top = 0;
				}
				if(bottom == config->getGestureVertical() || (abs(currentPos->xTopLeft-X) > 20))
				{
					std::cout << "Wykonano gest w dol" << std::endl;
					bottom = 0;
				}

				break;
		}
	}	

	/** wykrywanie skory metoda HSV Lookup */
	void findSkin()
	{ //translacja do innej przestrzeni barwnej, maska
		uchar *srcData = (uchar*) srcImg->imageData; 
		uchar *skinMaskData = (uchar*) skinMask->imageData;

		int nchannels = srcImg->nChannels;
		int Value, R, G, B, tmp;
		double Saturation, Hue, Cr, Cg, Cb;

		register int iStep = 0;
		for(int i = 0 ; i < imgHeight ; i++)
		{
			iStep += scrImgWidthStep;
			//int iStep = i * tmpWidthStep;
			int jNChannels = 0;
			for(int j = 0 ; j < imgWidth ; j++) 
			{
				//int jNChannels = j * nchannels;
				jNChannels += nchannels;
				B = srcData[iStep + jNChannels];
				G = srcData[iStep + jNChannels + 1];
				R = srcData[iStep + jNChannels + 2];
				///////////////////  VALUE
				// find maximum
				if ( (R >= G) && (R >= B))
				   Value = R;
				else if ( (G >= R) && (G >= B))
				   Value = G;
				else if ( (B >= G) && (B >= R))
				   Value = B;
				///////////////////  SATURATION
				//wyszukanie minimum
				if ( (R <= G) && (R <= B))
				   tmp = R;
				else if ( (G <= R) && (G <= B))
				   tmp = G;
				else if ( (B <= G) && (B <= R))
				   tmp = B;

				double valueMinusTmp = (double)Value - (double)tmp;
				if (Value == 0) {
				   Saturation = 0;
				} else {
				   Saturation = valueMinusTmp/(double)Value;
				}
				///////////////////  SATURATION
				if (Saturation == 0) {
				   Hue = -1;
				} else {
				   Cr = ((double)Value-(double)R)/(valueMinusTmp);
				   Cg = ((double)Value-(double)G)/(valueMinusTmp);
				   Cb = ((double)Value-(double)B)/(valueMinusTmp);
				   if (R == Value)
					  Hue = Cb - Cg;
				   if (G == Value)
					  Hue = 2 + Cr - Cb;
				   if (B == Value)
					  Hue = 4 + Cg - Cr;
				   Hue *= 60;
				   if (Hue < 0)
						Hue += 360;
				}
				/*
				 * wlasciwe wykrywanie koloru skory
				 */
				if ( (Hue >= 0.0) && (Hue <= 50.0) && (Saturation >= 0.23) && (Saturation <= 0.68) ) {
					skinMaskData[i * skinMaskWidthStep + j] = 255;
				} else {
					skinMaskData[i * skinMaskWidthStep + j] = 0;
				}
			}
		}
	}

	/** stosuje filtry w celu poprawy jakosci maski */
	void improveSkinMask() { 
		//filtr otwarcia utworzony z polaczenia nastepujacych:
		cvErode(skinMask, skinMask, NULL, 1); // usuniecie szumu
		cvDilate(skinMask, skinMask, NULL, 1); // ewentualnie dziury w rece zostaja zaklejone
		//cvSmooth(skinMask, skinMask, CV_MEDIAN, 1);
	}

};