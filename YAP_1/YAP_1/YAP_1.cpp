#include<fstream>


//Вар 27. Найти количество различных элементов массива, встречающихся больше b раз.

int get_b(std::ifstream& ifile) { //Получение числа b
	int b;
	ifile >> b;
	return b;
}


double* append(double *&array, int& size, double new_element) {	//Добавляем элемент в массив
	size++;
	double *new_array = new double[size];
	for (int i = 0; i < size - 1; ++i) {	//size -1 тк уже увеличили размер на 1
		new_array[i] = array[i];	//Копируем старые значения в новый массив с размером +1
	}
	new_array[size - 1] = new_element;	//В ячейку появившуюся после size+1 добавляем новый элемент
	delete[] array;	//Удаляем старую память
	array = new double[size];	//Выделяем новую память с размером + 1
	for (int i = 0; i < size; ++i) {
		array[i] = new_array[i];	//Копируем старые значения в новый массив с размером +1
	}
	delete[] new_array;
	return array;
}


int duplicate_count(double *array, int b, int size) {
	int result_counter = 0;
	bool* checked = new bool[size];	//создаем массив проверок для избежания повторного счета
	for (int i = 0; i < size; ++i)	//Все числа в массиве изначально непроверенные
		checked[i] = false;
	for (int i = 0; i < size; ++i) {
		if (!checked[i]) {	//Если число непроверенное
			int element_counter = 0;
			for (int j = i; j < size; ++j) {
				if (array[j] == array[i]) {	//Сверяем число с остальными
					element_counter++;	//Если совпадают, счетчик+1 и помечаем число проверенным
					checked[j] = true;
				}
			}
			if (element_counter > b)	//Поиск количества чисел больших b
				result_counter++;
		}
	}
	delete[] checked;
	return result_counter;
}


int main() {
	std::ifstream ifile("input.txt");
	std::ofstream ofile("output.txt");
	int size = 0, b, counter;
	double *arr = new double[size];
	b = get_b(ifile);
	double new_element;
	while (ifile >> new_element)
		arr = append(arr, size, new_element);
	counter = duplicate_count(arr, b, size);	//Функция по заданию
	ofile << counter;
	delete[] arr;
}