#include <string.h>
#include <stdio.h>
#include <stdbool.h>


// fução que ve se se a palavra é FIM para parar o programa
bool isFim(char* palavra){
    int i = 0;
    int tam = 0;
    while(palavra[i] != '\0'){
        tam++;
        i++;
    }
    bool resp = false;
        if(tam == 3 && palavra[0] == 'F' && palavra[1] == 'I' && palavra[2] == 'M'){
            resp = true;
        }
        return resp;
}

int main(void){
    char palavra[100];
    //scanf("%s", palavra);

    while(scanf(" %[^\n]", palavra) == 1){
        if(isFim(palavra)){
            break;
        }
        for(int i = strlen(palavra) - 1; i >= 0; i--){
            printf("%c", palavra[i]);
        }
        printf("\n");
    }

    return 0;
}