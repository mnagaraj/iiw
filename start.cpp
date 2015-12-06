#include <time.h>
#include <stdio.h>
#include <iostream>
#include<fstream>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv/highgui.h>

#define IMG_H						180
#define	IMG_W						240
#define FRAME_MEM_SIZE				129600
#define COLOR_KEY_SIZE				8

using namespace std;
using namespace cv;


class get_descriptors
{

	public:
				FILE *vidFile;									// Pointer to video file
				unsigned char* vidDataBlock = NULL;				// Pointer to allocate memory for input video
				short* audDataBlock = NULL ;					// Pointer to allocate memory for input audio
				unsigned int* MotionIndexArray = NULL ;			// for storing the video's (frameCount - 1) number of motion indexes

				unsigned int vidMemSize = 0;					// size of input video in bytes
				size_t bytesCopied = 0;

				int RHistogram[256] ;					// Arrays for storing each video's frame histogram
				int GHistogram[256] ;
				int BHistogram[256] ;
				int frameCount = 0 ;							// Counts the number of frames in the video

				int frame(int,char*);
				void AnalyzeMotion(int,unsigned char*,char *);
				int AnalyzeColor(int, unsigned char*, char*) ;
				int dominantColor(int, unsigned char*, char*) ;
				void InitializeHistArray(unsigned int *);
				void ComputeGrayscaleHistogram(unsigned int *, unsigned char *,  char*,char );
				unsigned long RetrieveHistKeyValues(unsigned int* );
				void SaveToFile(unsigned int* ,int, char* );
};

int get_descriptors::frame(int count,char* file)

{

	if(count != 2)
	{
		printf("Error - Incorrect Parameter Usage:") ;
		printf("ProgramName Inputvideo") ;
		return 1 ;
	}

	cout << file <<endl;
	vidFile = fopen(file,"rb");
	if (vidFile == NULL)
	{
		cout << "Cannot open file: " << file <<endl;
		exit(1);
	}

	char * filename;
	filename = strtok (file,".") ;
	cout<<"filename= "<<filename<<endl;
	fseek (vidFile , 0 , SEEK_END);
	long vidSize = ftell (vidFile);
	rewind (vidFile);
	cout<<"size="<<vidSize<<endl;
	vidMemSize = vidSize ;
	vidDataBlock = (unsigned char*) calloc(vidSize + 1, sizeof(char));
	if(vidDataBlock == NULL)
	{
		cout<< "Memory allocation error. Program terminated";
		exit(1);
	}

	bytesCopied = fread(vidDataBlock, sizeof(unsigned char), vidMemSize, vidFile);

	if(bytesCopied != vidMemSize)
	{
		cout<< "Error reading file stream. Program terminated";
		exit(1);
	}

	frameCount = vidMemSize / FRAME_MEM_SIZE ;
	//char*tempo=filename;
	string x=(string)filename;
	//AnalyzeColor(frameCount,vidDataBlock,file);
	//AnalyzeMotion(frameCount,vidDataBlock,filename);
	char*temp;
	temp = new char[x.size() + 1];
	memcpy(temp, x.c_str(), x.size() + 1);
	//dominantColor(frameCount,vidDataBlock,filename);
	AnalyzeColor(frameCount,vidDataBlock,file);
	printf("\n\n\n::: Video Processing metrics :::\n\n");
	printf("%s%s%s", "Video filename: ", filename, "\n");
	printf("%s%f%s", "Video duration: ", (double)frameCount / 30.0, "\n");
	printf("%s%d%s", "Total Number of frames: ", frameCount, "\n") ;
	//printf("%s%d%s", "Number of features generated: ", frameCount * 2, "\n");
	//printf("%s%f%s", "Total processing time: ", execTime, " ms\n");
	//printf("%s%f%s", "Feature extraction time per frame: ", execTime/frameCount, " ms\n");
	return 0;
}



