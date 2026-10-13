
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "figuras.h"

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

static int formaAtiva = RETANGULO;
static Cor corAtiva;

static int esperaSegundoClique = 0;

static float primeiroX = 0.0f;
static float primeiroY = 0.0f;

static float ratoX = 0.0f;
static float ratoY = 0.0f;

static int contadorJanela = 0;

static Cor paleta[10] = {
    {1.0f, 0.0f, 0.0f},  /* 0 - vermelho */
    {0.0f, 1.0f, 0.0f},  /* 1 - verde */
    {0.0f, 0.0f, 1.0f},  /* 2 - azul */
    {1.0f, 1.0f, 0.0f},  /* 3 - amarelo */
    {1.0f, 0.0f, 1.0f},  /* 4 - magenta */
    {0.0f, 1.0f, 1.0f},  /* 5 - ciano */
    {1.0f, 0.5f, 0.0f},  /* 6 - laranja */
    {1.0f, 1.0f, 1.0f},  /* 7 - branco */
    {0.0f, 0.0f, 0.0f},  /* 8 - preto */
    {0.5f, 0.0f, 0.5f}   /* 9 - roxo */
};

static void converterRato(int xRato, int yRato,
                          float *x, float *y)
{
    *x = (float)xRato;
    *y = (float)(ALTURA_JANELA - yRato);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    contadorJanela++;

    printf("Janela desenhada %d vezes.\n", contadorJanela);

    figurasDesenharTodas();

    if (esperaSegundoClique) {
        figurasDesenharPrevisualizacao(
            primeiroX, primeiroY,
            ratoX, ratoY,
            corAtiva
        );
    }

    glFlush();
}

static void rato(int botao, int estado, int xRato, int yRato)
{
    float x;
    float y;

    converterRato(xRato, yRato, &x, &y);

    printf("Botao: %d | Estado: %d | Posicao rato: (%d, %d)\n",
           botao, estado, xRato, yRato);

    printf("Posicao OpenGL: (%.1f, %.1f)\n", x, y);

    if (estado != GLUT_DOWN) {
        return;
    }

    /* Botao esquerdo: criar figura com dois cliques */
    if (botao == GLUT_LEFT_BUTTON) {

        if (!esperaSegundoClique) {
            /* Primeiro clique */
            primeiroX = x;
            primeiroY = y;

            ratoX = x;
            ratoY = y;

            esperaSegundoClique = 1;

            printf("Primeiro ponto selecionado.\n");
            printf("Move o rato e clica novamente.\n");
        }
        else {
            /* Segundo clique: cria a figura */
            figurasCriar(
                primeiroX, primeiroY,
                x, y,
                formaAtiva,
                corAtiva,
                figurasCorContorno(corAtiva)
            );

            esperaSegundoClique = 0;

            printf("Figura concluida.\n");
        }

        glutPostRedisplay();
    }

    else if (botao == GLUT_RIGHT_BUTTON) {
        figurasApagarAleatoria();

        esperaSegundoClique = 0;

        glutPostRedisplay();
    }
}

static void movimentoRato(int xRato, int yRato)
{
    converterRato(xRato, yRato, &ratoX, &ratoY);

    if (esperaSegundoClique) {
        glutPostRedisplay();
    }
}

static void teclado(unsigned char tecla, int x, int y)
{
    int numero;

    (void)x;
    (void)y;

    if (tecla == 'q' || tecla == 'Q') {
        exit(0);
    }

    if (tecla >= '0' && tecla <= '9') {
        numero = tecla - '0';

        corAtiva = paleta[numero];

        printf("Cor ativa: %d\n", numero);

        glutPostRedisplay();
        return;
    }

    switch (tecla) {

        case 'R':
        case 'r':
            formaAtiva = RETANGULO;
            printf("Forma ativa: Retangulo\n");
            break;

        case 'T':
        case 't':
            formaAtiva = TRIANGULO;
            printf("Forma ativa: Triangulo\n");
            break;

        case 'L':
        case 'l':
            formaAtiva = LINHA;
            printf("Forma ativa: Linha\n");
            break;

        case 'S':
        case 's':
            formaAtiva = QUADRADO;
            printf("Forma ativa: Quadrado\n");
            break;

        default:
            break;
    }
}

static void reshape(int largura, int altura)
{
    glViewport(0, 0, largura, altura);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        0.0, LARGURA_JANELA,
        0.0, ALTURA_JANELA
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutCreateWindow("CG - Ficha 2");

    /* Fundo preto */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* Cor e forma iniciais */
    corAtiva = paleta[0];
    formaAtiva = RETANGULO;

    figurasInicializar();

    glutDisplayFunc(display);
    glutMouseFunc(rato);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoRato);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
