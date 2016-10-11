#include "word_recognition.h"
#include <math.h>
#include "cv.h"
#include "highgui.h"
#include <iostream>
using std::cin;

void main()
{
	/*
	IplImage* file = cvLoadImage("C:\\b.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Image* src = new Image(file);
	//src->printImage();
	
	Pre_processing* test = new Pre_processing(src);
	test->preProcessingAll();
	Image* result = test->getOutput();
	
	delete test;
	result->printImage();
	cout<<"\n";

	Feature_extraction* test2 = new Feature_extraction(result);
	test2->getAllFeature(MDF);
	double* output = test2->getOutput();
	delete test2;
	free(output);
	
	cvReleaseImage(&file);
	*/

	//Create_feature_data_set* newCreate = new Create_feature_data_set(false, MDF);
	//newCreate->create();
	//delete newCreate;
	
	//Neural_network* newNetwork = new Neural_network(TRAINING_ALL_KIND_OF_CHARACTER, MDF);
	//newNetwork->Trainning();
	//delete newNetwork;

	/*
	cvNamedWindow("23423423",1);
	IplImage* file = cvLoadImage("C:\\test3.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	for( i = 0; i < 1000; i++);
	cvShowImage("23423423", file);
	Image* src = new Image(file);
	Recognition* test = new Recognition(src);
	test->recognize();
	*/
	

	/*
	initAll();
	IplImage* file = cvLoadImage("C:\\test1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Image* src = new Image(file);
	Word_recognition* test = new Word_recognition(src);
	test->slantCorrection();
	test->calculateStrokeWidth();
	test->baseline();
	test->segment();
	//test->recursiveValidation();
	delete src;
	*/
	//createGroup2();
	//createGroup3();
	//createGroup4();
	//createGroup5();
	//createGroup8();
	//trainningGroup8();
	
	createDictionary();
	cvNamedWindow("test", 1);
	initAll();
	IplImage* file = cvLoadImage("C:\\test1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Image* src = create_Image(file);
	int test2 = 0;
	//wordRecognition(src, &test2);ne`
	wordRecognition2(src);
	//characterRecognitionGroup1(src);
	cvShowImage("test", file);
	cvReleaseImage(&file);
	
	


	while( 1 ) 
	{
		
		if( cvWaitKey( 10 ) == 27 ) break;
		else if (cvWaitKey (10) == 'h')
		{	
			
			IplImage* file = cvLoadImage("C:\\test1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
			Image* src = create_Image(file);
			int test2 = 0;
			//wordRecognition(src, &test2);
			//characterRecognitionGroup3(src);
			wordRecognition2(src);
			cvShowImage("test", file);
			cvReleaseImage(&file);
			
			/*
			IplImage* file = cvLoadImage("C:\\b.jpg",CV_LOAD_IMAGE_GRAYSCALE);
			Image* src = create_Image(file);
			characterRecognitionGroup6(src);
			//characterRecognitionGroup4(src);
			//delete src;
			cvShowImage("test", file);
			cvReleaseImage(&file);
			*/
			
			/*
			int action = 1;
			int count = 0;
			while (action != 0)
			{
				cout<<"lam gi :";
				cin>>action;
				cout<<"\n";
				if (action == 1)
				{
					test->resetWord();
				}
				else if (action == 2)
				{
					char cha;
					cout<<" nhap ki tu :";
					cin>>cha;
					cout<<"\n";
					test->addChar(cha);
				}
				else if (action == 3)
				{
					char cha;
					cout<<" nhap type check :";
					cin>>cha;
					cout<<"\n";
					count++;
					switch(cha)
					{
						case 'a':
							char cha1;
							cout<<" nhap type check :";
							cin>>cha1;
							cout<<"\n";
							
							cout<<(int)(test->checkDictionaryTypeA(cha1));
							break;
						case 'b':
							char cha2;
							cout<<" nhap type check :";
							cin>>cha2;
							cout<<"\n";
							char cha3;
							cout<<" nhap type check :";
							cin>>cha3;
							cout<<"\n";
							char cha4;
							cout<<" nhap type check :";
							cin>>cha4;
							cout<<"\n";
							if (count == 3)
							{
								cout<<"fdfhsdkjfhskjhfsd\n";
							}
							cout<<(int)(test->checkDictionaryTypeB(cha2, cha3, cha4));
							break;
						case 'c':
							char cha5;
							cout<<" nhap type check :";
							cin>>cha5;
							cout<<"\n";
							char cha6;
							cout<<" nhap type check :";
							cin>>cha6;
							cout<<"\n";
							cout<<(int)(test->checkDictionaryTypeC(cha5, cha6));
							break;
						case 'd':
							char cha7;
							cout<<" nhap type check :";
							cin>>cha7;
							cout<<"\n";
							char cha8;
							cout<<" nhap type check :";
							cin>>cha8;
							cout<<"\n";
							char cha9;
							cout<<" nhap type check :";
							cin>>cha9;
							cout<<"\n";
							cout<<(int)(test->checkDictionaryTypeD(cha7, cha8, cha9));
							break;
					}
				}
			}*/
			
			
			
		}

			
	}
	
}