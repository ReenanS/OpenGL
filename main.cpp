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
const int quantidadeInimigo = 100;

// Determina os pontos de vida para definir quando morre
int quantidadeVida = 3;

// Determina o raio da esfera
float raio = 0.625f;

//Variaveis do FOG
static int fogOn = 0; // Fog on?
GLfloat fogColor[3] = {0.8f, 0.8f, 0.8f}; // Define a nice light grey
//char *TEXT_ID_ARRAY[] = {"cubo.stl","sphere.stl"};

STL modelo;

//Variaveis da textura
//static int textureOn = 1; // Fog on?

float tempo = 6;

bool isJumping = false; // Ta pulando? Nao

//Textura

GLuint trilhoTex;
GLuint chaoTex;
GLuint ceuTex;
GLuint tunelTex;

//Definição das estruturas

// Criando um (estrutura) objeto - esfera

struct Coordenada
{
    float x,y,z;
};

struct Objeto3D
{
    Coordenada posicao;
    Coordenada velocidade;
};

Objeto3D esfera;

// Criando um (estrutura) objeto inimigo (trem)

struct Objeto3DInimigo
{
    Coordenada posicao;
    bool isVivo;
};

Objeto3DInimigo trem;

Objeto3DInimigo vetInimigo[quantidadeInimigo];

void display();
void drawBitmapText(char *string, float x, float y, float z);

#define GRAVITY 9.81

void drawBitmapText(char *string, float x, float y, float z)
{
    char *c;
    glRasterPos3f(x, y, z);//define position on the screen where to draw text.

    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}


void SpawnInimigos() //criar meu inimigo no vetor //seta as posições do inimiogs / trem
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
            vetInimigo[i].posicao.z = -100;
            vetInimigo[i].isVivo = true;
            //printf("beef de calabresa posicao x escolhida %f e i escolhido %d",   vetInimigo[i].posicao.x, i);
            return;
        }
    }


}

float SmoothMovement(float flGoal, float flCurrent, float dt)
{
    float flDifference = flGoal - flCurrent;

    if (flDifference > dt)
        return flCurrent + dt;
    if (flDifference < -dt)
        return flCurrent - dt;

    return flGoal;
}


