// gcc eop.c -o eop -Wall && ./eop

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LINE_LEN 1024
#define MAX_PROFILES 10000
#define MAX_STR_LEN 70

int profile_data_nitems = 0;
int bdata_count = 0;
void exec_command(char cmd, char cmd2, char *param);

struct date{ //日付の構造体
  int y; //year
  int m; //month
  int d; //day of month 
};

struct profile{ //名簿データの構造体
  int i; //ID
  char s[MAX_STR_LEN]; //school name
  struct date sb; //school birthday
  char a[MAX_STR_LEN]; //address
  char *c; //comment
};

struct profile profile_data_store[MAX_PROFILES]; //名簿データの構造体profileの配列化


////////////////////////////////////////////////////////////////////////////////////////////////////////////


int subst(char *str, char c1, char c2){ //strの文字列に含まれるc1をc2に変換
  int s,i, n = 0;                       //返り値は変換した文字の個数
  s = strlen(str);
  for(i=0; i<s+1; i++){
    if(*(str+i) == c1){
      *(str+i) = c2;
      n++;
    }
  }
  return n;
}

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

int get_line(FILE *fp, char *line){ //標準入力文字列を配列lineに格納
  if(fgets(line, MAX_LINE_LEN + 1, fp) == NULL){
  return 0;
  }
  subst(line, '\n', '\0');
  
  return 1;
}

int uruu(int y){ //yの年が閏年かどうか判定．閏年なら1，閏年でないなら0を返す
  if(y % 400 == 0){
    return 1;
  }else if(y % 100 == 0){
    return 0;
  }else if(y % 4 == 0){
    return 1;
  }else{
    return 0;
  }
}

struct date *new_date(struct date *bd, char *dat){ //新しい日付の構造体の作成．引数は空の日付構造体と入れたい日付の文字列．
  char *day[3];
  int y, m, d;
  int u;

  if(split(dat, day, '-', 3) != 3){ //year-month-day を3つに分割
    printf("Date error\n");
    profile_data_nitems--;
    return NULL;
  }
  y = atoi(day[0]); m = atoi(day[1]); d = atoi(day[2]);
  u = uruu(y);
  if(y <= 0 || y > 9999){printf("Date error\n"); return NULL;} //存在する日付かどうか確認
  if(u == 1){
    if(m == 2){
      if(d < 1 || d > 29){printf("Date error\n"); return NULL;}
    }else if(m == 4 || m == 6 || m == 9 || m == 11){
      if(d < 1 || d > 30){printf("Date error\n"); return NULL;}
    }else if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12){
      if(d < 1 || d > 31){printf("Date error\n"); return NULL;}
    }else{return NULL;}
  }else if(u == 0){
    if(m == 2 || m == 4 || m == 6 || m == 9 || m == 11){
      if(d < 1 || d > 30){printf("Date error\n"); return NULL;}
    }else if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12){
      if(d < 1 || d > 31){printf("Date error\n"); return NULL;}
    }else{return NULL;}
  }else{return NULL;}

  bd->y = y;
  bd->m = m;
  bd->d = d;
  
  return bd;
}

struct profile *new_profile(struct profile *pro, char *line){ //新しい名簿データ構造体の作成
  char *ret[5];
  if(split(line, ret, ',', 5) != 5){ //入力された文字列を要素ごとに分解
    printf("Input error\n");
    profile_data_nitems--;
  return NULL;
  }
  /*-< それぞれのデータを構造体へ格納 >-*/
  pro->c = malloc(strlen(ret[4]) + 1);  
  pro->i = atoi(ret[0]);
  strncpy(pro->s, ret[1], MAX_STR_LEN);
  strncpy(pro->a, ret[3], MAX_STR_LEN);
  strcpy(pro->c, ret[4]);
  if(new_date(&pro->sb, ret[2]) == NULL){return NULL;}
  return pro;
}

int parse_line(char *line){ //入力された文字列が名簿データかコマンドかの判定
  if(line[0] == '%'){
    exec_command(line[1],line[2],&line[3]);
  } else if(profile_data_nitems < 10000){
      new_profile(&profile_data_store[profile_data_nitems++], line);
  } else {
      printf("The number of data over 10000\n");
      return 0;
  }
    return 1;
}


///////////////////////////////////////////////////////////////////////////////////////

/*-< コマンド実装部 >-*/

void print_profile(struct profile *pds, int i){
  printf("No.%d\n", i+1);
   printf("Id    : %d\nName  : %s\nBirth : %d-%d-%d\nAddr  : %s\nCom.  : %s\n\n",
          pds->i, pds->s, pds->sb.y, pds->sb.m, pds->sb.d, pds->a, pds->c);
}

void fprint_profile(FILE *fp, struct profile *pds){
  fprintf(fp, "%d,%s,%d-%d-%d,%s,%s\n",
          pds->i, pds->s, pds->sb.y, pds->sb.m, pds->sb.d, pds->a, pds->c);
}

int compare_date(struct date p1, struct date p2){
  char ps1[10], ps2[10];
  int r;

  sprintf(ps1,"%d-%d-%d", p1.y, p1.m, p1.d);
  sprintf(ps2,"%d-%d-%d", p2.y, p2.m, p2.d);
  r = strcmp(ps1, ps2);

  return r;
}

