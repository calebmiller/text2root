#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

const int HG_CUT=200;
const int CHMAP[64] = {10,8,7,50,12,11,14,2,1,15,0,5,4,19,6,9,25,21,24,13,30,17,31,27,18,16,28,23,26,44,20,22,40,41,52,43,58,45,46,47,32,33,60,35,29,37,38,39,56,57,3,59,34,61,62,63,48,49,36,51,42,53,54,55}
//						0,1,2, 3, 4, 5, 6 7 8  9 10111213 141516 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 5051 52 53 54 55 56 57 58 59 60 61 62 63


int getToA(std::string toa_LSB)
{
	try{
		int temp = boost::lexical_cast<int>(toa_LSB);
		return temp;
	}
	catch(...){
		return -1;
	}

}
void fillVec(std::vector<int>& chan, std::vector<int>& ADC, std::vector<int>& ToA, int ch, int hg, std::string toa_LSB)
{
	if(hg>HG_CUT){
		chan.push_back(ch);
		ADC.push_back(hg);
		ToA.push_back(getToA(toa_LSB));
	}

}
void writeEvent((std::vector<int>& chan, std::vector<int>& ADC, std::vector<int>& ToA)
{

 mytree->Fill();
}
int main(int argc, char** argv)
{	
	if(argc != 2){
		std::cout<<"Incorrect number of args"<<std::endl;
		std::cout<<"./txt2root /path/to/file"<<std::endl;
		return 0;
	}
	std::cout<<"Processing "<<argv[1]<<std::endl;

	std::ifstream testFile(argv[1]);	
	std::string line;
	
	double ts=0;
	std::vector<int> chan;	
	std::vector<int> ADC;	
	std::vector<int> ToA;	

	
	TFile* fout = new TFile("clean.root","RECREATE");
	TTree *mytree = new TTree("tree","tree");
	mytree->Branch("t_us", &ts, "t_us/D");
	mytree->Branch("Channels", &chan);
	mytree->Branch("ADC", &ADC);
	mytree->Branch("ToA", &ToA);
	
	while(getline(testFile, line)){

 		std::istringstream iss(line);
		
		double timestamp;
		int trgID, brd, ch, lg, hg;
		std::string toa_LSB, tot_LSB;
	
	
		// Attempt to extract data from the line
		if (iss >> timestamp >> trgID >> brd >> ch >> lg >> hg >> toa_LSB >> tot_LSB) {
			if(ADC.size()!=0) writeEvent(chan, ADC, ToA)

			chan.clear();
			ADC.clear();
			ToA.clear();

			ts=timestamp;
			fillVec(chan, ADC, ToA, ch, hg, toa_LSB);
		}
		else{
 			std::istringstream iss2(line);
			if (iss2 >> brd >> ch >> lg >> hg >> toa_LSB >> tot_LSB){
				fillVec(chan, ADC, ToA, ch, hg, toa_LSB);
			}	
		}
	}
	mytree->Write();
	fout->Close();
	std::cout<<"Finished"<<std::endl;
}
