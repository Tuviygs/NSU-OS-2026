#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main() {
    char buf[BUFSIZ];

    int cap = 16;
    int size = 0;
    char** sp = (char**)malloc(sizeof(char*)*cap);

    while (fgets(buf, sizeof(buf), stdin)[0] != '.') {

        if (size == cap - 1) {
            cap *= 2;
            sp = (char**)realloc(sp, sizeof(char*)*cap);
        }

        int ln = strlen(buf);
        char* newstr = (char*)malloc(sizeof(char)*(ln+1));

        strcpy(newstr, buf);

        sp[size++] = newstr;

    }
    
    for (int i = 0; i < size; i++) {
        printf("%s", sp[i]);
    }
    

    for (int i = 0; i < size; i++) {
        free(sp[i]);
    }
    free(sp);


    return 0;
}