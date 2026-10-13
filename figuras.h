

#ifndef FIGURAS_H
#define FIGURAS_H

/* Tipos de figuras */
#define RETANGULO 0
#define TRIANGULO 1
#define LINHA 2
#define QUADRADO 3

#define MAX_FIGURAS 100

/* Estrutura que representa uma cor RGB */
typedef struct {
    float r;
    float g;
    float b;
} Cor;

/* Estrutura que representa uma figura */
typedef struct {
    float x;
    float y;
    float largura;
    float altura;
    int tipo;
    Cor preenchimento;
    Cor limite;
} Figura;

void figurasInicializar(void);

void figurasDesenharTodas(void);

void figurasCriar(float x1, float y1,
                  float x2, float y2,
                  int tipo, Cor preenchimento,
                  Cor limite);

void figurasApagarAleatoria(void);

int figurasNumero(void);

Cor figurasCorAleatoria(void);

Cor figurasCorContorno(Cor cor);

/* Desenha a pre-visualizacao da bounding box */
void figurasDesenharPrevisualizacao(float x1, float y1,
                                    float x2, float y2,
                                    Cor cor);


 #endif