// In this Update() function we need to update all of our characters. Move them around or whatever we want to do.
void RunPhysics(float dt)
{

    esfera.velocidade.y=esfera.velocidade.y-GRAVITY*dt; //degivada do espaco

    esfera.posicao.x=esfera.posicao.x+esfera.velocidade.x*dt;
    esfera.posicao.y=esfera.posicao.y+esfera.velocidade.y*dt;
    esfera.posicao.z=esfera.posicao.z+esfera.velocidade.z*dt;
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

    if (esfera.velocidade.y<0)
    {
        //printf("%f", esfera.posicao.y);
        // if((esfera.posicao.y > 1.9f) && (esfera.posicao.y < 2.0f)){
        //        esfera.posicao.y = 2.0f;
        //    printf("wasertfgyjg");
        //            esfera.velocidade.y=esfera.velocidade.y+GRAVITY*dt; //degivada do espaco
        //}
        //sphere.position.y=0;
        //if (esfera.posicao.y<0.5)
        //{
            //if ((sphere.position.x>=-2.5))
           // esfera.velocidade.y=-GRAVITY*dt;
         //  esfera.velocidade.y = 0;
            //sphere.velocity.y=-sphere.velocity.y;
        //}

        if (esfera.posicao.y<0)
        {
            esfera.velocidade.y = 0;
            esfera.posicao.y=0;
            isJumping = false;
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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, trilhoTex);
    glBegin(GL_QUADS); //Desenha no sentido horario
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.25f,0.0f,2.0f); //Vertice inferior esquerdo
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-1.25f,0.0f,-100.0f); //Vertice superior esquerdo
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(1.25f,0.0f,-100.0f); //Vertice superior direito
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.25f,0.0f,2.0f); //Vertice inferior direito
    }
    glEnd();

    //Desenha Linha da direita
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.25f,0.0f,2.0f);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(1.25f,0.0f,-100.0f);
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(3.75f,0.0f,-100.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(3.75f,0.0f,2.0f);
    }
    glEnd();

    //Desenha Linha da esquerda
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-3.75f,0.0f,2.0f);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-3.75f,0.0f,-100.0f);
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(-1.25f,0.0f,-100.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.25f,0.0f,2.0f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void desenhaEsfera()
{
    glPushMatrix();

    glTranslated(esfera.posicao.x,esfera.posicao.y+raio,0);
    //glTranslatef(0,0,0);
    //glColor3f(0.0f,0.0f,1.0f);
    glutSolidSphere(raio,20,20);
    //glScalef(1.2,1.2,5);
    //DesenharSTL(modelo2);

    glPopMatrix();
}

// Comparacao de colisao do personagem (esfera) com os trem (cubo) - Colisao AABB
bool objetoColidiu(struct Objeto3D esfera, struct Objeto3DInimigo trem)
{
    // Verificar se houve colisao - metodo AABB - para eixos x e z
    if((esfera.posicao.x < trem.posicao.x + 2.5) && (esfera.posicao.x +  raio > trem.posicao.x) &&
            (esfera.posicao.z < trem.posicao.z + 2.5) && (esfera.posicao.z + raio > trem.posicao.z))
    {
        return true;
        if(trem.posicao.y = 2.5)
        {
            //return true;
            //glTranslatef(esfera.posicao.x, 4.5f, esfera.posicao.z);
        }
    }
    //}
    // Se nao houve colisao
    else
    {
        return false;
    }
}


//Desenha X Trem aleatoriamente
void desenhaTrem(struct Objeto3DInimigo trem)
{
    float vet[]= {-2.5, 0.0, 2.5}; //Vetor de posicao (faixa esquerda, central, direita) para desenhar o trem

    for (int i = 0; i < 2; i++) // Varre todo o vetor de posicao do trem
    {
        glPushMatrix();

        glTranslatef(trem.posicao.x-0.625, trem.posicao.y, trem.posicao.z);
        //Escala e Desenha modelo 3D
        glScalef(2.5,2.5,37);
        DesenharSTL(modelo);

        if(objetoColidiu(esfera,trem))
        {
            //printf("Colidiu");
            quantidadeVida = quantidadeVida - 1;
            printf("%f",quantidadeVida);
        }
        else
        {
            //printf("Nao Colidiu");
        }

        glPopMatrix();
    }

}


void AtualizarPosicao() //Atualiza e desenha meu inimigo
{
    for ( int i = 0; i < quantidadeInimigo; i++ ) // Varre todo o vetor de quantidade de inimigos
    {
        if(vetInimigo[i].isVivo == true) //Verifica se no vetor inimigos tem espaço livre
        {
            //vetInimigo[i];
            vetInimigo[i].posicao.z += 0.1f; //Se tá vivo ele muda a posicao e vem até o personagem
            // Ajusta velocidade do trem

            if(vetInimigo[i].posicao.z > 2)
            {
                vetInimigo[i].isVivo = false; //o trem tá morto
            }
            else
            {
                //vetInimigo[i].posicao.z = vetInimigo[rand()%3]; //Achou morto
                //printf("%f \n", vetInimigo[i].posicao.z);//desce a posição em z e desenha inimigo
                desenhaTrem(vetInimigo[i]);
            }

        }
    }
}

void idle()
{
    RunPhysics(0.01);
    display();
}

static void init(void)
{
    //Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat qaSpecularLight[]	= {0.8, 0.8, 0.8, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = { 0, 0, 0, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void greenGround()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, chaoTex);
    glTranslatef(0.0,-0.01,0.0);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-6.25f, 0.0f, 2.0f);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-6.25f, 0.0f, -100.0f);
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(6.25f, 0.0f, -100.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(6.25f, 0.0f, 2.0f);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void desenhaParede()
{
    glPushMatrix();
    //Desenha Linha central
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tunelTex);
    glBegin(GL_QUADS); //Desenha no sentido horario
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(6.25f,0.0f,0.0f); //Vertice inferior esquerdo
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(6.25f,2.0f,-100.0f); //Vertice superior esquerdo
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(8.75f,2.0f,-100.0f); //Vertice superior direito
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(8.75f,0.0f,0.0f); //Vertice inferior direito
    }
    glEnd();

    //Desenha Linha da direita
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-6.25f,0.0f,0.0f);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-6.25f,2.0f,-100.0f);
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(-8.75f,2.0f,-100.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-8.75f,0.0f,0.0f);
    }
    glEnd();

    //Desenha Linha da esquerda
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-3.75f,0.0f,0.0f);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-3.75f,0.0f,-100.0f);
        glTexCoord2f(1.0f, 20.0f);
        glVertex3f(-1.25f,0.0f,-100.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.25f,0.0f,0.0f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


}

