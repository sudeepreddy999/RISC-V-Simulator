#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

int bintoint(string s){
    int i;
    int t=1;
    int n=0;
    
    for(i=s.length()-1;i>=0;i--){
        if(s[i]=='0'){
            n=n+t*0;
            t=t*2;
        }else{
            n=n+t*1;
            t=t*2;
        }
    }
    
    return n;
}
//function to store an int
void insert(vector<string> &v,int n,int loc){
    string s = bitset<32>(n).to_string();
    v[loc]=s.substr(0,8);
    v[loc+1]=s.substr(9,8);
    v[loc+2]=s.substr(17,8);
    v[loc+3]=s.substr(25,8);
}
//function to get value from memory
int getData(vector<string> v,int loc){
    string s = "";
    s=s+v[loc]+v[loc+1]+v[loc+2]+v[loc+3];
    return bintoint(s);
}

int cacheData(string ***cache,int a,int b,int c){
    string s = cache[a][b][c]+cache[a][b][c+1]+cache[a][b][c+2]+cache[a][b][c+3];
    return bintoint(s);
}
void insertCache(string ***cache,int a,int b,int c,int address,vector<string> v){
    // cout<<"inserting the block into the cache"<<endl;
    int i;
    int k=0;
    // cout<<"address"<<address<<endl;
    int add = address >> 7;
    // cout<<"add aftre right shift "<<add<<endl;
    add = add << 7;
    // cout<<"initial add "<<add<<endl;
    for(i=0;i<32;i++){

        int data = getData(v,add);
        // cout<<data<<endl;
        // if(data == 222){
        //     cout<<"offset is "<<k<<endl;
        // }
        string s = bitset<32>(data).to_string();
        cache[a][b][k]=s.substr(0,8);
        cache[a][b][k+1]=s.substr(8,8);
        cache[a][b][k+2]=s.substr(16,8);
        cache[a][b][k+3]=s.substr(24,8);
        k = k+4;
        add = add +4;
    }
    
}
void writeCacheHit(string ***cache,int a,int b,int c,int data){
    

        
        string s = bitset<32>(data).to_string();
        cache[a][b][c]=s.substr(0,8);
        cache[a][b][c+1]=s.substr(9,8);
        cache[a][b][c+2]=s.substr(17,8);
        cache[a][b][c+3]=s.substr(25,8);
       
    
    
}
int getminInd(int **count,int index,int **bit){
    int max = count[index][0];
    int maxin=0;
    int i;
    for(i=0;i<16;i++){
        if(bit[index][i]==0){
            return i;
        }
        if(count[index][i]>max){
            max = count[index][i];
            maxin = i;
        }
    }
    return maxin;
}

int readCache(int **bit,int **tag,string ***cache,int **count,int address,vector<string> v,int &time){
    //convert address to a 20 bit string
    string s = bitset<20>(address).to_string();
    int tagc = bintoint(s.substr(0,9));
    int index = bintoint(s.substr(9,4));
    int offset = bintoint(s.substr(13,7));
    //search for tag;
    int i,j;
    for(i=0;i<16;i++){
        if(tag[index][i] == tagc){

            if(bit[index][i]==1){
                cout<<"giving form cache"<<endl;
                count[index][i] = time;
                time++;
                return cacheData(cache,index,i,offset);
                
            }
            if(bit[index][i]==2){
                return -2;//means wait
            }
        }
    }
    // the tag is not present;
    // cout<<"while reading no tag"<<endl;
    int rep = getminInd(count,index,bit);//implement LRU
    //pass the req to memory
    int memres = getData(v,address);
    //now put it back 
    insertCache(cache,index,rep,offset,address,v);
    tag[index][rep] = tagc;
    count[index][rep] = time;
    time ++;
    bit[index][rep]=1;
    cout<<"giving from memory"<<endl;
    return memres;
    
    
}
// write through and write allocate

