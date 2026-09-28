#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct para data
typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

// struct para veiculo
typedef struct {
    int id;
    char marca[50];
    char modelo[100];
    int ano;
    char categoria[50];
    char combustivel[5][30];
    int numCombustiveis;
    int cilindros;
    double cilindrada;
    char transmissao[30];
    char tracao[30];
    double consumoCidade;
    double consumoEstrada;
    double co2;
    bool turbo;
    Data dataRegistro;
} Veiculo;

// faz o parse da data no formato aaaa-mm-dd
Data parseData(char* s) {
    Data d = {0, 0, 0};
    if (s == NULL) return d;
    int ano, mes, dia;
    if (sscanf(s, "%d-%d-%d", &ano, &mes, &dia) == 3) {
        d.ano = ano;
        d.mes = mes;
        d.dia = dia;
    }
    return d;
}

// formata a data para dd/mm/yyyy
void formatData(Data d, char* buffer) {
    sprintf(buffer, "%02d/%02d/%d", d.dia, d.mes, d.ano);
}

// faz o parse de uma linha do csv para a struct veiculo
Veiculo parseVeiculo(char* s) {
    Veiculo v;
    v.id = 0;
    v.marca[0] = '\0';
    v.modelo[0] = '\0';
    v.ano = 0;
    v.categoria[0] = '\0';
    v.numCombustiveis = 0;
    v.cilindros = 0;
    v.cilindrada = 0.0;
    v.transmissao[0] = '\0';
    v.tracao[0] = '\0';
    v.consumoCidade = 0.0;
    v.consumoEstrada = 0.0;
    v.co2 = 0.0;
    v.turbo = false;
    
    s[strcspn(s, "\r\n")] = 0; // remove quebra de linha
    
    char* token = strtok(s, ",");
    if (token != NULL) v.id = atoi(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) strcpy(v.marca, token);
    
    token = strtok(NULL, ",");
    if (token != NULL) strcpy(v.modelo, token);
    
    token = strtok(NULL, ",");
    if (token != NULL) v.ano = atoi(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) strcpy(v.categoria, token);
    
    token = strtok(NULL, ",");
    if (token != NULL) {
        char tempComb[100];
        strcpy(tempComb, token);
        v.numCombustiveis = 0;
        char* p = tempComb;
        char* start = p;
        while (*p != '\0' && v.numCombustiveis < 5) {
            if (*p == ';') {
                *p = '\0';
                while (*start == ' ') start++;
                strcpy(v.combustivel[v.numCombustiveis++], start);
                start = p + 1;
            }
            p++;
        }
        if (*start != '\0' && v.numCombustiveis < 5) {
            while (*start == ' ') start++;
            int len = strlen(start);
            while (len > 0 && start[len-1] == ' ') {
                start[len-1] = '\0';
                len--;
            }
            strcpy(v.combustivel[v.numCombustiveis++], start);
        }
    }
    
    token = strtok(NULL, ",");
    if (token != NULL) v.cilindros = atoi(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) v.cilindrada = atof(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) strcpy(v.transmissao, token);
    
    token = strtok(NULL, ",");
    if (token != NULL) strcpy(v.tracao, token);
    
    token = strtok(NULL, ",");
    if (token != NULL) v.consumoCidade = atof(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) v.consumoEstrada = atof(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) v.co2 = atof(token);
    
    token = strtok(NULL, ",");
    if (token != NULL) {
        while (*token == ' ') token++;
        v.turbo = (strcmp(token, "true") == 0);
    }
    
    token = strtok(NULL, ",");
    if (token != NULL) {
        while (*token == ' ') token++;
        v.dataRegistro = parseData(token);
    }
    
    return v;
}

// le o csv e retorna o vetor alocado dinamicamente
Veiculo* lerCsv(char* caminhoArquivo, int* n) {
    FILE* f = fopen(caminhoArquivo, "r");
    if (f == NULL) {
        f = fopen("C:/tmp/VEICULOS.CSV", "r");
    }
    if (f == NULL) {
        *n = 0;
        return NULL;
    }
    
    char linha[1024];
    int total = 0;
    
    fgets(linha, sizeof(linha), f); // pula cabecalho
    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (strlen(linha) > 5) {
            total++;
        }
    }
    fclose(f);
    
    Veiculo* veiculos = (Veiculo*) malloc(total * sizeof(Veiculo));
    
    f = fopen(caminhoArquivo, "r");
    if (f == NULL) {
        f = fopen("C:/tmp/VEICULOS.CSV", "r");
    }
    if (f == NULL) {
        *n = 0;
        return veiculos;
    }
    
    fgets(linha, sizeof(linha), f); // pula cabecalho
    int idx = 0;
    while (fgets(linha, sizeof(linha), f) != NULL && idx < total) {
        if (strlen(linha) > 5) {
            veiculos[idx] = parseVeiculo(linha);
            idx++;
        }
    }
    fclose(f);
    
    *n = idx;
    return veiculos;
}

// algoritmo de pesquisa binaria por modelo
bool pesquisaBinaria(Veiculo* arr, int n, char* modeloBusca) {
    int esq = 0, dir = n - 1;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        int comp = strcmp(arr[meio].modelo, modeloBusca);
        if (comp == 0) {
            return true;
        } else if (comp < 0) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }
    return false;
}

int main() {
    int totalCsv = 0;
    Veiculo* todosVeiculos = lerCsv("/tmp/veiculos.csv", &totalCsv);
    if (todosVeiculos == NULL || totalCsv == 0) {
        return 1;
    }
    
    Veiculo* selecionados = NULL;
    int capacidade = 10;
    int qtdSelecionados = 0;
    
    selecionados = (Veiculo*) malloc(capacidade * sizeof(Veiculo));
    
    int idBusca;
    // le os ids da entrada padrao ate encontrar -1
    while (scanf("%d", &idBusca) == 1 && idBusca != -1) {
        for (int i = 0; i < totalCsv; i++) {
            if (todosVeiculos[i].id == idBusca) {
                if (qtdSelecionados >= capacidade) {
                    capacidade *= 2;
                    selecionados = (Veiculo*) realloc(selecionados, capacidade * sizeof(Veiculo));
                }
                selecionados[qtdSelecionados++] = todosVeiculos[i];
                break;
            }
        }
    }
    
    // ordena o arranjo por selecao usando o modelo
    for (int i = 0; i < qtdSelecionados - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < qtdSelecionados; j++) {
            int comp = strcmp(selecionados[j].modelo, selecionados[menor].modelo);
            if (comp < 0 || (comp == 0 && selecionados[j].id < selecionados[menor].id)) {
                menor = j;
            }
        }
        if (menor != i) {
            Veiculo temp = selecionados[i];
            selecionados[i] = selecionados[menor];
            selecionados[menor] = temp;
        }
    }
    
    // consome a quebra de linha pendente apos a leitura dos inteiros
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // le as linhas com os modelos a pesquisar ate encontrar FIM
    char linhaBusca[100];
    while (fgets(linhaBusca, sizeof(linhaBusca), stdin) != NULL) {
        linhaBusca[strcspn(linhaBusca, "\r\n")] = 0;
        if (strcmp(linhaBusca, "FIM") == 0) {
            break;
        }
        if (strlen(linhaBusca) == 0) continue;
        
        if (pesquisaBinaria(selecionados, qtdSelecionados, linhaBusca)) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }
    
    free(todosVeiculos);
    free(selecionados);
    return 0;
}