#include <stdio.h>
#include <string.h>

void separar_nome_extensao(const char *entrada, char *nome, char *extensao) {
    // Recebe um nome de arquivo
    // Separa o nome da extensão (até 4 chars)

    const char *ultima_barra = strrchr(entrada, '/');       // Encontra a última barra
    if(!ultima_barra){
        ultima_barra = strrchr(entrada, '\\');
    }

    const char *ponto = strrchr(entrada, '.');              // Encontra o último ponto

    if (!ponto || (ultima_barra && ponto < ultima_barra)) {
        // Se não houver ponto ou se ele estiver antes da última barra, não há extensão
        strncpy(nome, ultima_barra ? ultima_barra + 1 : entrada, 7);
        nome[7] = '\0';                                     // Limita a 7 caracteres
        extensao[0] = '\0';                                 // Não há extensão

    } else {
        // Copia o nome do arquivo sem a extensão (limita a 7 caracteres)
        strncpy(nome, ultima_barra ? ultima_barra + 1 : entrada, ponto - (ultima_barra ? ultima_barra + 1 : entrada));
        nome[ponto - (ultima_barra ? ultima_barra + 1 : entrada)] = '\0';  // Garante que o nome não ultrapasse o limite
        if (strlen(nome) > 7) nome[7] = '\0';  // Limita o nome a 7 caracteres
        
        // Copia a extensão sem o ponto (limita a 4 caracteres)
        strncpy(extensao, ponto + 1, 4);
        extensao[4] = '\0';  // Garante que a extensão tenha no máximo 4 caracteres
    }
}

void concatenar_nome_extensao(const char *nome, const char *extensao, char *resultado) {
    // Concatenar sem adicionar '\0' extra manualmente
    int i = 0, j = 0;
    
    // Copiar nome
    while (nome[i] != '\0' && i < 7) {
        resultado[i] = nome[i];
        i++;
    }

    // Adicionar espaço
    resultado[i] = ' ';
    i++;

    // Copiar extensão
    while (extensao[j] != '\0' && j < 4) {
        resultado[i] = extensao[j];
        i++;
        j++;
    }

    // resultado[i] = '\0';
}

int main() {
    char entrada[100] = "parte1/parte2/parte3111111111.extensao";
    char nome[100], extensao[100], resultado[100];

    separar_nome_extensao(entrada, nome, extensao);
    concatenar_nome_extensao(nome, extensao, resultado);

    printf("Resultado: '%s'\n", resultado);

    return 0;
}