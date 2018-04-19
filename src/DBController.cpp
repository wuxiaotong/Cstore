//created by wuxiaotong 
#include "DBController.h"

#define SIZE 1024

void DBController::getOperation(char* input) {
  
}
void DBController::loadorders() {
  FILE *f, *a, *b , *c , *d;
  int count = 0;
  char osta;
  char oprio[40], clerk[40], data[500],comm[120],date[20];
  int da, ye, mo;
  int orderkey[SIZE];
  int custkey[SIZE];
  double orderprice[SIZE];
  int shippriority[SIZE];
  f = fopen("orders.tbl", "rb");
  a = fopen("bin/o_orderkey.tbl", "wb");
  b = fopen("bin/o_custkey.tbl", "wb");
  c = fopen("bin/o_orderprice.tbl", "wb");
  d = fopen("bin/o_shippriority.tbl", "wb");
  //e = fopen("bin/o_orderkeylist.tbl", "wb");
  while(fgets(data,500,f)!=NULL) {

  	sscanf(data,"%d|%d|%c|%lf|10%s|5%s|16%s|%d|%s",&orderkey[count], &custkey[count], &osta,
                                                     &orderprice[count], date, oprio,
                                                     clerk, &shippriority[count], comm);
       count++;
  	if (count == SIZE) {
	  fwrite(orderkey,sizeof(int),count,a);
  	  fwrite(custkey,sizeof(int),count,b);
  	  fwrite(orderprice,sizeof(double),count,c);
          for(int i = 0; i < count;i++)printf("%d\n",shippriority[i]);
          fwrite(shippriority,sizeof(int),count,d);
          //fwrite(&orderkey[0],sizeof(int),1,e);
  	  count = 0;
    }
  }
  if (count != SIZE) {
	fwrite(orderkey,sizeof(int),count,a);
  	fwrite(custkey,sizeof(int),count,b);
  	fwrite(orderprice,sizeof(double),count,c);
  	fwrite(shippriority,sizeof(int),count,d);
  }
 
  fclose(a);
  fclose(b);
  fclose(c);
  fclose(d);
  fclose(f);
}

void DBController::loadcustomer(){
  FILE *f, *a;
  int count = 0;
  int nationkey;
  double acctbal;
  char data[300];
  char name[80], address[80], phone[20], mkt[20],comment[120];
  int custkey[SIZE];
  f = fopen("customer.tbl", "rb");
  a = fopen("bin/c_custkey.tbl", "wb");
  while(fgets(data,300,f)!=NULL) {
  	sscanf(data,"%d|%s|%s|%d|%s|%lf|%s|%s|", &custkey[count], name, address, &nationkey, phone, &acctbal, mkt, comment);
	  count++;
  	if (count == SIZE - 1) {
	    fwrite(custkey, sizeof(int), count, a);    
  	  count = 0;
    }
  }
  if (count != SIZE - 1) 
	  fwrite(custkey, sizeof(int), count, a);
  
  fclose(a);
  fclose(f);
}
void DBController::retrieve(int input) {
  int key = -1;
  bool flag = false;
  FILE *a, *b, *c, *d;
  a = fopen("bin/o_orderkey.tbl", "rb");
  b = fopen("bin/o_custkey.tbl", "rb");
  c = fopen("bin/o_orderprice.tbl", "rb");
  d = fopen("bin/o_shippriority.tbl", "rb");
  int count = 0;
  int data[1024];
  while(1) {
    	fread(data, 4, 1024, a);
    	if(data[0] <= input && data[1023] >= input) {
    	  int low = 0, high = 1023;
    	  int mid;
    	  if (data[low] == input) {
          flag = true;
          key = low + count * 1024; 
    	  	break;
    	  }
    	  if (data[high] == input) {
          flag = true;
          key = high + count * 1024; 
    	  	break;
    	  }
    	  while (low <= high) {
    	  	  mid = (high - low) / 2 + low;
            if (data[mid] == input) {
              flag = true;
              key = mid + count * 1024; 
              break;
            }
            if (data[mid] < input)
              low = mid + 1;
            else
    		      high = mid - 1;
        		if(low > high) {
        		   break;
        		}
        }
        break;
    	} else if(data[0] > data[1023]){
          for(int i = 0; i < 1024; i++)
              if(data[i] == input){
                flag = true;
                key = i + count * 1024;
                break;
              }
          break;
        }
    	count ++;
  }
  if (flag) {
      int offset, page;
      int cusk[1024], shipp[1024];
      double opri[1024];
      offset = key % 1024;
      page = key / 1024;
      while(page >= 0) {
          fread(cusk, sizeof(int), 1024, b);
          fread(opri, sizeof(double), 1024, c);
          fread(shipp, sizeof(int), 1024, d);
          if(page == 0) {
            printf("%d %d %.2lf %d\n",input, cusk[offset], opri[offset], shipp[offset]);
          }
          page--;
      }
  } else {
    printf("error orderkey input\n");
  }
  fclose(a);
  fclose(b);
  fclose(c);
  fclose(d);
}
void DBController::externsort(char* in1,char*in2,char* out1,char* out2) {
  Externsort a(in1, in2, out1, out2);
  a.comp_sort();
}
void DBController::join(char* in, char* out) {

}
