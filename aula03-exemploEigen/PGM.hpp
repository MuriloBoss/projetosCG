#ifndef PGM_H_INCLUDED
#define PGM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct PGM
{
    string tipo;
    int larg;
    int alt;
    int vmax;
    unsigned char *pixels; // unsigned char -> 0 a 255
    PGM()                  // construtor -> inicializar variaveis
    {
        pixels = nullptr;
        larg = 0;
        alt = 0;
        vmax = 255;
        tipo = "";
    }
};

void destruir(PGM *pgm)
{
    if (pgm->pixels)
        delete pgm->pixels;
    pgm->pixels = nullptr;
    pgm->larg = 0;
    pgm->alt = 0;
    pgm->tipo = "";
    pgm->vmax = 255;
}

void imprimir(PGM *pgm)
{
    cout << "\nPGM\n";
    cout << "Tipo: " << pgm->tipo << endl;
    cout << "Dimensao: " << pgm->larg << "x" << pgm->alt << endl;
    cout << "vMax: " << pgm->vmax << endl;
    if (pgm->pixels) // verifica se o vetor de pixels está alocado
        cout << "Vetor de pixels: " << &(pgm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}
/*
void criar(PGM *pgm, int largura, int altura, int)
{
    if (pgm->pixels)
        delete pgm->pixels;
    
    int tamanho = largura * altura;

    pgm->tipo = "P2";
    pgm->larg = largura;
    pgm->alt = altura;
    pgm->vmax = 255;
    pgm->pixels = new unsigned char[tamanho];

    // definir a cor preta para todos os pixels
    for (int i = 0; i < tamanho; i++)
        pgm->pixels[i] = 0;
}
*/

void criar(PGM *pgm, int largura, int altura, unsigned char planofundo)
{
    if (pgm->pixels)
        delete pgm->pixels;
    
    int tamanho = largura * altura;

    pgm->tipo = "P2";
    pgm->larg = largura;
    pgm->alt = altura;
    pgm->vmax = 255;
    pgm->pixels = new unsigned char[tamanho];

    // definir a cor preta para todos os pixels
    for (int i = 0; i < tamanho; i++)
        pgm->pixels[i] = planofundo;
}

bool gravar(PGM *pgm, string caminho)
{
    if (!pgm->pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PGM: endereco do arquivo invalido\n";
        return false;
    }

    arq << pgm->tipo << endl;

    arq << pgm->larg << " " << pgm->alt << endl;

    arq << 255 << endl; // valor máximo de cor fixo em 255

    int tam = pgm->larg * pgm->alt;
    for (int i = 0; i < tam; i++)
    {
        arq << (int)pgm->pixels[i] << endl;
        arq.flush();
    }

    arq.close();
    return true;
}


string lerLinhaArquivo(std::ifstream &arq)
{
    string linha = "", dadoLido = "";
    while (!arq.eof())
    {
        linha = "";
        std::getline(arq, linha);
		
		// Remove \r do final da linha (caso exista)
        if (!linha.empty() && linha.back() == '\r')
            linha.pop_back();
		
        if (linha.size() > 0 && linha[0] != '#')
            return linha;
    }
    return linha;
}

bool ler(PGM *pgm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PGM: endereco do arquivo invalido\n";
        return false;
    }

    // ler o tipo
    dado = lerLinhaArquivo(arq);
    if (dado == "P2")
        pgm->tipo = dado;
    else
    {
        cout << "PGM: erro ao ler o tipo da imagem\n";
        return false;
    }

    // ler largura e altura
    dado = lerLinhaArquivo(arq);
    str = stringstream(dado);
    if (!(str >> pgm->larg))
    {
        cout << "PGM: erro ao ler a largura\n";
        return false;
    }
    if (!(str >> pgm->alt))
    {
        cout << "PGM: erro ao ler a largura\n";
        return false;
    }

    // ler vmax
    dado = lerLinhaArquivo(arq);
    str = stringstream(dado);
    if (!(str >> pgm->vmax))
    {
        cout << "PGM: erro ao ler vmax\n";
        return false;
    }

    if (pgm->pixels)
        delete pgm->pixels;

    int tam = pgm->larg * pgm->alt;

    pgm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {

        if (arq >> p)
        {
            if (i < tam)
                pgm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PGM: erro ao ler os pixels\n";
        return false;
    }
    arq.close();
    return true;
}


unsigned char getPixel(PGM *pgm, int x, int y)
{
    if (!pgm->pixels)
        return 0;

    return pgm->pixels[y * pgm->larg + x];
}


void setPixel(PGM *pgm, int x, int y, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    pgm->pixels[y * pgm->larg + x] = cor;
}

void setLinha(PGM *pgm, int linhaInicial, unsigned char cor, int espessura)
{
    if(!pgm->pixels) return;

    for(int j = 0 ; j < espessura;j++){
        int linhaAtual = linhaInicial + j;

        if(linhaAtual >= pgm->alt || linhaAtual < 0) continue;

        for(int x = 0 ; x < pgm->larg; x++){
            pgm->pixels[linhaAtual * pgm->larg + x] = cor;
            }
    }
}

void setColuna(PGM *pgm, int colunaInicial, unsigned char cor, int espessura)
{
    if (!pgm->pixels) return;
     for(int j = 0 ; j < espessura;j++){
        int colunaAtual = colunaInicial + j;

        if(colunaAtual >= pgm->larg || colunaAtual < 0) continue;

        for(int y = 0 ; y < pgm->alt; y++){
            pgm->pixels[y * pgm->alt + colunaAtual] = cor;
        }
    }
}

bool coordValida(PGM *pgm, int x, int y){
    if (!pgm || !pgm->pixels)
        return false;

    return (x >= 0 && x < pgm->larg &&
            y >= 0 && y < pgm->alt);
}

void setFill(PGM *pgm, int x1, int y1, int x2, int y2, unsigned char cor){
    if (!pgm || !pgm->pixels)
        return;
    
    for(int y = y1 ; y <= y2 ; y++){
        for(int x = x1; x < x2; x++){
            if(coordValida(pgm, x,y)){
                pgm->pixels[y * pgm->larg + x] = cor;
            }
        }
    }
    
}

void setRecorte(PGM *pgm, PGM *pgmAux, int x1, int y1, int x2, int y2){
    if (!pgm || !pgm->pixels)
        return;
    

    for(int y = y1 ; y <= y2 ; y++){
        for(int x = x1; x < x2; x++){
           
            unsigned char aux = getPixel(pgm, x,y);
            setPixel(pgmAux, x,y, aux);
        }
    }
    
}

void setBorda(PGM *pgm, int espessura, unsigned char cor){

    setLinha(pgm,0,cor,espessura );
    
    setLinha(pgm,pgm->alt - espessura,cor,espessura );

    setColuna(pgm,0,cor,espessura);

    setColuna(pgm, pgm->larg - espessura,cor,espessura);
    
}

void setFillBorda(PGM *pgm, int x1, int y1, int x2, int y2, unsigned char cor){
    if (!pgm || !pgm->pixels)
        return;
    
    for(int y = y1; y <= y2; y++){
        for(int x = x1; x <= x2; x++){
            if(x == x1 || x == x2 || y == y1 || y == y2){
                if(coordValida(pgm, x, y)){
                    pgm->pixels[y * pgm->larg + x] = cor;
                }
            }
        }
    }
    
}
#endif