int profile_compare(struct profile *p1, struct profile *p2, int column){
  switch(column){
  case 1:
    return p1->i - p2->i;
  case 2:
    return strcmp(p1->s, p2->s);
  case 3:
    return compare_date(p1->sb, p2->sb);
  case 4:
    return strcmp(p1->a, p2->a);
  case 5:
    return strcmp(p1->c, p2->c);
  }
    return 0;
}

void swap_profile(struct profile *p1, struct profile *p2){
  struct profile tmp;

  tmp = *p2;
  *p2 = *p1;
  *p1 = tmp;
}

void b_sort(struct profile p[], int n){
  int min = 0, max = profile_data_nitems;
  int i, j;

  for(i = min; i <= max; i++){
    for(j = min; j <= max-2; j++){
      if(profile_compare(&p[j], &p[j + 1], n) > 0){
	swap_profile(&p[j], &p[j + 1]);
      }
    }
  }
}

void cmd_quit(){
  exit(0);
}

void cmd_check(){
  printf("%d profiles(s)\n", profile_data_nitems);
}

int cmd_print(int n){
  int i = 0, q;
 
  if(n == 0){
    n = profile_data_nitems;
    for(q = i; q < n; q++){
      print_profile(&profile_data_store[q], q);
   }
  } else if(n > 0){
    if(n > profile_data_nitems){printf("Not found %d data\n", n);return 0;}
    for(q = i; q < n; q++){
      print_profile(&profile_data_store[q], q);
    }
  }

  if(n < 0){
    if(-n > profile_data_nitems){printf("Not found %d data\n", n);return 0;}
    i = profile_data_nitems - (-n);
    for(q = profile_data_nitems - 1; q >= i; q--){
      print_profile(&profile_data_store[q], q);
  }
}
  return 0;
}

int cmd_read(char *filename){
  char line[1024];
  FILE *fp_r;

  fp_r = fopen(filename, "r");
  if(fp_r == NULL){
    fprintf(stderr, "Could not open '%s'\n", filename);
    return 0;
  }

  while(get_line(fp_r, line)){
      if(parse_line(line) == 0){return 0;}
  }

  fclose(fp_r);
  return 1;
}

int cmd_write(char *filename){
  FILE *fp_w;
  int q;
  
  fp_w = fopen(filename, "w");
  if(fp_w == NULL){
    fprintf(stderr, "Could not open '%s'\n", filename);
    return 0;
  }
  for(q = 0; q < profile_data_nitems; q++){
    fprint_profile(fp_w, &profile_data_store[q]);
    }
  return 1;
}

void cmd_find(char *word){
  int q;
  char i[1024], sb[1024];
  struct profile *prof;

  for(q = 0; q < profile_data_nitems; q++){
    prof = &profile_data_store[q];
    sprintf(i, "%d", prof->i);
    sprintf(sb, "%d-%d-%d", prof->sb.y, prof->sb.m, prof->sb.d);

    if(strcmp(i, word) == 0 || strcmp(prof->s, word) == 0 || strcmp(sb, word) == 0 ||
       strcmp(prof->a, word) == 0 || strcmp(prof->c, word) == 0){
      print_profile(&profile_data_store[q], q);      
    }
  }
}

void cmd_sort(int n){
  b_sort(profile_data_store, n);
}

void cmd_delete(int n){
  int i;
  for(i = n-1; i <= profile_data_nitems; i++){
    swap_profile(&profile_data_store[i], &profile_data_store[i + 1]);
  }
  profile_data_nitems = profile_data_nitems - 1;
}

int cmd_bread(char *filename){
    fpos_t  size;
    FILE *fp_br;
    
    fp_br = fopen(filename, "rb");
    if(fp_br == NULL){
        fprintf(stderr, "Could not open '%s'\n", filename);
        return 0;
    }

    fseek(fp_br, 0, SEEK_END);
    fgetpos(fp_br, &size);
    fseek(fp_br, 0, SEEK_SET);
    fread(&profile_data_store[profile_data_nitems], 168, 336, fp_br);
    profile_data_nitems = profile_data_nitems + size/168;

    fclose(fp_br);
    return 1;
}

int cmd_bwrite(char *filename){
    FILE *fp_bw;
    int cnt;
    
    fp_bw = fopen(filename, "wb");
    if(fp_bw == NULL){
        fprintf(stderr, "Could not open '%s'\n", filename);
        return 0;
    }
    
    cnt = sizeof(struct profile);
    fwrite(profile_data_store, cnt, profile_data_nitems, fp_bw);
    fclose(fp_bw);
    
    return 1;
}

void cmd_b(char cmd,char *filename){
    if(cmd == 'R'){cmd_bread(filename);}
    if(cmd == 'W'){cmd_bwrite(filename);}
}

void exec_command(char cmd, char cmd2, char *param){
  switch(cmd){
  case 'Q': cmd_quit(); break;
  case 'C': cmd_check(); break;
  case 'P': cmd_print(atoi(param)); break;
  case 'R': cmd_read(param); break;
  case 'W': cmd_write(param); break;
  case 'F': cmd_find(param); break;
  case 'S': cmd_sort(atoi(param)); break;
  case 'D': cmd_delete(atoi(param)); break;
  case 'B': cmd_b(cmd2, param); break;
  }
}

int main(){
  char line[MAX_LINE_LEN + 1];
  while(get_line(stdin, line)){
    parse_line(line);
    }
  return 0;
}
