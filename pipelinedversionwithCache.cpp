#include <iostream>
#include <fstream>
using namespace std;

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
string concompl(string s){
    int i=0;
    //invert
    while(i!=s.length()){
        if(s[i]=='0'){
            s[i]='1';
        }else{ 
            s[i]='0';
        }
        i++;
    }
    //add1
    
    int carry = 1;
    for (int i = s.length() - 1; i >= 0; i--) {
        if (s[i] == '0' && carry == 1) {
            s[i] = '1';
            carry = 0;
        } else if (s[i] == '1' && carry == 1) {
            s[i] = '0';
        }
    }
    
    

    return s;
}
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
int bintointImm(string s){
    int i;
    int t=1;
    int n=0;
    if(s[0]=='0'){
    for(i=s.length()-1;i>=0;i--){
        if(s[i]=='0'){
            n=n+t*0;
            t=t*2;
        }else{
            n=n+t*1;
            t=t*2;
        }
    }
    }else{
        s=concompl(s);
        for(i=s.length()-1;i>=0;i--){
        if(s[i]=='0'){
            n=n+t*0;
            t=t*2;
        }else{
            n=n+t*1;
            t=t*2;
        }
    }
    n=n*-1;
    }
    
    return n;
}
class pgc{
public:
    int stall;
    int av;
    int pc;

public:
    pgc(){
        stall=0;
        av=0;
        pc=0;
    }
};

class fede{
public:
    int stall;
    int id;
    int av;
    int dpc;
    string in;
public:
    fede(){
        stall=0;
        id=-1;
        av=0;
        dpc=0;
        in="";
    }
};

class idex{
public:
    int id;
    int av;
    int dpc;
    int rs1;
    int rs2;
    int imm;
    int rd1;
    string func;
    string AluOP;
    //control Word
    int Rread;
    int Rwrite;
    int RorI;
    int branch;
    int ALU;
    int zero;
    // control word for next part
    int Mread;
    int Mwrite;
    int jump;
    int immt;
    // for next to next part;
    int ResOrLd;
public:
    idex(){
    id=-1;
     av=0;
     dpc=0;
     rs1=0;
     rs2=0;
     imm=0;
     rd1=0;
     func="";
     AluOP="";
    Rread = 0;
     Rwrite=0;
     RorI=0;
     branch=0;
     jump=0;
     ALU=0;
     zero=0;
   immt=0;
     Mread=0;
     Mwrite=0;
    
     ResOrLd=0;
    }

    
};

class exmem{
public:
    int id;
    int av;
    int ALUout;
    int rs2;
    int rd1;
    //control words
    int Mread;
    int Mwrite;
    int Rwrite;
    //for next to next
    int ResOrLd;
public:
    exmem(){
        id=-1;
        av=0;
        ALUout=0;
        rs2=0;
        rd1=0;
        Mread=0;
        Mwrite=0;
        Rwrite=0;
        ResOrLd=0;
    }
};

class memwr{
public:
    int id;
    int av;
    int memOut;
    int ALUout;
    int rd1;
    //control word
    int Rwrite;
    int ResOrLd;
public:
    memwr(){
        id=-1;
        av=0;
        memOut=0;
        ALUout=0;
        rd1=0;
        ResOrLd=0;
        Rwrite =0;
    }

};

int inputinstruction(string inst[]){
    int i=0;
    string s;
    ifstream file;
    file.open("output.txt");
    while(file.good()){
        getline(file,s);
        inst[i]=s;
        i++;
    }
    file.close();
    return i+1;
}

