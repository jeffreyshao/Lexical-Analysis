#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Stack{
private:

  //basic item structure
  struct item{
    string name;
    //item pointer. points to item below
    item* prev;
  };

  //stack pointer. points to top of stack
  item* stackPtr;

public:
  //constructor
  Stack(){
    //makes the stackpointer point at nothing
    stackPtr = NULL;
  }

  void push(string id){
    item* n = new item;
    n->name = id;
    //if stack is empty
    if (stackPtr == NULL){
      //stackPtr now points to the new item
      stackPtr = n;
      //and it points to nothing
      stackPtr->prev = NULL;
    }
    //if there is a stack
    else{
      //connects the new item to the top of the stack
      n->prev = stackPtr;
      //resets the top stack pointer
      stackPtr = n;
    }
  }

  void pop(){
    //if the stack is empty
    if(stackPtr == NULL){
      cout<<"Ain't shit here" <<endl;
    }
    else{
      item* p = stackPtr;
      //lower the stackPtr
      stackPtr = stackPtr->prev;
      //separate item from stack
      p->prev = NULL;
    }
  }

  //returns string in current stack node
  string read(){
    item* p = stackPtr;
    return p->name;
  }

  //returns stackPtr since it was a private variable
  item* getStackPtr(Stack* Flapjack){
    return Flapjack->stackPtr;
  }

  //prints content of stack
  void vomit(){
    item* p = stackPtr;
    //while it's not pointing to NULL
    while(p){
      //print out contents
      cout << p->name << " ";
      //move it down
      p = p->prev;
    }
  }

  /*Quickly puts everything into a different stack, thereby reversing the order
    of the stack's contents*/
  void swap(Stack* Flapjack, Stack* Cash){
    string up;
    while(Flapjack->stackPtr){
      up = Flapjack->read();
      Cash->push(up);
      Flapjack->pop();
    }
  }

  //used to determine if the string intended to be pushed already exists
  bool exists(Stack* Flapjack, string homeless){
    item* p;
    p = Flapjack->stackPtr;
    bool ocupado = false;
    string checkit;
    while(p){
      checkit = p->name;
      //if it's already there
      if(homeless == checkit){
        //then it exists
        ocupado = true;
        break;
      }
      //move it down
      p = p->prev;
    }
    return ocupado;
  }

};
