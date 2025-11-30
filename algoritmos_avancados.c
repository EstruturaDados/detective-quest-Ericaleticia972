#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[40];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/* criaSala() – cria dinamicamente uma sala com o nome informado */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* explorarSalas() – permite navegar pela árvore escolhendo e/d até chegar em um nó-folha */
void explorarSalas(Sala* atual) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos. Você chegou ao fim da exploração.\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda\n");
        if (atual->direita != NULL) printf("  (d) Ir para a direita\n");
        printf("  (s) Sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } 
        else if (opcao == 's') {
            printf("\nExploração encerrada.\n");
            break;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

/* main() – monta a árvore da mansão e inicia a exploração */
int main() {
    Sala* hall = criarSala("Hall");

    hall->esquerda = criarSala("Sala de Estar");
    hall->direita  = criarSala("Cozinha");

    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita  = criarSala("Jardim");

    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita  = criarSala("Garagem");

    explorarSalas(hall);

    return 0;
}