int getRd(string s){
    
    string rd;
    int n;
    rd=s.substr(20,5);
    
    n=bintoint(rd);
    return n;
}
int getRs1(string s){
    string rs1;
    
    
    int n;
    rs1=s.substr(12,5);
    
    
    n=bintoint(rs1);
    return n;
}
int getRs2(string s){
    
    string rs2;
    int n;
    rs2=s.substr(7,5);
    n=bintoint(rs2);
    return n;
}
int getimm(string s,int imm){
    int n;
    string im;
    // cout<<"immt value is "<<imm<<endl;
    if(imm==1){
        im=s.substr(0,12);
        
    }else{
        if(imm==2){
            im=s.substr(0,7);
            im=im+s.substr(20,5);
        }else{
            if(imm==3){
                im=im+s[0]+s[24];
                im=im+s.substr(1,6)+s.substr(20,4);

                
            }
        }
    }
    // cout<<"imm substring "<<im<<endl;
    n=bintointImm(im);
    return n;
}
void flushout(pgc &PC,fede &FEDE, idex &IDEX,int npc){
    FEDE.av=0;
    IDEX.av=0;
    PC.pc = npc;
    return;
}
void generatecontrolsignal(string s,idex &IDEX){
    string op=s.substr(25,7);
    // cout<<"in decode opcode is "<<op<<endl;
    string func3=s.substr(17,3);
    string func7=s.substr(0,7);
    if(op=="0110011"){
        IDEX.RorI=0;IDEX.Rwrite=1;IDEX.Rread=1;IDEX.Mwrite=0;IDEX.Mread=0;IDEX.branch=0;IDEX.jump=0;IDEX.ResOrLd=0;IDEX.immt=0;IDEX.ALU=0;
        if(func3=="000"){
            if(func7=="0000000"){
                IDEX.ALU=0;
            }else{
                IDEX.ALU=1;
            }
        }else{
            if(func3=="100"){
                IDEX.ALU=2;
            }else{
                if(func3=="110"){
                    IDEX.ALU=3;
                }else{
                    IDEX.ALU=4;
                }
            }
        }
    }else{
        if(op=="0010011"){
            // cout<<"enters with immt=1"<<endl;
            IDEX.RorI=1;IDEX.Rwrite=1;IDEX.Rread=1;IDEX.Mwrite=0;IDEX.Mread=0;IDEX.branch=0;IDEX.jump=0;IDEX.ResOrLd=0;IDEX.immt=1;IDEX.ALU=0;
            if(func3=="000"){
            IDEX.ALU=0;
        }else{
            if(func3=="100"){
                IDEX.ALU=2;
            }else{
                if(func3=="110"){
                    IDEX.ALU=3;
                }else{
                    IDEX.ALU=4;
                }
            }
        }
        }else{
            if(op=="0000011"){
                IDEX.RorI=1;IDEX.Rwrite=1;IDEX.Rread=1;IDEX.Mwrite=0;IDEX.Mread=1;IDEX.branch=0;IDEX.jump=0;IDEX.ResOrLd=1;IDEX.immt=1;IDEX.ALU=0;
            }else{
                if(op=="0100011"){
                    IDEX.RorI=1;IDEX.Rwrite=0;IDEX.Rread=1;IDEX.Mwrite=1;IDEX.Mread=0;IDEX.branch=0;IDEX.jump=0;IDEX.ResOrLd=0;IDEX.immt=2;IDEX.ALU=0;
                }else{
                    if(op=="1100011"){
                        if(func3=="000"){
                        IDEX.RorI=0;IDEX.Rwrite=0;IDEX.Rread=1;IDEX.Mwrite=0;IDEX.Mread=0;IDEX.branch=1;IDEX.jump=0;IDEX.ResOrLd=0;IDEX.immt=3;IDEX.ALU=1;
                        }else{
                            if(func3=="001"){
                                IDEX.RorI=0;IDEX.Rwrite=0;IDEX.Rread=1;IDEX.Mwrite=0;IDEX.Mread=0;IDEX.branch=2;IDEX.jump=0;IDEX.ResOrLd=0;IDEX.immt=3;IDEX.ALU=1;

                            }
                        }
                    }else{
                        if(op=="1101111"){
                            IDEX.RorI=0;IDEX.Rwrite=0;IDEX.Rread=1;IDEX.Mwrite=0;IDEX.Mread=0;IDEX.branch=0;IDEX.jump=1;IDEX.ResOrLd=0;IDEX.immt=4;IDEX.ALU=0;
                        }
                    }
                }
            }
        }
        
    }                                                      
}
int calALU(int ip1,int ip2,int ALU){
    int n;
    if(ALU==0){
        n=ip1+ip2;
    }else{
        if(ALU==1){
            n=ip1-ip2;
        }else{
            if(ALU==2){
                n= ip1 xor ip2;
            }else{
                if(ALU==3){
                    n=ip1 or ip2;
                }else{
                    if(ALU==4){
                        n=ip1 and ip2;
                    }
                }
            }
        }
    }
    return n;
}

void fetch(string inst[],pgc &PC, fede &FEDE,int max){
    cout<<"entered fetch "<<PC.av<<"id is "<<PC.pc<<endl;
    if(PC.stall == 1){
        return;
    }
    if(PC.av == 0){
        FEDE.av=0;
        FEDE.id=-1;
        return;
    }
    
    
    //to check pc is in the range
    if(PC.pc>max-1){
        PC.av=0;
        FEDE.av=0;
        FEDE.id=-1;
        return;
    }
        string in = inst[PC.pc];
        FEDE.in = in;
        FEDE.dpc = PC.pc;
        FEDE.av = 1;
        FEDE.id = PC.pc;
        PC.pc = PC.pc+1;
    
}

