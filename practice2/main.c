//split.c とget_line.c を繋げたやつ

#include<stdio.h>
#define MAX_LINE_LEN 1024 //プログラム全体を通して変わらない値の定義

int split(char *str, char *ret[], char sep, int max){ //文字列strをsepで区切ってそれぞれの先頭アドレスをretに格納
  int p = 0;                                //maxは区切る数の最大．返り値は区切った数
  ret[p] = str;                                       //例）str:"ab,cd,ef" sep:','
  p++;                                                //結果）ret[0]:ab ret[1]:cd ret[2]:ef
  while(*str !='\0'){
    if(*str == sep){
      *str = '\0';
      ret[p] = str + 1;
      p++;
      if(p > max){return 0;}
    }
    str++;
  }
  return p;
}


int get_line(char *line){ //標準入力文字列を配列lineに格納
  if(fgets(line, MAX_LINE_LEN, stdin) == NULL){
  return 0;
  }
  //subst(line, '\n', '\0');
  
  return 1;
}

int main(){
    char line[MAX_LINE_LEN];
    char *ret[5];
    int i, split_count;

    while(1){               //プログラムを終了するときは必ずctrl+c
        get_line(line);     //ctrl+zは一時停止で，ジョブが残るので使わない
        split_count = split(line, ret, ',', 5);
        
        printf("分割数:%d\n", split_count);
        for(i=0;i<split_count;i++){
            printf("ret[%d]:%s\n", i, ret[i]);    
        }
    }
    return 0;
}

