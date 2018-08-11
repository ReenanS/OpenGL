#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <time.h>
#include <math.h>
#include "ImportarSTL.h"


#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

using namespace std;

// Variaveis Globais.
/*static float Xvalue = 0.0, Yvalue = -2.0; // Co-ordinates of the sphere.
static float Angle = 0.0; // Angle to rotate the sphere.
static int gameStarted = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static float t = 1.0; // Time parameter.
static float h = 0.5; // Horizontal component of initial velocity.
static float voy = 4.0; // Vertical component of initial velocity.
static float g = 9.8;  // Gravitational accelaration.
static int isFloor = 0;*/
const int quantidadeInimigo=5;
float hmax;
STL modelo;

void display();
void drawItem();

float tempo = 6;

struct TVector
{
    float x,y;
};

struct TObject3D
{
    TVector position;
    TVector velocity;
};

TObject3D sphere;

struct TVectorInimigo
{
    float x,y;
};

struct TObject3DInimigo
{
    TVector posicao;
    bool isVivo;
};

TObject3D inimigo;

TObject3DInimigo vetInimigo[quantidadeInimigo];

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
            printf("%f \n", vetInimigo[i].posicao.x);
            vetInimigo[i].posicao.y = 0;
            vetInimigo[i].isVivo = true;
            //printf("beef de calabresa posicao x escolhida %f e i escolhido %d",   vetInimigo[i].posicao.x, i);
            return;
        }
    }


}

//(float dt)
void RunPhysics(float dt)
{
    sphere.velocity.y=sphere.velocity.y-GRAVITY*dt; //degivada do espaco

    sphere.position.x=sphere.position.x+sphere.velocity.x*dt;
    sphere.position.y=sphere.position.y+sphere.velocity.y*dt;

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

    if (sphere.velocity.y<0)
    {
        //sphere.position.y=0;
        if (sphere.position.y<0.5)
        {

            //if ((sphere.position.x>=-2.5))
            sphere.velocity.y=-GRAVITY*dt;
            //sphere.velocity.y=-sphere.velocity.y;
        }

        if (sphere.position.y<0)
        {
            sphere.position.y=0;
        }

        //sphere.velocity.y=-sphere.velocity.y;
        //sphere.velocity.y=0;
        //sphere.position.y=0;

    }
}

void drawRoad()
{
    glPushMatrix();

    //Linha central
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(-0.7,0.0,10.0);
        glVertex3f(0.7,0.0,10.0);
        glVertex3f(0.7,0.0,-100.0);
        glVertex3f(-0.7,0.0,-100.0);
    }
    glEnd();

    //Linha da direita
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

    //Linha da esquerda
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
    /*
        glTranslatef(2.5,0.0,-120.0);
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(-0.7,0.0,10.0);
            glVertex3f(0.7,0.0,10.0);
            glVertex3f(0.7,0.0,-100.0);
            glVertex3f(-0.7,0.0,-100.0);
        }
        glEnd();

        glTranslatef(2.5,0.0,0.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(-0.7,0.0,10.0);
            glVertex3f(0.7,0.0,10.0);
            glVertex3f(0.7,0.0,-100.0);
            glVertex3f(-0.7,0.0,-100.0);
        }
        glEnd();

        glTranslatef(-5.0,0.0,0.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(-0.7,0.0,10.0);
            glVertex3f(0.7,0.0,10.0);
            glVertex3f(0.7,0.0,-100.0);
            glVertex3f(-0.7,0.0,-100.0);
        }
        glEnd();*/
    glPopMatrix();
}

/* This defines that point holds x and y coords.
typedef struct
{
    float x;
    float y;
} point;

// This is an array of 10 points
point points[10];


void pointsCoords(void)
{



// Generate the points (point coodrs)
    for ( int i = 0; i < 10; i++ )
    {
        points.x = rand() % 650 + 1;
        points.y = rand() % 650 + 1;
    }

}

void drawItem(void)
{

// Draw the points
    for ( int i = 0; i < 10; i++ )
    {
        glVertex3f(points.x, points.y,0.0);
    }
}

*/
void drawSphere()
{
    glPushMatrix(); // draw and place Sphere
    glTranslated(sphere.position.x,sphere.position.y,0);

    glColor3f(0.0, 0.0, 1.0);
    glutSolidSphere(0.625f,20,20);

    glPopMatrix();
}

