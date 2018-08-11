#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include "ImportarSTL.h"

void LerArquivo(STL *modelo, char *arquivo)
{
    FILE *arq;
    arq = fopen(arquivo, "r");
    char linha[255];
    char comando[255];
    char itens[255][255]; //?? vetor de strs
    int item, face=0, ponto=0;
    Ponto normal = {0.0, 0.0, 0.0};

    if(arq==NULL)
    {
        printf("Não consegui abrir o arquivo.");
        exit(EXIT_FAILURE);
    }

    (*modelo).faces = (Face*) malloc(sizeof(Face));

    // Lê o arquivo todo
    while (!feof(arq))
    {
        // faz a leitura dos itens de cada linha

        // faz a leitura de uma linha
        fgets(linha,255,arq);
        if(linha[strlen(linha)-1]=='\n')
            linha[strlen(linha)-1]='\0';

        char linha1[255];
        sprintf(linha1, "");

        // para eliminar espaços do início da linha
        int i = 0;
        while (linha[i] == ' ')
            i++;
        // para eliminar 2 espaços seguidos
        while (i < strlen(linha)){
            if ( !( (linha[i] == ' ') && (linha[i+1] == ' ') ) )
                sprintf(linha1, "%s%c", linha1,linha[i]);
            i++;
        }
        sprintf(linha, "%s%c", linha1, '\0');

        // separa os itens da linha
        item = 0;
        i = 0;
        while (i < strlen(linha))
        {
            sprintf(comando, "%s", "");
            while ((linha[i] != ' ') && (i<strlen(linha)))
            {
                sprintf(comando, "%s%c", comando, linha[i]);
                i++;
            }
            // acrescenta na lista de itens da linha
            sprintf(itens[item++], "%s", comando);
            i++;
        }


        // preenche os campos do STL
        if (strcmpi(itens[0], "solid") == 0){
            strcpy(modelo->nome, itens[1]);
        }

        if (strcmpi(itens[0], "facet") == 0)
        {
            // itens[1] = "normal"
            normal.x = atof(itens[2]);
            normal.y = atof(itens[3]);
            normal.z = atof(itens[4]);
        }

        if (strcmpi(itens[0], "outer") == 0)
            // itens[1] = "loop"
            ponto = 0;

        if (strcmpi(itens[0], "vertex") == 0){
            modelo->faces[face].normal = normal;
            modelo->faces[face].ponto[ponto].x = atof(itens[1]);
            modelo->faces[face].ponto[ponto].y = atof(itens[2]);
            modelo->faces[face].ponto[ponto].z = atof(itens[3]);

            // calc máx e min
            if ((face == 0) && (ponto == 0)){  // primeiro ponto
                modelo->minimo.x = modelo->faces[face].ponto[ponto].x;
                modelo->minimo.y = modelo->faces[face].ponto[ponto].y;
                modelo->minimo.z = modelo->faces[face].ponto[ponto].z;
                modelo->maximo.x = modelo->faces[face].ponto[ponto].x;
                modelo->maximo.y = modelo->faces[face].ponto[ponto].y;
                modelo->maximo.z = modelo->faces[face].ponto[ponto].z;
            }

            if (modelo->faces[face].ponto[ponto].x < modelo->minimo.x)
                modelo->minimo.x = modelo->faces[face].ponto[ponto].x;
            if (modelo->faces[face].ponto[ponto].y < modelo->minimo.y)
                modelo->minimo.y = modelo->faces[face].ponto[ponto].y;
            if (modelo->faces[face].ponto[ponto].z < modelo->minimo.z)
                modelo->minimo.z = modelo->faces[face].ponto[ponto].z;
            if (modelo->faces[face].ponto[ponto].x > modelo->maximo.x)
                modelo->maximo.x = modelo->faces[face].ponto[ponto].x;
            if (modelo->faces[face].ponto[ponto].y > modelo->maximo.y)
                modelo->maximo.y = modelo->faces[face].ponto[ponto].y;
            if (modelo->faces[face].ponto[ponto].z > modelo->maximo.z)
                modelo->maximo.z = modelo->faces[face].ponto[ponto].z;

            ponto++;
        }


        //if (strcmpi(itens[0], "endloop") == 0) {}

        if (strcmpi(itens[0], "endfacet") == 0){
            modelo->nFaces = ++face;
            modelo->faces = (Face*) realloc(modelo->faces, (face+1)*sizeof(Face));
        }

        //if (strcmpi(itens[0], "endsolid") == 0) {}
    }

    modelo->centro.x = (modelo->maximo.x + modelo->minimo.x) / 2.0;
    modelo->centro.y = (modelo->maximo.y + modelo->minimo.y) / 2.0;
    modelo->centro.z = (modelo->maximo.z + modelo->minimo.z) / 2.0;
    modelo->tamanho = sqrt(
                           pow(modelo->centro.x - modelo->maximo.x, 2.0)+
                           pow(modelo->centro.y - modelo->maximo.y, 2.0)+
                           pow(modelo->centro.z - modelo->maximo.z, 2.0)
                           );
    fclose(arq);
}

void DesenharSTL(STL modelo){
    int f;
    for (f=0; f<modelo.nFaces; f++){
// ******************************************************************************************************
// converte os valores de [-1.0; +1.0] do vetor normal para o intervalo [0.0; 1.0] das cores
// ******************************************************************************************************
        glColor3f(
                  (modelo.faces[f].normal.x + 1) / 2.0,
                  (modelo.faces[f].normal.y + 1) / 2.0,
                  (modelo.faces[f].normal.z + 1) / 2.0
                 );
// ******************************************************************************************************
        glNormal3f(modelo.faces[f].normal.x, modelo.faces[f].normal.y, modelo.faces[f].normal.z);
//        glBegin(GL_LINE_LOOP);
        glBegin(GL_POLYGON);
            glVertex3f(modelo.faces[f].ponto[0].x, modelo.faces[f].ponto[0].y, modelo.faces[f].ponto[0].z);
            glVertex3f(modelo.faces[f].ponto[1].x, modelo.faces[f].ponto[1].y, modelo.faces[f].ponto[1].z);
            glVertex3f(modelo.faces[f].ponto[2].x, modelo.faces[f].ponto[2].y, modelo.faces[f].ponto[2].z);
        glEnd();
    }
}
