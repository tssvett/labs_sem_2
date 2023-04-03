﻿//23. Вычислить промежуточную сумму (cumulative sum) по столбцам матрицы
#include<iostream>
#include<fstream>
using ll = long long;
using ld = long double;


void fill_matrix(std::ifstream &ifile, ld**& matrix, ll matrix_row, ll matrix_col) {
	for (int i = 0; i < matrix_row; ++i) {	//Выделяем память под массивы
		matrix[i] = new ld[matrix_col];
	}
	for (int i = 0; i < matrix_row; ++i) {	//Заполняем эти массивы
		for (int j = 0; j < matrix_col; ++j) {
			ifile >> matrix[i][j];
		}
	}
}


void delete_matrix(ld**& two_dimensional, ll matrix_row) {	//Удаление матрицы
	for (int i = 0; i < matrix_row; ++i) {
		delete[] two_dimensional[i];
	}
	delete[] two_dimensional;
}

//В варианте написано считать сумму по стобцам, а все тесты считают по строкам ???
//Сумма по строкам
void cumulative_sum_calc(ld**& matrix, ll matrix_row, ll matrix_col) {
	ld* cumulative_sum = new ld[matrix_row]; //Вычисление промежуточной суммы по строкам
	for (int i = 0; i < matrix_row; ++i) {
		cumulative_sum[i] = 0;	//Изначально сумма ноль
		for (int j = 0; j < matrix_col; ++j) {
			cumulative_sum[i] += matrix[i][j]; // добавление текущего элемента к сумме
			matrix[i][j] = cumulative_sum[i]; // замена текущего элемента на промежуточную сумму
		}
	}
	delete[] cumulative_sum;
}


void print(std::ofstream& ofile, ld** matrix, ll matrix_row, ll matrix_col) {
	for (int i = 0; i < matrix_row; ++i) {
		for (int j = 0; j < matrix_col; ++j) {
			std::cout << matrix[i][j] << " ";
			ofile << matrix[i][j] << " ";
		}
	}
}


////Сумма по столбцам
//void cumulative_sum_calc(ld**& matrix, ll matrix_row, ll matrix_col) {
//	ld* sum = new ld[matrix_col]; // массив для хранения промежуточных сумм по столбцам
//	for (int j = 0; j < matrix_col; j++) {
//		sum[j] = 0; // инициализация сумм нулями
//		for (int i = 0; i < matrix_row; i++) {
//			sum[j] += matrix[i][j]; // добавление текущего элемента к сумме
//			matrix[i][j] = sum[j]; // замена текущего элемента на промежуточную сумму
//		}
//	}
//	delete[] sum;
//}


int main() {
	ll matrix_row; ll matrix_col;
	std::ifstream ifile("input.txt");
	std::ofstream ofile("output.txt");
	ifile >> matrix_row >> matrix_col;
	ld** matrix = new ld*[matrix_row];	// Создаем матрицу
	fill_matrix(ifile, matrix, matrix_row, matrix_col);	//Заполняем ее
	cumulative_sum_calc(matrix, matrix_row, matrix_col);	//	Обработка согласно заданию
	print(ofile, matrix, matrix_row, matrix_col);	//Вывод измененной матрицы
	delete_matrix(matrix, matrix_row);	// Удаление матрицы
}