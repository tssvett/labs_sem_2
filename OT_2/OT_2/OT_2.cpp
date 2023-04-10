#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<iostream>
#include<locale.h>
#include<vector>

using namespace std;
const char VOWEL[] = { 'а','о','у','ы','э','е','ё','и','ю','я','А','О','У','Ы',
					   'Э','Е','Ё','И','Ю','Я' };
const char CONSONANT[] = { 'Б', 'В', 'Г', 'Д', 'Ж', 'З', 'Й', 'К', 'Л', 'М', 
						 'Н', 'П', 'Р', 'С', 'Т', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 
						 'Щ', 'Ь', 'Ъ','б', 'в', 'г', 'д', 'ж', 'з', 'й', 'к',
						 'л', 'м', 'н', 'п', 'р', 'с', 'т', 'ф', 'х', 'ц', 'ч', 
						 'ш', 'щ', 'ь', 'ъ' };
enum States { S, T, A, B, F, E };
enum Type { Capital_vowel, Capital_consonant, Vowel, Consonant, Border, Other };


bool is_vowel(char symbol) {
	for (long unsigned int i = 0; i < sizeof(VOWEL); ++i) {
		if (symbol == VOWEL[i])
			return 1;
	}
	return 0;
}


bool is_consonant(char symbol) {
	for (unsigned int i = 0; i < sizeof(CONSONANT); ++i) {
		if (symbol == CONSONANT[i])
			return 1;
	}
	return 0;
}


bool is_uppercase_rus(char symbol) {
	return ((symbol >= 'А' && symbol <= 'Я') || symbol == 'Ё' ? 1 : 0);
}


bool is_word_border(char symbol) {
	if (symbol == 'я') //Почему то при символе 'я' эта функция возвращает True
		return 0;	//Пофиксил
	else
		return (symbol == ' ' || symbol == '\n' || symbol == EOF
			|| symbol == '\t' || symbol == '\0' || symbol == '\0' 
			|| symbol == '\r' ? 1 : 0);
}


bool is_rus(char symbol) {
	return ((symbol >= 'А' && symbol <= 'я') || symbol == 'Ё'||symbol =='ё' ? 1 : 0);

}


void print(ofstream &ofile, vector<char*> result) {
	for (long unsigned int i = 0; i < result.size(); ++i) {
		cout << result[i] << ' ';
		ofile << result[i] << ' ';
	}
}


long long get_line_size(ifstream& ifile) {
	long long line_size;
	//Единичка	| нужна для того, чтобы учитывать терминальный символ \0 в конце строки
	ifile.seekg(1, ios_base::end);	// Установка указателя на конец файла
	line_size = ifile.tellg();	// Записываем в переменную количество символов
	ifile.seekg(0, ios_base::beg);	// Возвращаем обратно указатель на начало файла
	return line_size;

}


void filling_table(States(*table)[6]) { // Формируем матрицу состояний
	table[Capital_vowel][S] = T;
	table[Capital_consonant][S] = T;
	table[Vowel][S] = E;
	table[Consonant][S] = E;
	table[Border][S] = E;
	table[Other][S] = E;

	table[Capital_vowel][T] = A;
	table[Capital_consonant][T] = B;
	table[Vowel][T] = A;
	table[Consonant][T] = B;
	table[Border][T] = F;
	table[Other][T] = E;


	table[Capital_vowel][A] = A;
	table[Capital_consonant][A] = B;
	table[Vowel][A] = A;
	table[Consonant][A] = B;
	table[Border][A] = F;
	table[Other][A] = E;

	table[Capital_vowel][B] = A;
	table[Capital_consonant][B] = B;
	table[Vowel][B] = A;
	table[Consonant][B] = B;
	table[Border][B] = E;
	table[Other][B] = E;

	table[Capital_vowel][E] = E;
	table[Capital_consonant][E] = E;
	table[Vowel][E] = E;
	table[Consonant][E] = E;
	table[Border][E] = E;
	table[Other][E] = E;

	table[Capital_vowel][F] = F;
	table[Capital_consonant][F] = F;
	table[Vowel][F] = F;
	table[Consonant][F] = F;
	table[Border][F] = F;
	table[Other][F] = F;


}


char* get_word(char* lexema, char* line, int position, int lenght) {
	int index = 0;
	int lim = position + lenght;
	for (int k = position; k < lim; ++k) {
		lexema[index] = line[k];
		index++;
	}
	lexema[index] = '\0';
	return lexema;
}


Type get_type(char symbol) {
	if (is_uppercase_rus(symbol) && is_vowel(symbol))
		return Capital_vowel;
	else if (is_uppercase_rus(symbol) && is_consonant(symbol))
		return Capital_consonant;
	else if (is_vowel(symbol))
		return Vowel;
	else if (is_consonant(symbol))
		return Consonant;
	else if (is_word_border(symbol)) 
		return Border;
	else
		return Other;
}


void lexical_analysis(vector<char*>& result, char* const line, States(*table)[6], long long line_size) {
	int index = 0;
	States cur_state = States::S;
	char* lexema_text;
	int left_border = index;
	while (index != line_size) {	// Проходимся по строке
		char current_symbol = line[index];
		cur_state = table[get_type(current_symbol)][cur_state];	// Меняем текущее стостояние
		if (cur_state == States::E && is_word_border(current_symbol)) {	// Пропускаем слово 
			//если ошибочное состояние
			left_border = index + 1; // +1 т.к пропускаем пробел
			cur_state = States::S;	//Обновляем состояние
		}
		if (cur_state == States::F) {	// Если состояние выхода, записываем слово
			int word_lenght = index - left_border;	//Находим длину найденного слова
			lexema_text = new char[word_lenght + 1];	// Выделяем память под это слово + 1 символ \0
			lexema_text = get_word(lexema_text, line, left_border, word_lenght);	// Формируем слово
			result.push_back(lexema_text);	//Записываем его
			left_border = index + 1;	// Двигаем левую границу
			cur_state = States::S;	//Обновляем состояние
		}
		index++;
	}
}


int main() {
	setlocale(LC_ALL, "Russian");
	ifstream ifile("input.txt", ios::binary);	//замена \n на \t\n отсутствует из-за binary
	ofstream ofile("output.txt");
	vector<char*> words;
	long long line_size;
	line_size = get_line_size(ifile);	// Считываем длину строки
	char* line = new char[line_size];	// Выделяем память под строку (+1 учтен в функции)
	ifile.getline(line, line_size, '\0');	// Записываем в line текст из файла
	States table_of_states[6][6];
	filling_table(table_of_states);		// Заполняем табличку
	lexical_analysis(words, line, table_of_states, line_size);	// записываем в вектор слова по заданию
	print(ofile, words);	// Выводим результат
	delete[] line;
	ifile.close();
	ofile.close();	// Очищаем все грехи
	return 0;
}