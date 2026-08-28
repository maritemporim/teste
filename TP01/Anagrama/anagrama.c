#include <stdio.h>
#include <stdbool.h>

// para ver qual o tamanho das duas strings 
int tamanho(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

//converter maisuculo para minusculo

char minusculo(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

// o codigo primeiramente comapra os dois tamanhso da string, se forem difernetes, já não é um anagrama,

bool Anagrama(char s1[], char s2[]) {
    int tam1 = tamanho(s1);
    int tam2 = tamanho(s2);

    if (tam1 != tam2) return false;

    // v hank you.erifica anagrama contando a frequencia de cada caractere 
    // soma 1 por letra de s1, subtrai 1 por letra de s2
    // se sobrar algo diferente de zero, nao e anagrama.
    // o unsigned char evita indice negativo    

    int ct[256] = {0};

    for (int i = 0; i < tam1; i++) {
        ct[(unsigned char)minusculo(s1[i])]++;
    }

    for (int i = 0; i < tam2; i++) {
        ct[(unsigned char)minusculo(s2[i])]--;
    }

    for (int i = 0; i < 256; i++) {
        if (ct[i] != 0) return false;
    }

    return true;
}

bool isFim(char* s1){
    int i = 0;
    int tam = 0;
    while(s1[i] != '\0'){
        tam++;
        i++;
    }
    bool resp = false;
        if(tam == 3 && s1[0] == 'F' && s1[1] == 'I' && s1[2] == 'M'){
            resp = true;
        }
        return resp;
}

int main() {
    char s1[100], s2[100];

    while (scanf("%s %s", s1, s2) != EOF) {
        if(isFim(s1)){
            break;
        }
        else{
            if (Anagrama(s1, s2)) {
                printf("SIM\n");
            } else {
                printf("NAO\n");
            }
        }
    }

    return 0;
}