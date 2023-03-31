#include <bits/stdc++.h>
#include<fstream>
using namespace std;

std::vector<string> vec;
std::vector<string> cache;
std::vector<string> result;
std::vector<string> deleted;

bool not_present(int j){
    int count=0;
    for (int i = 0; i <  deleted.size()-1; i++) {
        if(deleted[i]==vec[j]){
            count++;
            break;
        }
    }
    if(count==0) return true;
    else return false;
}

string check(int j, int cap){
    int count=0, i=0;
    while (i < cap) {
        if(cache[i]==vec[j]){
            count++;
            break;
        }
        i++;
    }
    if(count==0) return "MISS";
    if(count==1) return "HIT";
}

void replace_LRU(int j){
    int cap=cache.size();
    int count=0, i=0;
    while(i<cap){
      if(cache[i]==vec[j]){
        count++;
        break;
      }
      i++;
    }
    if(count==1){
      string ci = cache[i];
      cache.erase(cache.begin()+i,cache.begin()+(i+1));
      cache.push_back(ci);
    }
    else{
      string del = cache[0];
      deleted.push_back(del);
      cache.erase(cache.begin(),cache.begin()+1);
      string ci = vec[j];
      cache.push_back(ci);
    }
}

void replace_OPTIMAL(int j){
    int entries  = cache.size();
    int farthest = j;
    int response=0;
    for(int i=0; i<entries; i++){
      int count=0;
      while(j<vec.size()){
        if(vec[j]==cache[i]){
          count++;
          if(farthest<j){
            farthest=j;
            response=i;
            break;
          }
        }
      }
      if(count==0){
        response=i;
        break;
      }
      j++;
    }
    string del = cache[response];
    deleted.push_back(del);
    cache[response]=vec[j];
}

int main(int argc, char *argv[]) // Don't forget first integral argument 'argc'
{
  string inputfile = argv[2];
  string policy = argv[1];
  string ent = argv[3];
  int entries = stoi(ent);

  // opening the input file

  ifstream opener;
  opener.open(inputfile);
  if(opener.is_open()){ 
    string data;
    while(!opener.eof() && getline(opener,data)){
        if(data == " ") continue;
        vec.push_back(data);
    }

    string a1 = vec[0];
    cache.push_back(a1);
    result.push_back("MISS");
    int t_miss = 1 , co_miss = 1;
    
    if(policy == "LRU"){
     
       for(int j=1; j<vec.size(); j++){
          int i = cache.size();
          string CHECK = check(j,i);
          result.push_back(CHECK);
          if(CHECK == "HIT"){
            replace_LRU(j);
            continue;
          }
          else{
            t_miss++;
            if(cache.size()<entries){
              string vj = vec[j];
              cache.push_back(vj);
          }
          else{
            replace_LRU(j);
            bool b=not_present(j);
            if(b){
                co_miss++;
            }
          }
        }
    }

   } else { 
        for(int j=1; j<vec.size(); j++){
          int i = cache.size();
          string CHECK = check(j,i);
          result.push_back(CHECK);
          if(CHECK == "HIT"){
            continue;
          }
          else{
            t_miss++;
            if(cache.size()<entries){
              string vj = vec[j];
              cache.push_back(vj);
            }
            else{
              replace_OPTIMAL(j);
              bool b=not_present(j);
              if(b){
                co_miss++;
              }
            }
          }
        }
    }
    
    int ca_miss = t_miss - co_miss;
    
    ofstream outputfile;
    outputfile.open("21116001_"+policy+"_"+inputfile+"_"+ent+".out");
    outputfile << "TOTAL_ACCESSES" << " = " << vec.size() <<endl;
    outputfile << "TOTAL_MISSES" << " = " << t_miss <<std::endl;
    outputfile << "COMPULSORY_MISSES" << " = " << co_miss <<std::endl;
    outputfile << "CAPACITY_MISSES" << " = " << ca_miss << std::endl;
    for (int w = 0; w < result.size(); w++) {
        outputfile << result[w] <<endl;
    }
    outputfile.close();
    
  }
  else std::cout << " ERROR OCCURRED FILE NOT FOUND" << std::endl;
  opener.close();
}
