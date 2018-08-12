/*
 * GLUT Subway Surfers
 *
 * Desenvolvido por Renan Souza Silva em Agosto 2018
 *
 * Esse programa is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ImportarSTL.h" // .h para importar arquivo STL
#include "LerBMP.h" // .h para ler arquivo BMP
#include <windows.h>
#include <gl/glu.h>
#include <math.h>
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

// Variaveis Globais

// Variavel que determina a quantidade de inimigos do jogo
const int quantidadeInimigo=10;

STL modelo;

float tempo = 6;

//Definição das estruturas

// Criando um (estrutura) objeto esfera

struct TVector
{
    float x,y,z;
};

struct TObject3D
{
    TVector posicao;
    TVector velocidade;
};

TObject3D sphere;

// Criando um (estrutura) objeto inimigo (trem)

struct TVectorInimigo
{
    float x,y,z;
};

struct TObject3DInimigo
{
    TVector posicao;
    bool isVivo;
};

TObject3D inimigo;

TObject3DInimigo vetInimigo[quantidadeInimigo];

void display();

#define GRAVITY 9.8

void SpawnInimigos() //criar meu inimigo no vetor
{
    for ( int i = 0; i < quantidadeInimigo; i++ )
    {
        if(vetInimigo[i].isVivo == true) //ve no vetor inimigos se tem espaço livre
        {

        }
        else
        {
            float vet[]= {-2.5, 0.0, 2.5};
            vetInimigo[i].posicao.x = vet[rand()%3]; //Achou morto
            //printf("%f \n", vetInimigo[i].posicao.x);
            vetInimigo[i].posicao.y = 0;
            vetInimigo[i].isVivo = true;
            //printf("beef de calabresa posicao x escolhida %f e i escolhido %d",   vetInimigo[i].posicao.x, i);
            return;
        }
    }


}

void RunPhysics(float dt)
{
    sphere.velocidade.y=sphere.velocidade.y-GRAVITY*dt; //degivada do espaco

    sphere.posicao.x=sphere.posicao.x+sphere.velocidade.x*dt;
    sphere.posicao.y=sphere.posicao.y+sphere.velocidade.y*dt;
    sphere.posicao.z=sphere.posicao.z+sphere.velocidade.z*dt;
    //sphere.velocity.y=0;

    //lançamento para cima a aceleração é negativa (g < 0).
    //sphere.position.y = (sphere.velocity.y)*dt - (GRAVITY*dt*dt)/2;
    //hmax = ((sphere.velocity.y)*(sphere.velocity.y))/(2*GRAVITY);
    //sphere.position.x = (sphere.velocity.x)*dt;


    /*if(sphere.velocity.y > 0)
    {
        sphere.position.y = (sphere.velocity.y)*dt - (GRAVITY*dt*dt)/2;
    }/*
    //And just for now, let's make it so that if the vertical position is less than zero, we assume the character's on the ground.
    if(sphere.position.y < 0.0)
    {
       sphere.position.y = 0;
       //OnGroud = true;
    } else if (sphere.position.y < 0.5 )
    {
        sphere.velocity.y=-GRAVITY*dt;
        //sphere.velocity.y=-sphere.velocity.y;
        //sphere.position.y = (sphere.velocity.y)*dt + (GRAVITY*dt*dt)/2;
        //OnGroud = false;
    }
    */

    if (sphere.velocidade.y<0)
    {
        //sphere.position.y=0;
        if (sphere.posicao.y<0.5)
        {

            //if ((sphere.position.x>=-2.5))
            sphere.velocidade.y=-GRAVITY*dt;
            //sphere.velocity.y=-sphere.velocity.y;
        }

        if (sphere.posicao.y<0)
        {
            sphere.posicao.y=0;
        }

        //sphere.velocity.y=-sphere.velocity.y;
        //sphere.velocity.y=0;
        //sphere.position.y=0;

    }
}

void desenhaEstrada()
{
    glPushMatrix();

    //Desenha Linha central
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(-0.7,0.0,10.0);
        glVertex3f(0.7,0.0,10.0);
        glVertex3f(0.7,0.0,-100.0);
        glVertex3f(-0.7,0.0,-100.0);
    }
    glEnd();

    //Desenha Linha da direita
    glTranslatef(2.5,0.0,0.0);
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(-0.7,0.0,10.0);
        glVertex3f(0.7,0.0,10.0);
        glVertex3f(0.7,0.0,-100.0);
        glVertex3f(-0.7,0.0,-100.0);
    }
    glEnd();

    //Desenha Linha da esquerda
    glTranslatef(-5.0,0.0,0.0);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(-0.7,0.0,10.0);
        glVertex3f(0.7,0.0,10.0);
        glVertex3f(0.7,0.0,-100.0);
        glVertex3f(-0.7,0.0,-100.0);
    }
    glEnd();

    glPopMatrix();
}