//Desenha X Obstaculos aleatoriamente
void drawObstacle()
{
    float vet[]= {-2.5, 0.0, 2.5};

    for (int i = 0; i < 2; i++)
    {
        glPushMatrix();
        glTranslatef(vet[rand()%3], 0.0, -7.0);
        //glTranslatef(2,0,-20);
        glColor3f(1.0,1.0,0.0);
        glutSolidCube(0.625f);
        glPopMatrix();
    }

}

//Desenha X Trem aleatoriamente
void drawTrem(struct TObject3DInimigo inimigo)
{
    float vet[]= {-2.5, 0.0, 2.5};

    for (int i = 0; i < 2; i++)
    {
        glPushMatrix();
        glTranslatef(inimigo.posicao.x, inimigo.posicao.y, -7.0);
        //glTranslatef(2,0,-20);
        glColor3f(1.0,1.0,0.0);
        //glutSolidCube(0.625f);
        //Desenha modelo 3D
        glScalef(1.2,1.2,1.2);
        DesenharSTL(modelo);

        glPopMatrix();
    }

}

void AtualizarPosicao() //Atualiza e desenha meu inimigo
{
    for ( int i = 0; i < quantidadeInimigo; i++ )
    {
        if(vetInimigo[i].isVivo == true) //ve no vetor inimigos se tem espaço livre
        {
            vetInimigo[i]; //Paramos aqui
            drawTrem(vetInimigo[i]);

        }
    }

}

void colisao()
{

}

void myIdle()
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //Define a posição da camera
    glTranslatef(0.0,-2.0,-7.0);
    glRotatef(20,1.0,0.0,0.0);



    //    glMatrixMode(GL_MODELVIEW);
    //glEnable(GL_CULL_FACE);
    //glRotatef(-20,1.0,0.0,0.0);
    //glTranslatef(0.0,2.0,7.0);
    //glScalef(0.5f,0.5f,0.0f);
    //glTranslatef(0.0,-2.0,7-.0);
    //glRotatef(20,1.0,0.0,0.0);

    // glRotatef(20,1.0,0.0,0.0);
    //   glColor3f(1,0,0);
    //   DesenharSTL(modelo);
    // glPopMatrix();



    //Desenha a estrada
    drawRoad();

    /*Desenha modelo 3D
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    DesenharSTL(modelo);
    glPopMatrix();
    */
    AtualizarPosicao();


    //drawObstacle();
    drawSphere();

    //glMatrixMode(GL_PROJECTION);
// modo de configuração do modelo
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();





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

/* Routine to increase the rotation angle.
void increaseYComponent(void)
{
    voy += 0.2;
}

// Timer function.
void animate(int value)
{
    if (gameStarted)
    {
        increaseYComponent();

        glutPostRedisplay();

    }
}
*/

// Rotina de Inicialização.
void setup(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
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
        exit(0);
        break;
    case 'w':
        //voy += 0.1;
        //sphere.position.y += 1;
        sphere.velocity.y +=10;
        //printf("%f \n",sphere.velocity.y);
        break;
    case 'a':
        if(sphere.position.x<=-2.5)
        {
        }
        else
        {
            sphere.position.x -= 2.5;
        }
        break;
    case 'd':
        if(sphere.position.x>=2.5)
        {
        }
        else
        {
            sphere.position.x += 2.5;
        }
        break;
    case 'r':
        sphere.position.x=0;
        sphere.position.y=0;
        //glTranslated(sphere.position.x,sphere.position.y,0);
        //RunPhysics(0.0);
        break;
    case 't':
        /*if (gameStarted)
            gameStarted = 0; //Press t to toggle between animation on and off.
        else
        {
            gameStarted = 1;
            //animate(1);
        }*/
        //Start_physics = 0.01;
        //glutPostRedisplay();
        //drawItem();
        SpawnInimigos();

        //drawTrem(TObject3DInimigo );
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


// Rotina principal.
int main(int argc, char **argv)
{
    srand(time(NULL)); // Randomiza o rand()
    printInteraction();
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Only 1 Sphere");

    LerArquivo(&modelo, "cubo.stl");
    printf("Nome: %s\n", modelo.nome);
    printf("Numero de faces: %i\n", modelo.nFaces);
    //glScalef(0.5f,0.5f,0.5f);

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(myIdle);
    glutKeyboardFunc(key);
    glutFullScreen();             // Put into full screen

    glewInit();

    glutMainLoop();
}

