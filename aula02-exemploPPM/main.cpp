#include <iostream>
#include "PPM.hpp"
#include <ctime>

using namespace std;
	

/*Exemplos básicos

//#Exemplo1#: criando uma imagem e gravando no disco
	PPM img1;
	criar(&img1, 5, 7);
	gravar(&img1, "exemplo1.ppm");
	destruir(&img1);

	//#Exemplo2#: imprimindo os atributos da imagem 
	PPM img2;
	cout << "#Exemplo 2#\n";
	imprimir(&img2);
	criar(&img2, 700, 500);
	imprimir(&img2);
	gravar(&img2, "exemplo2.ppm");
	destruir(&img2);
	imprimir(&img2);

	//#Exemplo3#: ler imagem, getPixel, setPixel 
	PPM img3;
	RGB corP;
	cout << "\n\n#Exemplo 3#\n";
	ler(&img3, "exemploPPM.ppm");

	//ler o valor do pixel de coordenadas (x=1, y=1)
	corP = getPixel(&img3, 0,0);
	cout << "A cor do pixel(0,0): [" << corP << "] " << endl;

	setPixel(&img3, 1, 0, RGB(0, 128, 128)); //alterar a cor do pixel(1, 0) para RGB(0, 128, 128)
	setPixel(&img3, 2, 0, RGB(255, 0, 255)); //alterar a cor do pixel(2, 0) para rosa
	setPixel(&img3, 2, 4, RGB(128, 0, 128)); //alterar a cor do pixel(2, 4) para roxo
	gravar(&img3, "exemplo3.ppm"); //gravar o resultado no disco
	destruir(&img3);
	*/

