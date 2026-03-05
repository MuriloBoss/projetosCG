#include <iostream>
#include<cmath>
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

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	cout << "Chamando exercicio 3f\n";
	 exerLista3F();

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