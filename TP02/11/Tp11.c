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
    v.dataRegistro.ano = 0;
    v.dataRegistro.mes = 0;
    v.dataRegistro.dia = 0;

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

    char combStr[200] = "";
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

// celula para a lista encadeada
typedef struct Celula {
    Veiculo elemento;
    struct Celula* prox;
} Celula;

// estrutura da lista flexivel
typedef struct {
    Celula* primeiro;
    Celula* ultimo;
} ListaFlexivel;

// inicializa a lista com celula cabeca
void inicializarLista(ListaFlexivel* lista) {
    lista->primeiro = (Celula*) malloc(sizeof(Celula));
    lista->primeiro->prox = NULL;
    lista->ultimo = lista->primeiro;
}

// retorna o tamanho da lista
int tamanhoLista(ListaFlexivel* lista) {
    int tam = 0;
    for (Celula* i = lista->primeiro; i != lista->ultimo; i = i->prox) {
        tam++;
    }
    return tam;
}

// insere no inicio da lista
void inserirInicio(ListaFlexivel* lista, Veiculo v) {
    Celula* tmp = (Celula*) malloc(sizeof(Celula));
    tmp->elemento = v;
    tmp->prox = lista->primeiro->prox;
    lista->primeiro->prox = tmp;
    if (lista->primeiro == lista->ultimo) {
        lista->ultimo = tmp;
    }
}

// insere no fim da lista
void inserirFim(ListaFlexivel* lista, Veiculo v) {
    lista->ultimo->prox = (Celula*) malloc(sizeof(Celula));
    lista->ultimo = lista->ultimo->prox;
    lista->ultimo->elemento = v;
    lista->ultimo->prox = NULL;
}

// insere na posicao informada
void inserir(ListaFlexivel* lista, Veiculo v, int pos) {
    int tam = tamanhoLista(lista);
    if (pos < 0 || pos > tam) {
        return;
    } else if (pos == 0) {
        inserirInicio(lista, v);
    } else if (pos == tam) {
        inserirFim(lista, v);
    } else {
        Celula* i = lista->primeiro;
        for (int j = 0; j < pos; j++, i = i->prox);
        Celula* tmp = (Celula*) malloc(sizeof(Celula));
        tmp->elemento = v;
        tmp->prox = i->prox;
        i->prox = tmp;
    }
}

// remove do inicio da lista
Veiculo removerInicio(ListaFlexivel* lista) {
    if (lista->primeiro == lista->ultimo) {
        exit(1);
    }
    Celula* tmp = lista->primeiro->prox;
    lista->primeiro->prox = tmp->prox;
    Veiculo resp = tmp->elemento;
    if (tmp == lista->ultimo) {
        lista->ultimo = lista->primeiro;
    }
    free(tmp);
    return resp;
}

// remove do fim da lista
Veiculo removerFim(ListaFlexivel* lista) {
    if (lista->primeiro == lista->ultimo) {
        exit(1);
    }
    Celula* i;
    for (i = lista->primeiro; i->prox != lista->ultimo; i = i->prox);
    Veiculo resp = lista->ultimo->elemento;
    free(lista->ultimo);
    lista->ultimo = i;
    lista->ultimo->prox = NULL;
    return resp;
}

// remove da posicao informada
Veiculo remover(ListaFlexivel* lista, int pos) {
    int tam = tamanhoLista(lista);
    if (lista->primeiro == lista->ultimo || pos < 0 || pos >= tam) {
        exit(1);
    } else if (pos == 0) {
        return removerInicio(lista);
    } else if (pos == tam - 1) {
        return removerFim(lista);
    } else {
        Celula* i = lista->primeiro;
        for (int j = 0; j < pos; j++, i = i->prox);
        Celula* tmp = i->prox;
        i->prox = tmp->prox;
        Veiculo resp = tmp->elemento;
        free(tmp);
        return resp;
    }
}

// mostra todos os elementos da lista do primeiro ao ultimo
void mostrarLista(ListaFlexivel* lista) {
    for (Celula* i = lista->primeiro->prox; i != NULL; i = i->prox) {
        char buffer[1024];
        formatVeiculo(i->elemento, buffer);
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

    ListaFlexivel lista;
    inicializarLista(&lista);

    int idBusca;
    // primeira parte: le os ids ate encontrar -1 e insere no fim da lista
    while (scanf("%d", &idBusca) == 1 && idBusca != -1) {
        Veiculo* v = buscarVeiculo(todosVeiculos, totalCsv, idBusca);
        if (v != NULL) {
            inserirFim(&lista, *v);
        }
    }

    // segunda parte: le a quantidade de comandos e executa
    int nComandos;
    if (scanf("%d", &nComandos) == 1) {
        for (int i = 0; i < nComandos; i++) {
            char cmd[10];
            if (scanf("%9s", cmd) != 1) break;

            if (strcmp(cmd, "II") == 0) {
                int id;
                scanf("%d", &id);
                Veiculo* v = buscarVeiculo(todosVeiculos, totalCsv, id);
                if (v != NULL) inserirInicio(&lista, *v);
            } else if (strcmp(cmd, "I*") == 0) {
                int pos, id;
                scanf("%d %d", &pos, &id);
                Veiculo* v = buscarVeiculo(todosVeiculos, totalCsv, id);
                if (v != NULL) inserir(&lista, *v, pos);
            } else if (strcmp(cmd, "IF") == 0) {
                int id;
                scanf("%d", &id);
                Veiculo* v = buscarVeiculo(todosVeiculos, totalCsv, id);
                if (v != NULL) inserirFim(&lista, *v);
            } else if (strcmp(cmd, "RI") == 0) {
                Veiculo v = removerInicio(&lista);
                printf("(R)%s %s\n", v.marca, v.modelo);
            } else if (strcmp(cmd, "R*") == 0) {
                int pos;
                scanf("%d", &pos);
                Veiculo v = remover(&lista, pos);
                printf("(R)%s %s\n", v.marca, v.modelo);
            } else if (strcmp(cmd, "RF") == 0) {
                Veiculo v = removerFim(&lista);
                printf("(R)%s %s\n", v.marca, v.modelo);
            }
        }
    }

    // mostra os elementos presentes na lista ao final
    mostrarLista(&lista);

    free(todosVeiculos);
    return 0;
}