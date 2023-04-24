#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<iostream>
#include <cstring>
#include<vector>

using namespace std;
/*
wb- граница слова
vl - константы
co_l - <
co_r - >
ao - арифметические операции
eq - присваивание
id - идентификаторы(переменные)
wl - некорректная лексема
kw - ключевое слово
*/
enum Type {wb, vl, co_l,co_r, ao, eq, id, wl, kw };
//Необходимо для преобразование порядкого номера из перечесление в выводную лексему
const char* print_types[] = { "[wb]", "[vl]", "[co]","[co]", "[ao]", "[eq]", "[id]", "[wl]", "[kw]" };
//Состояния
enum States { S, VL, CO_L,  CO_R, NOT_CO, AO, EQ, ID, WL, F };

//Структура лексемы
struct Lex {
	char* text;
	Type type;
};

void filling_table(States table[8][10]) { // Формируем матрицу состояний
	table[wb][S] = S;
	table[vl][S] = VL;
	table[co_l][S] = CO_L;
	table[co_r][S] = CO_R;
	table[ao][S] = AO;
	table[eq][S] = EQ;
	table[id][S] = ID;
	table[wl][S] = WL;

	table[wb][ID] = F;
	table[vl][ID] = ID;
	table[co_l][ID] = F;
	table[co_r][ID] = F;
	table[ao][ID] = F;
	table[eq][ID] = F;
	table[id][ID] = ID;
	table[wl][ID] = WL;

	table[wb][VL] = F;
	table[vl][VL] = VL;
	table[co_l][VL] = F;
	table[co_r][VL] = F;
	table[ao][VL] = F;
	table[eq][VL] = F;
	table[id][VL] = WL;
	table[wl][VL] = WL;

	table[wb][CO_L] = F;
	table[vl][CO_L] = F;
	table[co_l][CO_L] = F;
	table[co_r][CO_L] = NOT_CO;
	table[ao][CO_L] = F;
	table[eq][CO_L] = NOT_CO;
	table[id][CO_L] = F;
	table[wl][CO_L] = WL;

	table[wb][CO_R] = F;
	table[vl][CO_R] = F;
	table[co_l][CO_R] = F;
	table[co_r][CO_R] = F;
	table[ao][CO_R] = F;
	table[eq][CO_R] = NOT_CO;
	table[id][CO_R] = F;
	table[wl][CO_R] = F;

	table[wb][NOT_CO] = F;
	table[vl][NOT_CO] = F;
	table[co_l][NOT_CO] = F;
	table[co_r][NOT_CO] = F;
	table[ao][NOT_CO] = F;
	table[eq][NOT_CO] = F;
	table[id][NOT_CO] = F;
	table[wl][NOT_CO] = F;

	table[wb][AO] = F;
	table[vl][AO] = F;
	table[co_l][AO] = F;
	table[co_r][AO] = F;
	table[ao][AO] = F;
	table[eq][AO] = F;
	table[id][AO] = F;
	table[wl][AO] = WL;

	table[wb][EQ] = F;
	table[vl][EQ] = F;
	table[co_l][EQ] = F;
	table[co_r][EQ] = F;
	table[ao][EQ] = F;
	table[eq][EQ] = F;
	table[id][EQ] = F;
	table[wl][EQ] = WL;

	table[wb][WL] = F;
	table[vl][WL] = WL;
	table[co_l][WL] = WL;
	table[co_r][WL] = WL;
	table[ao][WL] = WL;
	table[eq][WL] = WL;
	table[id][WL] = WL;
	table[wl][WL] = WL;

	table[wb][F] = S;
	table[vl][F] = F;
	table[co_l][F] = F;
	table[co_r][F] = F;
	table[ao][F] = F;
	table[eq][F] = F;
	table[id][F] = F;
	table[wl][F] = F;
}

//Проверка на граничный символ
bool is_word_border(const char symbol) {
	return (symbol == ' ' || symbol == '-' || symbol == '>' || symbol == '<' || symbol == '='||
		symbol == ' ' || symbol == '\n' || symbol == EOF || symbol == '\t'
		|| symbol == '\0' || symbol == '\0' || symbol == '\r' ? 1 : 0);
}

//Проверка на английский алфавит
bool is_eng(const char symbol) {
	return ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z')) ? 1 : 0;
}

//Проверка на цифру
bool is_num(const char symbol) {
	return (symbol >= '0' && symbol <= '9') ? 1 : 0;
}

//Проверка на <
bool is_co_l(const char symbol) {
	return (symbol == '<') ? 1 : 0;
}

//Проверка на >
bool is_co_r(const char symbol) {
	return (symbol == '>') ? 1 : 0;
}

//Проверка на =
bool is_eq(const char symbol) {
	return (symbol == '=') ? 1 : 0;
}

//Проверка на арифметические операции
bool is_ao(const char symbol) {
	return (symbol == '+' || symbol == '-') ? 1 : 0;

}

//Проверка на ключевые слова
bool is_key_word(char* word) {
	return (strcmp(word, "select") && strcmp(word, "case") && strcmp(word, "default")
		&& strcmp(word, "end")) ? 0 : 1;
	//Здесь возвращаем 0:1, а не наоборот, потому что strcmp() возвращает 0, если строки равны
}

