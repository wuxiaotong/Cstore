#ifndef EXTERN_SORT_H
#define EXTERN_SORT_H
#include <stdio.h>

/**********        Externsort        *************
 * This class can sort and compress big data and * 
 * it's specially used for sort two columns      *
 * synchronously(one column changed in pace of   *
 * another).                                     */ 
class Externsort{
  public:
  	Externsort(char *in1, char *in2, char *out1, char *out2);    // initialize the paths of input and output
  	void file_sort();                                            // pass0, use quicksort to generated sorted temp files 
        void merge_sort();
  	void comp_sort();                                            
        void compress();
  private:
  	char *in_file1, *in_file2;
  	char *out_file1, *out_file2;                                 
  	int count;                                                  // store the numbers of files generated in pass0
  protected:
  	char* temp_filename1(int num);                              // generate the names of temp files for the first columns
  	char* temp_filename2(int num);                              // generate the names of temp files for the second counmns
};

#endif


