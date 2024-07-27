#include <iostream>
#include <fstream>
using namespace std;

//function to convert int to bin
int getindex(string sd,int j){
    int i=j;
    while(!(sd[i]==','||sd[i]=='(')){
        i++;
    }
    return i+1;
}
int getint(string sd,int i){
    int n=0;
    while(!(sd[i]==','||sd[i]=='('||sd[i]==')')&&i<sd.length()){
        n=n*10+sd[i]-48;
        i++;
    }
    return n;
}
string IntTOBin(int n){
    string s="";
    while(n>0){
        if(n%2==0){
            s="0"+s;
        }else{
            s="1"+s;
        }
        n=n/2;
    }
    return s;
}

string regtobin(int n){
    string s=IntTOBin(n);
    while(s.length()<5){
        s="0"+s;
    }
    return s;
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
string convimm12(int n){
    string s="";
    if(n>=0){
        s=IntTOBin(n);
        
        while(s.length()<12){
            s="0"+s;
        }
        if(s[0]=='1'){
            
            s='0'+s;
        }

    }else{
        n=n*-1;
        s=IntTOBin(n);
        while(s.length()<12){
            s="0"+s;
        }
        if(s[0]=='1'){
            s='0'+s;
        }
        s=concompl(s);
    }
    return s;
}

bool checkimmediate(string si){
    if(si=="ADDI"||si=="SLTI"||si=="SLTIU"||si=="XORI"||si=="ORI"||si=="ANDI"||si=="SLLI"||si=="SRLI"||si=="SRAI"){
        return true;
    }else{
        return false;
    }
}
bool checkRtype(string si){
    if(si=="ADD"||si=="SUB"||si=="XOR"||si=="OR"||si=="AND"){
        return true;
    }else{
        return false;
    }
}
bool checkLtype(string si){
    if(si=="LB"||si=="LH"||si=="LW"||si=="LD"){
        return true;
    }else{
        return false;
    }
}
bool checkStype(string si){
    if(si=="SB"||si=="SH"||si=="SW"||si=="SD"){
        return true;
    }else{
        return false;
    }
}
bool checkBtype(string si){
    if(si=="BEQ"||si=="BNE"||si=="BLT"){
        return true;

    }else{
        return false;
    }
}

string func3imm(string si){
    string s="";
    if(si=="ADDI"){
        s="000";
    }else{
        if(si=="ORI"){
            s="110";
        }else{
            if(si=="XORI"){
                    s="100";
                }else{
                    if(si=="ANDI"){
                        s="111";
                    }
                
            }
        }
    }
    return s;
}
string func3Rtype(string si){
    string s="";
    if(si=="ADD"){
        s="0000000000";
    }else{
        if(si=="SUB"){
            s="0100000000";
        }else{
            if(si=="XOR"){
                s="0000000100";
            }else{
                if(si=="OR"){
                    s="0000000110";
                }else{
                    if(si=="AND"){
                        s="0000000111";
                    }
                }
            }
        }
    }
    return s;
}

string func3Ltype(string si){
    string s;
    if(si=="LB"){
        s="000";
    }else{
        if(si=="LH"){
            s="001";
        }else{
            if(si=="LW"){
                s="010";
            }else{
                if(si=="LD"){
                    s="011";
                }
            }
        }
    }
    return s;
}
string func3Stype(string si){
    string s;
    if(si=="SB"){
        s="000";
    }else{
        if(si=="SH"){
            s="001";
        }else{
            if(si=="SW"){
                s="010";
            }else{
                if(si=="SD"){
                    s="011";
                }
            }
        }
    }
    return s;
}
string func3Btype(string si){
    string s;
    if(si=="BEQ"){
        s="000";
    }else{
        if(si=="BNE"){
            s="001";
        }else{
            if(si=="BLT"){
                s="100";
            }
        }
    }
    return s;
}
string converttoBIN(string si,string sd){
    string s="";
    if(checkimmediate(si)){
        int i=1;
        int rd=getint(sd,i);
        i=getindex(sd,i);
        i++;
        int rs1=getint(sd,i);
        i=getindex(sd,i);
        int imm;
        
        
        if(sd[i]=='-'){
            i++;
            imm=getint(sd,i);
            imm=imm*-1;
        }else{
            imm=getint(sd,i);
        }
        
        s=s+convimm12(imm);
        s=s+regtobin(rs1);
        s=s+func3imm(si);
        s=s+regtobin(rd);
        s=s+"0010011";
    }else{
        if(checkRtype(si)){
            int i=1;
            int rd=getint(sd,i);
            string t=func3Rtype(si);
            i=getindex(sd,0);
            i++;
            int rs1=getint(sd,i);
            i=getindex(sd,i);
            i++;
            int rs2=getint(sd,i);
            for(i=0;i<7;i++){
                s=s+t[i];
            }
            s=s+regtobin(rs2);
            s=s+regtobin(rs1);
            s=s+t[7]+t[8]+t[9];
            s=s+regtobin(rd);
            s=s+"0110011";
        }else{
            if(checkLtype(si)){
                int i=1;
                int rd=getint(sd,i);
                string t;
                i=getindex(sd,0);
                
                int imm;
                if(sd[i]=='-'){
                    i++;
                    
                    imm=getint(sd,i);
                    imm=imm*-1;
                }else{
                    imm=getint(sd,i);
                }
                
                s=s+convimm12(imm);

                i=getindex(sd,i);
                i++;
                int rs1=getint(sd,i);
                s=s+regtobin(rs1);
                s=s+func3Ltype(si);
                s=s+regtobin(rd);
                s=s+"0000011";
                

            }else{
                if(checkStype(si)){
                    
                    int i=1;
                    int rs2=getint(sd,i);
                    // cout<<"rs2 "<<rs2<<endl;
                    int imm;
                    i=getindex(sd,0);
                    if(sd[i]=='-'){
                        i++;
                        imm=getint(sd,i);
                        imm=imm*-1;   
                    }else{
                     imm=getint(sd,i);
                    }
                    // cout<<"immediate "<<imm<<endl;
                    int j=i;
                    string t=convimm12(imm);
                    for(i=0;i<7;i++){
                        s=s+t[i];
                    }
                    // cout<<"immediate String "<<t<<endl;
                    i=getindex(sd,j);
                    i++;
                    int rs1=getint(sd,i);
                    // cout<<"rs1 "<<rs1<<endl;
                    s=s+regtobin(rs2);
                    s=s+regtobin(rs1);
                    s=s+func3Stype(si);
                    // cout<<"functype "<<func3Stype(si)<<endl;
                    s=s+t[7]+t[8]+t[9]+t[10]+t[11];
                    s=s+"0100011";
                    
                }else{
                    if(checkBtype(si)){
                        int i=1;
                        int rs1=getint(sd,i);
                        i=getindex(sd,i);
                        i++;
                        int rs2=getint(sd,i);
                        i=getindex(sd,i);
                        int imm;
                        
                        
                        if(sd[i]=='-'){
                            i++;
                            imm=getint(sd,i);
                            imm=imm*-1;
                        }else{
                            imm=getint(sd,i);
                        }
                        
                        string t =convimm12(imm);
                        
                        
                        if(t.length()==12){
                            char c=t[0];
                            t=c+t;
                        }
                        
                        
                        
                        s=s+t[0];
                        for(i=2;i<=7;i++){
                            s=s+t[i];
                        }

                        s=s+regtobin(rs2);
                        s=s+regtobin(rs1);
                        s=s+func3Btype(si);
                        for(i=8;i<=11;i++){
                            s=s+t[i];
                        }
                        s=s+t[1];
                        s=s+"1100011";
                        

                    }else{
                        if(si=="JUMP"){
                        int i=0;
                        int imm=getint(sd,i);
                        string t= convimm12(imm);
                         if(t[0]=='0'){
                            while(t.length()<25){
                                t='0'+t;
                            }
                         }else{
                            while(t.length()<25){
                                t='1'+t;
                            }
                         }
                         s=s+t;
                         s=s+"1101111";
                    }
                    }
                }
            }
        }
    }





    return s;
}



int main(){
    string s;
    ofstream myFile;
    myFile.open("output.txt");
    ifstream file;
    file.open("input.txt");
    string si,sd;
    while(file.good()){
    getline(file,s);
    si=s.substr(0,s.find(" "));
    sd=s.substr(s.find(" ")+1,s.length()-si.length()-1);
    myFile<<converttoBIN(si,sd)<<endl;
    }
    file.close();
    
    myFile.close();
    }

    
