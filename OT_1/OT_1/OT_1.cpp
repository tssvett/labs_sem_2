#include<fstream>
#include<iostream>
#include<locale.h>
#include<cctype>
#include<vector>
/*	Вариант 16. Словом текста считается любая последовательность букв русского алфавита. 
Найти те слова, которые начинаются с прописной буквы и заканчиваются на гласную 
(слово из одной буквы удовлетворяет условию).	*/

using namespace std;
const int VOWEL_SIZE = 22;
char VOWEL[VOWEL_SIZE] = { 'а', 'у', 'о', 'ы', 'и', 'э', 'я', 'ю', 'ё', 'е',
							'А', 'У', 'О', 'Ы', 'И', 'Э', 'Я', 'Ю', 'Ё', 'Е'};


char* append(char *&array, int& size, char new_element) {	//Добавляем элемент в массив
	size++;
	char *new_array = new char[size];
	for (int i = 0; i < size - 1; i++) {	//size -1 тк уже увеличили размер на 1
		new_array[i] = array[i];	//Копируем старые значения в новый массив с размером +1
	}
	new_array[size - 1] = new_element;	//В ячейку появившуюся после size+1 добавляем новый элемент
	delete[] array;	//Удаляем старую память
	array = new char[size];	//Выделяем новую память с размером + 1
	for (int i = 0; i < size; i++) {
		array[i] = new_array[i];	//Копируем старые значения в новый массив с размером +1
	}
	delete[] new_array;
	return array;
}


bool is_vowel(char symbol) {
	int counter = 0;
	for (int i = 0; i < VOWEL_SIZE; ++i) {
		if (symbol == VOWEL[i]) 
			counter++;
	}
	if (counter > 0)
		return 1;
	else
		return 0;
}


bool is_uppercase_rus(char symbol) {
	return (symbol >= 'А' && symbol <= 'Я' ? 1 : 0);
}
 

bool is_up_vowel(char symbol, int line_size) {
	return (line_size == 1 && is_uppercase_rus(symbol) && is_vowel(symbol) ? 1 : 0);
}


bool is_word_border(char symbol) {
	if (symbol == 'я') //Почему то при символе 'я' эта функция возвращает True
		return 0;	//Пофиксил
	else
		return (symbol == ' ' || symbol == '\n' || symbol == EOF || symbol == '\t'|| symbol == '\0' ? 1 : 0);
}


vector<char*> find(char* const line, int line_size) {
	vector<char*> result;
	int i = 0; 
	char next_symbol;
	while (i < line_size) {
		int left = i; int right = 0;
		int delete_count = 0;
		int size = 0; int word_size = 0;
		char* word = new char[size];
		char symbol = line[left];
		if (is_up_vowel(symbol, line_size)) {	// Проверка одиночного символа на заглавность и гласность
			word = append(word, size, symbol);
			word = append(word, size, '\0');
			result.push_back(word);
			break;
		}
		while (!is_uppercase_rus(symbol)) { //Определяем левую границу слова
			symbol = line[++left];	//пока символ не заглавный двигаем левую границу вправо
			delete_count++;
		}
		right = left;
		symbol = line[right];
		next_symbol = line[right + 1];
		while (!is_word_border(next_symbol)) {	//Определяем правую границу слова
			if (right < line_size) {	// Пока символ не пограничный двигаем правую границу вправо
				symbol = line[right++];
				next_symbol = line[right];
				delete_count++;
			}
			else
				break;	//Если вышли за границу строки выходим из цикла
		}
		if (right <= line_size)	//тк граница останавливается на пограничном символе
			right -= 1;	//нужно сдвинуть на 1 влево для указания на последнюю букву слова
		if (is_uppercase_rus(line[left]) && is_vowel(line[right]) && right < line_size) {
			for (int j = left; j <= right; j++, word_size++) {	//Проверяем слово на удовлетворение варианту
				word = append(word, size, line[j]);	//Если соответствует, собираем слово
			}
			word = append(word, size, '\0');	//Добавляем конечный символ
			result.push_back(word);	// И добавляем в список слов
		}
		i += delete_count;

	}
	return result;
}


void print(ofstream &ofile, vector<char*> result) {
	for (long unsigned int i = 0; i < result.size(); ++i) {
		cout << result[i] << ' ';
	}
	for (long unsigned int i = 0; i < result.size(); ++i) {
		ofile << result[i] << ' ';
	}
}


int main() {
	setlocale(LC_ALL, "Russian");
	int line_size = 0; char new_element;
	ifstream ifile("input.txt"); ofstream ofile("output.txt");
	vector<char*> words;
	char* line = new char[line_size];
	while (ifile >> noskipws >> new_element)	//Считываем все что есть в файле
		line = append(line, line_size, new_element);	//noskipws позволяет считывать даже с пробелами 
	//и концами строк
	words = find(line, line_size);
	print(ofile, words);
	delete[] line;
	ifile.close();
}