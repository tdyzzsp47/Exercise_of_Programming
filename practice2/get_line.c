#include<stdio.h>
#define MAX_LINE_LEN 1024 //プログラム全体を通して変わらない値の定義

int get_line(char *line){ //標準入力文字列を配列lineに格納
  if(fgets(line, MAX_LINE_LEN, stdin) == NULL){
  return 0;
  }
  //subst(line, '\n', '\0');
  
  return 1;
}

int main(){
    char line[MAX_LINE_LEN];

    while(1){               //プログラムを終了するときは必ずctrl+c
        get_line(line);     //ctrl+zは一時停止で，ジョブが残るので使わない
        printf("line:%s\n", line);
    }
}
