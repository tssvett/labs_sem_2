#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<iostream>
#include <cstring>
#include<vector>
using namespace std;
/*
vl - константы
co_l - <
co_r - >
mi - минус
pl - плюс
eq - присваивание
id - идентификаторы(переменные)
wl - некорректная лексема
sel - ключевое слово select
cas - ключевое слово case
def - ключевое слово default
en - ключевое слово end
*/

//Множество типов лексем
enum Lexema_Types { vl, co_l, co_r, mi, pl, eq, id, wl, sel, cas, def, en };

//Структура лексемы
struct Lex {
	char* text;
	Lexema_Types type;
};

//Класс лексического анализатора
class LexicalAnalyzer {
private:

	enum States { S, VL, CO_L, CO_R, NOT_CO, AO, EQ, ID, WL, F };
	enum Symbol_Types { wo_b, vl, co_l, co_r, ao, eq, id, wl, kw };
	States table[8][10];


public:

	//Заполнение матриц состояний для лексического анализа
	void form_table() {
		table[wo_b][S] = S;
		table[vl][S] = VL;
		table[co_l][S] = CO_L;
		table[co_r][S] = CO_R;
		table[ao][S] = AO;
		table[eq][S] = EQ;
		table[id][S] = ID;
		table[wl][S] = WL;

		table[wo_b][ID] = F;
		table[vl][ID] = ID;
		table[co_l][ID] = F;
		table[co_r][ID] = F;
		table[ao][ID] = F;
		table[eq][ID] = F;
		table[id][ID] = ID;
		table[wl][ID] = WL;

		table[wo_b][VL] = F;
		table[vl][VL] = VL;
		table[co_l][VL] = F;
		table[co_r][VL] = F;
		table[ao][VL] = F;
		table[eq][VL] = F;
		table[id][VL] = WL;
		table[wl][VL] = WL;

		table[wo_b][CO_L] = F;
		table[vl][CO_L] = F;
		table[co_l][CO_L] = F;
		table[co_r][CO_L] = NOT_CO;
		table[ao][CO_L] = F;
		table[eq][CO_L] = NOT_CO;
		table[id][CO_L] = F;
		table[wl][CO_L] = WL;

		table[wo_b][CO_R] = F;
		table[vl][CO_R] = F;
		table[co_l][CO_R] = F;
		table[co_r][CO_R] = F;
		table[ao][CO_R] = F;
		table[eq][CO_R] = NOT_CO;
		table[id][CO_R] = F;
		table[wl][CO_R] = F;

		table[wo_b][NOT_CO] = F;
		table[vl][NOT_CO] = F;
		table[co_l][NOT_CO] = F;
		table[co_r][NOT_CO] = F;
		table[ao][NOT_CO] = F;
		table[eq][NOT_CO] = F;
		table[id][NOT_CO] = F;
		table[wl][NOT_CO] = F;

		table[wo_b][AO] = F;
		table[vl][AO] = F;
		table[co_l][AO] = F;
		table[co_r][AO] = F;
		table[ao][AO] = F;
		table[eq][AO] = F;
		table[id][AO] = F;
		table[wl][AO] = F;

		table[wo_b][EQ] = F;
		table[vl][EQ] = F;
		table[co_l][EQ] = F;
		table[co_r][EQ] = F;
		table[ao][EQ] = F;
		table[eq][EQ] = F;
		table[id][EQ] = F;
		table[wl][EQ] = WL;

		table[wo_b][WL] = F;
		table[vl][WL] = WL;
		table[co_l][WL] = WL;
		table[co_r][WL] = WL;
		table[ao][WL] = WL;
		table[eq][WL] = WL;
		table[id][WL] = WL;
		table[wl][WL] = WL;

		table[wo_b][F] = S;
		table[vl][F] = F;
		table[co_l][F] = F;
		table[co_r][F] = F;
		table[ao][F] = F;
		table[eq][F] = F;
		table[id][F] = F;
		table[wl][F] = F;
	}

	//Функция, которая применяет проверки на символ. Возвращает тип символа.
	Symbol_Types get_type(char symbol) {
		if (is_num(symbol))
			return Symbol_Types::vl;
		else if (is_co_l(symbol))
			return Symbol_Types::co_l;
		else if (is_co_r(symbol))
			return Symbol_Types::co_r;
		else if (is_eq(symbol))
			return Symbol_Types::eq;
		else if (is_ao(symbol))
			return Symbol_Types::ao;
		else if (is_word_border(symbol))
			return Symbol_Types::wo_b;
		else if (is_eng(symbol))
			return Symbol_Types::id;
		else
			return Symbol_Types::wl;
	}

