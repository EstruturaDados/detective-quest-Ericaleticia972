
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -----------------------------------------
   STRUCTS
----------------------------------------- */

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
} HashNode;

/* Tamanho fixo da tabela hash */
#define TAM_HASH 20
HashNode* tabela[TAM_HASH];

/* -----------------------------------------
   FUNÇÕES DA HASH
----------------------------------------- */

int hashFunc(char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) soma += chave[i];
    return soma % TAM_HASH;
}

void inserirNaHash(char* pista, char* suspeito) {
    int h = hashFunc(pista);
    tabela[h] = malloc(sizeof(HashNode));
    strcpy(tabela[h]->pista, pista);
    strcpy(tabela[h]->suspeito, suspeito);
}

char* encontrarSuspeito(char* pista) {
    int h = hashFunc(pista);
    if (tabela[h] == NULL) return NULL;
    return tabela[h]->suspeito;
}

void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        if (tabela[i] != NULL) free(tabela[i]);
    }
}

/* -----------------------------------------
   ÁRVORE DE SALAS
----------------------------------------- */

Sala* criarSala(char* nome, char* pista) {
    Sala* nova = malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


void liberarSalasRec(Sala* node) {
    if (!node) return;
    liberarSalasRec(node->esquerda);
    liberarSalasRec(node->direita);
    free(node);
}

/* -----------------------------------------
   ÁRVORE BST DE PISTAS
----------------------------------------- */

PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if (!raiz) {
        PistaNode* novo = malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

void liberarPistas(PistaNode* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

/* -----------------------------------------
   EXPLORAÇÃO DAS SALAS
----------------------------------------- */

void explorarSalas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("Escolha caminho: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita) {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nSaindo da exploração...\n");
            return;
        }
        else {
            printf("Caminho inválido!\n");
        }
    }
}

/* -----------------------------------------
   JULGAMENTO FINAL
----------------------------------------- */

void verificarSuspeitoFinal(PistaNode* raiz) {
    char acusacao[50];
    int contador = 0;

    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]s", acusacao);

    printf("\nVerificando pistas...\n");

    // percorre BST
    PistaNode* pilha[50];
    int topo = -1;
    PistaNode* atual = raiz;

    while (atual || topo != -1) {
        while (atual) {
            pilha[++topo] = atual;
            atual = atual->esquerda;
        }

        atual = pilha[topo--];

        char* suspeito = encontrarSuspeito(atual->pista);
        if (suspeito && strcmp(suspeito, acusacao) == 0)
            contador++;

        atual = atual->direita;
    }

    if (contador >= 2)
        printf("\nResultado: %s é o culpado!\n", acusacao);
    else
        printf("\nResultado: Evidências insuficientes para condenar %s.\n", acusacao);
}

/* -----------------------------------------
   MAIN
----------------------------------------- */

int main() {

    /* montar mapa fixo */
    Sala* hall = criarSala("Hall de Entrada", "Luvas sujas de tinta");
    hall->esquerda = criarSala("Biblioteca", "Página arrancada do diário");
    hall->direita = criarSala("Cozinha", "Faca limpa demais");

    hall->esquerda->esquerda = criarSala("Porão", "Pegadas recentes");
    hall->esquerda->direita = criarSala("Escritório", "Chave dourada");

    hall->direita->esquerda = criarSala("Sala de Jantar", "Guardanapo rasgado");
    hall->direita->direita = criarSala("Jardim", "Pegada de bota");

    /* Configurar tabela hash: pista -> suspeito */
    inserirNaHash("Luvas sujas de tinta", "Marcos");
    inserirNaHash("Página arrancada do diário", "Ana");
    inserirNaHash("Faca limpa demais", "Carlos");
    inserirNaHash("Pegadas recentes", "Marcos");
    inserirNaHash("Chave dourada", "Ana");
    inserirNaHash("Guardanapo rasgado", "Carlos");
    inserirNaHash("Pegada de bota", "Marcos");

    PistaNode* arvorePistas = NULL;

    printf("\n=== Detective Quest – Nível Mestre ===\n");

    explorarSalas(hall, &arvorePistas);

    printf("\nPistas coletadas:\n");
    exibirPistas(arvorePistas);

    verificarSuspeitoFinal(arvorePistas);

    /* liberar memória */
    liberarPistas(arvorePistas);
    liberarHash();
    liberarSalasRec(hall);

    printf("\nObrigado por jogar Detective Quest - Nível Mestre.\n");
    return 0;
}