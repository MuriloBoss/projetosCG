#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "PGM.hpp"
using namespace std;


#include <Eigen/Dense>
using namespace Eigen;

#define _PI	3.14159265358979323846

float degree2rad(float ang) {
	float r = ang * (float)(_PI / 180.0);

	return r;
}

Matrix3f getTranslacao(float tx, float ty)
{
	Matrix3f T = Matrix3f::Identity();
	T(0,2) = tx;
	T(1,2) = ty;
	return T;
}

Matrix3f getRotacao(float ang)
{
	float rad = degree2rad(ang);
	float s = sin(rad);
	float c = cos(rad);

	Matrix3f T = Matrix3f::Identity();
	T(0,0) = c;
	T(0,1) = -s;
	T(1,0) = s;
	T(1,1) = c;
	return T;
}


//horizontal muda o x e vertical o y
Matrix3f getCisalhamento(float fatorX, float fatorY){
	Matrix3f T = Matrix3f::Identity();
	T(0,1) = fatorX;
	T(1,0) = fatorY;
	return T;
}

Matrix3f getEscala(float sx, float sy)
{
	Matrix3f E = Matrix3f::Identity();
	E(0, 0) = sx;
	E(1, 1) = sy;
	return E;
}

void exerLista3F()
{
	Vector2f pontoCentro(6.0f,4.0f);

	Matrix3f M = getTranslacao(pontoCentro.x(), pontoCentro.y()) * getRotacao(-30.0f) * getTranslacao(-pontoCentro.x(),-pontoCentro.y());

	//criando um vetor com os pontos
	vector<Vector3f> pontos;
	pontos.push_back(Vector3f(4.0f, 2.0f, 1.0f));
	pontos.push_back(Vector3f(8.0f, 2.0f, 1.0f));
	pontos.push_back(Vector3f(6.0f, 7.0f, 1.0f));

	for (int i = 0; i < pontos.size(); i++)
	{
		Vector3f p  = M * pontos[i];
		cout << "Ponto " << i+1 << ": " << p.transpose() << endl;
	}
}

//ler a imagem de entrada e criar a imagem de saida antes de chamar a função
void transf2D(PGM *imgE, PGM *imgS, Matrix3f M){
	if(imgE->larg!=imgS->larg || imgE->alt!=imgS->alt)
		return;
	
	//percorrer a imagem de entrada
	for (size_t yE = 0; yE < imgE->alt; yE++)
	{
		for (size_t xE = 0; xE < imgE->larg; xE++)
		{
			Vector3f coodSaida = M * Vector3f(xE, yE, 1);
			float xS = round(coodSaida.x());
			float yS = round(coodSaida.y());

			if(coordValida(imgS, xS, yS)){
				setPixel(imgS, xS, yS, getPixel(imgE, xE, yE));
			}
		}
		
	}
	
}

void transf2DInv(PGM* imgE, PGM* imgS, Matrix3f Minv)
{
	if (imgE->larg!=imgS->larg || imgE->alt!=imgS->alt)
		return;
	
	for (size_t yS = 0; yS < imgE->alt; yS++) 
	{
		for (size_t xS = 0; xS < imgE->larg; xS++)
		{
			Vector3f coordEntrada = Minv * Vector3f(xS, yS, 1);
			float xE = round(coordEntrada.x());
			float yE = round(coordEntrada.y());
			if (coordValida(imgE, xE, yE))
			{
				setPixel (imgS, xS, yS, getPixel(imgE, xE, yE));
			}
		}
	}
}

unsigned char calcularMediana(PGM *pgm, int x, int y)
{
	int dx[] = {1, -1, 0,  0, -1,  1, -1, 1};
	int dy[] = {0,  0, 1, -1, -1, -1,  1, 1};

	vector<int> vizinhos;
	for (int i = 0; i < 8; i++)
	{
		int nx = x + dx[i], ny = y + dy[i];
		if (coordValida(pgm, nx, ny))
		{
			int cor = getPixel(pgm, nx, ny);
			if (cor != 0)
				vizinhos.push_back(cor);
		}
	}

	if (vizinhos.empty()) return 0;

	sort(vizinhos.begin(), vizinhos.end());
	int n = vizinhos.size();
	if (n % 2 == 1)
		return (unsigned char)vizinhos[n / 2];
	else
		return (unsigned char)((vizinhos[n/2 - 1] + vizinhos[n/2]) / 2);
}

