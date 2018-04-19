#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "externsort.h"
#define MAXSIZE 131072
#define MAX    100000000
#define MP 1024*35

/*    this struct is used in compressing     *
 *    with 2 attributes:  num is the value,  *
 *    and len calculates how many times the  *
 *    value appears                          */
struct Comp{
  int num;
  int len;
};

/*********        QuickSort        *************
 * this version of quicksort is specially      *
 * written for sort two columns, the colunm b  *
 *  is sorted along with columns a.            *  
 *  After sorted, column a will be ordered and *
 *  the correspondence of a and b will remain  * 
 *  the same.                                  */  
void swap(int& a, int& b) {
  int temp;
  temp = a;
  a = b;
  b = temp;
}
int partition(int *a, int* b,int left, int right, int pivotIndex) {
  int pivotValue = a[pivotIndex];
  swap(a[pivotIndex], a[right]);
  swap(b[pivotIndex], b[right]);
  int storeIndex = left;
  for(int i = left; i < right; i++) {
    if (a[i] < pivotValue) {
swap(a[storeIndex], a[i]);
swap(b[storeIndex], b[i]);
storeIndex = storeIndex+1;
    }
  }
  swap(a[right], a[storeIndex]);
  swap(b[right], b[storeIndex]);
  return storeIndex;
}
void  quicksort_two_columns(int* a, int*b, int left, int right) {
  if (right > left) {
    int pivotIndex = left;
    int pivotNewIndex;
    pivotNewIndex = partition(a,b, left, right, pivotIndex);
    quicksort_two_columns(a,b, left, pivotNewIndex - 1);
    quicksort_two_columns(a,b, pivotNewIndex + 1, right);
  }
}
// initialization
Externsort::Externsort(char *in1, char* in2,  char *out1, char* out2) {
  in_file1 = new char[strlen(in1) + 1];
  strcpy(in_file1, in1);
  in_file2 = new char[strlen(in2) + 1];
  strcpy(in_file2, in2);
  out_file1 = new char[strlen(out1) + 1];
  strcpy(out_file1, out1);
  out_file2 = new char[strlen(out2) + 1];
  strcpy(out_file2, out2);
  count = 0;
}

void Externsort::comp_sort() {
  file_sort();
  merge_sort();
  compress();
}

void Externsort::compress() {
  FILE *f = fopen(out_file1, "rb");
  FILE *a = fopen("bin/compcustkey.tbl","wb");
  int data[1024];
  Comp comp[1024];
  int count, order = 0;
  while (fread(data, sizeof(int), 1024, f)) {
    for(int i = 0; i < 1024; i++) {
      count = 1;
      while(data[i] == data[i + 1] && i+1 < 1024) {
       count++;
       i++;
      }
      if ( i+1-count == 0 && data[i] == comp[order-1].num) {
        comp[order-1].len += count; 
      } else {
        comp[order].num = data[i];
        comp[order].len = count;
        order++;
      }
      if(order == 1023) {
        fwrite(comp, sizeof(Comp), 1024, a);
    order = 0;
      }
    }
  }
  fclose(f);
  fclose(a);
}
char* Externsort::temp_filename1(int num) {
  char *tempfile = new char[15];
  sprintf(tempfile, "bin/a%d.tbl", num);                 // generate the file names in order
  return tempfile;
}
char* Externsort::temp_filename2(int num) {
  char *tempfile = new char[15];
  sprintf(tempfile, "bin/b%d.tbl", num);                // generate the file names in order
  return tempfile;
}

/*********        FileSort        *************
 * This function will seperate the column into* 
 * several parts and then sort each part and  * 
 * store them into different temp files.      *
 * The number of temp files will be store in  *
 * the private member count.                  */
void Externsort::file_sort() {
  FILE *f1 = fopen(in_file1,"rb");
  FILE *f2 = fopen(in_file2,"rb");
  int data1[MAXSIZE/2+1];
  int data2[MAXSIZE/2+1];
  int num = 0, record;
  while(fread(data1, 4, MAXSIZE/2, f1)) {

fread(data2, 4, MAXSIZE/2, f2);
if (data1[MAXSIZE/2 -1 ] != -1) {
  quicksort_two_columns(data1, data2, 0, MAXSIZE/2-1);         // sort the two columns
  record = MAXSIZE/2;
} else {
  record = 1500000 % (MAXSIZE/2);
      quicksort_two_columns(data1, data2, 0, record - 1);
}
    char *filename1 = temp_filename1(num);
    FILE *temp1 = fopen(filename1,"wb+");              // write the ordered arrays into temp files
    delete []filename1;
    fwrite(data1, sizeof(int), record, temp1);
    fclose(temp1);
    FILE *temp2 = fopen(temp_filename2(num++),"wb+");            //wirte the ordered arrays into temp files
    fwrite(data2, sizeof(int), record, temp2);
    fclose(temp2);
    data1[MAXSIZE/2 - 1] = -1;
  }
  fclose(f1);
  fclose(f2);
  count = num;
}

/**********        ExternSort        *************
 * Using merge sort to integrate the temp files  *
 * generated in file_sort into a completed orderd*
 * file for each column                          * 
 * The ordered files will be store in out_file1  *
 * and out_file2                                 */  
void Externsort::merge_sort() {
  if(count <= 0) return;
  FILE *f1 = fopen(out_file1, "wb");
  FILE *f2 = fopen(out_file2, "wb");
  FILE* *temp1 =  new FILE*[count];
  FILE* *temp2 =  new FILE*[count];
  int i, data1[count], data2[count];                                 // data[count] store the first unsorted 
                                                              // number of correspoinding temp file
  bool hasNext[count];                                               // detect if the file is over
  memset(data1, 0, sizeof(int)*count);
  memset(data2, 0, sizeof(int)*count);
  memset(hasNext, 1, sizeof(bool)*count);
  
  for (i = 0; i < count; i++) {
   temp1[i] = fopen(temp_filename1(i),"rb");
   temp2[i] = fopen(temp_filename2(i),"rb");
   fread(&data1[i], 4, 1, temp1[i]);
   fread(&data2[i], 4, 1, temp2[i]);
  }
  int sort1[MP], sort2[MP],p = 0;
  int nm;
  int j = 0;
  while(j != count ) {
    int min = MAX     ;                                 
for (i = 0; i < count; i++) {
        if (hasNext[i] && min > data1[i]) {
          min = data1[i];
          nm = i;
      }
    }
    sort1[p] = data1[nm];
    sort2[p] = data2[nm];
    p++;
    if(p == MP) {                                              // if the arrays are filled, write them into files
p = 0;
fwrite(sort1, sizeof(int), MP, f1);
fwrite(sort2, sizeof(int), MP, f2);
    }

    if(fread(&data1[nm],sizeof(int),1,temp1[nm])==0){
      hasNext[nm] = 0;
  j++;
           fclose(temp1[nm]);
           fclose(temp2[nm]);
} else {
fread(&data2[nm], sizeof(int),1, temp2[nm]);
}
  }
  
  if(p != MP) {
    fwrite(sort1, sizeof(int), p, f1);
    fwrite(sort2, sizeof(int), p, f2);
  }
  /*for(i = 0; i < count; i++) {
   fclose(temp1[i]);
   fclose(temp2[i]);
  }*/
  delete []temp1;
  delete []temp2;
  fclose(f1);
  fclose(f2);
  
// the temp files will not be used anymore, remove them
  for(i = 0; i < count; i++) {
    remove(temp_filename1(i));
    remove(temp_filename2(i));
  }
}



