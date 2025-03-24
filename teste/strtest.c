#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>



int main(){
    char arquivo[12];
    char nome[] = "teste txt o ";

    strncpy(arquivo, nome, 12);
    
    // Remove os espaços finais do nome do arquivo
    int len = strlen(arquivo);
    while (len > 0 && arquivo[len - 1] == ' ') {
        arquivo[len - 1] = '\0';
        len--;
    }
    
    // Substitui todos os espaços no meio do nome do arquivo por pontos
    char *ptr = strchr(arquivo, ' ');

    while(ptr){
        if (ptr) {
            *ptr = '.';
        }
        ptr = strchr(arquivo, ' ');
    }
    
    len = strlen(arquivo);
    while (len > 0 && arquivo[len - 1] == '.') {
        arquivo[len - 1] = '\0';
        len--;
    }

    printf("Final: '%s'", arquivo);

    return 0;
}