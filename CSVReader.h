/***** CSVReader.h *****/

// Built upon and inspired by:
//	- https://github.com/abhinav1602/.csv-to-.cpp-2D-Array/blob/master/main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <ctype.h>
#include <stdlib.h>
#include <vector>
using namespace std;

// Function to get the number of rows of .csv file
int getSizeCSV(string filename){
		
	string line;
	// int i=0;
	int sizeCSV = 0;
	// string num = "";
	// float arr[1000][10];
	// std::vector<float> gData;
	
	ifstream infile (filename);

	if(infile.is_open())
	{
		while (getline(infile,line))
			{
			    //increase row number for array
			    sizeCSV++;
			}
		// Remove first row (header) and get number of rows
		sizeCSV -= 1;
		//close the file
		infile.close();
	}
	return sizeCSV;
}

// Function to get data from one collumn of the .csv file
std::vector<float> getDataCSV(string filename, int column){
		
	string line;
	int i=0, j=0, k=0, len = 0, last=0;
	int SizeCSV = 0;
	string num = "";
	float arr[1000][10];
	std::vector<float> gData;
	
	ifstream infile (filename);

	if(infile.is_open())
	{
	while (getline(infile,line) )
	{
	   k=0,last=0,j=0;
	   len=line.length();
	
	    while(k!=len){
	            if(line[k]==','||k==len-1){
	                //for converting string into number
	                arr[i][j]=atof(num.append(line,last,k).c_str());
	                //Emtying string for getting next data
	                num="";
	                //increasing column number after saving data
	                j++;
	                if(k!=len-1)
	                last=k+1;
	            }
	            k++;
	    }
	    //increase row number for array
	    i++;
	}
	}
	else cout << "Unable to open file";
	
	// Remove first row (header) and get number of rows
	SizeCSV = getSizeCSV(filename);
	
	// Save values from column to global array
	for(int x=0; x < SizeCSV; x++){
	  gData.push_back(arr[x+1][column]);
	}
	
	return gData;
}