//Функция, которая применяет проверки на символ. Возвращает тип символа.
Type get_type(char symbol) {
	if (is_num(symbol))
		return vl;
	else if (is_co_l(symbol)) {
		return co_l;
	}
	else if (is_co_r(symbol)) {
		return co_r;
	}
	else if (is_eq(symbol))
		return eq;
	else if (is_ao(symbol))
		return ao;
	else if (is_word_border(symbol))
		return wb;
	else if (is_eng(symbol))
		return id;
		else
			return wl;
}

//Функция, которая возвращает размера файла
long long get_line_size(ifstream& ifile) {
	long long line_size;
	//Единичка	| нужна для того, чтобы учитывать терминальный символ \0 в конце строки
	ifile.seekg(1, ios_base::end);	// Установка указателя на конец файла
	line_size = ifile.tellg();	// Записываем в переменную количество символов
	ifile.seekg(0, ios_base::beg);	// Возвращаем обратно указатель на начало файла
	return line_size;
}

//Функция, которая возвращает текст лексемы из файла
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

//Функция, которая преобразует прошлое состояние автомата в тип лексемы. Возвращает тип лексемы.
Type states_to_types(char* text, const States state) {
	int word_lenght = strlen(text);
	long long number = atoi(text);
	if (state == ID && is_key_word(text))
		return kw;
	else if (state == ID && word_lenght > 5)
		return wl;
	else if (state == ID)
		return id;
	else if (state == VL && number >= -32768 && number <= 32767)
		return vl;
	else if (state == CO_L || state == NOT_CO)
		return co_l;
	else if (state == CO_R)
		return co_r;
	else if (state == AO)
		return ao;
	else if (state == EQ)
		return eq;
	else 
		return wl;
}

//Функция Лексического анализа
//lexems заполняется списком лексем с их [типом]
//ids заполняется списком идентификаторов
//vals заполняется списком констант
void lexical_analysis(vector<Lex>& lexems, vector<Lex>& ids, vector<Lex>& vals, 
	char* const line, States table[8][10]) {
	int index = 0;
	States cur_state = States::S;
	int left_border = index;
	long long line_size = strlen(line) + 1;	//не забываем про \0
	while (index != line_size) {	// Проходимся по строке
		States previous_state = cur_state;
		char current_symbol = line[index];
		cur_state = table[get_type(current_symbol)][previous_state];	// Меняем текущее стостояние
		if (cur_state == States::F) {	// Если состояние выхода, записываем слово
			Lex lexema;
			int word_lenght = index - left_border;
			lexema.text = new char[word_lenght + 1];	// Выделяем память под это слово + 1 символ \0
			lexema.text = get_word(lexema.text, line, left_border, word_lenght);	// Формируем слово
			lexema.type = states_to_types(lexema.text, previous_state);
			lexems.push_back(lexema);	//Записываем его
			if (lexema.type == id)
				ids.push_back(lexema);
			if (lexema.type == vl)
				vals.push_back(lexema);
			cur_state = table[get_type(current_symbol)][States::S];
			//После того, как нашли лексему мы ставим состояние текущего символа из S,
			//Чтобы избежать ситуаций, при которых несколько символов стоят без пробелов(a=3)
			//Иначе мы просто теряем символ
			left_border = index;
		}
		index++;
		//Проверка после index++ потому что необходимо записывать лексемы без пограничных символов 	
		if (cur_state == States::S)
			left_border = index;
		
	}
}

//Функция, которая выводит полученные вектора в консоль и в файл
void print(vector<Lex>& words, vector<Lex>& ids, vector<Lex>& vals, ofstream& ofile) {
	for (long unsigned int i = 0; i < words.size(); i++) {
		cout << words[i].text << print_types[words[i].type] << ' ';
		ofile << words[i].text << print_types[words[i].type] << ' ';
	}
	cout << endl;
	ofile << endl;
	for (long unsigned int i = 0; i < ids.size(); i++) {
		cout << ids[i].text << ' ';
		ofile << ids[i].text << ' ';
	}
	cout << endl;
	ofile << endl;
	for (long unsigned int i = 0; i < vals.size(); i++) {
		cout << vals[i].text << ' ';
		ofile << vals[i].text << ' ';
	}
}

//Функция, которая удаляет текст лексем внутри векторов, который мы выделяли при помощи new
//Устранение утечек памяти
void vector_delete(vector<Lex>& vector) {
	for (long unsigned int i = 0; i < vector.size(); i++) {
		delete[] vector[i].text;
	}
}

int main() {
	ifstream ifile("input.txt", ios::binary);	//замена \n на \t\n отсутствует из-за binary
	ofstream ofile("output.txt");
	vector<Lex> words; vector<Lex> ids; vector<Lex> vals;
	States table_of_states[8][10];
	char* line;
	unsigned int line_size = get_line_size(ifile);
	line = new char[line_size];	// Выделяем память под строку (+1 учтен в функции)
	ifile.getline(line, line_size, '\0');	// Записываем в line текст из файла
	filling_table(table_of_states);		// Заполняем табличку
	lexical_analysis(words,ids, vals, line, table_of_states);	// Лексический анализ
	print(words, ids, vals, ofile);	//Вывод анализа
	delete[] line;
	ifile.close();
	ofile.close();	// Очищаем все грехи
	vector_delete(words);
	words.clear();
	ids.clear();
	vals.clear();
	return 0;
}