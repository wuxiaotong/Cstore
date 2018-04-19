#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include<stdio.h>
#include"externsort.cpp"
class DBController {
  public:
  	void getOperation(char* input);
  	void loadorders();
        void loadcustomer();
  	void retrieve(int input);
  	void externsort(char* in1,char* in2,
                        char* out1,char* out2);
  	void join(char* in, char* out);
};

#endif
