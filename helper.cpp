/*
 * helper.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: madhuri
 */


#include<iostream>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
#include<sstream>
#include<cstring>
#include<stdio.h>
#include<fstream>
#include<math.h>
#include<algorithm>
#include<vector>

using namespace std;

double res_color[15];
double res1[14];
double res2[14];

void comparison(double* audio,double* video,double* color,string filename[])
{
	double final_results[15];
	double temp[15];
	for(int i = 0; i<15 ;i++)
	{
		double sum=audio[i]+video[i]+(1/color[i]);
		double mean=sum/3.0;
		final_results[i]=mean;
		temp[i]=mean;
	//cout<<audio[i]<<":"<<video[i]<<":"<<color[i]<<endl;
	//cout<<mean<<endl;
	}
	//std::cout << "The least element is "  << *std::min_element(final_results,final_results+15) << '\n';
	std::sort(final_results,final_results+15);
	int one,two,three,four;
	for(int k=0;k<15;k++)
	{
		if(temp[k] == final_results[0])
		{
			one=k;
		}
		else if(temp[k] == final_results[1])
		{
			two=k;
		}
		else if(temp[k] == final_results[2])
		{
			three=k;
		}
		else if(temp[k] == final_results[3])
		{
			four=k;
		}
	}

	cout<<"****************QUERY MATCHING RESULTS****************"<<endl;
	cout<<"MATCH 1 = "<<filename[one]<<endl; //<<":"<<one<<endl;
	cout<<"MATCH 2 = "<<filename[two]<<endl;  //":"<<two<<endl;
	cout<<"MATCH 3 = "<<filename[three]<<endl; //":"<<three<<endl;
	cout<<"MATCH 4 ="<<filename[four]<<endl;  //":"<<four<<endl;
	cout<<"******************************************************";
	   /*for (int index = smallestIndex; index < 15; index++) {
	        if (final_results[smallestIndex] > final_results[index]) {
	            smallestIndex = index;
	        }
	    }*/
	    	//cout<<filename[smallestIndex]<<endl;
	/*iterator result= *std::min_element(final_results,final_results+15);
	 std::cout << "min element at: " << std::distance(final_results, result);*/
}



double* compare_audio(string filename[])
  {

 	 for(int i=0;i<14;i++)
 	 {
 		 ifstream myfile1,myfile2;
 		 string temp=filename[i];
 		 string x="/home/madhuri/db/audio_desc/";
 		 string path=x+temp+"_audio.txt";
 		 const char* p=path.c_str();
 		 //cout<<"path="<<path<<endl;
 		 myfile1.open(p);
 		 myfile2.open("/home/madhuri/db/sports_test_audio.txt");
 		 //myfile2.open("/home/madhuri/db/result/query_audio.txt"); //FOR QUERY VIDEO
 		 string a,b;
 		 double c,d;
 		 double err=0;
 		 int count=0,j=0;
 		 double arr1[3],arr2[3];
 		 std::size_t found;
 		 while(myfile1 >> a)
 		 {
 			 	myfile2>>b;
 			 	c=atof(a.c_str())-atof(b.c_str());
 			 	double qc=c*c;
 			 	err+=qc;
 			 	count++;
 			 		//cout<<"a:::"<<a<<" b:::"<<b<<endl;

 		 }
 		 //cout<<"Errorr::"<<err<<" Count::"<<count<<endl;
 		 res1[i]=err/count;
 		 //cout<<fixed;
 		 //cout<<res1[i]<<endl;
 	 }
 	 //cout<<fixed;
 	 //std::cout << "The least element is "  << *std::min_element(res1,res1+14) << '\n';
 	 return res1;
  }


double* compare_video(string filename[])
  {

 	 for(int i=0;i<15;i++)
 	 {
 		 ifstream myfile1,myfile2;
 		 string temp=filename[i];
 		 string x="/home/madhuri/db/motion_desc/";
 		 string path=x+temp+"_motion.txt";
 		 const char* p=path.c_str();
 		 //cout<<"path="<<path<<endl;
 		 myfile1.open(p);
 		 myfile2.open("/home/madhuri/db/sports_test_motion.txt");
 		 //myfile2.open("/home/madhuri/db/result/query_motion.txt"); //FOR QUERY VIDEO
 		 string a,b;
 		 double c,d;
 		 double err=0;
 		 int count=0,j=0;
 		 double arr1[3],arr2[3];
 		 std::size_t found;
 		 while(myfile1 >> a)
 		 {
 			 	myfile2>>b;

 			 		//cout<<"else"<<endl;
 			 		c=atof(a.c_str())-atof(b.c_str());
 			 		double qc=c*c;
 			 		err+=qc;
 			 		count++;
 			 		//cout<<"a:::"<<a<<" b:::"<<b<<endl;

 		 }
 		// cout<<"Errorr::"<<err<<" Count::"<<count<<endl;
 		 res2[i]=err/count;
 		 //cout<<fixed;
 		 //cout<<res[i]<<endl;
 	 }
 	 //cout<<fixed;
 	 //std::cout << "The least element is "  << *std::min_element(res2,res2+15) << '\n';
 	 return res2;
  }

