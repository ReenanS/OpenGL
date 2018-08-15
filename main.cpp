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
static int fogOn = 1; // Fog on?
//
//char *TEXT_ID_ARRAY[] = {"cubo.stl","sphere.stl"};

STL modelo;
//STL modelo2;

float tempo = 6;

static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?

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
void drawBitmapText(char *string, float x, float y, float z);

#define GRAVITY 9.8

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

        if (sphere.posicao.y <0)
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

void greenGround()
{
    //GLuint texture2 = LoadTexture("download.bmp");
    //glBindTexture(GL_TEXTURE_2D, texture2);
    //glColor3f(0.0,1.0,0.0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(0.0,-0.1,0.0);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f); /*glColor3f(0, 1, 0)*/;
        glVertex3f(-5.0f, 0.0f, 5.0f);
        glTexCoord2f(5.0f, 0.0f); /*glColor3f(0, 1, 0)*/;
        glVertex3f(+5.0f, 0.0f, 5.0f);
        glTexCoord2f(5.0f, 50.0f);/*glColor3f(0, 1, 0)*/;
        glVertex3f(+5.0f, 0.0f, -195.0f);
        glTexCoord2f(0.0f, 50.0f);/*glColor3f(0, 1, 0)*/;
        glVertex3f(-5.0f, 0.0f, -195.0f);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void ceuAzul()
{
    GLuint texture3 = LoadTexture("sky.bmp");
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //glColor3f(0.0,1.0,0.0);
    glPushMatrix();
    glTranslatef(0.0,-140.0,-300.0);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glColor3f(0, 0, 1);
        glVertex2f(-280.0f, -280.0f);
        glTexCoord2f(5.0f, 0.0f);
        glColor3f(0, 0, 1);
        glVertex2f(+280.0f, -280.0f);
        glTexCoord2f(5.0f, 50.0f);
        glColor3f(0, 0, 1);
        glVertex2f(280.0f, 280.0f);
        glTexCoord2f(0.0f, 50.0f);
        glColor3f(0, 0, 1);
        glVertex2f(-280.0f, 280.0f);
    }
    glEnd();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPopMatrix();

}

void trilho()
{

    GLuint texture4 = LoadTexture("train2.bmp");
    glBindTexture(GL_TEXTURE_2D, texture4);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);
        glColor3f(0, 0, 1);
        glVertex3f(-0.7f, 0.0f, 10.0f);
        glTexCoord2f(1.0f, 0.0f);
        glColor3f(0, 0, 1);
        glVertex3f(0.7f, 0.0f, 10.0f);
        glTexCoord2f(1.0f, 50.0f);
        glColor3f(0, 0, 1);
        glVertex3f(0.7f, 0.0f, -190.0f);
        glTexCoord2f(0.0f, 50.0f);
        glColor3f(0, 0, 1);
        glVertex3f(-0.7f, 0.0f, -190.0f);
    }
    glEnd();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPopMatrix();

}

void desenhaEsfera()
{
    glPushMatrix();

    glTranslated(sphere.posicao.x,sphere.posicao.y,0);
    //glTranslatef(0,0,0);
    glutSolidSphere(raio,20,20);
    //glScalef(1.2,1.2,5);
    //DesenharSTL(modelo2);

    glPopMatrix();
}