void get_descriptors::AnalyzeMotion(int frameCount, unsigned char* vidDataBlock, char* vidFileName)
{
	unsigned int difFrameAccum = 0 ;
	unsigned int iFrameMotionIndex = 0 ;

	unsigned int* MotionIndexArray = new unsigned int[frameCount-1] ;
	float* NMotionIndexArray = new float[frameCount - 1];
	unsigned int img[150][150],cimg[150][150];
	unsigned char* arr;
	unsigned char* prevFrameIterator = vidDataBlock ;		// Iterator to go through each frame
	unsigned char* nextFrameIterator = vidDataBlock + FRAME_MEM_SIZE ;

	unsigned char* prevByteIterator = prevFrameIterator ;
	unsigned char* nextByteIterator = nextFrameIterator ;

	int difByteValue = 0 ;

	// Build output filename
	//char* filename = new char[100] ;

	//memcpy(filename, (strtok(vidFileName,".")), 30) ;
	//strcat(filename, "query_motion.txt") ;
	char* temp;
	char* temp1;
	temp=vidFileName;
	temp=strtok(temp,"/");

	while (temp) {
	    //printf ("Token: %s\n", temp);
	    temp1=temp;
	    temp = strtok(NULL, "/");
	}
	cout<<"temo:"<<temp1<<endl;
	strcat(temp1, "_motion.txt") ;
	cout << temp1<<endl;
	char* x;
	x="/home/madhuri/db/";
	//cout<<x<<temp1;
	size_t len1 = strlen(x);
	size_t len2 = strlen(temp1);

	char *totalLine = (char*)malloc(len1 + len2 + 1);
	if (!totalLine) abort();

	memcpy(totalLine,x, len1);
	memcpy(totalLine + len1, temp1, len2);
	totalLine[len1 + len2] = '\0';

	cout<<"path= "<<totalLine;
	for(int i = 0 ; i < (frameCount - 1) ; i++)
	{

		for(int j = 0 ; j < FRAME_MEM_SIZE ; j ++)
		{
			difByteValue = (int)prevFrameIterator[j] - (int)nextFrameIterator[j] ;
			//printf("%d\n",difFrameAccum);
			difFrameAccum = difFrameAccum + abs(difByteValue) ;

		}
		//printf("%d\n",difFrameAccum);
		MotionIndexArray[i] = difFrameAccum / FRAME_MEM_SIZE ;
		//printf("%d\n",MotionIndexArray[i]);
		//MotionIndexArray[i]=255;
		prevFrameIterator = prevFrameIterator + FRAME_MEM_SIZE ;
		nextFrameIterator = nextFrameIterator + FRAME_MEM_SIZE ;
		difFrameAccum = 0 ;

	}

	int t=0;
	double tray[150];
	for(int i=0;i<150;i++)
	{
	  if((int)MotionIndexArray[i]>t)
	  {
		  //tray[i]=MotionIndexArray[i];
		  t=MotionIndexArray[i];
	  }
	}
	    cout << "The biggest number is: " << t << endl;

	for(int i=0;i<150;i++)
	{
		tray[i]=(double)MotionIndexArray[i]/(double)t;
		//cout<<MotionIndexArray[i]<<endl;
		cout<<tray[i]<<endl;
	}

	Mat A(150, 750, CV_8UC1,0.0f);
	A.create(150, 750, CV_8UC1);
	int b=0;

	for(int k=0;k<750;k+=5)
	{
		cout<<"k="<<k<<endl;
		for(int i=0;i<150;i++)
		{
			for(int j=k;j<5+k;j++)
			{
				//cout<<"b="<<j<<endl;
				//cout<<tray[b]*255<<endl;
				A.at<float>(i,j)=tray[b]*255;
			}
		}
		cout<<"b="<<b<<endl;
		b++;

	}

	imshow("buzz",A);
	waitKey(0);

	/*int cnt1=0;
	int cnt2=0;
	int cnt3=0;
	int cnt12=0;



	for(int l = 0 ;l < 150 ;l++)
	{
		for(int k = 0;k < 150;k++)
		{
			img[l][k]=0;

			if(MotionIndexArray[l] ==0)
			{
				cnt1++;
				img[l][k]=255;
			}

			else if(MotionIndexArray[l] > 0 && MotionIndexArray[l]<25)
			{
							cnt12++;
							img[l][k]=123;

			}
			else if(MotionIndexArray[l] >=25 && MotionIndexArray[l] <=50)
			{
				cnt2++;
				img[l][k]=255;
			}
			else
			{
				cnt3++;
				img[l][k]=0;
			}
			//img[l][k]=MotionIndexArray[l];
			//cout<<img[l][k];

		}
	}

	cout <<cnt1<<" "<<cnt2<<" "<<cnt3<<" "<<cnt12<<endl;*/
	/*float min = *min_element(MotionIndexArray, MotionIndexArray + frameCount-1);
	float max = *max_element(MotionIndexArray, MotionIndexArray + frameCount-1);

	for (int i = 0; i < (frameCount - 1); i++)
	{

		NMotionIndexArray[i] = ((float)(MotionIndexArray[i] - min)) / ((max - min));
	}*/

	//cv::imwrite("/home/madhuri/imgOut.bmp",  cv::Mat(150, CV_32FC1, MotionIndexArray));

	Mat s;
	//s= Mat(150,150,CV_8UC1,Scalar(0));
	//imshow("buzz",s);
	/*unsigned int imgs[8][8];


	for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				imgs[i][j]=(i+j);
				//cout <<imgs[i][j] <<'\t';
			}
			//cout<<endl;

		}

	imgs[7][7]=255;
	Mat im3(8, 8, CV_8UC1);
	int k=0;
*/
	/*for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				//if(j!=0){k=8*j;}
				s= Mat(1,8,CV_8UC1,Scalar(imgs[i][j]));
				im3.adjustROI(0, 0, 0,-7);
				s.copyTo(im3);
				//namedWindow("t",CV_WINDOW_AUTOSIZE);
				cout << s<<endl<<endl;

			}

		}*/
	//imshow("t",im3);
	//const cv::Mat s(cv::Size(150, 150), CV_16UC1, (unsigned char*)MotionIndexArray);


	/*int len=150;
	Mat m[150];
	for(int i=0;i<150;i++)
	{
		for(int j=0;j<150;j++)
		{
			m[i]= Mat(1,1,CV_8UC1,img[i][j]);
		}


	}

	for(int i=0;i<150;i++)
	{
		imshow("help",m[i]);
	}*/
	//cout<<"kkk"<<endl;
	  //cout<<"here1"<<endl;
	  //SaveToFile(MotionIndexArray, frameCount-1, totalLine);
	  //cout<<"here1"<<endl;
	//SaveToFileN(NMotionIndexArray, frameCount, filename) ;

}

