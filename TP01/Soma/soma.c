#include <stdio.h>

int main(void) {
    
    int n;
    int soma = 0;

    while(scanf("%d", &n) != EOF){
        while (n > 0) {
            soma += n % 10;
            n /= 10;
        }
        printf("%d\n", soma);
        soma = 0;
    }
    return 0;
}