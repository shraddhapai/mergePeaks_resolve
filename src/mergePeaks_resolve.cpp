/* 
* mergePeaks_resolve.cpp
* Converts the output from HOMER's mergePeaks utility into a file of contributing peak intensities.
* mergePeaks provides columns with peak names corresponding to those that overlap in individual files.
* This utility takes the source peak files and puts the corresponding peak intensities into a tab-delimited file format
* Author: Shraddha Pai
* 
* Input parameters:
* -m	mergePeaks output file
* -i 	files that were merged using HOMER ; comma-separated
* -o 	output file
* 
* Written by Shraddha Pai
*/

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <boost/thread.hpp>
#include <map>
#include <time.h>
using namespace std;

static void show_usage(std::string name) {
		std::cerr << "Usage: " << name << " <options>"
		<< "Options:\n" 
		<< "\t-h\tShow this usage message\n"
		<< "\t-i\tfiles that were merged using the HOMER mergePeaks call. Order of input files must match that of column ordering in mergedFile\n"
		<< "\t-m\toutput of HOMER mergePeaks call, a file with the merged peaks ID\n"
		<< "\t-o\tfile to write results of this script\n"
		<< std::endl;
}


class Worker{
public:
	Worker(string m, string i, string o, string cFile, unsigned cnum )
		: m (m),
		  inFile (i),
          outFile (o),
          coordFile (cFile),
		  myCol (cnum)
	{
	}

  //  int doIt() // SERIAL DEBUGGING
    void operator() () // PARALLEL VERSION
	{
		ifstream mFile, iFile; 
        ofstream oFile, cFile; 

        const int MAX_LINELEN=100000;
        char line[MAX_LINELEN];
        vector <string> t; 
        
		// ID -> peak size map
		iFile.open(inFile.c_str()); // file has no header so we can just get to it.
		std::map <string,double> id2peak_map;
        time_t start=time(0);
        unsigned i_ctr; i_ctr=0;
		while(iFile.good()) {
			iFile.getline(line,MAX_LINELEN);
			if (std::strcmp(line,"")>0){
					boost::split(t,line,boost::is_any_of("\t"));
					id2peak_map[t[3]] = atof(t[4].c_str()); 
                i_ctr++;
               //if (i_ctr % 1000000==0) cout << "." << endl;
			}
		}
		iFile.close();
        time_t end=time(0);
        cout << myCol << ": Input map Time = " << difftime(end,start) << " seconds" << endl;
        
      //  cout <<"Looking for offending entry:" << endl;
       // cout << id2peak_map.count("1614_2021_34") << endl;
      //  cout << "past it" << endl;
        
        
        if (coordFile.compare("")!=0) {
            cFile.open(coordFile.c_str());
            cFile << "chrom" << "\t" << "start" << "\t" << "end" << endl;
        } 
        
        cout << "* Reading mergedFile: " << m << endl;
        // now we read the merged peak file and map back peak ids to intensities
        mFile.open(m.c_str()); oFile.open(outFile.c_str());
		mFile.getline(line,MAX_LINELEN); 
        boost::split(t,line,boost::is_any_of("\t"));        
        oFile << t[myCol] << endl; // column header
        
        unsigned ctr; ctr=2;
        vector<string> missingKeys;
		while (mFile.good()) {
          //  cout << myCol-7 << ":" << ctr << ": about to get line" << endl;
			mFile.getline(line,MAX_LINELEN);
          // cout << myCol-7 << ":" << ctr << ": got line" << endl;
		  // cout << line << endl;
			if (std::strcmp(line,"")!=0){
			boost::split(t,line,boost::is_any_of("\t"));
                
            if (coordFile.compare("")!=0)
                cFile << t[1] << "\t" << t[2] << "\t" << t[3] << endl;
			
			if (ctr % 1000 == 0) cout << "K";
			if (ctr % 10000 ==0) cout << endl;
                
             //   cout << ctr << ": " << t[myCol] << endl;
			vector<string> t2; boost::split(t2,t[myCol],boost::is_any_of(","));
			double sum; sum=0;
                for (unsigned i=0; i < t2.size(); i++) {
                //    cout << "\tLooking for **" << t2[i] << "**" << endl;
                    unsigned keyct = id2peak_map.count(t2[i]);
                    if (keyct==0) 
                        missingKeys.push_back(t2[i]);
                    else {
                        map<string,double>::iterator it=id2peak_map.find(t2[i]);
                        sum+= it->second;
                    };
                };
				if (t2.size() > 0) {
              //  cout << ctr <<": computing mean" << endl;
              //  cout << ctr << sum << "/" << t2.size() << endl;
            	oFile << sum/((double)t2.size()) << endl;;
				} else 
						oFile << "NA" << endl;
           //     cout << myCol-7 <<":" << ctr << "done!" << endl;
			}
            ctr++;
            }
        cout << myCol-7 << ": closefile" << endl;
		mFile.close();
        oFile.close();
        if (coordFile.compare("")!=0)
            cFile.close();
        cout << "File " << myCol-7 << ": " << ctr << " records" << endl;
        for (unsigned i=0; i < missingKeys.size(); i++) {
            cout << myCol-7 << ": missing key=" << missingKeys[i] << endl;
        }
		}
		
private:
		string m;
		string inFile;
        string coordFile;
		unsigned myCol;
        string outFile;
};


