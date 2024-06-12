#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_FRAME 256
#define TAMANHO_MEMORIA_FISICA 128
#define TAMANHO_TLB 16

void inicializar();
void atualizar_tlb(int numero_pagina, int numero_frame);
int substituir_fifo(int numero_pagina);
int substituir_fifo(int numero_pagina);
int substituir_lru(int numero_pagina);
int substituir_lru(int numero_pagina);
void processar_endereco(int endereco_logico, char *algoritmo, FILE *arquivo_saida);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    exit(1); // Sucesso na passagem dos argumentos (nome do programa, nome do
             // arq e algoritmo de subst.)
  }

  // Abertura do arquivo Address.txt para leitura
  FILE *arquivo_enderecos = fopen(argv[1], "r");
  if (arquivo_enderecos == NULL) {
    exit(1);
  }

  // Criação do artquivo de saída para escrita : Correct.txt
  FILE *arquivo_saida = fopen("Correct.txt", "w");
  if (arquivo_saida == NULL) {
    fclose(arquivo_enderecos);
    exit(1);
  }

  char *algoritmo = argv[2];
  inicializar();

  char linha[256];
  while (fgets(linha, sizeof(linha), arquivo_enderecos)) {
    int endereco_logico = atoi(
        linha); // Converte a linha em endereço lógico (ASCII|- função atoi))
    processar_endereco(endereco_logico, algoritmo, arquivo_saida);
  }

  fclose(arquivo_enderecos);
  fclose(arquivo_saida);

  return 0;
}

// Memória física simulada
unsigned char memoria_fisica[TAMANHO_MEMORIA_FISICA * TAMANHO_FRAME];

// Tabela de páginas
int tabela_paginas[TAMANHO_MEMORIA_FISICA];

// TLB (Translation Lookaside Buffer)
int tlb[TAMANHO_TLB];
int tlb_frames[TAMANHO_TLB];
int tlb_indice = 0; // Índice para a próxima inserção na TLB

// Variáveis do algoritmo FIFO
int fila_fifo[TAMANHO_MEMORIA_FISICA];
int fifo_indice = 0; // Índice para a próxima inserção na fila FIFO

// Variáveis do algoritmo LRU
int tempo_lru[TAMANHO_MEMORIA_FISICA]; // Tempo de uso para cada frame
int contador_tempo_lru = 0;            // Contador de tempo global para LRU

// Função para inicializar as estruturas de dados
// Inicializar com -1: indicar que a posição está vazia (não está sendo
// utilizada)
void inicializar() {
  for (int i = 0; i < TAMANHO_MEMORIA_FISICA; i++) {
    tabela_paginas[i] = -1; // Inicializa a tabela de páginas com -1
    fila_fifo[i] = -1;      // Inicializa a fila FIFO com -1
    tempo_lru[i] = 0;       // Inicializa o tempo de uso LRU com 0
  }
  for (int i = 0; i < TAMANHO_TLB; i++) {
    tlb[i] = -1;        // Inicializa a TLB com -1
    tlb_frames[i] = -1; // Inicializa os frames da TLB com -1
  }
}

// Função para atualizar a TLB
void atualizar_tlb(int numero_pagina, int numero_frame) {
  tlb[tlb_indice] = numero_pagina;       // Atualiza o número da página na TLB
  tlb_frames[tlb_indice] = numero_frame; // Atualiza o número do frame na TLB
  tlb_indice =
      (tlb_indice + 1) % TAMANHO_TLB; // Incrementa o índice de forma circular
}

// Função para substituir páginas usando FIFO
int substituir_fifo(int numero_pagina) {
  int numero_frame = fifo_indice;         // Obtém o frame a ser substituído
  fila_fifo[fifo_indice] = numero_pagina; // Coloca a nova página na fila FIFO
  fifo_indice = (fifo_indice + 1) %
                TAMANHO_MEMORIA_FISICA; // Incrementa o índice de forma circular
  return numero_frame;
}

// Função para substituir páginas usando LRU - (Least Recently Used)
int substituir_lru(int numero_pagina) {
  int tempo_mais_antigo = tempo_lru[0]; // Tempo mais antigo
  int indice_mais_antigo = 0;           // Índice do frame mais antigo
  for (int i = 1; i < TAMANHO_MEMORIA_FISICA;
       i++) { // Encontrar o frame na memória física que não foi usado por mais
              // tempo, para que ele possa ser substituído por uma nova página
    if (tempo_lru[i] <
        tempo_mais_antigo) { // Frame atual é comparado com o mais antigo, se o
                             // atual for menor: o atual está há mais tempo que
                             // o antigo
      tempo_mais_antigo = tempo_lru[i];
      indice_mais_antigo = i;
    }
  }
  tabela_paginas[indice_mais_antigo] =
      numero_pagina; // Substitui a página no frame mais antigo
  tempo_lru[indice_mais_antigo] =
      contador_tempo_lru++; // Atualiza o tempo de uso
  return indice_mais_antigo;
}

// Função para atualizar o tempo de uso no LRU
void atualizar_lru(int numero_frame) {
  tempo_lru[numero_frame] =
      contador_tempo_lru++; // Atualiza o tempo de uso do frame
}

// Função para processar um endereço lógico
void processar_endereco(int endereco_logico, char *algoritmo, FILE *arquivo_saida) {
  int numero_pagina =
      (endereco_logico >> 8) &
      0xFF; // Obtém o número da página:  desloca o endereço 8 bits para a
            // direita e aplica uma máscara para manter apenas os 8 bits menos
            // significativos (representando o número da página).
  int offset = endereco_logico & 0xFF; // Obtém o offset

  int numero_frame = -1;

  // Verifica a TLB
  for (int i = 0; i < TAMANHO_TLB; i++) {
    if (tlb[i] == numero_pagina) {
      numero_frame = tlb_frames[i]; // Encontra o frame na TLB
      break;
    }
  }

  // Se não estiver na TLB, verifica a tabela de páginas
  if (numero_frame == -1) {
    for (int i = 0; i < TAMANHO_MEMORIA_FISICA; i++) {
      if (tabela_paginas[i] == numero_pagina) {
        numero_frame = i; // Encontra o frame na tabela de páginas
        break;
      }
    }
  }

  // Se não estiver na tabela de páginas, usa o algoritmo de substituição
  if (numero_frame == -1) {
    if (strcmp(algoritmo, "fifo") == 0) {
      numero_frame = substituir_fifo(numero_pagina); // Usa FIFO
    } else if (strcmp(algoritmo, "lru") == 0) {
      numero_frame = substituir_lru(numero_pagina); // Usa LRU
    } else {
      exit(1); // Saída : algoritmo desconhecido
    }
  }

  // Atualiza o tempo de uso no LRU, se necessário
  if (strcmp(algoritmo, "lru") == 0) {
    atualizar_lru(numero_frame);
  }

  // Atualiza a TLB
  atualizar_tlb(numero_pagina, numero_frame);

  // Calcula o endereço físico corretamente
  int endereco_fisico = (numero_frame * TAMANHO_FRAME) + offset;

  // Escreve no arquivo de saída
  fprintf(arquivo_saida,
          "Virtual address: %d TLB: %d Physical address: %d Value: %d\n",
          endereco_logico, tlb_indice - 1, endereco_fisico,
          memoria_fisica[endereco_fisico]);
}
