#include<iostream>
#include<fstream>
using namespace std;


//Тестовая лабораторная работа №0
//Для проверки системы Lab Bug Finder
//Самарского университета


int main() {
	ifstream ifile("input.txt");
	ofstream ofile("output.txt");
	int a, b;
	ifile >> a >> b;
	ofile << a + b;
}
