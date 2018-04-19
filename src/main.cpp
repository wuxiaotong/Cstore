//created by wuxiaotong 
#include<stdio.h>
#include<math.h>
#include<string.h>
#include"DBController.cpp"

#define SIZE 1024
void text(){
  /*FILE *fp = fopen("bin/o_shippriority.tbl","rb+");
  int count = 100000;
  int data[100000];
  while(fread(data, sizeof(int), count, fp)!=0){
    for(int i = 0; i < count; i++)
      printf("%d\n",data[i]);
  }
  fclose(fp);*/
  FILE *f, *a, *b , *c , *d;
  int count = 0;
  char osta;
  char oprio[40], clerk[40], data[500],comm[90];
  int da, ye, mo, le, nu;
  int orderkey[SIZE];
  int custkey[SIZE];
  double orderprice[SIZE];
  int shippriority[SIZE];
  f = fopen("orders.tbl", "rb");
  a = fopen("bin/o_orderkey.tbl", "rb");
  b = fopen("bin/o_custkey.tbl", "rb");
  c = fopen("bin/o_orderprice.tbl", "rb");
  d = fopen("bin/o_shippriority.tbl", "rb");
  
  int f1[SIZE],f2[SIZE],f4[SIZE];
  double f3[SIZE];
  while(fgets(data,500,f)!=NULL) {

  	sscanf(data,"%d|%d|%c|%lf|%d-%d-%d|%d-%s|%s#%d|%d|%s|",&orderkey[count], &custkey[count], &osta,
                                                     &orderprice[count], &da, &ye, &mo, &le, oprio,
                                                     clerk, &nu, &shippriority[count], comm);
	  count++;
  	if (count == SIZE) {
	    fread(f1,sizeof(int),SIZE,a);
  	    fread(f2,sizeof(int),SIZE,b);
  	    fread(f3,sizeof(double),SIZE,c);
  	    fread(f4,sizeof(int),SIZE,d);    
  	    for(int i = 0; i < count; i++) {
              if(shippriority[i] != f4[i])
                printf("before:%d error:%d\n",shippriority[i],f4[i]);
              
            }
            count = 0;
         }
  }
  if (count != SIZE) {

  }
 
  fclose(a);
  fclose(b);
  fclose(c);
  fclose(d);
  fclose(f);
}

int main(int argc, char* argv[]){
  //text();
  DBController a;
  if(argc > 2){
    if(strcmp(argv[1],"load") == 0 &&strcmp(argv[2],"orders")==0) {
      a.loadorders(); 
    } else if(strcmp(argv[1],"load") == 0 &&strcmp(argv[2],"customer")==0) {
      a.loadcustomer();
    } else if(strcmp(argv[1],"retrieve") == 0 && strcmp(argv[2],"orders") == 0){
      int tmp;
      while(scanf("%d", &tmp) != EOF) {
        a.retrieve(tmp);
      }
    } else if(strcmp(argv[1],"compress") == 0 && strcmp(argv[2],"orders") == 0 
              && strcmp(argv[3],"1") == 0) {
       char f1[] = "bin/o_custkey.tbl";
       char f2[] = "bin/o_orderkey.tbl";
       char f3[] = "bin/sort_o_custkey.tbl";
       char f4[] = "bin/sort_o_orderkey.tbl";
       a.externsort(f1, f2, f3, f4);
    }
  }
} 
