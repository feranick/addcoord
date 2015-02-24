//*****************************************************************************
//                                                         
//		Addcoord
//
//      Add coordinates to txt matrix for LabSpec Maps
//														 
//		v. 1.3 - 20150224
//
//		2013-2015 (C) Copyright - Nicola Ferralis <ferralis@mit.edu>
//                                                        
//		This program (source code and binaries) is free software; 
//		you can redistribute it and/or modify it under the terms of the
//		GNU General Public License as published by the Free Software 
//		Foundation, in version 3 of the License.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//		GNU General Public License for more details.
//
//		You can find a complete copy of the GNU General Public License at:
//		http://www.gnu.org/licenses/gpl.txt
//												             
//******************************************************************************


#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>
using namespace std;

int operate(char *namein, char *nameinfo);
void createNew();


char version[]="1.2 - 20140702";
char extension[]="map-";
char nameout[]="addcoord.txt";


int main(int argc, char *argv[])
{
    
    if(argc<2)
	{cout<< "\n addcoord v."<<version<< "\n";
        cout << " Add coordinates to txt matrix for LabSpec Maps \n\n";
        cout<< " Usage:  addcoord <filename> <info-filename> \n";
        cout<<" To create a new parameter file: addcoord -n \n\n";
        }
    
    if(argc==2)
        {   if(strcmp(argv[1],"-n")==0)
                {createNew();}
            else
                {cout<< " Usage:  addcoord <filename> <info-filename> \n\n";}
        }
    
    if(argc>2)
	{  operate(argv[1], argv[2]);
        
        return 0;
    }
}


//OPERATE 
int operate(char *namein, char *nameinfo)

{   ifstream infofile(nameinfo);
    double xmin, xmax, ymin, ymax = 0.0;
    
    if(!infofile)
    {cout<<"\n file '"<< nameinfo<<"' not found\n\n";
		return 0;}
    
    string lineinfo;
    
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    xmin= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    xmax= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    ymin= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    ymax= atof(lineinfo.c_str());
    
    infofile.close();
    
    ////////////////////////////////
    

	
   	ifstream infile(namein);
    unsigned long ncols = 0;
    unsigned long nrows = 0;

	if(!infile)
        {
        cout<<"\n file '"<< namein<<"' not found\n";
        return 0;
        }
    
    string str;
    getline(infile, str);
    istringstream iss(str);
    while (iss >> str)
        {ncols++;}

    infile.close();

    cout<<"\n Number of columns: "<<ncols<<endl;
    
    infile.open(namein);
    string tmp;
    while (!infile.eof()) {
        
        getline(infile, tmp);
        nrows++;
    }
    infile.close();
    nrows-=1;
    
    cout<< " Number of rows: "<<nrows <<endl;
    
    double xgrid, ygrid = 0.0;
    
	if (nrows ==0 && ncols ==0) {
		cout<<"\n number of rows or columns is zero.";
		return 0;
	}
	else {
	
		xgrid = (xmax - xmin)/nrows;
		ygrid = (ymax - ymin)/ncols;
	}
	
    cout << " xmin: "<<xmin<<endl;
    cout << " xmax: "<<xmax<<endl;
    cout << " ymin: "<<ymin<<endl;
    cout << " ymax: "<<ymax<<endl;
    cout << " xgrid: "<<xgrid<<endl;
    cout << " ygrid: "<<ygrid<<endl<<endl;
    
    char* outname = new char[strlen(namein)+strlen(extension)+1];
    sprintf(outname, "%.*s%s", (int)  strlen(namein)+5, extension, namein);
	
    ofstream outfile(outname);
    infile.open(namein);
    
    outfile<<"\t";

    for (unsigned int i = 0; i<ncols; i++) {
        
        outfile<<ymin+i*ygrid<<"\t";
        }
    
    outfile<<endl;
        
    for (unsigned int i = 0; i<nrows; i++) {
        
        getline(infile, tmp);
        outfile<<xmin+i*xgrid<<"\t"<<tmp<<endl;
    }
    
    infile.close();
    outfile.close();
    cout<<" Map saved in: "<<outname<<"\n\n";
    delete [] outname;
    
    return 0;
}


void createNew(){
    ofstream outfile(nameout);
    outfile<<"# xmin\n-50.5\n";
    outfile<<"# xmax\n50.5\n";
    outfile<<"# ymin\n-50.5\n";
    outfile<<"# ymax\n50.5\n";
    
    outfile.close();
    cout<<"\n Parameter file saved in: "<<nameout<<"\n\n";
    
}
