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
#include "helper.h"

#define IMG_H						180
#define	IMG_W						240
#define FRAME_MEM_SIZE				129600
#define COLOR_KEY_SIZE				8

using namespace std;
using namespace cv;

// WAVE PCM soundfile format (you can find more in https://ccrma.stanford.edu/courses/422/projects/WaveFormat/ )
typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size; // subchunk2_size denotes the number of samples.
    //char data; // actual data : Added by tarmizi
} header;

typedef struct header_file* header_p;

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

				int frame(int,char*,char*);
				void AnalyzeMotion(int,unsigned char*,char *);
				int AnalyzeColor(int, unsigned char*, char*) ;
				int AnalyzeAudio(char*) ;
				void InitializeHistArray(unsigned int *);
				void ComputeGrayscaleHistogram(unsigned int *, unsigned char *,  char*,char );
				unsigned long RetrieveHistKeyValues(unsigned int* );
				void SaveToFile(unsigned int* ,int, char* );
};

int get_descriptors::frame(int count,char* file,char* audiofile)

{

	if(count != 3)
	{
		printf("Error - Incorrect Parameter Usage:") ;
		printf("ProgramName Inputvideo InputAudio") ;
		return 1 ;
	}

	cout << file <<endl;
	cout <<audiofile<<endl;
	vidFile = fopen(file,"rb");
	if (vidFile == NULL)
	{
		cout << "Cannot open file: " << file <<endl;
		exit(1);
	}

	char * filename;
	char * audio;
	filename = strtok (file,".") ;
	//audio = strtok (audiofile,".") ;
	cout<<"filename= "<<filename<<endl;
	//cout<<"audiofilename= "<<audio<<endl;
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

	AnalyzeMotion(frameCount,vidDataBlock,filename);
	char*temp;
	temp = new char[x.size() + 1];
	memcpy(temp, x.c_str(), x.size() + 1);
	//AnalyzeColor(frameCount,vidDataBlock,file);
	//AnalyzeAudio(audiofile);
	printf("\n\n\n::: Video Processing metrics :::\n\n");
	printf("%s%s%s", "Video filename: ", filename, "\n");
	printf("%s%f%s", "Video duration: ", (double)frameCount / 30.0, "\n");
	printf("%s%d%s", "Total Number of frames: ", frameCount, "\n") ;
	//printf("%s%d%s", "Number of features generated: ", frameCount * 2, "\n");
	//printf("%s%f%s", "Total processing time: ", execTime, " ms\n");
	//printf("%s%f%s", "Feature extraction time per frame: ", execTime/frameCount, " ms\n");
	return 0;
}

