#include <iostream>
#include <algorithm>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sgtty.h>
const int MATRIXSIZE_Y=25,MATRIXSIZE_X=50;
std::list<char>CharList;
int CharListPos=0;
class gamegrid{
  public:
  std::string Gamegrid[MATRIXSIZE_Y][MATRIXSIZE_X];
  gamegrid(){
    floop_structure('.');
  }
  void floop_structure(char Character=' '){
    for(int y=0;y<MATRIXSIZE_Y;y++){
      for(int x=0;x<MATRIXSIZE_X;x++){
        if(Character==' '){
          Gamegrid[y][x]=CharList.empty()?'\0':*(std::next(CharList.begin(),CharListPos+x+y));
        }else{
          Gamegrid[y][x]=Character;
        }
      }
    }
  }
  void reset_gamegrid(){
    floop_structure('.');
  }
  void set_gamegrid_full_char(){
    floop_structure();
  }
  void gamegrid_dictionary(int GamegridSelection,char Character=CharList.empty()?'\0':*(std::next(CharList.begin(),CharListPos))){
    switch(GamegridSelection){
      case 1:
      reset_gamegrid();
      break;
      case 2:
      set_gamegrid_full_char();
      break;
    }
  }
  void display_gamegrid(){
    for(int y=0;y<MATRIXSIZE_Y;y++){
      for(int x=0;x<MATRIXSIZE_X;x++){
        std::cout<<Gamegrid[y][x];
        if(x==MATRIXSIZE_X-1){
          std::cout<<"\n";
        }
      }
    }
  }
};
gamegrid Gamegrid1;
void set_charlist(){
  for(char c=33;c<=126;++c){
    if(c!=' '){
      CharList.push_back(c);
    }
  }
}
void show_charlist_size(){
  int Count=0;
  for(auto Pos=CharList.begin();Pos!=CharList.end();Pos++){
    Count++;
    std::cout<<*Pos;
  }
  std::cout<<Count<<std::endl;
}
void increace_decreace_charlist(int IcrDcr){
  if(IcrDcr==1){
    if(CharListPos==93){
      CharListPos=0;
    }else{
      CharListPos++;
    }
  }else if(IcrDcr==2){
    if(CharListPos==0){
      CharListPos=93;
    }else{
      CharListPos--;
    }
  }
}
void input_dictionary(fd_set&ReadFds,struct timeval&Tv){
  char ch=' ';
  ch=getchar();
  if(ch!=EOF){
    switch(ch){
      case 'r':
      Gamegrid1.gamegrid_dictionary(1);
      break;
      case 's':
      Gamegrid1.gamegrid_dictionary(2);
      break;
      case 'i':
      increace_decreace_charlist(1);
      Gamegrid1.gamegrid_dictionary(2);
      break;
      case 'o':
      increace_decreace_charlist(2);
      Gamegrid1.gamegrid_dictionary(2);
      break;
    }
  }
}
int main(){
  set_charlist();
  srand((unsigned int)time(NULL));
  struct termios oldt,newt;
  tcgetattr(STDIN_FILENO,&oldt);
  newt=oldt;
  newt.c_lflag&=~(ICANON|ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&newt);
  struct timeval Tv;
  fd_set ReadFds;
  Tv.tv_sec=0;
  Tv.tv_usec=0;
  while(true){
    Gamegrid1.display_gamegrid();
    input_dictionary(ReadFds,Tv);
    std::cout<<"\033[2J\033[1;1H";
  }
}