/*
 * GLUT Subway Surfers
 *
 * Desenvolvido por Renan Souza Silva em Agosto 2018
 *
 * Esse programa é baseada no jogo Subway Surfers. Com perspectiva 3D, o jogo importa arquivos
 * 3D STL e carrega o modelo para criacao dos inimigos, cria texturas BMP, FOG, e a mecânica do pulo segue os principios da fisica.
 *
 * Controles:
 * W - Personagem pula
 * A e D - Movimenta para esquerda e direita
 * R - Reinicia o jogo
 * F - Ativa/Desativa o FOG
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

//Definicao da gravidade
#define gravidade 9.81

//Variaveis Globais

//Define a quantidade de inimigos do jogo
const int quantidadeInimigo = 10;

//Define os pontos de vida do personagem
int quantidadeVida = 3;

//Define o raio da esfera
float raio = 0.625f;

//Variaveis do FOG
bool fogLigado = false; //O Fog está ligado? Não!
GLfloat corFog[3] = {0.8f, 0.8f, 0.8f}; //Define uma luz cinza para o FOG

//Variavel do Modelo 3D STL
STL modelo;

//Define o tempo para aparecer os inimigos
float tempo = 6.0f;

bool estaPulando = false; //O personagem está pulando? Nao!

//Variaveis da Textura
GLuint trilhoTex;
GLuint chaoTex;
GLuint ceuTex;
GLuint tunelTex;
GLuint muroTex;

//Estruturas

//Criando uma (estrutura) objeto - esfera
struct Coordenada
{
    float x,y,z;
};

struct Objeto3D
{
    Coordenada posicao; //Posicao da esfera
    Coordenada velocidade; //Velocidade da esfera
};

Objeto3D esfera;

//Criando uma (estrutura) objeto - inimigo (trem)
struct Objeto3DInimigo
{
    Coordenada posicao;
    bool estaVivo;
};

Objeto3DInimigo trem;

Objeto3DInimigo vetInimigo[quantidadeInimigo]; //Vetor com a quantidade de inimigos do jogo

void display();
void desenhaStringNaTela(char *string, float x, float y, float z);

// ******************************************************************************************************
// Inicio das funções que desenham
// ******************************************************************************************************

//Essa funcao desenha o objeto esfera
void desenhaEsfera()
{
    glPushMatrix(); //Salva as transformações atuais na pilha interna do OpenGL
        glColor3f(0.0f,0.0f,1.0f); //Define uma cor para esfera
        glTranslated(esfera.posicao.x,esfera.posicao.y+raio,0); //Desenha a esfera descolado
        glutSolidSphere(raio,30,30); //Cria uma esfera solida com uma raio determinado e a quantidade de "slices" e "stacks"
    glPopMatrix(); //Restaura as transformações anteriores
}

//Essa funcao desenha a estrada (3 linhas paralelas - esquerda, central, direita) e aplica textura em seus vertices
void desenhaEstrada()
{
    glPushMatrix(); //Salva as transformações atuais na pilha interna do OpenGL
        glEnable(GL_TEXTURE_2D); //Ativa a textura
        glBindTexture(GL_TEXTURE_2D, trilhoTex); //"Liga" a textura com a imagem que ela deve ser mapeada
        //Desenha Linha central
        glBegin(GL_QUADS); //Desenha os verices no sentido horario
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.25f,0.0f,2.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 20.0f); glVertex3f(-1.25f,0.0f,-100.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 20.0f); glVertex3f(1.25f,0.0f,-100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f);  glVertex3f(1.25f,0.0f,2.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();

        //Desenha Linha da direita
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.25f,0.0f,2.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 20.0f); glVertex3f(1.25f,0.0f,-100.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 20.0f); glVertex3f(3.75f,0.0f,-100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.75f,0.0f,2.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();

        //Desenha Linha da esquerda
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.75f,0.0f,2.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 20.0f); glVertex3f(-3.75f,0.0f,-100.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 20.0f); glVertex3f(-1.25f,0.0f,-100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.25f,0.0f,2.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();

        glDisable(GL_TEXTURE_2D); //Desativa a textura
    glPopMatrix(); //Restaura as transformações anteriores
}

//Essa funcao desenha trem a partir do modelo STL 3D
void desenhaTrem(struct Objeto3DInimigo trem)
{
    glPushMatrix(); //Salva as transformações atuais na pilha interna do OpenGL
        glTranslatef(trem.posicao.x-0.625, trem.posicao.y, trem.posicao.z); //Desenha o trem descolado (Para ficar junto com o trilho)
        glScalef(2.5,2.5,37); //Aumenta a escala do objeto (x,y,z) em relação ao original
        DesenharSTL(modelo); //Desenha modelo 3D
    glPopMatrix(); //Restaura as transformações anteriores
}

//Essa funcao desenha o chao (como se fosse um plano)
void desenhaChao()
{
    glPushMatrix(); //Salva as transformações atuais na pilha interna do OpenGL
        glEnable(GL_TEXTURE_2D); //Ativa a textura
        glBindTexture(GL_TEXTURE_2D, chaoTex); //"Liga" a textura com a imagem que ela deve ser mapeada
        glTranslatef(0.0,-0.01,0.0); //Desenha o chao descolado (define o y negativo para ficar levemente abaixo das estradas)
        //Desenha um plano (chao)
        glBegin(GL_QUADS); //Desenha os verices no sentido horario
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.25f, 0.0f, 2.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 20.0f); glVertex3f(-6.25f, 0.0f, -100.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 20.0f); glVertex3f(6.25f, 0.0f, -100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f); glVertex3f(6.25f, 0.0f, 2.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();

        glDisable(GL_TEXTURE_2D); //Desativa a textura
    glPopMatrix(); //Restaura as transformações anteriores
}

//Essa funcao desenha o muro
void desenhaMuro()
{
    glPushMatrix(); //Salva as transformações atuais na pilha interna do OpenGL
        glEnable(GL_TEXTURE_2D); //Ativa a textura
        glBindTexture(GL_TEXTURE_2D, muroTex); //"Liga" a textura com a imagem que ela deve ser mapeada
        //Desenha um muro (parede)
        glBegin(GL_QUADS); //Desenha os verices no sentido horario
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.05f,0.0f,0.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.05f,4.0f,0.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.05f,4.0f,-100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.05f,0.0f,-100.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();

        glBegin(GL_QUADS); //Desenha os verices no sentido horario
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(6.05f,0.0f,0.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 1.0f); glVertex3f(6.05f,4.0f,0.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 1.0f); glVertex3f(6.05f,4.0f,-100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f); glVertex3f(6.05f,0.0f,-100.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();

        glDisable(GL_TEXTURE_2D); //Desativa a textura
    glPopMatrix(); //Restaura as transformações anteriores
}

//Essa funcao desenha o tunel
void desenhaTunel()
{
    glPushMatrix(); //Salva as transformações atuais na pilha interna do OpenGL
        glEnable(GL_TEXTURE_2D); //Ativa a textura
        glBindTexture(GL_TEXTURE_2D, tunelTex); //"Liga" a textura com a imagem que ela deve ser mapeada
        //Desenha Tunel
        glBegin(GL_QUADS); //Desenha os verices no sentido horario
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.25f,0.0f,-100.0f); //Aplica textura no Vertice inferior esquerdo
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.25f,10.0f,-100.0f); //Aplica textura no Vertice superior esquerdo
            glTexCoord2f(1.0f, 1.0f); glVertex3f(6.25f,10.0f,-100.0f); //Aplica textura no Vertice superior direito
            glTexCoord2f(1.0f, 0.0f); glVertex3f(6.25f,0.0f,-100.0f); //Aplica textura no Vertice inferior direito
        }
        glEnd();
        glDisable(GL_TEXTURE_2D); //Desativa a textura
    glPopMatrix(); //Restaura as transformações anteriores
}

//Essa funcao escreve um string na tela
void desenhaStringNaTela(char *string, float x, float y, float z)
{
    char *c;
    glRasterPos3f(x, y, z);//Define a posicao da tela onde o texto vai ser escrito

    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); //Define o tipo de fonte
    }
}

// ******************************************************************************************************
// Inicio da mecanica de colisão
// ******************************************************************************************************

//Essa funcao compara a colisao do personagem (esfera) com os inimigos (trem - cubo) - Colisao AABB
bool objetoColidiu(struct Objeto3D esfera, struct Objeto3DInimigo trem)
{
    //Verifica se houve colisao para os eixos X e Z - Metodo AABB
    if((esfera.posicao.x < trem.posicao.x + 2.5) && (esfera.posicao.x +  raio > trem.posicao.x) &&
            (esfera.posicao.z < trem.posicao.z + 2.5) && (esfera.posicao.z + raio > trem.posicao.z))
    {
        return true; //Retorna verdadeiro se houve colisao
    }
    else
    {
        return false; //Retorna falso se não houve colisao
    }
}

// ******************************************************************************************************
// Inicio das funções que definem a logica de programação do jogo
// ******************************************************************************************************

//Essa funcao cria um inimigo (trem) no vetor e define as posições do inimigos (trem)
void DefinePosicaoInimigos()
{
    for (int i = 0; i < quantidadeInimigo; i++) //Laço de repeticao para varrer de 0 até o valor quantidadeInimigo
    {
        if(vetInimigo[i].estaVivo == true) //Verifica se no vetor inimigos tem espaço livre
        {
            //Se verdadeiro, não faz nada
        }
        else
        {
            float vet[]= {-2.5, 0.0, 2.5}; //Vetor com as posicoes da estrada (esquerda, central, direita)
            vetInimigo[i].posicao.x = vet[rand()%3]; //Randomiza a posicao do inimigo no eixo x
            vetInimigo[i].posicao.y = 0; //Define posicao do inimigo em y = 0
            vetInimigo[i].posicao.z = -100; //Define a posicao do inimigo em z = -100 (impressao de que os objetos vem do infinito)
            vetInimigo[i].estaVivo = true; //Define que no vetor inimigo tem espaço livre
            return;
        }
    }
}

//Essa funcao atualiza posicao do inimigo (trem), verifica se colidiu e desenha o trem
void AtualizarPosicao()
{
    for (int i = 0; i < quantidadeInimigo; i++) //Laço de repeticao para varrer de 0 até o valor quantidadeInimigo
    {
        if(vetInimigo[i].estaVivo == true) //Verifica se no vetor inimigos tem espaço livre
        {
            vetInimigo[i].posicao.z += 0.1f; //Se tá vivo ele muda a posicao e vem até o personagem - Ajusta velocidade do trem
            if(objetoColidiu(esfera,vetInimigo[i])) //Se objeto colidiu com inimigo
            {
                    vetInimigo[i].estaVivo = false; //Define que o trem está morto
                    if(quantidadeVida>0) //Nao deixa a vida negativa
                    {
                        quantidadeVida = quantidadeVida - 1; //Subtrai um da quantidade de vida do personagem
                    }
            }
            if(vetInimigo[i].posicao.z > 2) //Se o trem passou de uma certa posicao de z, o trem "morre"
            {
                vetInimigo[i].estaVivo = false; //Define que o trem está morto
            }
            else
            {
                if(quantidadeVida>0) //Desenha objeto somente se o personagem tiver vida
                {
                desenhaTrem(vetInimigo[i]); //Desenha objeto inimigo - trem
                }
            }
        }
    }
}

// ******************************************************************************************************
// Inicio da física do jogo
// ******************************************************************************************************

//Essa função define toda a mecanica de física do jogo
void Fisica(float dt)
{
    esfera.velocidade.y = esfera.velocidade.y - gravidade*dt; //Função horario da velocidade
    esfera.posicao.y = esfera.posicao.y + esfera.velocidade.y*dt - 0.5*gravidade*dt*dt; //Função horario do espaço

    if (esfera.velocidade.y < 0) //Se a velocidade da esfera for negativa (objeto caindo)
    {
        if(estaPulando == true) //Se o personagem está pulando
        {
                esfera.velocidade.y = esfera.velocidade.y - gravidade*dt; //A gravidade puxa ele para baixo
        }
        if (esfera.posicao.y < 0) //Se a posicao vertical é menor que zero, assume que o personagem está no chao.
        {
            esfera.velocidade.y = 0; //Define a velocidade para 0
            esfera.posicao.y = 0; //Define a posicao para 0
            estaPulando = false; //Nao está pulando
        }
    }
}

// ******************************************************************************************************
// Inicio do carregamento da textura
// ******************************************************************************************************

//Funcao para definir e carregar todas as texturas utilizadas durante o jogo
void carregarTodasTexturas()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    trilhoTex = LoadTexture("trilho.bmp");
    chaoTex = LoadTexture("grama.bmp");
    tunelTex = LoadTexture("tunel.bmp");
    muroTex = LoadTexture("muro.bmp");
}

//Rotina para gerar instruções de interação na janela do C ++
void InteracaoPrint(void)
{
    cout << "Interacao:" << endl;
    cout << "Pressione as teclas de A, W, S, D para mover a esfera." << endl
         << "Pressione R para reiniciar o jogo." << endl
         << "Pressione F para ativar ou desativar o FOG." << endl
         << "Pressione Q para sair." << endl
         << "Por favor, nao ativar o Caps Lock." << endl;
}

// ******************************************************************************************************
// Inicio das rotinas do OpenGL GLUT
// ******************************************************************************************************

void idle()
{
    Fisica(0.01);
    display();
}

//Função que inicia a renderização 3D
static void init(void)
{
    GLfloat luzAmbiente[4] = {0.8f, 0.8f, 0.8f, 1.0f }; //Define a luz ambiente
    GLfloat luzDifusa[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Define a luz difusa
    GLfloat luzEspecular[4]	= {1.0f, 1.0f, 1.0f, 1.0f }; //Define a luz especular
    GLfloat posicaoLuz[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //Define a posicao da luz

    //Define parametros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    glEnable(GL_LIGHTING); //Ativa a iluminação
    glEnable(GL_LIGHT0); //Ativa a luz#0

    glEnable(GL_DEPTH_TEST); //Habilita o depth-buffering

    glEnable(GL_COLOR_MATERIAL); //Habilita a definição da cor do material a partir da cor corrente
}

//Rotina para exibição na tela
void display(void)
{
    //Laço de repeticao para simular o tempo
    if(tempo > 0.2f)
    {
        tempo = tempo - (tempo/500.0f);
    }
    else
    {
        tempo = 60.0f;
        DefinePosicaoInimigos();
    }

    glClearColor(0.8, 0.8, 0.8, 0.0); // Especifica que a cor de fundo da janela será cinza (Igual do FOG)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa a janela e o depth buffer

    //Controle do FOG - Ativa e Desativa
    if (fogLigado)
        glEnable(GL_FOG); //Ativa o FOG
    else
        glDisable(GL_FOG); //Desativa o FOG
    {
        glFogfv(GL_FOG_COLOR, corFog); //Define a cor do FOG
        glFogi(GL_FOG_MODE, GL_LINEAR); //Define o modo do FOG (Linear, EXP, EXP2)
        glFogf(GL_FOG_START, 17.0); //Define quao perto da tela o FOG deve começar
        glFogf(GL_FOG_END, 32.0); //Define quao longe da tela o FOG deve terminar
        glHint(GL_FOG_HINT, GL_NICEST);
    }

    glLoadIdentity(); //Reinicializa as transformações

    //Define a posição da camera
    glTranslatef(0.0f,-2.0f,-7.0f); //Descola a cena -2 unidades em X e -7 em Y
    glRotatef(20.0f,1.0f,0.0f,0.0f); //Gira a cena 20 graus ao redor do eixo X

    desenhaEsfera(); //Desenha Esfera

    desenhaEstrada(); //Desenha Estrada
    desenhaChao(); //Desenha Chao
    desenhaTunel(); //Desenha Tunel
    desenhaMuro(); //Desenha Muro
    AtualizarPosicao(); //Atualiza Posicao do Trem

    //Caso o personagem morra após as colisoes
    if(quantidadeVida == 0)
    {
        glColor3f(0, 0, 0);
        desenhaStringNaTela("Voce morreu! Aperte R para reiniciar", 2 , 3.5, -2); //Escreve na tela que morreu
    }

    glColor3f(0, 0, 0);
    char texto[5];
    sprintf(texto,"Vida: %i",quantidadeVida);
    desenhaStringNaTela(texto, 2,4, -2); //Escreve na tela a quantidade de vida

    glFlush(); //Esvazia todos os buffers
    glutSwapBuffers(); //Troca os buffers da janela atual se estiver em buffer duplo
}

//Rotina da janela de desenho do OpenGL
static void resize(int width, int height)
{
    glViewport(0, 0, width, height); //Define a área que se pode utilizar para efetuar desenhos
    glMatrixMode(GL_PROJECTION); //Define a pilha de matrizes de projeção
    glLoadIdentity(); //Reinicializa as transformações
    gluPerspective(45, (GLdouble)width/(GLdouble)height,1,400); //Define a transformação de perspectiva
    glMatrixMode(GL_MODELVIEW); //Define a pilha de matrizes de modelview
}

//Rotina de processamento da entrada do teclado
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0); //Encerra o jogo
        break;
    case 'w':
        estaPulando = true; //Define que o personagem está pulando
        esfera.velocidade.y +=10; //Permite pular ("impulso") na direcao y
        break;
    case 'a':
        if(estaPulando == false) //Define que o personagem está no chao
        {
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
        if(estaPulando == false) //Define que o personagem está no chao
        {
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
            esfera.posicao.x = 0;
            esfera.posicao.y = 0;
            //Reinicia a quantidade de vida
            quantidadeVida = 3;
        break;
    case 'f': //Ativa e Desliga o FOG atraves de um flag
        if (fogLigado)
        {
            fogLigado = false; //FOG Desligado
        }
        else
        {
            fogLigado = true; //FOG Ligado
        }
        break;
    }
    glutPostRedisplay();
}

//Rotina principal.
int main(int argc, char **argv)
{
    srand(time(NULL)); //Randomiza o rand()
    InteracaoPrint(); //Mostra os comandos do jogo na janela de instruções

    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Subway Surfers");

    init();
    LerArquivo(&modelo, "cubo.stl"); //Lê arquivo .STL

    glutReshapeFunc(resize);
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutFullScreen(); //Coloca a janela no modo full screen
    carregarTodasTexturas(); //Carrega todas as texturas
    glutMainLoop();

    return EXIT_SUCCESS;
}