int get_descriptors::AnalyzeAudio(char* name)
{

		ofstream myFile1,myFiletemp;

			myFiletemp.open("mizi.txt",ios::out);
			cout<<name<<endl;

		FILE * infile = fopen("/home/madhuri/Downloads/audio_wav/animation1.wav","rb");		// Open wave file in read mode
		//FILE * infile = fopen("/home/madhuri/Downloads/testing/interview_test.wav","rb");		// Open wave file in read mode
		double bucket[128];
		int BUFSIZE = 256;					// BUFSIZE can be changed according to the frame size required (eg:512)
		int count = 0;						// For counting number of frames in wave file.
		short int buff16[BUFSIZE];				// short int used for 16 bit as input data format is 16 bit PCM audio
		header_p meta = (header_p)malloc(sizeof(header));	// header_p points to a header struct that contains the wave file metadata fields
		int nb;							// variable storing number of bytes returned

		if (infile)
		{
			fread(meta, 1, sizeof(header), infile);
			//fwrite(meta,1, sizeof(*meta), outfile);


			cout << "first chunk is :" << sizeof(meta->chunk_id) << " bytes in size" << endl;
			cout << "The file is a :" << meta->chunk_id << " format" << endl;
			cout << " Size of Header file is "<<sizeof(*meta)<<" bytes" << endl;
			cout << " Sampling rate of the input wave file is "<< meta->sample_rate <<" Hz" << endl;
			cout << " Number of bits per sample is: "<< meta->bits_per_sample <<"bits" << endl;
			cout << " Size of data in the audio is: " << sizeof(meta->subchunk2_size)<< " bytes" << endl;
			cout << " The number of channels of the file is "<< meta->num_channels << " channels" << endl;
			cout << " The audio format is PCM:"<< meta->audio_format << endl;
	        //cout << " The size of actual data is "<< sizeof(meta->data) << "bytes" << endl;



			while ((nb = fread(buff16,sizeof(short int),BUFSIZE,infile))>0)          //(nb = fread(buff16,1,BUFSIZE,infile))>0
			{

				count++;
	                            // Incrementing > of frame
	            for (int i = 0; i<BUFSIZE; i++) //  BUFSIZE = 256, meta->num_channels = 1
	                {

	                       int c = (buff16[i]<<8) | buff16[1+i];
	                        double t = c/32768.0;

	                        myFiletemp << t << endl;

	                }
			}


		cout << " Number of frames in the input wave file are " <<count << endl;

		std::ifstream infile("mizi.txt");
		double a;
		double sum=0;
		int c=0;

		for(int i=0;i<331264;i=i+2588)
		{	sum=0;
			for(int j=0;j<2588;j++)
			{
				infile >> a;

				sum+=a;
			}

			//cout<<fixed;
			bucket[c]=sum;
			c++;
		}
		char* temp;
		char* temp1;
		temp=name;
		temp = strtok (temp,".") ;
		temp=strtok(temp,"/");

		while (temp) {
			    //printf ("Token: %s\n", temp);
			    temp1=temp;
			    temp = strtok(NULL, "/");
			}
			cout<<"temo:"<<temp1<<endl;
			strcat(temp1, "_audio.txt") ;
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
			myFile1.open("/home/madhuri/db/audio_desc/animation1_audio.txt",ios::out);
		std::cout << "The largest element is "  << *std::max_element(bucket,bucket+128) << '\n';
		double max=*std::max_element(bucket,bucket+128);
		for(int i=0;i<128;i++)
		{
			myFile1 << bucket[i] << endl;
			bucket[i]=bucket[i]/max;
			cout<<bucket[i]<<endl;

		}
	}

		double tray[128];

			for(int i=0;i<150;i++)
			{
				tray[i]= bucket[i];
				//cout<<MotionIndexArray[i]<<endl;
				cout<<tray[i]<<endl;
			}

		Mat A(150, 640, CV_8U,0.0f);
			A.create(150, 640, CV_8U);
			int b=0;

			for(int k=0;k<640;k+=5)
			{
				cout<<"k="<<k<<endl;
				for(int i=0;i<150;i++)
				{
					for(int j=k;j<5+k;j++)
					{
						//cout<<"b="<<j<<endl;
						//cout<<tray[b]*255<<endl;
						A.at<uchar>(i,j)= tray[b]*255;
					}
				}
				cout<<"b="<<b<<endl;
				b++;

			}

			imshow("Audio Descriptor",A);
			waitKey(0);


	return 0;
}

void get_descriptors::AnalyzeMotion(int frameCount, unsigned char* vidDataBlock, char* vidFileName)
{
	unsigned int difFrameAccum = 0 ;
	unsigned int iFrameMotionIndex = 0 ;
	ofstream myfile2;
	unsigned int* MotionIndexArray = new unsigned int[frameCount-1] ;
	float* NMotionIndexArray = new float[frameCount - 1];
	unsigned int img[150][150],cimg[150][150];
	unsigned char* arr;
	unsigned char* prevFrameIterator = vidDataBlock ;		// Iterator to go through each frame
	unsigned char* nextFrameIterator = vidDataBlock + FRAME_MEM_SIZE ;

	unsigned char* prevByteIterator = prevFrameIterator ;
	unsigned char* nextByteIterator = nextFrameIterator ;

	int difByteValue = 0 ;

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

	    myfile2.open("/home/madhuri/db/drama_test_motion.txt",ios::out);

	for(int i=0;i<150;i++)
	{
		tray[i]=(double)MotionIndexArray[i]/(double)t;
		//cout<<MotionIndexArray[i]<<endl;
		cout<<tray[i]<<endl;
		myfile2<<MotionIndexArray[i]<<endl;

	}
	myfile2.close();

	Mat A(150, 750, CV_8U,0.0f);
	A.create(150, 750, CV_8U);
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
				A.at<uchar>(i,j)= tray[b]*255;
			}
		}
		cout<<"b="<<b<<endl;
		b++;

	}

	imshow("Motion Descriptor",A);
	waitKey(0);
}