/* **********************************************
 Main function 
 ************************************************ */
int main (int argc, char* argv[]) {
char* inDir;
char* mergedFile;
char* outFile;

// parse command-line args
for (int i = 1; i < argc; ++i) {
	std::string arg = argv[i];
	if (arg=="-h") {
		show_usage(argv[0]);
		return 0;
	} else if (arg == "-i") {
		inDir = argv[++i];
		//cout << "input files: " << inDir << endl;
	} else if (arg == "-m") {
		mergedFile = argv[++i];
		cout << "merged file (output of HOMER mergePeaks): " << mergedFile << endl;
	} else if (arg == "-o") {
		outFile = argv[++i];
	//	cout << "output file: " << outFile << endl;
	} else {
		cerr << "Invalid command. Please see usage " << endl;
		show_usage(argv[0]);
	}
}

// TODO : should test if any of these is empty

// I/O
ifstream m_in; ofstream out;
m_in.open(mergedFile);
    const int MAX_LINELEN=5000;
    char line[MAX_LINELEN];
    m_in.getline(line, MAX_LINELEN);
vector<string> t; boost::split(t,line,boost::is_any_of("\t"));
int ssize=t.size();
m_in.close();
// cout << "File has: " << ssize << " columns" << endl;

std::vector<boost::thread *> wvec;
    
    // spawn threads
    char oWriteCmd[10000];
    char delCmd[10000];
    strcpy(oWriteCmd, "paste ");
    strcpy(delCmd, "rm ");
    
    for (unsigned i = 8; i < ssize; i++) {
        char ifile[500], ofile[500];
        strcpy(ifile, inDir); strcat(ifile, "/"); 
        strcat(ifile, t[i].c_str()); 
        strcpy(ofile, outFile); strcat(ofile,"_"); strcat(ofile,t[i].c_str()); strcat(ofile,".tmp");
        
        char coordFile[500]; strcpy(coordFile,"");
        if (i==8) {
            strcpy(coordFile,outFile); strcat(coordFile, ".coords.tmp");
            strcat(oWriteCmd, coordFile); strcat(oWriteCmd," ");
            strcat(delCmd, coordFile); strcat(delCmd, " ");
        } 
        /*
        cout << "********" << i <<  "***********" << endl;
        cout << "val: " << t[i] << endl;
        cout << "I:" << ifile << endl;
        cout << "O:" << ofile << endl;
        */

        strcat(oWriteCmd, ofile); strcat(oWriteCmd," ");
        strcat(delCmd,ofile); strcat(delCmd, " ");
        wvec.push_back(new boost::thread(Worker(mergedFile, ifile, ofile,coordFile,i)));

		/* START SERIAL for DEBUGGING 
		cout << i-7 << ": " << ifile << endl;
        Worker w (mergedFile, ifile, ofile,coordFile,i); //));
        int outval; outval = w.doIt();
        if (outval == -1) return -1;
		END SERIAL FOR DEBUGGING */
    }
    
    // wait for all jobs to finish
    for (unsigned i=0; i < wvec.size(); i++) 
        wvec[i]->join();
    
    // finally merge output.
    strcat(oWriteCmd, "> "); strcat(oWriteCmd, outFile);
    cout << oWriteCmd << endl;
    system(oWriteCmd);
    cout << "Delete command: " << endl;
    cout << delCmd << endl;
    //system(delCmd); // delete temporary files

cout << "done" << endl;

}