void desenhaEsfera()
{

GLuint texture = LoadTexture("Zuma.bmp");

GLUquadric *quad;

glPushMatrix();

//draw sun
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);

quad = gluNewQuadric();
gluQuadricTexture(quad, 1);

glTranslated(sphere.posicao.x,sphere.posicao.y,0);

gluSphere(quad, 0.625f,20,20);

glPopMatrix();
}

//Desenha X Trem aleatoriamente
void desenhaTrem(struct TObject3DInimigo inimigo)
{
    float vet[]= {-2.5, 0.0, 2.5}; //Vetor de posicao (faixa esquerda, central, direita) para desenhar o trem

    for (int i = 0; i < 2; i++) // Varre todo o vetor de posicao do trem
    {
        glPushMatrix();

        glTranslatef(inimigo.posicao.x, inimigo.posicao.y, -7.0);
        //Escala e Desenha modelo 3D
        glScalef(1.2,1.2,5);
        DesenharSTL(modelo);

        glPopMatrix();
    }

}

void AtualizarPosicao() //Atualiza e desenha meu inimigo
{
    for ( int i = 0; i < quantidadeInimigo; i++ ) // Varre todo o vetor de quantidade de inimigos
    {
        if(vetInimigo[i].isVivo == true) //Verifica se no vetor inimigos tem espaço livre
        {
            vetInimigo[i]; //Se tá vivo ele muda a posicao e vem até o personagem
            //vetInimigo[i].posicao.z = vetInimigo[rand()%3]; //Achou morto
            //printf("%f \n", vetInimigo[i].posicao.z);//desce a posição em z e desenha inimigo
            desenhaTrem(vetInimigo[i]);
        }
    }

}

void idle()
{
    RunPhysics(0.01);
    display();
}

// Drawing routine. DrawScene
void display(void)
{
    if(tempo > 0.2f)
    {
        tempo = tempo - (tempo/500.0f);
    }
    else
    {
        tempo = 6.0f;
        SpawnInimigos();
    }

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //Define a posição da camera
    glTranslatef(0.0,-2.0,-7.0);
    glRotatef(20,1.0,0.0,0.0);

    //Desenha estrada
    desenhaEstrada();

    AtualizarPosicao();

    // Desenha esfera
    //desenhaEsfera();

    //Se o jogo já começou, inicia o personagem e desenha o personagem
    /*
    if(!gameStarted)
    {
    // Defina a posição da esfera.
    //glTranslatef(Xvalue, Yvalue,-7.0);
    // Apply equations of motion to transform sphere.
    glTranslatef(Xvalue, voy*t - (g/2.0)*t*t, -7.0);
    glRotatef(Angle,1.0,0.0,0.0);

    //Desenha a esfera
    glColor3f(0.0, 0.0, 1.0);
    glutSolidSphere(0.77f,20,20);

    */

    glutSwapBuffers();

}

// Janela OpenGL reformula a rotina.
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLdouble)width/(GLdouble)height,1,100);
    glMatrixMode(GL_MODELVIEW);
}

//Rotina de processamento de entrada de teclado.
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0); //Sair do jogo
        break;
    case 'w':
        sphere.velocidade.y +=10; // Permite pular na direcao y
        break;
    case 'a':
        // Trava a esfera em x para não ultrapassar o limite da faixa da esquerda
        if(sphere.posicao.x<=-2.5)
        {
        }
        else
        {
            sphere.posicao.x -= 2.5; // Permite andar para faixa da esquerda na direcao x
        }
        break;
    case 'd':
        // Trava a esfera em x para não ultrapassar o limite da faixa da direita
        if(sphere.posicao.x>=2.5)
        {
        }
        else
        {
            sphere.posicao.x += 2.5; // Permite andar para faixa da direita na direcao x
        }
        break;
    case 'r': //Reseta o jogo
        //Reinicia a posicao da esfera
        sphere.posicao.x=0;
        sphere.posicao.y=0;
        break;
    case 't':
        break;
    }
    glutPostRedisplay();
}

// Rotina para gerar instruções de interação na janela do C ++.
void printInteraction(void)
{
    cout << "Interacao:" << endl;
    cout << "Pressione as teclas de A, W, S, D para mover a esfera." << endl
         << "Pressione R para redefinir." << endl
         << "Pressione T para iniciar o jogo." << endl
         << "Pressione Q para sair." << endl;
}

/* Program entry point */
// Rotina principal.
int main(int argc, char **argv)
{
    srand(time(NULL)); // Randomiza o rand()
    printInteraction();
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Subway Surfers");

    LerArquivo(&modelo, "cubo.stl");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //glutFullScreen();             // Put into full screen
    glutMainLoop();

    return EXIT_SUCCESS;
}

