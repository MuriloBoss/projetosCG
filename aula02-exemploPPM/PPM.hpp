#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct PPM
{
    string tipo;
    int larg;
    int alt;
    int vmax;
    unsigned char *pixels; // unsigned char -> 0 a 255
    PPM()                  // construtor -> inicializar variaveis
    {
        pixels = nullptr;
        larg = 0;
        alt = 0;
        vmax = 255;
        tipo = "";
    }
};

struct RGB{
    unsigned char r, g, b;
    RGB()
    {
        r=0;
        g=0;
        b=0;
    }
    RGB(unsigned char _r, unsigned char _g,unsigned char _b)
    {
        r=_r;
        g=_g;
        b=_b;
    }
};
ostream& operator<<(ostream& os, const RGB rgb)
{
    return os << (int) rgb.r << ", " << (int) rgb.g << ", " << (int) rgb.b ;
}

void destruir(PPM *ppm)
{
    if (ppm->pixels)
        delete ppm->pixels;
    ppm->pixels = nullptr;
    ppm->larg = 0;
    ppm->alt = 0;
    ppm->tipo = "";
    ppm->vmax = 255;
}

void imprimir(PPM *ppm)
{
    cout << "\nPPM\n";
    cout << "Tipo: " << ppm->tipo << endl;
    cout << "Dimensao: " << ppm->larg << "x" << ppm->alt << endl;
    cout << "vMax: " << ppm->vmax << endl;
    if (ppm->pixels) // verifica se o vetor de pixels está alocado
        cout << "Vetor de pixels: " << &(ppm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}

void criar(PPM *ppm, int largura, int altura, const RGB planofundo)
{
    if (ppm->pixels)
        delete ppm->pixels;
    
    int tamanho = largura * altura * 3; //vezes 3, pois cada pixel possui RGB

    ppm->tipo = "P3";
    ppm->larg = largura;
    ppm->alt = altura;
    ppm->vmax = 255;
    ppm->pixels = new unsigned char[tamanho];

    for (int i = 0; i < tamanho; i+=3){
        ppm->pixels[i] = planofundo.r;

        ppm->pixels[i+1] = planofundo.g;

        ppm->pixels[i+2] = planofundo.b;
    }
}

void criarPGM(PPM *pgm, int largura, int altura)
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

bool gravar(PPM *ppm, string caminho)
{
    // Boa prática: checar se o ponteiro ppm não é nulo antes de acessar ppm->pixels
    if (!ppm || !ppm->pixels)
        return false;

    ofstream arq;
    arq.open(caminho);
    
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    // Se o tipo estiver vazio (por falha no 'criar'), forçamos "P3" para não quebrar a imagem
    string tipo = (ppm->tipo == "") ? "P3" : ppm->tipo;
    arq << tipo << "\n";

    arq << ppm->larg << " " << ppm->alt << "\n";
    arq << 255 << "\n"; // valor máximo de cor

    int tam = ppm->larg * ppm->alt * 3;  
    for (int i = 0; i < tam; i++)
    {
        // Usamos espaço em vez de endl/flush. É absurdamente mais rápido!
        arq << (int)ppm->pixels[i] << " ";
        
        // Pula uma linha a cada pixel (3 cores) só para o arquivo ficar fácil de ler no bloco de notas
        if ((i + 1) % 3 == 0) {
            arq << "\n";
        }
    }

    // O close() já se encarrega de dar o flush final de uma vez só!
    arq.close();
    return true;
}


string lerLinhaArquivoPPM(std::ifstream &arq)
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

bool ler(PPM *ppm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    // ler o tipo
    dado = lerLinhaArquivoPPM(arq);
    if (dado == "P3")
        ppm->tipo = dado;
    else
    {
        cout << "PPM: erro ao ler o tipo da imagem\n";
        return false;
    }

    // LER LARGURA, ALTURA e VMAX
    // Em vez de ler por linha, alimentamos uma única stringstream com as 
    // próximas linhas até conseguirmos extrair os 3 valores, ignorando comentários.
    int valoresLidos = 0;
    int dimensoes[3]; // Guarda: [0] = larg, [1] = alt, [2] = vmax

    while (valoresLidos < 3 && !arq.eof()) {
        dado = lerLinhaArquivoPPM(arq);
        str = stringstream(dado);
        int valor;
        while (str >> valor) {
            dimensoes[valoresLidos] = valor;
            valoresLidos++;
            if (valoresLidos == 3) break;
        }
    }

    if (valoresLidos < 3) {
        cout << "PPM: erro ao ler as dimensoes ou vmax do cabecalho\n";
        return false;
    }

    ppm->larg = dimensoes[0];
    ppm->alt = dimensoes[1];
    ppm->vmax = dimensoes[2];

    // CORREÇÃO DO DELETE: Usar delete[] para arrays
    if (ppm->pixels)
        delete[] ppm->pixels;

    int tam = ppm->larg * ppm->alt * 3; // vezes 3, pois cada pixel possui RGB

    ppm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {
        if (arq >> p)
        {
            if (i < tam)
                ppm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PPM: erro ao ler os pixels\n";
        return false;
    }
    
    arq.close();
    return true;
}


RGB getPixel(PPM *ppm, int x, int y)
{
    RGB rgb;
    if (!ppm->pixels)
        return rgb;

    rgb.r = ppm->pixels[y * ppm->larg*3 + x*3];
    rgb.g = ppm->pixels[y * ppm->larg*3 + x*3 +1];
    rgb.b = ppm->pixels[y * ppm->larg*3 + x*3 +2];

    return rgb;
}
void setPixel(PPM *ppm, int x, int y, RGB rgb)
{
    if (!ppm->pixels)
        return;

    ppm->pixels[y * ppm->larg*3 + x*3] = rgb.r;
    ppm->pixels[y * ppm->larg*3 + x*3 +1] = rgb.g;
    ppm->pixels[y * ppm->larg*3 + x*3 +2] = rgb.b;
}

void setLinha(PPM *pgm, int linhaInicial, const RGB cor, int espessura)
{
    if(!pgm->pixels) return;

    for(int j = 0 ; j < espessura;j++){
        int linhaAtual = linhaInicial + j;

        if(linhaAtual >= pgm->alt || linhaAtual < 0) continue;

        for(int x = 0 ; x < pgm->larg; x++){
            pgm->pixels[linhaAtual * pgm->larg*3 + x*3] = cor.r;
            pgm->pixels[linhaAtual * pgm->larg*3 + x*3+1] = cor.g;
            pgm->pixels[linhaAtual * pgm->larg*3 + x*3+2] = cor.b;
            }
    }
}

bool coordValida(PPM *pgm, int x, int y){
    if (!pgm || !pgm->pixels)
        return false;

    return (x >= 0 && x < pgm->larg &&
            y >= 0 && y < pgm->alt);
}

void setPreencher(PPM *pgm, int x1, int y1, int x2, int y2, const RGB cor){
    if (!pgm || !pgm->pixels)
        return;
    
    for(int y = y1 ; y <= y2 ; y++){
        for(int x = x1; x < x2; x++){
            if(coordValida(pgm, x,y)){
                pgm->pixels[y * pgm->larg + x] = cor.r;
                pgm->pixels[y * pgm->larg + x+1] = cor.g;
                pgm->pixels[y * pgm->larg + x+2] = cor.b;
            }
        }
    }
    
}


void setBorda(PPM *ppm, int espessura, const RGB cor){

    setLinha(ppm,0,cor,espessura );
    
    setLinha(ppm,ppm->alt - espessura,cor,espessura );

   // setColuna(ppm,0,cor,espessura);

    //setColuna(ppm, ppm->larg - espessura,cor,espessura);
    
}
struct Ponto{
    int x;
    int y;
};

void setFillBorda(PPM *ppm, Ponto p1, Ponto p2, const RGB cor){
    if (!ppm || !ppm->pixels)
        return;
    
    for(int y = p1.y; y <= p2.y; y++){
        for(int x = p1.x; x <= p2.x; x++){
            if(x == p1.x || x == p2.x || y == p1.y || y == p2.y){
                if(coordValida(ppm, x, y)){
                    ppm->pixels[y * ppm->larg*3 + x*3] = cor.r;
                    ppm->pixels[y * ppm->larg*3 + x*3 + 1] = cor.g;
                    ppm->pixels[y * ppm->larg*3 + x*3 + 2] = cor.b;
                }
            }
        }
    }
    
}

void setRecorte(PPM *ppm, PPM *pgmAux, Ponto p1, Ponto p2){
    if (!ppm || !ppm->pixels)
        return;
    
    for(int y = p1.y ; y <= p2.y ; y++){
        for(int x = p1.x; x < p2.x; x++){
            RGB rgb = getPixel(ppm, x,y);
            setPixel(pgmAux, x,y, rgb);
        }
    }
}

void converterRGB(PPM *ppm ){
    PPM pgm;
    criarPGM(&pgm, ppm->larg, ppm->alt);
    int tamanho = ppm->larg * ppm->alt * 3;
    RGB rgb;
    for (int i = 0; i < tamanho; i+=3){
        
        //get
        rgb.r = ppm->pixels[i];
        rgb.g = ppm->pixels[i+1];
        rgb.b = ppm->pixels[i+2];
        //set
        ppm->pixels[i] = rgb.r*0.299 + rgb.g*0.587 + rgb.b*0.114;
        ppm->pixels[i+1] = rgb.r*0.299 + rgb.g*0.587 + rgb.b*0.114;
        ppm->pixels[i+2] = rgb.r*0.299 + rgb.g*0.587 + rgb.b*0.114;
     }
    
}

#endif