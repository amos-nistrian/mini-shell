#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX	1024

int main() {

  int result = 0;
  char sIn[MAX]; // string for storing stdin
  while(NULL != fgets(sIn, MAX, stdin)) { // get input from stdin and store it into sIn

    char operator;
    int oploc; // location of operator
    result = 0;

  int x = strlen(sIn);
  for (int i = 0 ; i < x ; i++) {
      switch(sIn[i]){
        case '+' :
            operator = sIn[i]; // copy char into operator
            sIn[i] = 0; // set to null
            oploc = i; // set oploc to the location to where the operator was found
            i = x; // break out of this switch
            break;
        case '-' :
            operator = sIn[i];
            sIn[i] = 0;
            oploc = i;
            i = x;
            break;
        case '*' :
            operator = sIn[i];
            sIn[i] = 0;
            oploc = i;
            i = x;
            break;
        case '/' :
            operator = sIn[i];
            sIn[i] = 0;
            oploc = i;
            i = x;
            break;

      } // EOswitch
    } // EOfor

          int lval = atoi(sIn);
          int rval = atoi(&sIn[oploc+1]);
          switch(operator) {
            case '+' :
                result = lval + rval;
                break;
            case '-' :
                result = lval - rval;
                break;
            case '*' :
                result = lval * rval;
                break;
            case '/' :
                if (0 == rval){
                  printf("CAN'T DIVIDE BY ZERO\n");
                  break;
                }
                result = lval / rval;
                break;
          } // EOswitch
          printf("%d %c %d = %d\n",lval,operator,rval,result);

          if ( sIn[0] ==  EOF ) {
              printf("CTL-D ENCOUNTERED!!\n");
              exit(0);
              return (0);
          }
} // Eowhile

} // EOmain
