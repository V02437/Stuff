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
#ifdef _WIN32
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#endif
const int MATRIXSIZE_Y=30,MATRIXSIZE_X=30;
std::string Object="null",Action="null";
int FrameLag=0,n=10;
bool LagFrame=false,DevBreak=false;
char ch=' ';
std::string ball_matrix_structure(std::string Matrix[MATRIXSIZE_Y][MATRIXSIZE_X],int nChange=0){
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      int x=j-n/2;
      int y=i-n/2;
      if(x*x+y*y<=(n/2)*(n/2)){
        Matrix[i][j]="o";
      }else{
        Matrix[i][j]=" ";
      }
    }
  }
  return Matrix[MATRIXSIZE_Y-1][MATRIXSIZE_X-1];
}
class ball{
  public:
  std::string Matrix[MATRIXSIZE_Y][MATRIXSIZE_X];
  ball(){
    n=10;
    end_transformation();
  }
  void reset_size(){
    n=10;
    Action="reset";
    end_transformation();
  }
  void do_squeak(){
    static int SqueakCount=LagFrame;
    if(SqueakCount-LagFrame==0){
      SqueakCount=0;
    }else if(SqueakCount%LagFrame==0){
      n=n-(n/3);
    }
    SqueakCount++;
    Action="squeak";
    end_transformation();
    //play squeak sound//
  }
  void increace_size(){
    n++;
    Action="increace";
    end_transformation();
  }
  private:
  void end_transformation(){
    Matrix[MATRIXSIZE_Y-1][MATRIXSIZE_X-1]=ball_matrix_structure(Matrix);
  }
};
ball Ball1;
void display_matrix(std::string Matrix[MATRIXSIZE_Y][MATRIXSIZE_X]){
  for(int Column=0;Column<MATRIXSIZE_Y;Column++){
    for(int Row=0;Row<MATRIXSIZE_X;Row++){
      std::cout<<Matrix[Column][Row];
      if(Row==MATRIXSIZE_X-1){
        std::cout<<"\n";
      }
    }
  }
}
int object_input(std::string InputType="null"){
  if(InputType=="reset"){
    Ball1.reset_size();
    return 1;
  }else if(InputType=="squeak"){
    Ball1.do_squeak();
    return 2;
  }else if(InputType=="increace"){
    Ball1.increace_size();
    return 3;
  }
  return 0;
}
int do_frame_lag(){
  if(FrameLag==0){
    FrameLag=50;
  }
  if(FrameLag>0){
    if(FrameLag==1){
      FrameLag--;
      return 2;
    }
    FrameLag--;
    return 1;
  }
  return 0;
}
void action_lag_dictionary(){
  if(LagFrame){
    if(Object=="ball"){
      if(Action=="squeak"){
        if(int Result=do_frame_lag()==2){
          Ball1.reset_size();
        }
      }
    }
  }
}
void input_check(fd_set&ReadFds,struct timeval&Tv){
  FD_ZERO(&ReadFds);
  FD_SET(STDIN_FILENO,&ReadFds);
  if(select(STDIN_FILENO+1,&ReadFds,NULL,NULL,&Tv)==1){
    char PressedKey;
    ssize_t Result=read(STDIN_FILENO,&PressedKey,1);
    if(Result==-1){
      std::cerr<<"Error reading input"<<std::endl;
      DevBreak=true;
    }else if(PressedKey=='s'){
      if(int Result=object_input("squeak")==1){
        LagFrame=true;
        Object="ball";
      }
    }else if(PressedKey=='i'){
      object_input("increace");
    }else if(PressedKey=='r'){
      object_input("reset");
    }
  }
}
int main(){
  srand((unsigned int)time(NULL));
  using namespace std::this_thread;
  using namespace std::chrono;
  struct termios oldt,newt;
  tcgetattr(STDIN_FILENO,&oldt);
  newt=oldt;
  newt.c_lflag&=~(ICANON|ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&newt);
  struct timeval Tv;
  fd_set ReadFds;
  Tv.tv_sec=0;
  Tv.tv_usec=0;
  display_matrix(Ball1.Matrix);
  while(true){
    if(DevBreak){
      break;
    }
    input_check(ReadFds,Tv);
    display_matrix(Ball1.Matrix);
    action_lag_dictionary();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout<<"\033[2J\033[1;1H";
  }
  tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
  return 0;
}