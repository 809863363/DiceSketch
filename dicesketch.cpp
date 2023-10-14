#include "dicesketch.hpp"
#include <math.h> 
#include <random>
int seeds = 0;int ct=0;
int MAXINT=1000000000;
int dicelength=2; 
int tim=0;
int wr;
int ggg=0;int times =0;int times2 =0;
int k;
unsigned long bucket = 0;
unsigned long bucket1 = 0;
int keylen = 8;
long int newk;
int loc = -1; int ii = 0;
unsigned int gg = 999999999;
static std::random_device rd;
char name[] = "DiceSketch";
unsigned long seed1 = AwareHash((unsigned char*)name, strlen(name), 130912042814, 2282047327515, 66208308896);
DiceSketch::DiceSketch(int depth, int width, int lgn) {
        
	Dice_.depth = depth;
	Dice_.width = width;
	Dice_.lgn = lgn;
	Dice_.counts = new SBucket *[depth*width];
	for (int i = 0; i < depth*width; i++) {
		Dice_.counts[i] = (SBucket*)calloc(1, sizeof(SBucket));
		memset(Dice_.counts[i], 0, sizeof(SBucket));
		Dice_.counts[i]->key[0] = '\0';
		Dice_.counts[i]->flag = 0;
	}
        wr=dicelength;
	Dice_.hash = new unsigned long[depth];
	Dice_.scale = new unsigned long[depth];
	Dice_.hardner = new unsigned long[depth + 1];	
	for (int i = 0; i < depth; i++) {
		Dice_.hash[i] = GenHashSeed(seed1++);
	}
	for (int i = 0; i < depth; i++) {
		Dice_.scale[i] = GenHashSeed(seed1++);
	}
	for (int i = 0; i < depth + 1; i++) {
		Dice_.hardner[i] = GenHashSeed(seed1++);
	}
}

DiceSketch::~DiceSketch() {
	for (int i = 0; i < Dice_.depth*Dice_.width; i++) {
		free(Dice_.counts[i]);
	}
	delete[] Dice_.hash;
	delete[] Dice_.scale;
	delete[] Dice_.hardner;
	delete[] Dice_.counts;
}



void DiceSketch::Update(unsigned char* key, val_tp val) {
	long min = 99999999,index; 
	DiceSketch::SBucket* sbucket;
	DiceSketch::SBucket* sbucket1;
	for (int i = 0; i < Dice_.depth; i++) {
		bucket = MurmurHash64A(key, keylen, Dice_.hardner[i]) % Dice_.width;
		index = i * Dice_.width + bucket;
		sbucket = Dice_.counts[index];
		if (sbucket->count == 0) {
			memcpy(sbucket->key, key, keylen);
			//sbucket->count ++; 
			sbucket->flag = 1;
			return;
		}
		if (memcmp(key, sbucket->key, keylen) == 0) {
			if (sbucket->flag == 1)return;
                        sbucket->flag = 1;
			return;
		}
		if (sbucket->count < min)
		{
			min = sbucket->count;
			loc = index; bucket1 = bucket; ii = i;
		}
	
	}

	if (min == 99999999)return;
	sbucket = Dice_.counts[loc];
	k = MurmurHash64A(key, keylen, Dice_.hardner[Dice_.depth]) % (sbucket->count + 1);
	if (k == 0)
	{
		memcpy(sbucket->key, key, keylen);
		//sbucket->count+=1;
		sbucket->flag = 1;
	}

}
 



mymap DiceSketch::Query2(val_tp thresh) 
{mymap ground;key_tp key;
	for (int i = 0; i < Dice_.width*Dice_.depth; i++) {
			memcpy(key.key, Dice_.counts[i]->key, Dice_.lgn / 8);
              	        ground[key] += Dice_.counts[i]->count;                  
	}

return ground;
}




void DiceSketch::Query(val_tp thresh, std::vector<std::pair<key_tp, val_tp> >&results) {
          std::cout << "threshold = " << thresh << std::endl;
        mymap ground;key_tp key;

	for (int i = 0; i < Dice_.width*Dice_.depth; i++) {
			memcpy(key.key, Dice_.counts[i]->key, Dice_.lgn / 8);
              	        ground[key] +=(Dice_.counts[i]->count + Dice_.counts[i]->flag);                
	}
for (auto it = ground.begin(); it != ground.end(); ){
	if (it->second < (int)thresh) 
	{
		it = ground.erase(it);
       	}
	else {it++;}
}

for (auto it = ground.begin(); it != ground.end();it++ ){
	                key_tp key;
			memcpy(key.key, it->first.key, Dice_.lgn / 8);
			std::pair<key_tp, val_tp> node;
			node.first = key;
			node.second = it->second;
			results.push_back(node);
}



	std::cout << "results.size = " << results.size() << std::endl;
}




void DiceSketch::Reset() {
	Dice_.hardner[Dice_.depth] = GenHashSeed(seed1+rd());
	for (int i = 0; i < Dice_.depth*Dice_.width; i++) {
                Dice_.counts[i]->count += Dice_.counts[i]->flag;
		Dice_.counts[i]->flag = 0;
	}
}

void DiceSketch::SetBucket(int row, int column, val_tp sum, long count, unsigned char* key) {
	int index = row * Dice_.width + column;
	Dice_.counts[index]->count = count;
	memcpy(Dice_.counts[index]->key, key, Dice_.lgn / 8);
        
}

DiceSketch::SBucket** DiceSketch::GetTable() {
	return Dice_.counts;
}


int DiceSketch::QueryKey(unsigned char* key){
	long min = 99999999,index; 
	DiceSketch::SBucket* sbucket;
	DiceSketch::SBucket* sbucket1;
	for (int i = 0; i < Dice_.depth; i++) {
		bucket = MurmurHash64A(key, keylen, Dice_.hardner[i]) % Dice_.width;
		index = i * Dice_.width + bucket;
		sbucket = Dice_.counts[index];
		if (memcmp(key, sbucket->key, keylen) == 0)return sbucket->count;
		if(sbucket->count < min)min = sbucket->count;
}
if(min > 1)return min -1;
return min;
}





