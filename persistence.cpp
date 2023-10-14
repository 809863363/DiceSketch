#include "dicesketch.hpp"
#include "adaptor.hpp" 
#include <unordered_map> 
#include<math.h>   
#include <bits/stdc++.h>       
#include <utility>    
#include <iomanip>     
#include "datatypes.hpp"   
#include "util.h"       
#include <random>         
double  throughput=0;uint64_t t1=0, t2=0;uint64_t time_sum = 0;
/*persistence query*/
	int tt=0,ttt=0,tttt=0;
void PersistenceQuery(DiceSketch* Dice, mymap ground) {
		double aae = 0, are = 0;
		for (auto it = ground.begin(); it != ground.end(); ++it) {
			 auto value = Dice->QueryKey((unsigned char*)&(it->first)); if(it->second<800)tt+=it->second,ttt++;else tttt++;
			double hh = value > it->second ? value - it->second : it->second - value;
			aae += hh;
                        are += hh/double(it->second);
		}
		std::cout << "AAE: " << aae / ground.size() << std::endl;
		std::cout << "ARE: " << are / ground.size() << std::endl;
	} 

int main(int argc, char* argv[]) {   
   
	int memory_size;  
        std::cin>>memory_size;  
	double aae=0;  
	int bucket_num=memory_size*1024/(12*4); 
    int sumerror=0; 
	const char* filenames = "iptraces.txt"; 
	unsigned long long buf_size = 5000000000;  
	double thresh = 0.5; 
	int Dice_width =bucket_num; 
	int Dice_depth = 4;  
	std::vector<std::pair<key_tp, val_tp> > results;
	int numfile = 0; 
	double precision = 0, recall = 0, error = 0;
	std::ifstream tracefiles(filenames); 
	if (!tracefiles.is_open()) {
		std::cout << "Error opening file" << std::endl;
		return -1; 
	}     
  
	for (std::string file; getline(tracefiles, file);) {
		Adaptor* adaptor = new Adaptor(file, buf_size);
		std::cout << "[Dataset]: " << file << std::endl;
		std::cout << "[Message] Finish read data." << std::endl;    
		adaptor->Reset(); 
		mymap ground; mymap thisT; 
		val_tp sum = 0;     
		tuple_t t; 
		int number = 0, LENGTH = 5000, T = 0; 
		while (adaptor->GetNext(&t) == 1) {
		 	if (sum % LENGTH == 0)T += 1; 
			sum += 1;  
			key_tp key;   
			memcpy(key.key, &(t.key), LGN);
			if (thisT[key] != T ) {
				thisT[key] = T; ground[key] += 1;
			}       
		}
		std::cout << "epoch: " << T << std::endl;
		std::cout << "[Message] Finish Insert hash table" << std::endl;
		val_tp threshold = thresh * T;  
              
		DiceSketch* Dice = new DiceSketch(Dice_depth, Dice_width, 8 * LGN);
		adaptor->Reset();   
		memset(&t, 0, sizeof(tuple_t)); int gg = 0; T = 0;
		while (adaptor->GetNext(&t) == 1) {
			if (number % LENGTH == 0) { 
				Dice->Reset();T++;
			} number++; 
			t1 = now_us(); 
			Dice->Update((unsigned char*)&(t.key), 1);
			t2 = now_us();   
			time_sum = time_sum + (t2-t1);
		} 
		results.clear();  
		Dice->Query(threshold, results);
		PersistenceQuery(Dice, ground);		
		error = 0;int tp = 0, cnt = 0; aae=0;
		for (auto it = ground.begin(); it != ground.end(); it++) {    
		int flag = 0;  
			if (it->second >= threshold) { 
				cnt++;
				for (auto res = results.begin(); res != results.end(); res++) {
					if (memcmp(it->first.key, res->first.key, sizeof(res->first.key)) == 0) {     double hh = res->second > it->second ? res->second - it->second : it->second - res->second;
						flag = 1;sumerror+=(int)hh;
						error = hh * 1.0 / it->second + error;
						aae+=hh;  
	  					tp++;		
					} 
				} 


			} 
		}  
			precision = tp * 1.0 / results.size();
			recall = tp * 1.0 / cnt; 
			error = error / tp;
			aae=aae*1.0/tp;
			delete Dice;
			delete adaptor;
			numfile++;
     			std::cout<<"\nMemory    "<<memory_size<<std::endl;
        		std::cout<<"\nprecision "<<precision<<std::endl;
			std::cout<<"\nRecall    "<<recall<<std::endl;
			std::cout<<"\nARE       "<<error<<std::endl;

	}


}






