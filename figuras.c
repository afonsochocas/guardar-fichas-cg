

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "figuras.h"

static Figura minhas_figuras[MAX_FIGURAS];
static int numero_figuras = 0;

static float randFloat(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

Cor figurasCorAleatoria(void)
{
    Cor cor;

    cor.r = randFloat(0.0f, 1.0f);
    cor.g = randFloat(0.0f, 1.0f);
    cor.b = randFloat(0.0f, 1.0f);

    return cor;
}

Cor figurasCorContorno(Cor cor)
{
    Cor resultado;

    resultado.r = cor.r * 0.35f;
    resultado.g = cor.g * 0.35f;
    resultado.b = cor.b * 0.35f;

    return resultado;
}

void figurasCriar(float x1, float y1,
                  float x2, float y2,
                  int tipo, Cor preenchimento,
                  Cor limite)
{
    Figura *f;

    if (numero_figuras >= MAX_FIGURAS) {
        printf("Limite maximo de figuras atingido!\n");
        return;
    }

    f = &minhas_figuras[numero_figuras];

    if (x1 < x2)
        f->x = x1;
    else
        f->x = x2;

    if (y1 < y2)
        f->y = y1;
    else
        f->y = y2;

    if (x1 > x2)
        f->largura = x1 - x2;
    else
        f->largura = x2 - x1;

    if (y1 > y2)
        f->altura = y1 - y2;
    else
        f->altura = y2 - y1;

    f->tipo = tipo;
    f->preenchimento = preenchimento;
    f->limite = limite;

    numero_figuras++;

    printf("Figura criada. Total: %d\n", numero_figuras);
}

static void desenharFigura(Figura f)
{
    float x = f.x;
    float y = f.y;
    float w = f.largura;
    float h = f.altura;

    float cx = x + w / 2.0f;
    float cy = y + h / 2.0f;
    float lado;

    lado = (w < h) ? w : h;

    glColor3f(f.preenchimento.r,
              f.preenchimento.g,
              f.preenchimento.b);

    switch (f.tipo) {

        case RETANGULO:
            glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + w, y);
                glVertex2f(x + w, y + h);
                glVertex2f(x, y + h);
            glEnd();
            break;

        case TRIANGULO:
            glBegin(GL_TRIANGLES);
                glVertex2f(cx, y + h);
                glVertex2f(x, y);
                glVertex2f(x + w, y);
            glEnd();
            break;

        case LINHA:
            break;

        case QUADRADO:
            glBegin(GL_QUADS);
                glVertex2f(cx - lado / 2.0f,
                           cy - lado / 2.0f);
                glVertex2f(cx + lado / 2.0f,
                           cy - lado / 2.0f);
                glVertex2f(cx + lado / 2.0f,
                           cy + lado / 2.0f);
                glVertex2f(cx - lado / 2.0f,
                           cy + lado / 2.0f);
            glEnd();
            break;
    }

    /* Desenho do contorno */
    glColor3f(f.limite.r, f.limite.g, f.limite.b);

    switch (f.tipo) {

        case RETANGULO:
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y);
                glVertex2f(x + w, y);
                glVertex2f(x + w, y + h);
                glVertex2f(x, y + h);
            glEnd();
            break;

        case TRIANGULO:
            glBegin(GL_LINE_LOOP);
                glVertex2f(cx, y + h);
                glVertex2f(x, y);
                glVertex2f(x + w, y);
            glEnd();
            break;

        case LINHA:
            glBegin(GL_LINES);
                glVertex2f(x, y);
                glVertex2f(x + w, y + h);
            glEnd();
            break;

        case QUADRADO:
            glBegin(GL_LINE_LOOP);
                glVertex2f(cx - lado / 2.0f,
                           cy - lado / 2.0f);
                glVertex2f(cx + lado / 2.0f,
                           cy - lado / 2.0f);
                glVertex2f(cx + lado / 2.0f,
                           cy + lado / 2.0f);
                glVertex2f(cx - lado / 2.0f,
                           cy + lado / 2.0f);
            glEnd();
            break;
    }
}

void figurasDesenharTodas(void)
{
    int i;

    for (i = 0; i < numero_figuras; i++) {
        desenharFigura(minhas_figuras[i]);
    }
}

void figurasInicializar(void)
{
    int i;

    srand((unsigned int)time(NULL));

    numero_figuras = 0;

    for (i = 0; i < 5; i++) {
        float x;
        float y;
        float largura;
        float altura;
        int tipo;
        Cor preenchimento;
        Cor limite;

        x = randFloat(20.0f, 600.0f);
        y = randFloat(20.0f, 400.0f);
        largura = randFloat(50.0f, 150.0f);
        altura = randFloat(50.0f, 150.0f);

        tipo = rand() % 4;

        preenchimento = figurasCorAleatoria();
        limite = figurasCorAleatoria();

        figurasCriar(x, y,
                     x + largura, y + altura,
                     tipo, preenchimento, limite);
    }
}

void figurasApagarAleatoria(void)
{
    int indice;
    int i;

    if (numero_figuras == 0) {
        printf("Nao existem figuras para apagar.\n");
        return;
    }

    indice = rand() % numero_figuras;

    for (i = indice; i < numero_figuras - 1; i++) {
        minhas_figuras[i] = minhas_figuras[i + 1];
    }

    numero_figuras--;

    printf("Figura %d apagada. Total: %d\n",
           indice, numero_figuras);
}

int figurasNumero(void)
{
    return numero_figuras;
}

/* Desenha a bounding box provisoria */
void figurasDesenharPrevisualizacao(float x1, float y1,
                                    float x2, float y2,
                                    Cor cor)
{
    glColor3f(cor.r, cor.g, cor.b);

    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}
