#include <stdio.h>
#include <string.h>

int subst(char *str, char c1, char c2){
    int s, i, n=0;
    s = strlen(str);
    for(i=0; i<s+1; i++){
        if(*(str+i) == c1){
            *(str+i) = c2;
            n++;
        }
    }
    return n;
}

int main(){
    char word_csv[6] = "a,b,c";
    char word[6];
    int n;

    strncpy(word, word_csv, 6);
    
    n = subst(word, ',', ' ');
    printf("%d文字置き換えました\n", n);
    printf("%s -> %s\n", word_csv, word);

    return 0;
}
