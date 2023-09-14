#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <chrono>

int main()
{	
	auto start = std::chrono::high_resolution_clock::now();

	std::ifstream testFile("Run1_list.txt");	
	std::string line;
	
	auto loadf = std::chrono::high_resolution_clock::now(); 

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
			if(ADC.size()!=0) mytree->Fill();
			chan.clear();
			ADC.clear();
			ToA.clear();

			ts=timestamp;
			if(hg>100){
				chan.push_back(ch);
				ADC.push_back(hg);
				try{
					int temp = boost::lexical_cast<int>(toa_LSB);
					ToA.push_back(2*temp);
				}
				catch(...){
					ToA.push_back(-1);
				}
			}
		}
		else{
			
 			std::istringstream iss2(line);
			if (iss2 >> brd >> ch >> lg >> hg >> toa_LSB >> tot_LSB){
				
				if(hg>100){
					chan.push_back(ch);
					ADC.push_back(hg);
					try{
						int temp = boost::lexical_cast<int>(toa_LSB);
						ToA.push_back(2*temp);
					}
					catch(...){
						ToA.push_back(-1);
					}
				}
			}	
		}
	}
	auto loop = std::chrono::high_resolution_clock::now(); 
	mytree->Write();
	fout->Close();
	auto write = std::chrono::high_resolution_clock::now(); 

	auto lt = std::chrono::duration_cast<std::chrono::microseconds>(loadf - start);
	auto ot = std::chrono::duration_cast<std::chrono::microseconds>(loop - loadf);
	auto wt = std::chrono::duration_cast<std::chrono::microseconds>(write - loop);

	std::cout<<"load time :"<<lt.count() << std::endl;
	std::cout<<"loop time :"<<ot.count() << std::endl;
	std::cout<<"write time :"<<wt.count() << std::endl;
}