double* compare_color(string filename[])
  {

 	 for(int i=0;i<15;i++)
 	 {
 		 ifstream myfile1,myfile2;
 		 string temp=filename[i];
 		 string x="/home/madhuri/db/color_desc/";
 		 string path=x+temp+"_color.txt";
 		 const char* p=path.c_str();
 		 //cout<<"path="<<path<<endl;
 		 myfile1.open(p);
 		 myfile2.open("/home/madhuri/db/sports_test_color.txt");
 		 //myfile2.open("/home/madhuri/db/result/query_color.txt"); //FOR QUERY VIDEO
 		 string a,b;
 		 double c,d;
 		 double err=0;
 		 int count=0,j=0;
 		 double arr1[3],arr2[3];
 		 std::size_t found;
 		 while(myfile1 >> a)
 		 {
 			 	myfile2>>b;
 			 	//cout <<a<<" "<<b<<endl;
 			 	/*ostringstream sstream;
 			 	sstream << a;
 			 	string a1 = sstream.str();*/
 			 	found=a.find(':');
 			 	//cout<<found<<endl;
 			 	if (found!=std::string::npos)
 			 	{
 			 		//cout<<"IF"<<endl;
 			 		char* pch;
 			 		const char* pch1;
 			 		pch1=a.c_str();
 			 		char* str=const_cast<char*>(pch1);
 			 		pch = strtok(str,":");
 			 		  while (pch != NULL)
 			 		  {

 			 			 sscanf(pch,"%lf",&arr1[j]);
 			 			 j++;

 			 		    pch = strtok (NULL, ":");
 			 		  }

 			 		  j=0;

 			 		  found=b.find(':');
 			 		  if (found!=std::string::npos)
 			 		  {
 			 			  char* pch;
 			 			  const char* pch1;
 			 			  pch1=b.c_str();
 			 			  char* str=const_cast<char*>(pch1);
 			 			  pch = strtok(str,":");
 			 			  while (pch != NULL)
 			 			  {
 			 				  //printf ("%s\n",pch);
 			 				  //cout<<"pch"<<pch<<endl;
 			 				  sscanf(pch,"%lf",&arr2[j]);
 			 				  j++;
 			 				  //cout<<"arr2"<<arr2[j]<<endl;
 			 				  //arr2[j++]=*pch;
 			 				  pch = strtok (NULL, ":");
 			 			  }

 			 			  for(int k=0;k<3;k++)
 			 			  {
 			 				  //cout<<"else"<<endl;
 			 				 // cout<<"arr1:"<<arr1[k]<<"arr2:"<<arr2[k]<<endl;
 			 				  c=arr1[k]-arr2[k];
 			 				  //cout<<"diff:"<<c<<endl;
 			 				  double qc=c*c;
 			 				  err+=qc;
 			 			  }
 			 		  }
 			 		  j=0;
 			 		  count++;
 			 	}

 			 	else
 			 	{
 			 		//cout<<"else"<<endl;
 			 		c=atof(a.c_str())-atof(b.c_str());
 			 		double qc=c*c;
 			 		err+=qc;
 			 		count++;
 			 		//cout<<"a:::"<<a<<" b:::"<<b<<endl;
 			 	}
 		 }
 		 //cout<<"Errorr::"<<err<<" Count::"<<count<<endl;
 		 res_color[i]=err/count;
 		 //cout<<fixed;
 		 //cout<<res[i]<<endl;
 	 }
 	 //cout<<fixed;
 	 //std::cout << "The least element is "  << *std::min_element(res_color,res_color+15) << '\n';
 	 return res_color;
  }







 int help()
 {


	 DIR *dir;
	 string audio_files[14],files[15];
	 struct dirent *ent;
	 int k=0;
	 if ((dir = opendir ("/home/madhuri/db/audio_desc")) != NULL) {
	   /* print all the files and directories within directory */
	   while ((ent = readdir (dir)) != NULL) {
	     //printf ("%s\n", ent->d_name);
		   if(strcmp(ent->d_name,".")==0)
		   {	continue;

		   }
		   if(strcmp(ent->d_name,"..")==0)
		   {
			   continue;
		   }
		   audio_files[k++]=ent->d_name;

	   }
	   closedir (dir);
	 } else {
	   /* could not open directory */
	   perror ("");
	   return EXIT_FAILURE;
	 }

	 //Get the main files folder
	 for(int m=0;m<14;m++)
	 {

		 istringstream is(audio_files[m]);
		 string part;
		 getline(is, part, '_');
		 files[m]=part;

	 }
	files[14]="sports1";
	double* arr_audio=compare_audio(files);
	double* arr_video=compare_video(files);
	double* arr_color=compare_color(files);
	comparison(arr_audio,arr_video,arr_color,files);

	return 1;
 }
