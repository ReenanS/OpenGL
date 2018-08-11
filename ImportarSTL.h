#ifndef IMPORTARSTL_H_INCLUDED
#define IMPORTARSTL_H_INCLUDED

typedef struct
{
    float x,y,z;
} Ponto;

typedef struct
{
    Ponto ponto[3];
    Ponto normal;
} Face;

typedef struct
{
    char nome[255];
    Face *faces;
    Ponto minimo, maximo, centro;
    float tamanho;
    int nFaces;
} STL;

void LerArquivo(STL*, char*);
void DesenharSTL(STL modelo);

#endif // IMPORTARSTL_H_INCLUDED
