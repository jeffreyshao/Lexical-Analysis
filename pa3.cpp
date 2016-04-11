#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "pa3.h"

using namespace std;

//Allow command line arguments so that we can extract the name of the .txt file
int main(int argc, const char* argv[]){

  //Symbols that may need to be separated from "variables"
  //Anything not explicitly listed I don't count. Parenthesis will be deleted though
  string symbols[10] = {"(",")",",",";","++","*","/","+","-","="};

  //Assorted variables used
  int pos;                                 //used to determine position of symbol
  string node;                             //stores the value of the stack node
  string comp;                             //stores the value of the symbol
  string words;                            //stores value of symbol-free string
  bool tiny;                               //determines if the string is lowercase
  bool digit;                              //determines if the string contains just numbers
  int depth = 0;                           //initial loop-depth counter
  int maxDepth = 0;                        //final loop-depth counter
  bool present;                            //determines if the node is already present

  //We will make use of multiple stacks to determine the outputs
  Stack* Main = new Stack();               //orignal stack
  Stack* Temp = new Stack();               //temporary stack for symbol-free strings
  Stack* KeywordsT = new Stack();          //Initial Keyword Stack
  Stack* Keywords = new Stack();           //Keyword Stack in the correct order
  Stack* Operators = new Stack();          //Rest are self-explanatory
  Stack* Identifiers = new Stack();
  Stack* Constants = new Stack();
  Stack* Delimiters = new Stack();
  Stack* SyntaxErrors = new Stack();

  //Read in the file
  ifstream file(argv[1]);

  //This is used to make the first stack where everything is stored
  string str;
  char x[100];
  while(file.good()){
    file >> x;
    str = x;
    Main->push(str);
  }
  //For some reason, the final string is pushed twice, so we pop it here once
  Main->pop();

  //1. check for delimiters and operators within the same string

  //while your stack is still occupied
  while(Main->getStackPtr(Main)){
    //take in the first node
    node = Main->read();
    for(int i=0; i<10; i++){               //cycle through all symbols
      string sym = symbols[i];
      //because ++ and -- 2 characters, we deal with it elsewhere
      if(symbols[i].length() == 1){
        if((pos = node.find(sym)) != string::npos){
          string comp = node.substr(pos, 1);
          if (comp == "("){
            node.replace(pos,1," ");       //If it's a parenthesis, just erase it
          }
          else if (comp == ")"){
            node.replace(pos,1," ");
          }
          else if (comp == ","){
            //if it doesn't already exist, and it's not parenthesis, push it
            present = Delimiters->exists(Delimiters, comp);
            if(!present){
              Delimiters->push(comp);
            }
            node.replace(pos,1," ");
          }
          else if (comp == ";"){
            present = Delimiters->exists(Delimiters, comp);
            if(!present){
              Delimiters->push(comp);
            }
            node.replace(pos,1," ");
          }
          else if (comp == "+"){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,1," ");
          }
          /*I will also be assuming any - followed by a constant is not a negative
            number, but rather an attempt to subtract by a constant value*/
          else if (comp == "-"){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,1," ");
          }
          //any instance of "--","**","//", etc. will just be compiled as operators listed twice
          else if (comp == "*"){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,1," ");
          }
          else if (comp == "/"){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,1," ");
          }
          else if (comp == "="){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,1," ");
          }
          else if (comp == "%"){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,1," ");
          }
        }
      }
      //used for ++
      else{
        if((pos = node.find(sym)) != string::npos){
          //take the substr up to 2 spots now
          string comp = node.substr(pos, 2);
          //and do the same as before
          if (comp == "++"){
            present = Operators->exists(Operators, comp);
            if(!present){
              Operators->push(comp);
            }
            node.replace(pos,2," ");
          }
        }
      }
    }
    //by the end, all the symbols should be in the appropriate stacks

    //if there are any spaces in the new symbol-free string
    if(node != "" && (node.find(" ")) != string::npos){
      //this method splits them up into different strings
      string words;
      char* input = new char [node.length()+1];
      strcpy (input, node.c_str());
      char* token = strtok(input, " ");
      while (token != NULL) {
            words = token;
            Temp->push(words);             //here we put it into the temporary stack
            token = strtok(NULL, " ");
      }
    }
    //Otherwise, we just dump it into the temporary stack
    else{
      Temp->push(node);
    }
    Main->pop();
  }

  //2. Process the temporary stack and sort its contents accordingly
  while(Temp->getStackPtr(Temp)){
    node = Temp->read();
    //this determines if the string is all lowercase or all numbers
    char* str = new char [node.length()+1];
    strcpy (str, node.c_str());
    int i = 0;
    char c;
    //used for casing
    while(str[i]){
      c=str[i];
      if (islower(c)){
        tiny = true;
      }
      else{
        tiny = false;
        break;
      }
      i++;
    }
    //used for numbers
    i = 0;
    while(str[i]){
      c=str[i];
      if (isdigit(c)){
        digit = true;
      }
      else{
        digit = false;
        break;
      }
      i++;
    }

    //if it's a keyword
    if(node == "FOR" || node == "BEGIN" || node == "END"){
      Keywords->push(node);
    }
    //if it's just a jumble of lower case characters and doesn't already exist
    else if(tiny){
      present = Identifiers->exists(Identifiers, node);
      string c = node;
      if(!present){
        Identifiers->push(node);
      }
    }
    //if it's all numbers and doesn't already exist
    else if(digit){
      present = Constants->exists(Constants, node);
      if(!present){
        Constants->push(node);
      }
    }
    //everything else is considered a syntax error
    else{
      present = SyntaxErrors->exists(SyntaxErrors, node);
      if(!present){
        SyntaxErrors->push(node);
      }
    }
    Temp->pop();
  }

  //3. Determine the nested loop depth

  //Put all keywords into the main Keywords Stack so that they're in the right order
  Keywords->swap(Keywords, KeywordsT);
  while(KeywordsT->getStackPtr(KeywordsT)){
    node = KeywordsT->read();
    //if we reach a FOR, then we're at the start of a new loop
    if(node == "FOR"){
      //and we increase the depth
      depth++;
    }
    //if we reach an END, then we're ending a loop
    else if(node == "END"){
      //And if the depth number is higher than the current max depth
      if(depth > maxDepth){
        //then you update the max depth
        maxDepth = depth;
      }
      //and decrease the depth number, for future comparisons' sake
      if(depth !=0){
        depth--;
      }
    }
    present = Keywords->exists(Keywords, node);
    if(!present){
      Keywords->push(node);
    }
    KeywordsT->pop();
  }

  //4.Spit out all the necessary stuff
  cout << "The maximum depth of nested loop(s) is " << maxDepth <<endl;
  cout << "\n";
  cout << "Keywords: ";
  Keywords->vomit();
  cout << "\n";
  cout << "Identifiers: ";
  Identifiers->vomit();
  cout << "\n";
  cout << "Constants: ";
  Constants->vomit();
  cout << "\n";
  cout << "Operators: ";
  Operators->vomit();
  cout << "\n";
  cout << "Delimiters: ";
  Delimiters->vomit();
  cout << "\n";
  cout << "Syntax Errors: ";
  SyntaxErrors->vomit();
  cout << "\n \n";

  return 0;
}