//Desenha X Trem aleatoriamente
void desenhaTrem(struct TObject3DInimigo inimigo)
{
    float vet[]= {-2.5, 0.0, 2.5}; //Vetor de posicao (faixa esquerda, central, direita) para desenhar o trem

    for (int i = 0; i < 2; i++) // Varre todo o vetor de posicao do trem
    {
        glPushMatrix();

        glTranslatef(inimigo.posicao.x, inimigo.posicao.y, inimigo.posicao.z);
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
            vetInimigo[i].posicao.z += 0.1f;

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

/*
bool esfera_interseccao_trem(struct caixaColisao colBox1, struct caixaColisao colBox2)
{
    int f;
    //Para cada face da caixa / trem:
    for (f=0; f<modelo.nFaces; f++)
    {
        if((colBox1.pos[0] < colBox2.pos[0] + colBox2.largura) && (colBox1.pos[0] +  colBox1.largura > colBox2.pos[0]) &&
                (colBox1.pos[1] < colBox2.pos[1] + colBox2.altura) && (colBox1.pos[1] + colBox1.altura > colBox2.pos[1]))      //Teste de colisao esfera contra face.
        {
            printf("Colidiu");     //If collision occurs, within the bounds of the side, report that collision.
            return true;
        }
        else
        {
            printf("Nao colidiu");     //Else, report no collision.
            return false;
        }
    }
}

*/

/* Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, y, 0) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
bool checaColidiuComTrem()
{
    /*
        A funcao checaColidiuComTrem analisa se o trem estava vivo ou morto

    // Check for collision with each asteroid.
    for ( int i = 0; i < quantidadeInimigo; i++ ){ // Varre todo o vetor de quantidade de inimigos
        if (vetInimigo[i].isVivo == true ){ // If asteroid exists.
            printf("Era inimigo vivo");
            return true;
        }
    }
    printf("Nao era inimigo vivo");
    return false;
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.

bool checaColisaoBox(struct caixaColisao colBox1, struct caixaColisao colBox2){
    /*
        A funcao checaColisaoBox verifica se houve uma intersecção entre o trem a esfera,
        ou seja, dois objetos colidiram

    // Se houve colisao
    if((colBox1.pos[0] < colBox2.pos[0] + colBox2.largura) && (colBox1.pos[0] +  colBox1.largura > colBox2.pos[0]) &&
       (colBox1.pos[1] < colBox2.pos[1] + colBox2.altura) && (colBox1.pos[1] + colBox1.altura > colBox2.pos[1])){
       return true;
    }
    // Se nao houve colisao
    else{
        return false;
    }
}
*/

/*For each face we check if there is an intersection, and if there is an intersection we ensure it is within the region of the face.
//To do this we ensure it is inside (or at least intersecting) all the surrounding face planes.
bool sphere_intersects_box(sphere s, box b)
{

    bool in_left   = !sphere_outside_plane(s, b.left);
    bool in_right  = !sphere_outside_plane(s, b.right);
    bool in_front  = !sphere_outside_plane(s, b.front);
    bool in_back   = !sphere_outside_plane(s, b.back);
    bool in_top    = !sphere_outside_plane(s, b.top);
    bool in_bottom = !sphere_outside_plane(s, b.bottom);

    if (sphere_intersects_plane(s, b.top) &&
            in_left && in_right && in_front && in_back)
    {
        return true;
    }

    if (sphere_intersects_plane(s, b.bottom) &&
            in_left && in_right && in_front && in_back)
    {
        return true;
    }

    if (sphere_intersects_plane(s, b.left) &&
            in_top && in_bottom && in_front && in_back)
    {
        return true;
    }

    if (sphere_intersects_plane(s, b.right) &&
            in_top && in_bottom && in_front && in_back)
    {
        return true;
    }

    if (sphere_intersects_plane(s, b.front) &&
            in_top && in_bottom && in_left && in_right)
    {
        return true;
    }

    if (sphere_intersects_plane(s, b.back) &&
            in_top && in_bottom && in_left && in_right)
    {
        return true;
    }

    return false;
}

//checking if a box is outside is as simple as ensuring it is not inside or intersecting.
bool sphere_outside_box(sphere s, box b)
{
    return !(sphere_intersects_box(s, b));
}*/

/*void Fog() //Ajustar onde começa e onde termina
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
    //glFogi(GL_FOG_MODE, GL_EXP2);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    //glFogf(GL_FOG_DENSITY, density);
    glFogf(GL_FOG_START, 1);
    glFogf(GL_FOG_END, 100);
    glHint(GL_FOG_HINT, GL_NICEST);
}*/

static void init(void)
{
    /*GLfloat position[] = { 0.5, 0.5, 3.0, 0.0 };
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat mat[3] = {0.1745, 0.01175, 0.01175};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
        mat[0] = 0.61424;
        mat[1] = 0.04136;
        mat[2] = 0.04136;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
        mat[0] = 0.727811;
        mat[1] = 0.626959;
        mat[2] = 0.626959;
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
        glMaterialf(GL_FRONT, GL_SHININESS, 0.6*128.0);
    }*/
   // Turn lights off/on.
    glEnable(GL_FOG);
    {
        GLfloat fogColor[3] = {0.8f, 0.8f, 0.8f}; // Define a nice light grey

        glEnable(GL_DEPTH_TEST);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fogColor);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 17.0);
        glFogf(GL_FOG_END, 32.0);
    }
        glClearColor(0.8, 0.8, 0.8, 0.0); /* fog color */
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

    glClearColor(0.8, 0.8, 0.8, 0.0); //esquema de cores do fog
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //Define a posição da camera
    glTranslatef(0.0,-2.0,-7.0);
    glRotatef(20,1.0,0.0,0.0);

    //greenGround();
    //ceuAzul();
    //trilho();

    //Desenha estrada
    desenhaEstrada();

    // Desenha esfera
    desenhaEsfera();

    AtualizarPosicao();

    /* Escreve texto isolado (antes gluLookAt) .
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    if (isCollision)
        drawBitmapText("Nao pode - vai colidir!", -28.0, 25.0, -30.0);
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
    case 'f': //ativa e desliga o fog
		 if (fogOn)
         {
            fogOn = 0;
         } else
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
         << "Pressione F para ativar ou desligar o FOG." << endl
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
    init();

    //Lê arquivo .STL
    LerArquivo(&modelo, "cubo.stl");
    //printf("Nome: %s\n", modelo.nome);
    //printf("Numero de faces: %i\n", modelo.nFaces);
    //LerArquivo(&modelo2, "sphere.stl");

    //Lê textura
    //GLuint texture2 = LoadTexture("download.bmp");
    //glBindTexture(GL_TEXTURE_2D, texture2); //nao precisa

    glutReshapeFunc(resize);
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    //glutFullScreen();             // Put into full screen
    glutMainLoop();

    return EXIT_SUCCESS;
}

