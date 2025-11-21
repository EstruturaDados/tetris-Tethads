#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===========================
// Definições e Constantes
// ===========================
#define TAM_FILA 5
#define TAM_PILHA 3

// ===========================
// Estrutura da Peça
// ===========================
typedef struct {
    char nome;
    int id;
} Peca;

// ===========================
// Fila Circular de Peças
// ===========================
typedef struct {
    Peca itens[TAM_FILA];
    int frente, tras, tamanho;
} Fila;

// ===========================
// Pilha de Peças Reservadas
// ===========================
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ===========================
// Variável global para ID único
// ===========================
int contadorID = 0;

// ===========================
// Função para gerar uma peça nova aleatória
// ===========================
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// ===========================
// Funções da Fila
// ===========================
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->tamanho--;
    return removida;
}

// ===========================
// Funções da Pilha
// ===========================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->itens[++(p->topo)] = x;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    return p->itens[(p->topo)--];
}

Peca topoPilha(Pilha *p) {
    if (pilhaVazia(p)) {
        Peca nula = {'-', -1};
        return nula;
    }
    return p->itens[p->topo];
}

// ===========================
// Funções de Exibição
// ===========================
void exibirFila(Fila *f) {
    printf("Fila de peças\t");
    int i;
    int index = f->frente;
    for (i = 0; i < f->tamanho; i++) {
        printf("[%c %d] ", f->itens[index].nome, f->itens[index].id);
        index = (index + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n===============================\n");
    printf("ESTADO ATUAL:\n");
    exibirFila(f);
    exibirPilha(p);
    printf("===============================\n");
}

// ===========================
// Funções de Operações do Jogo
// ===========================
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca jogada = desenfileirar(f);
    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
    // Repor nova peça
    enfileirar(f, gerarPeca());
}

void reservarPeca(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("Nenhuma peça na fila para reservar.\n");
        return;
    }
    if (pilhaCheia(p)) {
        printf("A pilha de reserva está cheia!\n");
        return;
    }
    Peca movida = desenfileirar(f);
    empilhar(p, movida);
    printf("Peça reservada: [%c %d]\n", movida.nome, movida.id);
    // Repor nova peça na fila
    enfileirar(f, gerarPeca());
}

void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhuma peça na pilha de reserva!\n");
        return;
    }
    Peca usada = desempilhar(p);
    printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
}

void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Não é possível trocar: fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Troca entre a peça da frente da fila e o topo da pilha realizada!\n");
}

void trocarTres(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("Não é possível realizar a troca múltipla (3 peças necessárias em ambas as estruturas).\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int indexFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->itens[indexFila];
        f->itens[indexFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }
    printf("Troca múltipla (3 peças) realizada com sucesso!\n");
}

// ===========================
// Função Principal
// ===========================
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarTopo(&fila, &pilha);
                break;
            case 5:
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}