void get_descriptors::SaveToFile(unsigned int *FilePtr, int frameCount,  char* HistFileName)
{
	FILE *HistFile;
	cout<<HistFileName<<endl;
	ofstream myfile;
	myfile.open(HistFileName,ios::out | ios::app);


	//if (myfile.is_open()) { /* ok, proceed with output */ }
	//HistFile = fopen(HistFileName,"w+");
	cout<<"cnt="<<frameCount<<endl;
	 if (myfile.is_open())
	{
	for(int i = 0; i < frameCount; i++)
	{
		//fprintf(HistFile,"%d\r\n",FilePtr[i]);

		myfile<<*(FilePtr+i)<<endl;
		// added code to write to console

		cout<<"Frame "<<i<<":"<<*(FilePtr+i)<<endl;
		//printf("%s%d%s", "Frame ", i, ": ", FilePtr[i], "\n");

		// added code to write to XML

	}
	//fclose(HistFile);
	myfile.close();
	}
	cout<<"here";
}

void get_descriptors::ComputeGrayscaleHistogram(unsigned int *HistPtr, unsigned char *vidDataBlock, char* histName,char flag)
{
	int tempByteValue = 0 ;
	unsigned long i = 0 ;
	unsigned char *ImgPixelIterator = vidDataBlock ;

	InitializeHistArray(HistPtr) ;

	for(i = 0; i < (FRAME_MEM_SIZE/3) ; i++)			// i+3 skips the other 2 colors bytes
	{
		tempByteValue = (int)*ImgPixelIterator ;	//retrieve the value of each byte
		HistPtr[tempByteValue] ++ ;					//increment by one the corresponding value in the histogram

		if((ImgPixelIterator + 3) != nullptr)
		{

			ImgPixelIterator = ImgPixelIterator + 3 ;	// Jump to the next -same color- byte
		}

			//cout<<"i="<<i<<endl;
		/*if(flag=='g')
		{
			i++;
		}
		else if(flag=='b')
		{
			i+=2;
		}*/
	}

	//SaveToFile(HistPtr, 256, histName) ;
}

