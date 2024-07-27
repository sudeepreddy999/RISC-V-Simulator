#include <iostream>
#include <fstream>
using namespace std;

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

void inputinstruction(string inst[]){
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
}

void generatecontrolsignal(int *RorI,int *Rwrite,int *Rread,int *Mwrite,int *Mread,int *branch,int *jump,int *ResOrLd,int *imm,int *ALU,string s){
    string op=s.substr(25,7);
    string func3=s.substr(17,3);
    string func7=s.substr(0,7);
    if(op=="0110011"){
        *RorI=0;*Rwrite=1;*Rread=1;*Mwrite=0;*Mread=0;*branch=0;*jump=0;*ResOrLd=0;*imm=0;*ALU=0;
        if(func3=="000"){
            if(func7=="0000000"){
                *ALU=0;
            }else{
                *ALU=1;
            }
        }else{
            if(func3=="100"){
                *ALU=2;
            }else{
                if(func3=="110"){
                    *ALU=3;
                }else{
                    *ALU=4;
                }
            }
        }
    }else{
        if(op=="0010011"){
            *RorI=1;*Rwrite=1;*Rread=1;*Mwrite=0;*Mread=0;*branch=0;*jump=0;*ResOrLd=0;*imm=1;*ALU=0;
            if(func3=="000"){
            *ALU=0;
        }else{
            if(func3=="100"){
                *ALU=2;
            }else{
                if(func3=="110"){
                    *ALU=3;
                }else{
                    *ALU=4;
                }
            }
        }
        }else{
            if(op=="0000011"){
                *RorI=1;*Rwrite=1;*Rread=1;*Mwrite=0;*Mread=1;*branch=0;*jump=0;*ResOrLd=1;*imm=1;*ALU=0;
            }else{
                if(op=="0010011"){
                    *RorI=1;*Rwrite=0;*Rread=1;*Mwrite=1;*Mread=0;*branch=0;*jump=0;*ResOrLd=0;*imm=2;*ALU=0;
                }else{
                    if(op=="1100011"){
                        *RorI=0;*Rwrite=0;*Rread=1;*Mwrite=0;*Mread=0;*branch=1;*jump=0;*ResOrLd=0;*imm=3;*ALU=1;
                    }else{
                        if(op=="1101111"){
                            *RorI=0;*Rwrite=0;*Rread=1;*Mwrite=0;*Mread=0;*branch=0;*jump=1;*ResOrLd=0;*imm=4;*ALU=0;
                        }
                    }
                }
            }
        }
        
    }                                                      
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
int getimm(string s,int *imm){
    int n;
    string im;
    if(*imm==1){
        im=s.substr(0,12);
        
    }else{
        if(*imm==2){
            im=s.substr(0,7);
            im=im+s.substr(20,5);
        }else{
            if(*imm==3){
                im=im+s[0]+s[24];
                im=im+s.substr(1,6)+s.substr(20,4);

                
            }
        }
    }
    n=bintointImm(im);
    return n;
}
int calALU(int ip1,int ip2,int *ALU){
    int n;
    if(*ALU==0){
        n=ip1+ip2;
    }else{
        if(*ALU==1){
            n=ip1-ip2;
        }else{
            if(*ALU==2){
                n= ip1 xor ip2;
            }else{
                if(*ALU==3){
                    n=ip1 or ip2;
                }else{
                    if(*ALU==4){
                        n=ip1 and ip2;
                    }
                }
            }
        }
    }
    return n;
}

int main(){
    //create a register
    int reg[32];
    int i;
    for(i=0;i<32;i++){
        reg[i]=0;
    }
    //create instruction memery
    string inst[100];
    //create datamem;
    int mem[100];
    //create control signals
    int *RorI=new int;int *Rwrite=new int;int *Rread=new int;int *Mwrite=new int;int *Mread=new int;int *branch=new int;int *jump=new int;int *ResOrLd=new int;int *imm=new int;int *ALU=new int;
    int *zero=new int;
    int *lessthan=new int;
    //takeinput for string 
    inputinstruction(inst);
    // generatecontrolsignal(RorI,Rwrite,Rread,Mwrite,Mread,branch,jump,ResOrLd,imm,ALU,inst[0]);
    int pc=0;
    int npc;
    int bpc;
    int jpc;
    int n=0;
    while(inst[n]!=""){
        n++;
    }
    
    while(pc<n){
        cout<<"hi"<<endl;
        generatecontrolsignal(RorI,Rwrite,Rread,Mwrite,Mread,branch,jump,ResOrLd,imm,ALU,inst[pc]);
        int rd=reg[getRd(inst[pc])];
        int rdindex=getRd(inst[pc]);
        int rs1=reg[getRs1(inst[pc])];
        int rs2=reg[getRs2(inst[pc])];
        int imme=getimm(inst[pc],imm);
        int ip1=rs1;
        int ip2;
        
        if(*RorI==0){
            ip2=rs2;
        }else{
            ip2=imme;
        }
        cout<<rdindex<<" "<<ip1<<" "<<ip2<<endl;
        int res=calALU(ip1,ip2,ALU);
        if(res==0){
            *zero=1;
        }else{
            *zero=0;
        }
        cout<<res<<endl;
        if(*Mwrite==1){
            mem[res]=rs2;
        }
        int load=0;
        if(*Mread==1){
           load= mem[res];
        }
        int back=0;
        cout<<*ResOrLd<<endl;
        if(*ResOrLd==0){
            back=res;
        }else{
            back=load;
        }
        cout<<back<<endl;
        if(*Rwrite==1){
            reg[rdindex]=back;
        }
        npc=pc+1;
        bpc=pc+(imme*2)/8;
        if(*branch==1&&*zero==1){
            pc=bpc;
        }else{
            pc=npc;
        }
        
        
    }

    
    for(i=0;i<32;i++){
        cout<<i<<": "<<reg[i]<<endl;
    }
    
    
    

}