void get_descriptors::SaveToFile(unsigned int *FilePtr, int frameCount,  char* HistFileName)
{
	cout<<HistFileName<<endl;
	ofstream myfile;
	myfile.open(HistFileName,ios::out | ios::app);

	if (myfile.is_open())
	{
		for(int i = 0; i < frameCount; i++)
		{
			myfile<<*(FilePtr+i)<<endl;
			cout<<"Frame "<<i<<":"<<*(FilePtr+i)<<endl;
		}
	}
	myfile.close();
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
		sprintf(tempVal,"%d",iHistTempVal);
		//cout<<"ihisttempval="<<iHistTempVal<<"tempval="<<tempVal<<endl;
		strcat(colorKeyStr, tempVal) ;
	}

	if(strlen(colorKeyStr) > COLOR_KEY_SIZE)
	{
		strncpy(colorKeyTrunc, colorKeyStr, COLOR_KEY_SIZE) ;
		//cout<<"colortrunc="<<colorKeyTrunc<<endl;
	}

	iHistKeyVal = atoi(colorKeyTrunc) ;
	//cout<<"val="<<iHistKeyVal<<endl;

	return iHistKeyVal ;
}

int get_descriptors::AnalyzeColor(int frameCount, unsigned char* vidDataBlock, char * vidFileName)
{
	cout<<"buzz"<<endl;
	unsigned char* byteIterator = vidDataBlock ;
	ofstream myfile;


		unsigned int RHistogram[256] ;
		unsigned int GHistogram[256] ;
		unsigned int BHistogram[256] ;

		//unsigned long* HistKeyArray = new unsigned long[frameCount*3] ;
		double HistKeyArray[150];
		//double finalbar[150];
		unsigned long colorKey = 0 ;

		int iRKey = 0 ;
		int iGKey = 0 ;
		int iBKey = 0 ;
		unsigned int rgb;
		/*
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
			cout<<"path= "<<totalLine<<endl;*/

		InitializeHistArray(RHistogram) ;
		InitializeHistArray(GHistogram) ;
		InitializeHistArray(BHistogram) ;

		cout<<"framcnt="<<frameCount<<endl;
		myfile.open("/home/madhuri/db/interview_test_color.txt",ios::out);

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

			/*for(int i=0;i<43200;i++)
			{
				cout <<RHistogram[i]<<endl;
			}*/
			//int x=sizeof(RHistogram) / 4;
			//cout<<"x="<<x<<endl;
			//std::cout << "The largest element is "  << *std::max_element(RHistogram,RHistogram+256) << endl;

			iRKey = RetrieveHistKeyValues(RHistogram) ;
			iGKey = RetrieveHistKeyValues(GHistogram) ;
			iBKey = RetrieveHistKeyValues(BHistogram) ;

			/*iRKey=*std::max_element(RHistogram,RHistogram+256);
			iGKey=*std::max_element(GHistogram,GHistogram+256);
			iBKey=*std::max_element(BHistogram,BHistogram+256);*/

			cout <<"keys:"<< iRKey << ":"<<iGKey <<":" << iBKey <<":"<< endl;
			myfile << iRKey << ":"<<iGKey <<":" << iBKey << endl;
			//cout<<"count:"<<i<<endl;
			//colorKey = iRKey + iGKey + iBKey ; //Al code
			//cout<<"i="<<i<<"r="<<iRKey<<"g="<<iGKey<<"b="<<iBKey<<endl;
			rgb = ((iRKey&0x0ff)<<16)|((iGKey&0x0ff)<<8)|(iBKey&0x0ff);
			cout<<"rgb="<<rgb<<"i="<<i<<endl;
			HistKeyArray[i] = rgb ;
		}

		myfile.close();

		std::cout << "The largest element is "  << *std::max_element(HistKeyArray,HistKeyArray+150) << '\n';
		double max=*std::max_element(HistKeyArray,HistKeyArray+150);

		double trial[150];
		for(int i=0;i<150;i++)
		{
			trial[i]=HistKeyArray[i]/max;
			cout<<HistKeyArray[i]<<endl;

			//myfile<<rgb<<endl;
		}

		//Display barcode
		Mat A(150, 750, CV_8U,0.0f);
		A.create(150, 750, CV_8U);
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
									A.at<uchar>(i,j)= trial[b]*255;
					}
			}
							//cout<<"b="<<b<<endl;
			b++;

		}

		imshow("Color Descriptor",A);
		waitKey(0);

		//cout<<"lol"<<endl;
		//SaveToFile((unsigned int*)HistKeyArray, frameCount, totalLine) ;

	  return 0;
}

int main(int argc, char *argv[])
{
	get_descriptors desc;

	//cout<<"x="<<x<<endl;
	//desc.frame(argc,argv[1],argv[2]);
	help();
}