void get_descriptors::InitializeHistArray(unsigned int *HistPtr)
{
	for(int i = 0 ; i < 256 ; i++)
	{
		HistPtr[i] = 0 ;
	}
}

unsigned long get_descriptors::RetrieveHistKeyValues(unsigned int* histPtr)
{
	unsigned long iHistKeyVal = 0 ;
	unsigned long iHistTempVal = 0 ;
	char tempVal[COLOR_KEY_SIZE] ;
	char colorKeyTrunc[COLOR_KEY_SIZE] ;
	char colorKeyStr[COLOR_KEY_SIZE*3] ;

	for(int j = 0 ; j < COLOR_KEY_SIZE ; j++)
	{
		tempVal[j] = 0x00 ;
		colorKeyTrunc[j] = 0x00 ;
	}
	for(int k = 0 ; k < COLOR_KEY_SIZE*3 ; k++)
	{
		colorKeyStr[k] = 0x00 ;
	}

	for(int i = 0 ; i < 5 ; i ++)
	{
		iHistTempVal = histPtr[i*32] ;
		//itoa(iHistTempVal,tempVal,10) ;
		sprintf(tempVal,"%d",iHistTempVal);
		//sprintf(iHistTempVal,tempVal,10) ;
		strcat(colorKeyStr, tempVal) ;
	}

	if(strlen(colorKeyStr) > COLOR_KEY_SIZE)
	{
		strncpy(colorKeyTrunc, colorKeyStr, COLOR_KEY_SIZE) ;
	}

	iHistKeyVal = atoi(colorKeyTrunc) ;

	return iHistKeyVal ;
}

