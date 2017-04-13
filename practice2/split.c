#include<stdio.h>

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

int main(){
    char str[1024] = "1,okadai,2017-4-1,okayama,hoge";
    char *ret[5];
    int i, split_count;

    split_count = split(str, ret, ',', 5);
    printf("分割数:%d\n", split_count);
    for(i=0;i<split_count;i++){
        printf("ret[%d]:%s\n", i, ret[i]);    
    }

    return 0;
}
