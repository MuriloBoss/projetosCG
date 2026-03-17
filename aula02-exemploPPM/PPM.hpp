#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
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

void criarMosaico(PPM *ppm, int largura, int altura)
{
    if (ppm->pixels)
        delete ppm->pixels;
    
    int tamanho = largura * altura * 3; //vezes 3, pois cada pixel possui RGB

    ppm->tipo = "P3";
    ppm->larg = largura;
    ppm->alt = altura;
    ppm->vmax = 255;
    ppm->pixels = new unsigned char[tamanho];

    // definir a cor preta para todos os pixels
    for (int i = 0; i < tamanho; i++)
        ppm->pixels[i] = 0;
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

void setFillBorda(PPM *ppm, Ponto p1, Ponto p2, const RGB cor, int espessura){
    if (!ppm || !ppm->pixels)
        return;
    
    for(int y = p1.y; y <= p2.y; y++){
        for(int x = p1.x; x <= p2.x; x++){
            if(x < p1.x + espessura || x > p2.x - espessura|| 
               y < p1.y + espessura || y > p2.y - espessura){
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

void setFlip(PPM *ppm){
    for(int y=0; y < ppm->alt ;y++){
        for(int x=0; x<ppm->larg/2;x++){
            RGB temp = getPixel(ppm, x,y);
            setPixel(ppm,x,y, getPixel(ppm, ppm->larg-1-x, y));
            setPixel(ppm, ppm->larg-1-x,y, temp);
        }
    }
}

void setInverterRGB(PPM *ppm)
{
    PPM invertida;
    criar(&invertida, ppm->larg, ppm->alt, RGB(0,0,0));
    int tamanho = ppm->larg * ppm->alt * 3;
    RGB rgb;
    for (int i = 0; i < tamanho; i+=3){
        
        //get
        rgb.r = ppm->pixels[i];
        rgb.g = ppm->pixels[i+1];
        rgb.b = ppm->pixels[i+2];
        //set
        invertida.pixels[i] = 255- rgb.r;
        invertida.pixels[i+1] = 255 - rgb.g;
        invertida.pixels[i+2] = 255 - rgb.b;
     }
     gravar(&invertida, "imgInvertida.ppm");
     destruir(&invertida);
}

void mosaico()
{
    ifstream arq("imagens.txt");

    if (!arq.is_open())
    {
        cout << "Erro.\n";
        return;
    }

    vector<PPM> imagens;
    string nome;

    while (getline(arq, nome))
    {
        if (nome.empty())
            continue;

        PPM img;
        if (!ler(&img, nome))
        {
            cout << "Erro ao ler imagem: " << nome << endl;
            continue;
        }

        imagens.push_back(img);
    }

    arq.close();

    if (imagens.size() == 0)
    {
        cout << "Nenhuma imagem carregada.\n";
        return;
    }

    int L = imagens[0].larg;
    int A = imagens[0].alt;

    for (int i = 1; i < imagens.size(); i++)
    {
        if (imagens[i].larg != L || imagens[i].alt != A)
        {
            cout << "Erro: Todas as imagens devem ter a mesma dimensão.\n";
            return;
        }
    }

    int N = imagens.size();
    PPM saida;
    criarMosaico(&saida, L * N, A);

    for (int k = 0; k < N; k++)
    {
        for (int y = 0; y < A; y++)
        {
            for (int x = 0; x < L; x++)
            {
                RGB cor = getPixel(&imagens[k], x, y);
                setPixel(&saida, x + k * L, y, cor);
            }
        }
    }

    gravar(&saida, "exerc12.ppm");

    for (int i = 0; i < imagens.size(); i++)
        destruir(&imagens[i]);

    destruir(&saida);

    cout << "Mosaico criado com sucesso!\n";
}

void setQuantizacao(PPM *ppm){
    if(!ppm || !ppm->pixels){
        return;
    }

    int tamanho = ppm->larg * ppm->alt *3;

    //Q - 1 = 3 
    double salto = 255.0 / 3;

    for(int i =0; i < tamanho; i++){
        int corOriginal = ppm->pixels[i];
        int novaCor = round(corOriginal / salto) * salto;

        ppm->pixels[i] = (unsigned char)novaCor;
    }
}


void DDALine(PPM *img, int x0, int y0, int x1, int y1, RGB rgb)
{

    // Calculate dx and dy
    int dx = x1 - x0;
    int dy = y1 - y0;

    int step;

    // If dx > dy we will take step as dx
    // else we will take step as dy to draw the complete
    // line
    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);

    // Calculate x-increment and y-increment for each step
    float x_incr = (float)dx / step;
    float y_incr = (float)dy / step;

    // Take the initial points as x and y
    float x = x0;
    float y = y0;

    for (int i = 0; i < step; i++) {

		setPixel(img, x, y, rgb);
        // putpixel(round(x), round(y), WHITE);
        //cout << round(x) << " " << round(y) << "\n";
        x += x_incr;
        y += y_incr;
        // delay(10);
    }
}

void exer15(){
	PPM img;
	criar(&img, 200, 80, RGB(0,0,0));
	RGB corL(255,0,0);
	DDALine(&img, 0, 0, img.larg-1, img.alt-1, corL);
	DDALine(&img, 0, img.alt-1, img.larg-1, 0, corL);

	gravar(&img, "exercicio15.pgm");
}
#endif