int get_descriptors::dominantColor(int frameCount, unsigned char* vidDataBlock, char * vidFileName)
{
	cout<<"buzz"<<endl;
	//unsigned char* byteIterator = vidDataBlock ;
	int tempByteValue = 0,tempByteValue1=0,tempByteValue2=0,tempByteValue3=0 ;
	unsigned long i = 0;
	int count=0;
	unsigned char *ImgPixelIterator = vidDataBlock ;
	unsigned int HistPtr[FRAME_MEM_SIZE];

		InitializeHistArray(HistPtr) ;

		for(i = 0; i < (FRAME_MEM_SIZE) ; i++)			// i+3 skips the other 2 colors bytes
		{
			/*tempByteValue1 = (int)*ImgPixelIterator ;	//retrieve the value of each byte
			tempByteValue2 = (int)*(ImgPixelIterator+1) ;
			tempByteValue3 = (int)*(ImgPixelIterator+2) ;
			tempByteValue=(tempByteValue1+tempByteValue2+tempByteValue3)/3;*/
			//tempByteValue = ((tempByteValue1&0x0ff)<<16)|((tempByteValue2&0x0ff)<<8)|(tempByteValue3&0x0ff);
			tempByteValue1 = (int)*ImgPixelIterator ;
			HistPtr[tempByteValue] ++ ;					//increment by one the corresponding value in the histogram

			if((ImgPixelIterator + 3) != nullptr)
			{
				ImgPixelIterator = ImgPixelIterator + 3 ;	// Jump to the next -same color- byte
			}


			count++ ;
		}

		cout<<"count="<<count<<endl;
	Mat A(180, 240, CV_8UC1,DataType<int>::type);
	//A.create(180, 240, CV_8UC3);
	//imshow("x",A);
	int k=0;

	for(int i=0;i<180;i++)
	{
		for(int j=0;j<240;j++)
		{

			/*r=	(int)*byteIterator;

			g=	(int)*byteIterator;

			b=	(int)*byteIterator;

			rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);*/

			A.at<int>(i,j)=HistPtr[k];
			//cout<<"i="<<i<<"j="<<j<<"hist="<<HistPtr[k];
			k++;
		}
		//cout<<rgb<<endl;
	}
	//cout<<"Here"<<A<<endl;
	//imshow("x",A);
	 Mat image_hsv;

	 //Mat image_hsv(180, 240, CV_8UC3,DataType<long>::type);
	 cvtColor(A, image_hsv, CV_BGR2HSV);
	 // Quanta Ratio
	     int scale = 10;

	     int hbins = 36, sbins = 25, vbins = 25;
	     int histSize[] = {hbins, sbins, vbins};

	     float hranges[] = { 0, 180 };
	     float sranges[] = { 0, 256 };
	     float vranges[] = { 0, 256 };

	     const float* ranges[] = { hranges, sranges, vranges };
	     MatND hist;

	     int channels[] = {0, 1, 2};

	     calcHist( &image_hsv, 1, channels, Mat(), // do not use mask
	              hist, 3, histSize, ranges,
	              true, // the histogram is uniform
	              false );

	     int maxVal = 0;

	     int hue = 0;
	     int saturation = 0;
	     int value = 0;

	     for( int h = 0; h < hbins; h++ )
	     {
	         for( int s = 0; s < sbins; s++ )
	         {
	              for( int v = 0; v < vbins; v++ )
	                 {
	                       int binVal = hist.at<int>(h, s, v);
	                       if(binVal > maxVal)
	                       {
	                           maxVal = binVal;

	                           hue = h;
	                           saturation = s;
	                           value = v;
	                       }
	                 }
	         }
	     }

	     hue = hue * scale; // angle 0 - 360
	     saturation = saturation * scale; // 0 - 255
	     value = value * scale; // 0 - 255

	cout<<maxVal<<endl;
}