void desenhaTunel()
{
    //glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tunelTex);
    //Desenha Tunel
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-6.25f,0.0f,-100.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-6.25f,10.0f,-100.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(6.25f,10.0f,-100.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(6.25f,0.0f,-100.0f);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
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


    glClearColor(0.8, 0.8, 0.8, 0.0); /* fog color */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // Turn lights off/on.
    // Fog controls.
    if (fogOn)
        glEnable(GL_FOG);
    else
        glDisable(GL_FOG);
    {
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 17.0);
        glFogf(GL_FOG_END, 32.0);
        glHint(GL_FOG_HINT, GL_NICEST);
    }

    glLoadIdentity();

    //Define a posição da camera
    glTranslatef(0.0f,-2.0f,-7.0f);
    glRotatef(20.0f,1.0f,0.0f,0.0f);

    greenGround(); //Nao é no displau

    desenhaEsfera();

    //Desenha estrada
    desenhaEstrada();

    desenhaTunel();

    //desenhaParede();

    // Desenha esfera
    //if(checaColisaoBox() == true)
    //{

    //}

    AtualizarPosicao(); //Nao é no displau

    /* Escreve texto isolado (antes gluLookAt) .
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    if (isCollision)

    glPopMatrix();*/

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

    //glColor3f(0, 1, 0);
    //drawBitmapText("Vida:", 0, 0, 0);
    //drawBitmapText("Tempo:", 0, 2, 0);

    /* calculate the delta time between frames
     int time = glutGet(GLUT_ELAPSED_TIME);
     delta = ((GLfloat) time - game.time) / 1000.0;
     game.time = time;*/

    glutSwapBuffers();

}

// Janela OpenGL reformula a rotina.
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLdouble)width/(GLdouble)height,1,400);
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
        isJumping = true;
        esfera.velocidade.y +=10; // Permite pular na direcao y // Permite pular na direcao y
        //sphere.posicao.y += 3.65f;
        //printf("%f", sphere.posicao.y);
        break;
    case 'a':
        if(isJumping == false){
        // Trava a esfera em x para não ultrapassar o limite da faixa da esquerda
        if(esfera.posicao.x<=-2.5)
        {
        }
        else
        {
            esfera.posicao.x -= 2.5; // Permite andar para faixa da esquerda na direcao x
        }
        }
        break;
    case 'd':
        if(isJumping == false){
        // Trava a esfera em x para não ultrapassar o limite da faixa da direita
        if(esfera.posicao.x>=2.5)
        {
        }
        else
        {
            esfera.posicao.x += 2.5; // Permite andar para faixa da direita na direcao x
        }
        }
        break;
    case 'r': //Reseta o jogo
        //Reinicia a posicao da esfera
        esfera.posicao.x=0;
        esfera.posicao.y=0;
        break;
    case 'f': //ativa e desliga o fog
        if (fogOn)
        {
            fogOn = 0;
        }
        else
        {
            fogOn = 1;
        }
        glutPostRedisplay();
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
         << "Pressione I para iniciar o jogo." << endl
         << "Pressione F para ativar ou desativar o FOG." << endl
         << "Pressione Q para sair." << endl;
}

void loadAllTextures()
{
    trilhoTex = LoadTexture("rails.bmp");
    chaoTex = LoadTexture("grass.bmp");
    ceuTex = LoadTexture("sky.bmp");
    tunelTex = LoadTexture("tunel.bmp");
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
    init();

    //Lê arquivo .STL
    LerArquivo(&modelo, "cubo.stl");
    //printf("Nome: %s\n", modelo.nome);
    //printf("Numero de faces: %i\n", modelo.nFaces);
    //LerArquivo(&modelo2, "sphere.stl");

    glutReshapeFunc(resize);
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    //glutFullScreen();             // Put into full screen
    loadAllTextures();
    glutMainLoop();

    return EXIT_SUCCESS;
}