int main(void)
{
	srand(time(0));
	/*Exer 01 - 1. Considere o vetor abaixo, que armazena os valores de cor dos pixels de uma
	imagem PPM 2x2 (2 pixels de largura, 2 pixels de altura). Qual o valor RGB do
	pixel nas coordenadas (1, 0)?
	100 200 0 0 255 0 128 128 128 0 0 255

	R) (1,0) = 0 255 0

	2. Considere uma imagem PPM 4x4, sendo que a primeira linha possui a cor preta, a
	segunda linha a branca, a terceira linha a cor azul e a quarta linha a cor vermelha.
	Responda:
		a. Quantas posições um vetor deve ter para armazenar os valores de cor da
		imagem?
		48 posições ( 4x4 = 16, 16*3= 48)
		b. Quais as posições do vetor que armazenam a cor RGB do segundo pixel da linha
		de cor azul?
		Indice do pixel no vetor: (y*largura + x) = 2*4 + 1 = 9, 9*3 = 27. Posições 27,28,29;
	*/
	
	/*
	//Exercicio 3- Criar imagem com rgb
	PPM img1;
	criar(&img1, 5, 7, RGB(255,0,0));
	gravar(&img1, "exer3.ppm");
	destruir(&img1);
	*/

	/*
	//Exercicio 4- setlinha
	PPM img1;
	criar(&img1, 5, 7, RGB(255,0,0));
	setLinha(&img1, 2, RGB(255,255,0), 1);
	gravar(&img1, "exer4.ppm");
	destruir(&img1);
	*/

	/*
	//Exer 5 - Coord Válida
	PPM img1;
	criar(&img1, 5, 7, RGB(255,0,0));
	if(coordValida(&img1,4,5)){
		cout << "Coordenada Valida\n";
	}else{
		cout << "Coordenada Invalida.";
	};
	gravar(&img1, "exer5.ppm");
	destruir(&img1);
	*/

	/*
	//Exer 6 - preencher com RGB
	PPM img1;
	//criar(&img1, 100, 80, RGB(0,0,0));
	ler(&img1, "numeros.ppm");
	cout << "Imagem Lida\n";
	Ponto p1 = {200,0};
	Ponto p2 = {375,270};

	setFillBorda(&img1, p1, p2 , RGB(0,255,0));
	cout << "Função concluida\n";
	gravar(&img1, "exer6.pgm");
	cout << "Imagem gravada\n";
	destruir(&img1);
	*/

	/*
	//exer 07 - linha aleatoria a cada 50 linhas
	PPM imgLinhas;
	criar(&imgLinhas, 500, 500, RGB(0,0,0));
	
	for(int i = 50; i<= imgLinhas.alt; i+=50){
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		setLinha(&imgLinhas, i, RGB(r,g,b), 1);
	}
	gravar(&imgLinhas, "exercicio07.pgm");
	destruir(&imgLinhas);
	*/

	/*
	//exer 08 - 
	PPM imgRecorte;
	PPM imgRecorte_aux;
	ler(&imgRecorte, "numeros.ppm");

	criar(&imgRecorte_aux, imgRecorte.larg, imgRecorte.alt, RGB(0,0,0));

	

	Ponto p1 = {200,0};
	Ponto p2 = {375,270};

	//criar(&imgRecorte_aux, 1000, 900, RGB(0,0,0));

	setRecorte(&imgRecorte, &imgRecorte_aux, p1, p2 );

	gravar(&imgRecorte_aux, "exercicio8.pgm");

	destruir(&imgRecorte_aux);
	destruir(&imgRecorte);
	*/

	/*
	//exer 9 
	PPM imgOriginal;

	ler(&imgOriginal,"spider.ppm");
	converterRGB(&imgOriginal);
	gravar(&imgOriginal, "exer9.ppm");
	destruir(&imgOriginal);
	*/

	/*
	//exer 10
	PPM imgFlip;
    cout << "\n\n#Exemplo 10 - Flip Vertical#\n";
    ler(&imgFlip, "numeros.ppm");
	setFlip(&imgFlip);
	gravar(&imgFlip, "exercicio10.pgm");
    destruir(&imgFlip);
	*/

	/*
	//Exer 11 - borda p1 p2 com espessura
	PPM img1;
	//criar(&img1, 100, 80, RGB(0,0,0));
	ler(&img1, "numeros.ppm");
	cout << "Imagem Lida\n";
	Ponto p1 = {200,0};
	Ponto p2 = {375,270};

	setFillBorda(&img1, p1, p2 , RGB(0,255,0), 3);
	cout << "Função concluida\n";
	gravar(&img1, "exercicio11.ppm");
	cout << "Imagem gravada\n";
	destruir(&img1);
	*/
	/*
	//Exer 12 - 
	PPM img1, img2, img3, img4;
	criar(&img1, 500, 500, RGB(0,0,0));
	criar(&img2, 500, 500, RGB(255,0,0));
	criar(&img3, 500, 500, RGB(0,255,0));
	criar(&img4, 500, 500, RGB(0,0,255));
	gravar(&img1, "img1.ppm");
	gravar(&img2, "img2.ppm");
	gravar(&img3, "img3.ppm");
	gravar(&img4, "img4.ppm");

	mosaico();
	*/

	/*
	//Exer 13 - inverter cores RGB
	PPM imgOriginal;
	PPM imgInvertida;
	ler(&imgOriginal, "numeros.ppm");
	setInverterRGB(&imgOriginal);
	*/

	/*
	//Exer 14 - quantização
	PPM imgQ;
	cout<< "Rodando exer 14\n";
	ler(&imgQ, "numeros.ppm");
	PPM img1, img2, img3, img4;
	criar(&img1, 500, 500, RGB(225,0,0));
	criar(&img2, 500, 500, RGB(255,0,0));
	criar(&img3, 500, 500, RGB(240,0,0));
	criar(&img4, 500, 500, RGB(230,0,0));
	setQuantizacao(&imgQ);
	setQuantizacao(&img1);
	setQuantizacao(&img2);
	setQuantizacao(&img3);
	setQuantizacao(&img4);

	gravar(&img1, "img1.ppm");
	gravar(&img2, "img2.ppm");
	gravar(&img3, "img3.ppm");
	gravar(&img4, "img4.ppm");
	gravar(&imgQ, "exer14.ppm");

	destruir(&imgQ);
	destruir(&img1);
	destruir(&img2);
	destruir(&img3);
	destruir(&img4);
	*/

	//EXER 15 
	exer15();
	

	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}