	//Функция, которая преобразует прошлое состояние автомата в тип лексемы. Возвращает тип лексемы.
	Lexema_Types states_to_types(char* text, const States state) {
		size_t word_lenght = strlen(text);
		long long number = atoi(text);
		if (state == ID && !strcmp(text, "select"))
			return Lexema_Types::sel;
		else if (state == ID && !strcmp(text, "case"))
			return Lexema_Types::cas;
		else if (state == ID && !strcmp(text, "default"))
			return Lexema_Types::def;
		else if (state == ID && !strcmp(text, "end"))
			return Lexema_Types::en;
		else if (state == ID && word_lenght > 5)
			return Lexema_Types::wl;
		else if (state == ID)
			return Lexema_Types::id;
		else if (state == VL && number >= 0 && number <= 32768)
			return Lexema_Types::vl;
		else if (state == CO_L || state == NOT_CO)
			return Lexema_Types::co_l;
		else if (state == CO_R)
			return Lexema_Types::co_r;
		else if (state == AO && !strcmp(text, "-"))
			return Lexema_Types::mi;
		else if (state == AO && !strcmp(text, "+"))
			return Lexema_Types::pl;
		else if (state == EQ)
			return Lexema_Types::eq;
		else
			return Lexema_Types::wl;
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

	//Функция Лексического анализа
	void analysis(char* const line, vector<Lex> &words) {
		int index = 0;
		States cur_state = States::S;
		int left_border = index;
		long long line_size = strlen(line);	//не забываем про \0
		while (index != line_size + 1) {	// Проходимся по строке
			States previous_state = cur_state;
			char current_symbol = line[index];
			cur_state = table[get_type(current_symbol)][previous_state];	// Меняем текущее стостояние
			if (cur_state == States::F) {	// Если состояние выхода, записываем слово
				Lex lexema;
				int word_lenght = index - left_border;
				lexema.text = new char[word_lenght + 1];	// Выделяем память под это слово + 1 символ \0
				lexema.text = get_word(lexema.text, line, left_border, word_lenght);	// Формируем слово
				lexema.type = states_to_types(lexema.text, previous_state);
				words.push_back(lexema);	//Записываем его
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

	//Проверка на граничный символ
	bool is_word_border(const char symbol) {
		return (symbol == ' ' || symbol == '-' || symbol == '>' || symbol == '<' || symbol == '=' ||
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
};

//Класс синтаксического анализатора
class SyntaxAnalyzer {

private:

	enum States { S, SEL, OP, CAS, MIN, MINC, VL, ID,SEL_2, OP_2, CAS_2,
		MIN_2, MINC_2, VL_2, ID_2,SEL_3, OP_3, MIN_3, VL_3, ID_3, SEL_4,
		OP_4, MIN_4, F, E };
	States table[12][25];

public:

	//Метод формирования матрицы перехода
	void form_table() {
		table[sel][S] = SEL;
		table[cas][S] = E;
		table[def][S] = E;
		table[en][S] = E;
		table[vl][S] = E;
		table[id][S] = E;
		table[co_l][S] = E;
		table[co_r][S] = E;
		table[mi][S] = E;
		table[pl][S] = E;
		table[eq][S] = E;
		table[wl][S] = E;

		table[sel][SEL] = E;
		table[cas][SEL] = E;
		table[def][SEL] = E;
		table[en][SEL] = E;
		table[vl][SEL] = OP;
		table[id][SEL] = OP;
		table[co_l][SEL] = E;
		table[co_r][SEL] = E;
		table[mi][SEL] = MIN;
		table[pl][SEL] = E;
		table[eq][SEL] = E;
		table[wl][SEL] = E;

		table[sel][OP] = E;
		table[cas][OP] = CAS;
		table[def][OP] = E;
		table[en][OP] = E;
		table[vl][OP] = E;
		table[id][OP] = E;
		table[co_l][OP] = E;
		table[co_r][OP] = E;
		table[mi][OP] = SEL;
		table[pl][OP] = SEL;
		table[eq][OP] = E;
		table[wl][OP] = E;

		table[sel][CAS] = E;
		table[cas][CAS] = E;
		table[def][CAS] = E;
		table[en][CAS] = E;
		table[vl][CAS] = VL;
		table[id][CAS] = E;
		table[co_l][CAS] = E;
		table[co_r][CAS] = E;
		table[mi][CAS] = MINC;
		table[pl][CAS] = E;
		table[eq][CAS] = E;
		table[wl][CAS] = E;

		table[sel][MIN] = E;
		table[cas][MIN] = E;
		table[def][MIN] = E;
		table[en][MIN] = E;
		table[vl][MIN] = OP;
		table[id][MIN] = OP;
		table[co_l][MIN] = E;
		table[co_r][MIN] = E;
		table[mi][MIN] = E;
		table[pl][MIN] = E;
		table[eq][MIN] = E;
		table[wl][MIN] = E;

		table[sel][MINC] = E;
		table[cas][MINC] = E;
		table[def][MINC] = E;
		table[en][MINC] = E;
		table[vl][MINC] = VL;
		table[id][MINC] = VL;
		table[co_l][MINC] = E;
		table[co_r][MINC] = E;
		table[mi][MINC] = E;
		table[pl][MINC] = E;
		table[eq][MINC] = E;
		table[wl][MINC] = E;

		table[sel][VL] = E;
		table[cas][VL] = E;
		table[def][VL] = E;
		table[en][VL] = E;
		table[vl][VL] = E;
		table[id][VL] = ID;
		table[co_l][VL] = E;
		table[co_r][VL] = E;
		table[mi][VL] = E;
		table[pl][VL] = E;
		table[eq][VL] = E;
		table[wl][VL] = E;

		table[sel][ID] = E;
		table[cas][ID] = E;
		table[def][ID] = E;
		table[en][ID] = E;
		table[vl][ID] = E;
		table[id][ID] = E;
		table[co_l][ID] = E;
		table[co_r][ID] = E;
		table[mi][ID] = E;
		table[pl][ID] = E;
		table[eq][ID] = SEL_2;
		table[wl][ID] = E;

		table[sel][SEL_2] = E;
		table[cas][SEL_2] = E;
		table[def][SEL_2] = E;
		table[en][SEL_2] = E;
		table[vl][SEL_2] = OP_2;
		table[id][SEL_2] = OP_2;
		table[co_l][SEL_2] = E;
		table[co_r][SEL_2] = E;
		table[mi][SEL_2] = MIN_2;
		table[pl][SEL_2] = E;
		table[eq][SEL_2] = E;
		table[wl][SEL_2] = E;

		table[sel][OP_2] = E;
		table[cas][OP_2] = CAS_2;
		table[def][OP_2] = VL_3;
		table[en][OP_2] = F;
		table[vl][OP_2] = E;
		table[id][OP_2] = E;
		table[co_l][OP_2] = E;
		table[co_r][OP_2] = E;
		table[mi][OP_2] = SEL_2;
		table[pl][OP_2] = SEL_2;
		table[eq][OP_2] = E;
		table[wl][OP_2] = E;

		table[sel][CAS_2] = E;
		table[cas][CAS_2] = E;
		table[def][CAS_2] = E;
		table[en][CAS_2] = E;
		table[vl][CAS_2] = VL_2;
		table[id][CAS_2] = E;
		table[co_l][CAS_2] = E;
		table[co_r][CAS_2] = E;
		table[mi][CAS_2] = MINC_2;
		table[pl][CAS_2] = E;
		table[eq][CAS_2] = E;
		table[wl][CAS_2] = E;

		table[sel][MIN_2] = E;
		table[cas][MIN_2] = E;
		table[def][MIN_2] = E;
		table[en][MIN_2] = E;
		table[vl][MIN_2] = OP_2;
		table[id][MIN_2] = OP_2;
		table[co_l][MIN_2] = E;
		table[co_r][MIN_2] = E;
		table[mi][MIN_2] = E;
		table[pl][MIN_2] = E;
		table[eq][MIN_2] = E;
		table[wl][MIN_2] = E;

		table[sel][MINC_2] = E;
		table[cas][MINC_2] = E;
		table[def][MINC_2] = E;
		table[en][MINC_2] = E;
		table[vl][MINC_2] = VL_2;
		table[id][MINC_2] = VL_2;
		table[co_l][MINC_2] = E;
		table[co_r][MINC_2] = E;
		table[mi][MINC_2] = E;
		table[pl][MINC_2] = E;
		table[eq][MINC_2] = E;
		table[wl][MINC_2] = E;

		table[sel][VL_2] = E;
		table[cas][VL_2] = E;
		table[def][VL_2] = E;
		table[en][VL_2] = E;
		table[vl][VL_2] = E;
		table[id][VL_2] = ID_2;
		table[co_l][VL_2] = E;
		table[co_r][VL_2] = E;
		table[mi][VL_2] = E;
		table[pl][VL_2] = E;
		table[eq][VL_2] = E;
		table[wl][VL_2] = E;

		table[sel][ID_2] = E;
		table[cas][ID_2] = E;
		table[def][ID_2] = E;
		table[en][ID_2] = E;
		table[vl][ID_2] = E;
		table[id][ID_2] = E;
		table[co_l][ID_2] = E;
		table[co_r][ID_2] = E;
		table[mi][ID_2] = E;
		table[pl][ID_2] = E;
		table[eq][ID_2] = SEL_3;
		table[wl][ID_2] = E;

		table[sel][SEL_3] = E;
		table[cas][SEL_3] = E;
		table[def][SEL_3] = E;
		table[en][SEL_3] = E;
		table[vl][SEL_3] = OP_3;
		table[id][SEL_3] = OP_3;
		table[co_l][SEL_3] = E;
		table[co_r][SEL_3] = E;
		table[mi][SEL_3] = MIN_3;
		table[pl][SEL_3] = E;
		table[eq][SEL_3] = E;
		table[wl][SEL_3] = E;

		table[sel][OP_3] = E;
		table[cas][OP_3] = E;
		table[def][OP_3] = VL_3;
		table[en][OP_3] = F;
		table[vl][OP_3] = E;
		table[id][OP_3] = E;
		table[co_l][OP_3] = E;
		table[co_r][OP_3] = E;
		table[mi][OP_3] = SEL_3;
		table[pl][OP_3] = SEL_3;
		table[eq][OP_3] = E;
		table[wl][OP_3] = E;

		table[sel][MIN_3] = E;
		table[cas][MIN_3] = E;
		table[def][MIN_3] = E;
		table[en][MIN_3] = E;
		table[vl][MIN_3] = OP_3;
		table[id][MIN_3] = OP_3;
		table[co_l][MIN_3] = E;
		table[co_r][MIN_3] = E;
		table[mi][MIN_3] = E;
		table[pl][MIN_3] = E;
		table[eq][MIN_3] = E;
		table[wl][MIN_3] = E;

		table[sel][VL_3] = E;
		table[cas][VL_3] = E;
		table[def][VL_3] = E;
		table[en][VL_3] = E;
		table[vl][VL_3] = E;
		table[id][VL_3] = ID_3;
		table[co_l][VL_3] = E;
		table[co_r][VL_3] = E;
		table[mi][VL_3] = E;
		table[pl][VL_3] = E;
		table[eq][VL_3] = E;
		table[wl][VL_3] = E;

		table[sel][ID_3] = E;
		table[cas][ID_3] = E;
		table[def][ID_3] = E;
		table[en][ID_3] = E;
		table[vl][ID_3] = E;
		table[id][ID_3] = E;
		table[co_l][ID_3] = E;
		table[co_r][ID_3] = E;
		table[mi][ID_3] = E;
		table[pl][ID_3] = E;
		table[eq][ID_3] = SEL_4;
		table[wl][ID_3] = E;

		table[sel][SEL_4] = E;
		table[cas][SEL_4] = E;
		table[def][SEL_4] = E;
		table[en][SEL_4] = E;
		table[vl][SEL_4] = OP_4;
		table[id][SEL_4] = OP_4;
		table[co_l][SEL_4] = E;
		table[co_r][SEL_4] = E;
		table[mi][SEL_4] = MIN_4;
		table[pl][SEL_4] = E;
		table[eq][SEL_4] = E;
		table[wl][SEL_4] = E;

		table[sel][OP_4] = E;
		table[cas][OP_4] = E;
		table[def][OP_4] = E;
		table[en][OP_4] = F;
		table[vl][OP_4] = E;
		table[id][OP_4] = E;
		table[co_l][OP_4] = E;
		table[co_r][OP_4] = E;
		table[mi][OP_4] = SEL_4;
		table[pl][OP_4] = SEL_4;
		table[eq][OP_4] = E;
		table[wl][OP_4] = E;

		table[sel][MIN_4] = E;
		table[cas][MIN_4] = E;
		table[def][MIN_4] = E;
		table[en][MIN_4] = E;
		table[vl][MIN_4] = OP_4;
		table[id][MIN_4] = OP_4;
		table[co_l][MIN_4] = E;
		table[co_r][MIN_4] = E;
		table[mi][MIN_4] = E;
		table[pl][MIN_4] = E;
		table[eq][MIN_4] = E;
		table[wl][MIN_4] = E;

		table[sel][F] = E;
		table[cas][F] = E;
		table[def][F] = E;
		table[en][F] = E;
		table[vl][F] = E;
		table[id][F] = E;
		table[co_l][F] = E;
		table[co_r][F] = E;
		table[mi][F] = E;
		table[pl][F] = E;
		table[eq][F] = E;
		table[wl][F] = E;

		table[sel][E] = E;
		table[cas][E] = E;
		table[def][E] = E;
		table[en][E] = E;
		table[vl][E] = E;
		table[id][E] = E;
		table[co_l][E] = E;
		table[co_r][E] = E;
		table[mi][E] = E;
		table[pl][E] = E;
		table[eq][E] = E;
		table[wl][E] = E;
	}

	//Синтаксический анализ вектора лексем
	void analysis(vector<Lex>& words, ofstream& ofile) {
		States current_state = S; States previous_state = S;
		size_t i;
		//Непосредственно алгоритм синтаксического анализа
		for (i = 0; i < words.size(); ++i) {
			current_state = table[words[i].type][current_state];
			if (current_state == E) {
				error_throw(i, previous_state, ofile);
				break;
			}
			if (current_state == F) {
				current_state = S;
				OK_throw(ofile);
			}
			previous_state = current_state;
		}
		//Проверка на пустой файлик
		if (current_state == S && !i)
			error_throw(i, previous_state, ofile);
		else if( current_state != E  && current_state != S)
			error_throw(i, previous_state, ofile);
	}

	//Запись в файл ОК
	void OK_throw(ofstream& ofile){
		cout << "OK" << endl;
		ofile << "OK" << endl;
	}

	//Запись в файла места ошибки и типа лексемы, который должен быть
	void error_throw(size_t i, States previous_state, ofstream& ofile) {
		const char* error_message[] = { "se", "id vl", "ao ca", "vl", "id vl", "vl", "id", "eq", "id vl", "ao ca de en",
			"vl", "id vl", "vl", "id", "eq", "id vl", "ao de en", "idk", "id", "eq", "id vl", "ao en" };
		cout << i << " " << error_message[previous_state];
		ofile << i << " " << error_message[previous_state];
	}

};

//Функция, которая выводит полученные вектора в консоль и в файл
void print(vector<Lex>& words, ofstream& ofile) {
	//Необходимо для преобразование порядкого номера из перечесление в выводную лексему
	const char* lexical_output[] = { "[vl]", "[co]","[co]", "[ao]", "[ao]", "[eq]", "[id]", "[wl]", "[se]", "[ca]", "[de]", "[en]" };
	for (long unsigned int i = 0; i < words.size(); i++) {
		cout << words[i].text << lexical_output[words[i].type] << ' ';
		ofile << words[i].text << lexical_output[words[i].type] << ' ';
	}
	cout << endl;
	ofile << endl;
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

//Функция, которая удаляет текст лексем внутри векторов, который мы выделяли при помощи new
void vector_delete(vector<Lex>& vector) {
	for (long unsigned int i = 0; i < vector.size(); i++) {
		delete[] vector[i].text;
	}
}

int main() {
	ifstream ifile("input.txt", ios::binary);	//замена \n на \t\n отсутствует из-за binary
	ofstream ofile("output.txt");
	vector<Lex> words;	//Он вне класса тк необходимо работать с вектором по ссылке или указателю
	//Работа с файлом
	char* line;
	long long line_size = get_line_size(ifile);
	line = new char[line_size];
	ifile.getline(line, line_size, '\0');
	//Работа лексического анализатора
	LexicalAnalyzer lexical;
	lexical.form_table();
	lexical.analysis(line, words);
	//Работа синтаксического анализатора
	SyntaxAnalyzer syntax;
	print(words, ofile);
	syntax.form_table();
	syntax.analysis(words, ofile);
	//Очищение памяти
	delete[] line; 
	ifile.close();
	ofile.close();	
	vector_delete(words);
	return 0;
}
