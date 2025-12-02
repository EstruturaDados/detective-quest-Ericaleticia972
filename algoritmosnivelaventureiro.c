#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[40];
    char pista[80];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct PistaNode {
    char pista[80];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

/* criarSala() – cria dinamicamente um cômodo com nome e uma pista opcional */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* inserirPista() – insere a pista coletada na BST em ordem alfabética */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

/* exibirPistas() – exibe todas as pistas coletadas em ordem alfabética */
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/* explorarSalasComPistas() – navega pelas salas e coleta pistas automaticamente */
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda\n");
        if (atual->direita != NULL) printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nSaindo da mansão...\n");
            break;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

/* main() – monta o mapa e inicia a exploração com coleta de pistas */
int main() {
    PistaNode* arvorePistas = NULL;

    Sala* hall = criarSala("Hall de Entrada", "Pegadas de barro no tapete");

    hall->esquerda = criarSala("Sala de Estar", "Um perfume forte no ar");
    hall->direita  = criarSala("Cozinha", "Uma xícara quebrada");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Um livro arrancado da estante");
    hall->esquerda->direita  = criarSala("Jardim", "Terra remexida recentemente");

    hall->direita->esquerda = criarSala("Despensa", "");
    hall->direita->direita  = criarSala("Garagem", "Marcas de pneus recentes");

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n=== Pistas coletadas ===\n");
    exibirPistas(arvorePistas);

    return 0;
}