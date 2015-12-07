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


using namespace std;

 int help()
 {

 /*files=[{"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_color.txt"},
	               {"/home/madhuri/db/animation1_audio.txt"},
	               {"/home/madhuri/db/animation2_motion.txt"},
	               {"/home/madhuri/db/animation2_color.txt"},
	               {"/home/madhuri/db/animation2_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"},
	               {"/home/madhuri/db/animation1_motion.txt"}];*/

	 DIR *dir;
	 string files[15];
	 struct dirent *ent;
	 int k=0;
	 if ((dir = opendir ("/home/madhuri/db/color_desc")) != NULL) {
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
		   files[k++]=ent->d_name;

	   }
	   closedir (dir);
	 } else {
	   /* could not open directory */
	   perror ("");
	   return EXIT_FAILURE;
	 }

	 cout<<k<<endl;
	 double res[15];
	 for(int i=0;i<15;i++)
	 {
		 ifstream myfile1,myfile2;
		 string temp=files[i];
		 string x="/home/madhuri/db/color_desc/";
		 string path=x+temp;
		 const char* p=path.c_str();
		 cout<<"path="<<path<<endl;
		 myfile1.open(p);
		 myfile2.open("/home/madhuri/db/interview_test_color.txt");
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
			 		    //printf ("%s\n",pch);
			 			  //cout<<"pch="<<pch<<endl;
			 			 sscanf(pch,"%lf",&arr1[j]);
			 			 j++;
			 		    //arr1[j++]=pch;
			 		    //cout<<"arr1:"<<arr1[j]<<endl;
			 		    pch = strtok (NULL, ":");
			 		  }

			 		  j=0;
			 		  /*sstream << b;
			 			string b1 = sstream.str();*/
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
			 		cout<<"else"<<endl;
			 		c=atof(a.c_str())-atof(b.c_str());
			 		double qc=c*c;
			 		err+=qc;
			 		count++;
			 		//cout<<"a:::"<<a<<" b:::"<<b<<endl;
			 	}
		 }
		 cout<<"Errorr::"<<err<<" Count::"<<count<<endl;
		 res[i]=err/count;
		 cout<<fixed;
		 cout<<res[i]<<endl;
	 }
	 std::cout << "The least element is "  << *std::min_element(res,res+15) << '\n';
      return 1;
 }