void decode(pgc &PC,fede &FEDE,int reg[], idex &IDEX,int regSt[]){
    cout<<"entered decode "<<FEDE.av<<" "<<"id is "<<FEDE.id<<" "<<endl;
    // cout<<"imm "<<IDEX.imm<<" "<<"rs1 "<<IDEX.rs1<<endl;
    if (FEDE.av == 0){
        IDEX.av=0;
        IDEX.id=-1;
        return;
    }
    string in = FEDE.in;
    string op=in.substr(25,7);
    int rs1 = getRs1(in);
    int rs2 = getRs2(in);
    // if(regSt[rs1]!=-1){
    //     FEDE.stall=1;
    //     PC.stall=1;
    // }else{
    //     FEDE.stall=0;
    //     PC.stall=0;
    // }s
    // if(op=="1100011"||op=="0110011"){
    //     if(regSt[rs2]!=-1){
    //     FEDE.stall=1;
    //     PC.stall=1;
    //     }else{
            
    //     }
    // }
    if(regSt[rs1]!=-1||((op=="1100011"||op=="0110011")&&regSt[rs2]!=-1)){
        cout<<"entered stall"<<endl;
         FEDE.stall=1;
        PC.stall=1;
    }else{
        cout<<"no stall"<<endl;
            FEDE.stall=0;
            PC.stall=0;
    }
    if(FEDE.stall == 1){
        IDEX.av=0;
        return;
    }

    int rd = getRd(in);
    if(op=="1100011"||op=="0100011"||op=="1101111"||op=="1100111"){

    }else{
        regSt[rd]=FEDE.id;
        cout<<"updating the st with "<<FEDE.id<<" at "<<rd<<endl;
    }
    generatecontrolsignal(in,IDEX);
    // cout<<"the type of immt is "<<IDEX.immt<<endl;
    int imm = getimm(in,IDEX.immt);
    // cout<<"string inside decode "<<in<<endl; 
    // cout<<"imm inside decode "<<imm<<endl;
    IDEX.av=1;
    IDEX.dpc = FEDE.dpc;
    IDEX.rs1 = reg[rs1];
    IDEX.rs2 = reg[rs2];
    IDEX.rd1 = rd;
    IDEX.imm = imm;
    IDEX.id = FEDE.id;


}

void execute(pgc &PC,fede &FEDE,idex &IDEX,exmem &EXMEM){
    cout<<"entered execute "<<IDEX.av<<" "<<"id id "<<IDEX.id<<" "<<endl;
    // cout<<"imm "<<IDEX.imm<<" "<<"rs1 "<<IDEX.rs1<<endl;
    if(IDEX.av==0){
        EXMEM.av=0;
        EXMEM.id=-1;
        return;
    }
    int ip1 = IDEX.rs1;
    int ip2=0;
    // cout<<"IDEX.RorI is "<<IDEX.RorI<<endl;
    if(IDEX.RorI == 0){
        // cout<<"selected rs2 "<<IDEX.rs2<<endl;
        ip2 = IDEX.rs2;
    }else{
        // cout<<"selected imm"<<endl;
        ip2 = IDEX.imm;
    }
    // cout<<"ALU type is "<<IDEX.ALU<<endl;
    int ALUout = calALU(ip1,ip2,IDEX.ALU);
    int zero = 0;
    if(ALUout == 0){
        zero =1;
    }
    int pc = IDEX.dpc;
    int imme = IDEX.imm;
    int bpc = pc+(imme*2)/4;
    if((zero == 1&&IDEX.branch == 1)||(IDEX.branch == 2&&ALUout!=0)){
        flushout(PC,FEDE,IDEX,bpc);
    }
    // cout<<"ALUout is "<<ALUout<<" "<<endl;
    EXMEM.av=1;
    EXMEM.ALUout = ALUout;
    EXMEM.rs2 = IDEX.rs2;
    EXMEM.rd1 = IDEX.rd1;
    EXMEM.Mread = IDEX.Mread;
    EXMEM.Mwrite = IDEX.Mwrite;
    EXMEM.Rwrite = IDEX.Rwrite;
    EXMEM.ResOrLd = IDEX.ResOrLd;
    EXMEM.id = IDEX.id;
}