void writeCache(int **bit,int **tag,string ***cache,int **count,int address,vector<string> &v,int data,int &time){
// cout<<"entered write cache "<<endl;
    string s = bitset<20>(address).to_string();
    int tagc = bintoint(s.substr(0,9));
    int index = bintoint(s.substr(9,4));
    int offset = bintoint(s.substr(13,7));
    //search for tag;
    // cout<<"index is "<<index<<endl;
    int i,j;
    for(i=0;i<16;i++){
        if(tag[index][i] == tagc){
            if(bit[index][i]==2){
                return; //means wait
            }
            if(bit[index][i]!=0){
                writeCacheHit(cache,index,i,offset,data);
            //insertCache(cache,index,i,offset,data);
            //insert(v,data,address);
                bit[index][i] = 3;// make M
                count[index][i] = time;
                time++;
                return;
            }
            
            
        }
    }
    // no tag
    // cout<<"tag not found"<<endl;
    //write in memory
    
    insert(v,data,address);
    // int rep = getminInd(count,index,bit);
    // if(bit[index][rep] == 3){
        
    //     // insert(v,dataCache,address);
    // }
    // insertCache(cache,index,rep,offset,address,v);
    
}

int main(){
    //declare a main memory of 1MB i.e 2^20 bytes
    
    
//using a set associative cache of 16 ways and 16 sets i.e 20 bits for the whole addess
// 4 bits for index; 7 bits for offset; 9 bits for tag
    int time=0;
    int i;int j;
    vector<string> mem(1<<20);
    string*** cache = new string**[16];
    for (int i = 0; i < 16; ++i) {
        cache[i] = new std::string*[16];
        for (int j = 0; j < 16; ++j) {
            cache[i][j] = new std::string[128];
        }
    }

    int **bit;// 0 for invalid; 1 for valid; 2 for MP; // 3 for M state
    bit = new int *[16];
    for(int i = 0; i <16; i++)
    bit[i] = new int[16];

    int **tag;// to store the tag
    tag = new int *[16];
    for(int i = 0; i <16; i++)
    tag[i] = new int[16];

    int **count;// to store the count for LRU
    count = new int *[16];
    for(int i = 0; i <16; i++)
    count[i] = new int[16];
    
    
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            count[i][j]=0;
            tag[i][j]=-1;// make sure no block has a tag
            bit[i][j]=0;//set all to invalid
        }
        
    }
    insert(mem,15,4);
    insert(mem,26,8);
    insert(mem,250,12);
    insert(mem,222,20);
    time++;
    
    cout<<readCache(bit,tag,cache,count,4,mem,time)<<endl;
    
    cout<<readCache(bit,tag,cache,count,4,mem,time)<<endl;
    
    cout<<readCache(bit,tag,cache,count,8,mem,time)<<endl;
    cout<<readCache(bit,tag,cache,count,12,mem,time)<<endl;
    cout<<readCache(bit,tag,cache,count,20,mem,time)<<endl;
    
    writeCache(bit,tag,cache,count,20,mem,120,time);
    
    cout<<readCache(bit,tag,cache,count,20,mem,time)<<endl;
    
    cout<<"checking in memory "<<getData(mem,20)<<endl;
    // cout<<"hi"<<bintoint("00000000100010000010")<<endl;
    writeCache(bit,tag,cache,count,2180,mem,222,time);
    writeCache(bit,tag,cache,count,10000,mem,1000,time);
    cout<<readCache(bit,tag,cache,count,2180,mem,time)<<endl;
    cout<<readCache(bit,tag,cache,count,2180,mem,time)<<endl;
    cout<<readCache(bit,tag,cache,count,10000,mem,time)<<endl;
    cout<<readCache(bit,tag,cache,count,10000,mem,time)<<endl;
    cout<<"checking in memory "<<getData(mem,2178)<<endl;
    cout<<"checking in memory "<<getData(mem,10000)<<endl;
    


    

}