int get_descriptors::AnalyzeColor(int frameCount, unsigned char* vidDataBlock, char * vidFileName)
{
	cout<<"buzz"<<endl;
	unsigned char* byteIterator = vidDataBlock ;
	ofstream myfile;


		unsigned int RHistogram[256] ;
		unsigned int GHistogram[256] ;
		unsigned int BHistogram[256] ;

		unsigned long* HistKeyArray = new unsigned long[frameCount*3] ;
		unsigned long colorKey = 0 ;

		int iRKey = 0 ;
		int iGKey = 0 ;
		int iBKey = 0 ;
		unsigned int rgb;

		// Build output filename
		/*char* filename = new char[100] ;
		memcpy(filename, (strtok(file,".")), 30) ;
		strcat(filename, "Color.txt") ;

		cout<<filename<<endl;*/

			cout<<"org"<<vidFileName<<endl;
			char* temp;
			char* temp1;
			temp=vidFileName;
			temp=strtok(temp,"/");
			cout<<"temp="<<temp<<endl;
			while (temp) {
			    //printf ("Token: %s\n", temp);
			    temp1=temp;
			    temp = strtok(NULL, "/");
			}
			cout<<"temo:"<<temp1<<endl;
			strcat(temp1, "_colour.txt") ;
			cout << temp1<<endl;
			char* x;
			x="/home/madhuri/db/";
			//cout<<x<<temp1;
			size_t len1 = strlen(x);
			size_t len2 = strlen(temp1);

			char *totalLine = (char*)malloc(len1 + len2 + 1);
			if (!totalLine) {abort();}


			memcpy(totalLine,x, len1);

			memcpy(totalLine + len1, temp1, len2);

			totalLine[len1 + len2] = '\0';
			cout<<"lol"<<endl;
			cout<<"path= "<<totalLine<<endl;


		//filename = "K:\\Academics\\2nd sem\\CSCI576\\assignments\\project\\GUI\\query\\query_color.txt";
		// Initialize histogram arrays

		InitializeHistArray(RHistogram) ;
		InitializeHistArray(GHistogram) ;
		InitializeHistArray(BHistogram) ;

		cout<<"framcnt="<<frameCount<<endl;
		myfile.open(totalLine,ios::out);

		for(int i = 0 ; i < frameCount ; i++)
		{
			//cout<<"itr:"<<(void*)byteIterator<<endl;
			ComputeGrayscaleHistogram(RHistogram, byteIterator, "/home/madhuri/db/RedHist.txt",'r') ;							// RED Histogram

			//cout<<"itr:"<<(void*)byteIterator<<endl;
			//cout<<"itr:"<<(void*)byteIterator+FRAME_MEM_SIZE/3<<endl;
			ComputeGrayscaleHistogram(GHistogram, byteIterator+1, "/home/madhuri/db/GreenHist.txt",'g') ;		// GREEN Histogram
			//cout<<"beforei="<<i<<endl;
			ComputeGrayscaleHistogram(BHistogram, byteIterator+2, "/home/madhuri/db/BlueHist.txt",'b') ;		// BLUE Histogram

			//cout<<"i="<<i<<endl;
			byteIterator = byteIterator + FRAME_MEM_SIZE ;

			iRKey = RetrieveHistKeyValues(RHistogram) ;
			iGKey = RetrieveHistKeyValues(GHistogram) ;
			iBKey = RetrieveHistKeyValues(BHistogram) ;
			//cout<<"count:"<<i<<endl;
			colorKey = iRKey + iGKey + iBKey ; //Al code
			//cout<<"i="<<i<<"r="<<iRKey<<"g="<<iGKey<<"b="<<iBKey<<endl;
			rgb = ((iRKey&0x0ff)<<16)|((iGKey&0x0ff)<<8)|(iBKey&0x0ff);
			cout<<"rgb="<<rgb<<"i="<<i<<endl;
			HistKeyArray[i] = rgb ;
			myfile<<rgb<<endl;

			//HistKeyArray[i] = colorKey ;//Al code

			/*HistKeyArray[i] = iRKey; //NL
			HistKeyArray[i + frameCount] = iGKey; //NL
			HistKeyArray[i + 2 * frameCount] = iBKey; //NL*/
			//cout<<"lol"<<endl;

		}
		myfile.close();

		//cout<<"lol"<<endl;
		//SaveToFile((unsigned int*)HistKeyArray, frameCount, totalLine) ;



	/*Mat src, dst;
	FILE *name = fopen(file,"rb");
	//src = imread( "/home/madhuri/Downloads/video_samples/animation1.v576.rgb", 1 );
	//src = imdecode( "/home/madhuri/Downloads/video_samples/animation1.v576.rgb", 1 );
	src = VideoCapture('/home/madhuri/Downloads/video_samples/animation1.v576.rgb');

	if( !src.data )
	  { return -1; }

	vector<Mat> bgr_planes;
	split( src, bgr_planes );
	int histSize = 256; //from 0 to 255
	float range[] = { 0, 256 } ; //the upper boundary is exclusive
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	Mat b_hist, g_hist, r_hist;
	/// Compute the histograms:
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	// Draw the histograms for R, G and B
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	cout<<g_hist;

	/// Draw for each channel
	  for( int i = 1; i < histSize; i++ )
	  {
	      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
	                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
	                       Scalar( 255, 0, 0), 2, 8, 0  );
	      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
	                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
	                       Scalar( 0, 255, 0), 2, 8, 0  );
	      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
	                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
	                       Scalar( 0, 0, 255), 2, 8, 0  );
	  }

	  /// Display
	  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	  imshow("calcHist Demo", histImage );

	  waitKey(0);*/

	  return 0;
}

int main(int argc, char *argv[])
{
	get_descriptors desc;
	desc.frame(argc,argv[1]);
}
