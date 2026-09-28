#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 6 // tamanho 6 para gerenciar a fila circular de capacidade 5

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

// formata o veiculo no padrao exigido
void formatVeiculo(Veiculo v, char* buffer) {
    char dataStr[20];
    formatData(v.dataRegistro, dataStr);
    
    char combStr[100] = "";
    for (int i = 0; i < v.numCombustiveis; i++) {
        strcat(combStr, v.combustivel[i]);
        if (i < v.numCombustiveis - 1) {
            strcat(combStr, ",");
        }
    }
    
    sprintf(buffer, "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %s ## %s]",
            v.id, v.marca, v.modelo, v.ano, v.categoria, combStr, v.cilindros,
            v.cilindrada, v.transmissao, v.tracao, v.consumoCidade, v.consumoEstrada,
            v.co2, v.turbo ? "true" : "false", dataStr);
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

// estrutura da fila circular
typedef struct {
    Veiculo array[MAX];
    int primeiro;
    int ultimo;
} FilaCircular;

// inicializa a fila circular
void inicializarFila(FilaCircular* f) {
    f->primeiro = 0;
    f->ultimo = 0;
}

// verifica se a fila esta vazia
bool filaVazia(FilaCircular* f) {
    return (f->primeiro == f->ultimo);
}

// verifica se a fila esta cheia 
bool filaCheia(FilaCircular* f) {
    return ((f->ultimo + 1) % MAX == f->primeiro);
}

// insere elemento na fila
void inserirFila(FilaCircular* f, Veiculo v) {
    // se estiver cheia, remove o primeiro antes de inserir
    if (filaCheia(f)) {
        Veiculo removido = f->array[f->primeiro];
        f->primeiro = (f->primeiro + 1) % MAX;
        printf("(R)%s %s\n", removido.marca, removido.modelo);
    }
    f->array[f->ultimo] = v;
    f->ultimo = (f->ultimo + 1) % MAX;
}

// remove elemento da fila 
Veiculo removerFila(FilaCircular* f) {
    if (filaVazia(f)) {
        printf("erro: fila vazia!\n");
        exit(1);
    }
    Veiculo resp = f->array[f->primeiro];
    f->primeiro = (f->primeiro + 1) % MAX;
    return resp;
}

// mostra os elementos da fila do primeiro ao ultimo
void mostrarFila(FilaCircular* f) {
    for (int i = f->primeiro; i != f->ultimo; i = (i + 1) % MAX) {
        char buffer[1024];
        formatVeiculo(f->array[i], buffer);
        printf("%s\n", buffer);
    }
}

// busca um veiculo pelo id no vetor completo
Veiculo* buscarVeiculo(Veiculo* veiculos, int total, int id) {
    for (int i = 0; i < total; i++) {
        if (veiculos[i].id == id) {
            return &veiculos[i];
        }
    }
    return NULL;
}

int main() {
    int totalCsv = 0;
    Veiculo* todosVeiculos = lerCsv("/tmp/veiculos.csv", &totalCsv);
    if (todosVeiculos == NULL || totalCsv == 0) {
        return 1;
    }
    
    FilaCircular fila;
    inicializarFila(&fila);
    
    int idBusca;
    // le os ids da entrada ate encontrar -1
    while (scanf("%d", &idBusca) == 1 && idBusca != -1) {
        Veiculo* v = buscarVeiculo(todosVeiculos, totalCsv, idBusca);
        if (v != NULL) {
            inserirFila(&fila, *v);
        }
    }
    
    // le a quantidade de comandos e executa
    int nComandos;
    if (scanf("%d", &nComandos) == 1) {
        for (int i = 0; i < nComandos; i++) {
            char cmd[10];
            scanf("%s", cmd);
            
            if (strcmp(cmd, "I") == 0) {
                int id;
                scanf("%d", &id);
                Veiculo* v = buscarVeiculo(todosVeiculos, totalCsv, id);
                if (v != NULL) {
                    inserirFila(&fila, *v);
                }
            } else if (strcmp(cmd, "R") == 0) {
                Veiculo v = removerFila(&fila);
                printf("(R)%s %s\n", v.marca, v.modelo);
            }
        }
    }
    
    // mostra os elementos presentes na fila ao final
    mostrarFila(&fila);
    
    free(todosVeiculos);
    return 0;
}