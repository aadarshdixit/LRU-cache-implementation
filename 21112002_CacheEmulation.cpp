#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// optimal start
bool search(std::vector<string>cache, string x){
    for (int i = 0; i < cache.size(); i++) {
        if(cache[i] == x) return true;
    }
    return false;
}

int replace(std::vector<string>arr,std::vector<string>cache ,int index){
    int ans =-1;
    int ischange = index;
    for (int i = 0; i < cache.size(); i++) {
        int j;
        
        for ( j = index; j < arr.size(); j++) {
            if(cache[i] == arr[j]){
                if(j>ischange) {
                    ischange  = j;
                    ans = i;
                }
                break;
            }
        }
        if(j == cache.size()) return i;
        
    }
    if(ans == -1) return 0;
    else return ans;
}

int optimal(std::vector<string>arr, int capacity,std::vector<string>&v){
    int hit =0;
    std::vector<string>cache ;
    for (int i = 0; i < arr.size(); i++) {
        if(search(cache,arr[i])) {
            hit++;
            v.push_back("HIT");
        }
        else if(cache.size()<capacity){
            cache.push_back(arr[i]);
            v.push_back("MISS");
        }
        else{
            int j  =  replace(arr,cache,i);
            cache[j] =  arr[i];
            v.push_back("MISS");
        }
    }
    return hit;
}

//optimal end


//LRU start

class LRU {
  public:
    class node {
      public:
      string address;
      node * next;
      node * prev;
      node(string _address) {
        address = _address;
      }
    };

  node * head = new node("-1");
  node * tail = new node("-1");

  int cachesize;
  unordered_map < string, node * > m;

  LRU(int capacity) {
    cachesize = capacity;
    head -> next = tail;
    tail -> prev = head;
  }

  void addnode(node * newnode) {
    node * temp = head -> next;
    newnode -> next = temp;
    newnode -> prev = head;
    head -> next = newnode;
    temp -> prev = newnode;
  }

  void deletenode(node * delnode) {
    node * delprev = delnode -> prev;
    node * delnext = delnode -> next;
    delprev -> next = delnext;
    delnext -> prev = delprev;
  }
    std::map<string, int>hash;
  int get(string address,int &capacitymiss) {
    hash[address] ++;
    if (m.find(address) != m.end()) {
      node * resnode = m[address];
      m.erase(address);
      deletenode(resnode);
      addnode(resnode);
      m[address] = head -> next;
      return 1;
    }
    put(address,capacitymiss);
    return 0;
   
  }
  void miss(int &capacitymiss) {
      capacitymiss ++;
  }

  void put(string address, int &capacitymiss) {
     
    if (m.size() == cachesize) {
     if(hash[address] > 1 )   miss(capacitymiss);
      m.erase(tail -> prev -> address);
      deletenode(tail -> prev);
    }
    addnode(new node(address));
    m[address] = head -> next;
  }
};

//LRU end
int main(int argc, char *argv[]) {
  string replacementpolicy = argv[1];
  string filename= argv[2];
  string NumberOfCacheEntry= argv[3];
  //cin>>replacementpolicy>>filename>>NumberOfCacheEntry;
	ifstream inputfile;
  inputfile.open(filename.c_str());
	std::vector<string>arr;
    std::set<string>s;
    if(inputfile.is_open())
    { 
      string data ;   
      while(!inputfile.eof() && getline(inputfile, data) ){
            if(data == " ") continue;
            arr.push_back(data);
            s.insert(data);
        }
    }
    else std::cout << " ERROR OCCUR INPUT FILE NOT FOUND" << std::endl;
    inputfile.close();
    int total = arr.size();
	int miss =0;
	int capacity = stoi(NumberOfCacheEntry);
	int compulsory = s.size();
	int capacitymiss =0;
	
	std::vector<string> v1;
  if(replacementpolicy == "LRU")

    {LRU *cache = new LRU(capacity);
    for (int i = 0; i < arr.size(); i++) {
        
        if(!cache->get(arr[i],capacitymiss)) {
            v1.push_back("MISS");
            miss++;
        }
        else{
            v1.push_back("HIT");
        }
    }}
  else {
      miss = total - optimal(arr,capacity,v1);
  }
  string Rollno = "21112002";
  ofstream output;
  string outfilename  = Rollno+"_"+replacementpolicy+"_"+filename+"_"+NumberOfCacheEntry+".out";
  output.open(outfilename);
    output<< "TOTAL_ACCESSES" << " = " <<  total<<endl;
    output << "TOTAL_MISSES" << " = " << miss<<std::endl;
    output  << "COMPULSORY_MISSES" << " = " << compulsory<<std::endl;
    output << "CAPACITY_MISSES" << " = " << capacitymiss<< std::endl;
    for (int i = 0; i < v1.size(); i++) {
        output<< v1[i]<<endl;
    }
    output.close();
	return 0;
}