void memory(exmem &EXMEM,memwr &MEMWR,vector<string> &mem,int **bit,int **tag,string ***cache,int **count,int &time){
    cout<<"entered memory "<<EXMEM.av<<"id is "<<EXMEM.id<<endl;
    if(EXMEM.av == 0){
        MEMWR.av=0;
        MEMWR.id=-1;
        return;
    }
    MEMWR.ALUout = EXMEM.ALUout;
    MEMWR.av=1;
    if(EXMEM.Mwrite == 1){
        // cout<<"wrting into the mem "<<EXMEM.rs2<<endl;
        writeCache(bit,tag,cache,count,MEMWR.ALUout,mem,EXMEM.rs2,time);
        // mem[MEMWR.ALUout] = EXMEM.rs2;
       
    }
    if(EXMEM.Mread == 1){
        // cout<<"Reading the memeory "<<MEMWR.ALUout<<" "<<mem[MEMWR.ALUout]<<endl;
        // MEMWR.memOut = mem[MEMWR.ALUout];
        MEMWR.memOut=readCache(bit,tag,cache,count,MEMWR.ALUout,mem,time);

    }
    MEMWR.Rwrite = EXMEM.Rwrite;
    MEMWR.rd1 = EXMEM.rd1;
    MEMWR.ResOrLd = EXMEM.ResOrLd;
    MEMWR.id = EXMEM.id;


}

void writeback(memwr &MEMWR,int reg[],int regSt[]){
    cout<<"entered writeback "<<MEMWR.av<<"id is "<<MEMWR.id<<endl;
    if(MEMWR.av == 0){
        return;
    }

    int ip =0;
    if(MEMWR.ResOrLd ==0){
        // cout<<"chose ALU "<<MEMWR.ALUout<<endl;
        ip = MEMWR.ALUout;
    }else{
        // cout<<"chose memout "<<MEMWR.memOut<<endl;
        ip = MEMWR.memOut;
    }
    if(MEMWR.Rwrite == 1){
        // cout<<"before writing writing into "<<MEMWR.rd1<<"writing ip "<<ip <<endl;
        reg[MEMWR.rd1]=ip;
        if(regSt[MEMWR.rd1] == MEMWR.id){
            regSt[MEMWR.rd1]=-1;
            cout<<"removing st at "<<MEMWR.rd1<<endl;
        }
        
    }
    
}

void run(exmem &EXMEM,memwr &MEMWR,pgc &PC,idex &IDEX,fede &FEDE,int reg[], vector<string> &mem,string inst[],int max,int regSt[],int **bit,int **tag,string ***cache,int **count,int &time){
    writeback(MEMWR,reg,regSt);
    memory(EXMEM,MEMWR,mem,bit,tag,cache,count,time);
    execute(PC,FEDE,IDEX,EXMEM);
    decode(PC,FEDE,reg,IDEX,regSt);
    fetch(inst,PC,FEDE,max);
}

bool check(exmem &EXMEM,memwr &MEMWR,pgc &PC,idex &IDEX,fede &FEDE){
    bool t=false;
    if(EXMEM.av==0&&MEMWR.av==0&&PC.av==0&&IDEX.av==0&&FEDE.av==0){
        t= true;
    }
    return t;
}
int main(){
     //create a register
    int reg[32];
    int regSt[32];

    int i,j;
    for(i=0;i<32;i++){
        reg[i]=0;
        regSt[i]=-1;
    }
    // reg[6]=12;
    // reg[7]=13;
    //create instruction memery
    string inst[100];
    // initialize registers
    pgc PC;
    fede FEDE;
    idex IDEX;
    exmem EXMEM;
    memwr MEMWR;
    
    
   // mem[5]=13;
    int time=0;
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
    int max=inputinstruction(inst);
    // cout<<"done tll here"<<endl;
    time++;
    max=max-2;
    cout<<"max "<<max<<endl;
    //initiate;
    PC.av=1;
    PC.pc=0;
    while(!check(EXMEM,MEMWR,PC,IDEX,FEDE)){
        run(EXMEM,MEMWR,PC,IDEX,FEDE,reg,mem,inst,max,regSt,bit,tag,cache,count,time);
    }
    cout<<"for the registers"<<endl;
    for(i=0;i<32;i++){
        cout<<i<<": "<<reg[i]<<endl;
    }
    cout<<"for the registerstatus"<<endl;
    for(i=0;i<32;i++){
        cout<<i<<": "<<regSt[i]<<endl;
    }
    // cout<<"first ten in memory"<<endl;
    // for(i=0;i<10;i++){
    //     cout<<i<<": "<<mem[i]<<endl;
    // }


    
    
}
