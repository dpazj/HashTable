//Joe Riemersma
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "HashTable.h"

typedef HashTable<string,int> HTSI;

struct Item{
    string token;
    int occurence;
};

HTSI OT; //occurence table
vector<Item> scoreboard;

//Options
string filename = "inputfile.txt";
string keyword = "char";
unsigned ngramSize = 3;
unsigned topNGrams = 10;

int numberitems = 0;

vector<string> wordNGram;

using namespace std;


void DisplayScoreBoard(){
    float frequency;
    for(unsigned i=0;i<scoreboard.size();i++){
        frequency = (scoreboard.at(i).occurence / (float) numberitems) *100;
        frequency = roundf(frequency * 100) / 100;
        cout << scoreboard.at(i).token << ":" << frequency << endl;
    }


}

int SearchSB(string token){
    for(unsigned i=0;i<scoreboard.size();i++){
        if(scoreboard.at(i).token == token){
            return i;
        }
    }
    return -1;
}

void CompareScoreBoard(string token, int occurence){
    int index;
    int delIndex;
    bool toinsert = false;
    for(unsigned i =0;i < scoreboard.size(); i++){   //finds if we insert
        if(occurence >= scoreboard.at(i).occurence){
            index = i;
            toinsert = true;
            break;
        }
    }
    if(!toinsert){return;}

    Item temp;temp.token = token;temp.occurence = occurence;

    //remove old number if present
    delIndex = SearchSB(token);
    if(delIndex != -1){
        unsigned i = delIndex;
        while(i < scoreboard.size()-1){
            scoreboard.at(i) = scoreboard.at(i+1);
            i++;
        }
        scoreboard.pop_back();
        scoreboard.insert(scoreboard.begin()+index,temp);
    }else{
        scoreboard.insert(scoreboard.begin()+index,temp);
        scoreboard.pop_back();
    }
}

void ProcessChars(string line){
    string token;
    int occurence;
    if(line.length() < ngramSize){return;}

    for(unsigned i=0;i<line.length()-(ngramSize +1); i++){
        token = line.substr(i, ngramSize);
        try{
            OT.insert(token,1);
            occurence = 1;
        }catch(HashTableError e){
            occurence = OT.getValue(token) + 1;
            OT.replaceValue(token, occurence);
        }
        numberitems++;
        CompareScoreBoard(token,occurence);
    }
}

void AddWords(){
    int occurence;
    string token;
    if(wordNGram.size() < ngramSize){return;}

    for(unsigned i=0;i< wordNGram.size()-(ngramSize + 1);i++){
        token.erase();
        for(unsigned j=i;j<ngramSize+i;j++){
            token += wordNGram.at(j);
            if(j != (ngramSize+i)-1){
                token += " ";
            }
        }
        try{
            OT.insert(token,1);
            occurence = 1;
        }catch(HashTableError e){
            occurence = OT.getValue(token) + 1;
            OT.replaceValue(token, occurence);
        }
        numberitems++;
        CompareScoreBoard(token,occurence);
    }
}

void ProcessWord(string line){
    string toAdd;
	for(stringstream s(line); s >> toAdd;){
	    wordNGram.push_back(toAdd);
	}
}

bool ProcessFile(){
    string line;
    ifstream file(filename.c_str());
    if(file.is_open()){
        while(getline(file,line)){
            if(keyword == "char"){
                ProcessChars(line);
            }else if(keyword == "word"){
                ProcessWord(line);
            }else if(keyword == "decimal"){
                cout << "Was unable to understand what was being asked here :)" << endl;
            }
        }
        file.close();
    }else{
        cout << "Unable to open '" << filename << "', make sure it exist." << endl;
        return false;
    }
    if(keyword == "word"){
        AddWords();
    }
    return true;
}


bool TestInt(char* argv){ //Test argument char array for an integer
	string s(argv);
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    char * p ;
    strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

bool ParseArgs(int argc, char* argv[]){
    if(argc > 1){
        if(argc < 6){
            if(argc > 4){
                string a = string(argv[4]);
                if(a != "word" && a != "char" && a != "decimal"){
                    cout << a << endl;
                    cout << "Parameter 4 should be either: 'word' | 'char' | 'decimal'" << endl;
                    return false;
                }
                keyword = a;
            }
            if(argc > 3){
                if(!TestInt(argv[3])){
                    cout << "Parameter 3 should be an integer"<< endl;
                    return false;
                }
                topNGrams = atoi(argv[3]);
            }
            if(argc > 2){
                if(!TestInt(argv[2])){
                    cout << "Parameter 2 should be an integer" << endl;
                    return false;
                }
                ngramSize = atoi(argv[2]);
            }
            filename = string(argv[1]);
        }else{
            cout << "Too many Parameters!" << endl;
        }
    }
    return true;
}

void initScoreBoard(){
    Item newItem;
    newItem.occurence = -1;
    newItem.token = "";
    for(unsigned i=0; i<topNGrams;i++){
        scoreboard.push_back(newItem);
    }
}

int main(int argc, char* argv[]){
    if(!ParseArgs(argc,argv)){return 1;}
    initScoreBoard();
    if(!ProcessFile()){return 1;}
    DisplayScoreBoard();

    return 0;
}
