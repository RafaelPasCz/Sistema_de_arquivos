#include <stdio.h>
#include <string.h>

void separar_nome_extensao(const char *entrada, char *nome, char *extensao) {
    const char *ultima_barra = strrchr(entrada, '/'); // Encontra a última barra
    const char *ponto = strrchr(entrada, '.'); // Encontra o último ponto

    if (!ponto || (ultima_barra && ponto < ultima_barra)) {
        // Se não houver ponto ou se ele estiver antes da última barra, não há extensão
        strncpy(nome, ultima_barra ? ultima_barra + 1 : entrada, 10);
        nome[10] = '\0';  // Garante que o nome tenha no máximo 10 caracteres
        extensao[0] = '\0';
    } else {
        // Copia o nome do arquivo sem a extensão (limita a 10 caracteres)
        strncpy(nome, ultima_barra ? ultima_barra + 1 : entrada, ponto - (ultima_barra ? ultima_barra + 1 : entrada));
        nome[ponto - (ultima_barra ? ultima_barra + 1 : entrada)] = '\0';
        if (strlen(nome) > 10) nome[10] = '\0';  // Limita o nome a 10 caracteres
        
        // Copia a extensão sem o ponto (limita a 4 caracteres)
        strncpy(extensao, ponto + 1, 4);
        extensao[4] = '\0';  // Garante que a extensão tenha no máximo 4 caracteres
    }
}

int main() {
    char entrada[100] = "parte1/parte2/parte1234567890.extensao";
    char nome[100], extensao[100];

    separar_nome_extensao(entrada, nome, extensao);

    printf("Nome: %s\n", nome);
    printf("Extensao: %s\n", extensao);

    return 0;
}
