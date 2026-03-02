#include <iostream>
#include "PGM.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

void DDALine(PGM *img, int x0, int y0, int x1, int y1, unsigned char corL)
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

		setPixel(img, x, y, corL);
        // putpixel(round(x), round(y), WHITE);
        //cout << round(x) << " " << round(y) << "\n";
        x += x_incr;
        y += y_incr;
        // delay(10);
    }
}

void exerc12(){
	PGM img;
	criar(&img, 200, 80, 0);
	unsigned char corL = 255;
	DDALine(&img, 0, 0, img.larg-1, img.alt-1, corL);
	DDALine(&img, 0, img.alt-1, img.larg-1, 0, corL);

	gravar(&img, "exer12.pgm");
}

int main(void)
{
	

/*
	//#Exemplo2#: imprimindo os atributos da imagem 
	PGM img2;
	cout << "#Exemplo 2#\n";
	imprimir(&img2);
	criar(&img2, 700, 500,0);
	imprimir(&img2);
	gravar(&img2, "exemplo2.pgm");
	destruir(&img2);
	imprimir(&img2);
	*/
	//#Exemplo3#: ler imagem, getPixel, setPixel 


	/*
	PGM img3;
	unsigned char corP;
	cout << "\n\n#Exemplo 3#\n";
	ler(&img3, "feep.pgm");

	//ler o valor do pixel de coordenadas (x=1, y=1)
	corP = getPixel(&img3, 1,1);
	cout << "A cor do pixel(1,1): " << (int) corP << endl;

	setPixel(&img3, 1, 1, 255); //alterar a cor do pixel(1,1) para branco(255)
	gravar(&img3, "exemplo3.pgm"); //gravar o resultado no disco
	destruir(&img3);
	*/


	//exer 07 - preencher uma região
	PGM img1;
	criar(&img1, 100, 80, 0);
	if(coordValida(&img1,5,5)){
		cout << "Coordenada Valida\n";
	}else{
		cout << "Coordenada Invalida.";
	};
	setFillBorda(&img1, 0, 2, 8, 4 , 255);
	gravar(&img1, "exemplo1.pgm");
	destruir(&img1);

	/*
	//exer 08 - linha branca a cada 50 linhas
	PGM imgLinhas;
	criar(&imgLinhas, 500, 500, 0);
	
	for(int i = 50; i<= imgLinhas.alt; i+=50){
		setLinha(&imgLinhas, i, 255);
	}
	gravar(&imgLinhas, "exercicio08.pgm");
	destruir(&imgLinhas);
	*/

	//exer 09 inverter a imagem(flip)
	PGM imgFlip;
    cout << "\n\n#Exemplo 09 - Flip Vertical#\n";
    ler(&imgFlip, "feep.pgm");
    
    int tamanho = imgFlip.alt * imgFlip.larg;
    unsigned char* aux = new unsigned char[tamanho];
    
    for(int y = 0; y < imgFlip.alt; y++){
        for(int x = 0; x < imgFlip.larg; x++){
            
            int indiceOriginal = y * imgFlip.larg + x;
            
            int linhaInvertida = (imgFlip.alt - 1) - y;
            
            int indiceNovo = linhaInvertida * imgFlip.larg + x;
            
            aux[indiceNovo] = imgFlip.pixels[indiceOriginal];
        }
    }

    for(int i = 0; i < tamanho; i++){
         imgFlip.pixels[i] = aux[i];
    }

    delete[] aux; 

    gravar(&imgFlip, "exercicio09.pgm"); // gravar o resultado no disco
    destruir(&imgFlip);
	
	//setFill(&imgNum,120,0,220,170, 128);
	//gravar(&imgNum, "exer10.pgm");
	//destruir(&imgNum);
	
	//setFill(&imgNum,120,220,15,160, 0);


	//EXER 11 - função para desenhar uma borda.
	PGM imgBorda;
	criar(&imgBorda, 50, 50, 0);
	setBorda(&imgBorda,3,255);
	gravar(&imgBorda, "exercicio11.pgm");
	destruir(&imgBorda);


	//EXER 12 - implementar x
	//exerc12();

	//Exer 14 - Criar borda em uma região;


	//exer 15 -
	PGM imgOriginal;
	ler(&imgOriginal, "numeros.pgm");
	PGM imgConvertida;
	criar(&imgConvertida, imgOriginal.larg, imgOriginal.alt, 0);
	tamanho = imgOriginal.alt *imgOriginal.larg;

	for(int i = 0; i < tamanho; i++ ){
		if(imgOriginal.pixels[i] > 128){
			imgConvertida.pixels[i] = 255;
		}else{
			imgConvertida.pixels[i] = 0;
		}
	}
	gravar(&imgConvertida, "exercicio15.pgm");
	destruir(&imgOriginal);
	destruir(&imgConvertida);
	
	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}