void preencherBuracos(PGM *pgm)
{
	for (int y = 0; y < pgm->alt; y++)
		for (int x = 0; x < pgm->larg; x++)
			if (getPixel(pgm, x, y) == 0)
				setPixel(pgm, x, y, calcularMediana(pgm, x, y));
}

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	PGM imgE;
	ler(&imgE, "entrada.pgm");
	preencherBuracos(&imgE);
	gravar(&imgE, "saida.pgm");
	cout << "Imagem salva em saida.pgm\n";

	//cout << "Chamando exercicio 3f\n";
	// exerLista3F();

	PGM imgS;
	ler(&imgE, "numeros.pgm");
	criar(&imgS, imgE.larg, imgE.alt, 0);

	Vector2f pontoCentro((imgE.larg-1)/2.0, (imgE.alt-1)/2.0);
	Matrix3f T = getTranslacao(pontoCentro.x(), pontoCentro.y());
	Matrix3f Tinv = getTranslacao(-pontoCentro.x(), -pontoCentro.y());
	Matrix3f R = getRotacao(45.0f);
	Matrix3f M = T * R * Tinv;

	Matrix3f Minv = M.inverse();
	transf2DInv(&imgE, &imgS, Minv);
	gravar(&imgS, "numeros_rotacaoinv.pgm");
	transf2D (&imgE, &imgS, M);
	gravar(&imgS, "numeros_rotacao45.pgm");
}

void exer1Lista4(){
	PGM imgE, imgS;
	ler(&imgE, "numeros.pgm");
	criar(&imgS, imgE.larg, imgE.alt, 0);

	Vector2f pontoCentro(imgE.larg/2.0, imgE.alt/2.0);

	Matrix3f T = getTranslacao(pontoCentro.x(), pontoCentro.y());
	Matrix3f Tinv = getTranslacao(-pontoCentro.x(), -pontoCentro.y());

	Matrix3f CIS = getCisalhamento(0.9f,0.0f);

	Matrix3f REF = getEscala(-1.0f, 1.0f);

	//A ordem de leitura/execução é da DIREITA para a ESQUeRDA;
	//Então -> Tinv, depois CIS, depois REF, e por fim T
	Matrix3f M = T * REF * CIS * Tinv;

	//Calcular a inversa da matriz composta para usarmos no Mapeamento Inverso
	Matrix3f Minv = M.inverse();

	transf2DInv(&imgE, &imgS, Minv);
	gravar(&imgS, "numeros_cisalhamento.pgm");
	
}

void exer2Lista4(){
	PGM imgE, imgS;
	ler(&imgE, "numeros.pgm");
	criar(&imgS, imgE.larg, imgE.alt, 0);

	Vector2f pontoCentro(imgE.larg/2.0, imgE.alt/2.0);

	Matrix3f T = getTranslacao(pontoCentro.x(), pontoCentro.y());
	Matrix3f Tinv = getTranslacao(-pontoCentro.x(), -pontoCentro.y());

	Matrix3f R = getRotacao(15.0f);
	Matrix3f E = getEscala(0.5f, 0.5f);
	Matrix3f R2 = getRotacao(20.0f);
	Matrix3f CIS = getCisalhamento(0.0f, 0.7f);

	Matrix3f M = T * CIS * R2 * E * R * Tinv;

	Matrix3f Minv = M.inverse();

	transf2DInv(&imgE, &imgS, Minv);
	gravar(&imgS, "saidaExer2.pgm");
}

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	// cout << "Chamando exercicio 3f\n";
	//  exerLista3F();

	//exemploRotacao();

	// cout << "Chamando exercicio 1 - lista 4\n";
	// exer1Lista4();

	cout << "Chamando exercicio 2 - lista 4\n";
	exer2Lista4();


	

	/*
	 //pontos do triângulo em coordenadas homogeneas 
	Vector3f pA = Vector3f(4.0f, 2.0f, 1.0f);
	Vector3f pB = Vector3f(8.0f, 2.0f, 1.0f);
	Vector3f pC = Vector3f(6.0f, 7.0f, 1.0f);


	//definindo uma matriz de translação
	Matrix3f mT = Matrix3f::Identity();
	mT(0,2) = 3; // Tx
	mT(1,2) = 1; // Ty

	//OU, usando uma funcao
	//Matrix3f mT =  getTranslacao(3, 1); 

	// Matriz x ponto
	Vector3f pA_  = mT * pA;
	Vector3f pB_  = mT * pB;
	Vector3f pC_  = mT * pC;


	//imprimindo a matriz
	cout << "Matriz:\n";
	cout << endl << mT << endl;

	//imprimindo os pontos
	cout << "Ponto A: " << pA_.transpose() << endl;
	cout << "Ponto B: " << pB_.transpose() << endl;
	cout << "Ponto C: " << pC_.transpose() << endl;


	//criando um vetor com os pontos
	vector<Vector3f> pontos;
	pontos.push_back(Vector3f(4.0f, 2.0f, 1.0f));
	pontos.push_back(Vector3f(8.0f, 2.0f, 1.0f));
	pontos.push_back(Vector3f(6.0f, 7.0f, 1.0f));

	for (int i = 0; i < pontos.size(); i++)
	{
		Vector3f p  = mT * pontos[i];
		cout << "Ponto " << i+1 << ": " << p.transpose() << endl;
	}
	
	*/

	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}