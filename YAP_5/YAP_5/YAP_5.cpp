#include<iostream>
#include<fstream>
#include<string>
using namespace std;
using ll = long long;
using sui = short unsigned int;

enum Cases {
	Constructor, Copy_Constructor, Init_Constructor, Rational, Decimal,
	Shorten, Plus, Subtraction, Multiply, Division, Comparasion
};

/*Краткая документация класса Дробей.
Перегруженные операции <<, >> для файла и консоли. 
Читают из потока числитель и знаменатель.

Перегруженные арифметические операции: +, -, *, /
Все операции возвращают новые СОКРАЩЕННЫЕ дроби.

Перегруженные операции сравнения: ==, !=, >, >=, <, <= 
Все операции возвращают булево выражение. Внутри перегрузок создается
два объекта - tmp и second. Нужно для того, чтобы не менять после операций
исходные дроби.

-Fraction
Метод сокращения дроби 
Возвращает новую сокращенную дробь.

to_general(Fraction &another)
Метод приведения к общему знаменателю .
Принимает на вход дробь,приводит к общему знаменателю две дроби
и выполняет сокращение дроби. Метод меняет обе изначальные дроби.

flip()
Метод переворота дроби.
Возвращает новую дробь, в которой местами поменяны числитель и знаменатель.

to_rational()
Метод рационального представления дроби.
Возвращает строку в формате "числитель/знаменатель".

to_decimal()
Метод десятичного представления дроби.
Возвращает результат преобразования дроби в число с типом double.

ll NOK(ll a, ll b)
Закрытый метод нахождения НОК внутри класса.
Возвращает НОК двух чисел типа long long.

ll NOD(ll a, ll b)
Закрытый метод нахождения НОД внутри класса.
Возвращает НОК двух чисел типа long long.
*/
class Fraction {
private:
	ll numerator;	//Числитель
	ll denominator;	//Знаменатель

	//Нахождение НОД
	ll NOD(ll a, ll b) const {
		return b ? NOD(b, a % b) : a;
	}

	//Нахождение НОК
	ll NOK(ll a, ll b) const {
		return (a * b) / NOD(a, b);
	}

public:
	friend ostream& operator<<(ostream& out, const Fraction& fraction);
	friend ofstream& operator<<(ofstream& out, const Fraction& fraction);
	friend istream& operator>>(istream& in, Fraction& fraction);
	friend ifstream& operator>>(ifstream& in, Fraction& fraction);

	//Конструктор по умолчанию
	Fraction() {
		numerator = 0;
		denominator = 1;
	}

	//Констуктор копирования
	Fraction(const Fraction &another) {
		numerator = another.numerator;
		denominator = another.denominator;
	}

	//Конструктор инициализации
	Fraction(ll other_num, ll other_denom) {
		numerator = other_num;
		denominator = other_denom;
	}

	//Перегрузка оператора присваивания
	Fraction& operator=(const Fraction& another) {
		numerator = another.numerator;
		denominator = another.denominator;
		return *this;
	}

	//Деструктор
	~Fraction() {
		numerator = 0;
		denominator = 0;
	}

	//Метод приводит к общему знаменателю ДВЕ дроби
	void to_general(Fraction& another) {
		if (denominator != another.denominator) {
			ll tmp = another.denominator;
			ll tmp1 = denominator;

			another.denominator = NOK(denominator, another.denominator);
			denominator = another.denominator;

			another.numerator = another.numerator * (denominator / tmp);
			numerator = numerator * (denominator / tmp1);
		}
		-*this; -another;
	}

	//Метод переворота дроби, возвращает новую дробь
	Fraction flip() const {
		return Fraction(denominator, numerator);
	}

	//Метод рационального представление дроби
	string to_rational() const  {
		return to_string(numerator) + '/' + to_string(denominator);
	}

	//Метод десятичного представление дроби
	double to_decimal() const {
		return double(numerator) / double(denominator);
	}

	//Перегрузка унарного оператора -, сокращение дроби
	//Оператор возвращает новую дробь, не изменяя старую
	Fraction operator-() const {
		ll nod = NOD(numerator, denominator);
		return Fraction(numerator / nod, denominator / nod);
	}

	//Перегрузка оператора сложения
	Fraction operator+(Fraction another){
		Fraction tmp = *this;
		tmp.to_general(another);
		return -Fraction(tmp.numerator + another.numerator, tmp.denominator);
	}

	//Перегрузка оператора вычитания
	Fraction operator-(Fraction another) {
		Fraction tmp = *this;
		tmp.to_general(another);
		return -Fraction(tmp.numerator - another.numerator, tmp.denominator);
	}

	//Перегрузка оператора умножения
	Fraction operator*(const Fraction &another) {
		return -Fraction(numerator * another.numerator, denominator * another.denominator);
	}

	//Перегрузка оператора деления
	Fraction operator/(const Fraction &another) {
		Fraction second = another.flip();
		return -(*this * second);
	}

	//Перегрузка оператора равенства
	bool operator==(Fraction second) {
		Fraction tmp = *this;
		tmp.to_general(second);
		return (tmp.numerator == second.numerator);
	}

	//Перегрузка оператора неравенства
	bool operator!=(Fraction second) {
		Fraction tmp = *this;
		tmp.to_general(second);
		return !(tmp == second);
	}

	//Перегрузка оператора больше
	bool operator>(Fraction second) {
		Fraction tmp = *this;
		tmp.to_general(second);
		return (tmp.numerator > second.numerator);
	}

	//Перегрузка оператора меньше
	bool operator<(Fraction second) {
		Fraction tmp = *this;
		tmp.to_general(second);
		return (tmp.numerator < second.numerator);
	}

	//Перегрузка оператора меньше или равно
	bool operator<=(Fraction second) {
		Fraction tmp = *this;
		tmp.to_general(second);
		return (tmp.numerator <= second.numerator);
	}

	//Перегрузка оператора больше или равно
	bool operator>=(Fraction second) {
		Fraction tmp = *this;
		tmp.to_general(second);
		return (tmp.numerator >= second.numerator);
	}
};


//Перегрузка оператора вывода в консоль
ostream& operator<<(ostream& out, const Fraction& fraction) {
	out << fraction.numerator << ' ' << fraction.denominator << ' ';
	return out;
}

//Перегрузка оператора вывода в файл
ofstream& operator<<(ofstream& out, const Fraction& fraction) {
	out << fraction.numerator << ' ' << fraction.denominator<< ' ';
	return out;
}

//Перегрузка оператора чтения из консоли
istream& operator>>(istream& in, Fraction& fraction) {
	in >> fraction.numerator;
	in >> fraction.denominator;
	return in;
}

//Перегрузка оператора чтения из файла
ifstream& operator>>(ifstream& in, Fraction& fraction) {
	in >> fraction.numerator;
	in >> fraction.denominator;
	return in;
}

//Извлечение ключа из файла
sui get_key(ifstream& ifile){
	sui key;
	ifile >> key;
	return key;
}

//Вывод в консоль и файл для дроби
void print(ofstream& ofile, Fraction fraction) {
	cout << fraction;
	ofile << fraction;
}

//Вывод в консоль и файл для строки
void print(ofstream& ofile, string fraction) {
	cout << fraction;
	ofile << fraction;
}

//Вывод в консоль и файл для числа с плавающей точкой
void print(ofstream& ofile, double fraction) {
	cout << fraction;
	ofile << fraction;
}

//Вывод в консоль и файл для логического выражения
void print(ofstream& ofile, bool fraction) {
	cout << fraction << ' ';
	ofile << fraction << ' ';
}


int main() {
	ifstream ifile("input.txt");
	ofstream ofile("output.txt");
	sui key = get_key(ifile);
	switch (key) {

	//Конструктор 0
	case Constructor: {
		Fraction first;
		print(ofile, first);
		break;
	}

	//Конструктор копирования 1
	case Copy_Constructor: {
		Fraction first;
		Fraction second;
		ifile >> first;
		second = first;
		print(ofile, second);
		break;
	}

	//Конструктор инициализации 2
	case Init_Constructor: {
		ll numerator; ll denominator;
		ifile >> numerator >> denominator;
		Fraction first(numerator, denominator);
		print(ofile, first);
		break;
	}

	//Рациональное представление 4
	case Rational: {
		Fraction first;
		ifile >> first;
		print(ofile, first.to_rational());
		break;
	}

	//Десятичное представление 5
	case Decimal: {
		Fraction first;
		ifile >> first;
		print(ofile, first.to_decimal());
		break;
	}

	//Сокращение дроби 6
	case Shorten: {
		Fraction first;
		ifile >> first;
		print(ofile, -first);
		break;
	}

	//Сложение дробей 6
	case Plus: {
		Fraction first;
		Fraction second;
		Fraction plused;
		ifile >> first;
		ifile >> second;
		plused = first + second;
		print(ofile, plused);
		break;
	}

	//Вычитание дробей 7
	case Subtraction: {
		Fraction first;
		Fraction second;
		ifile >> first;
		ifile >> second;
		print(ofile, first - second);
		break;
	}

	//Умножение дробей 8
	case Multiply: {
		Fraction first;
		Fraction second;
		ifile >> first;
		ifile >> second;
		print(ofile, first * second);
		break;
	}

	//Деление дробей 9
	case Division: {
		Fraction first;
		Fraction second;
		ifile >> first;
		ifile >> second;
		print(ofile, first / second);
		break;
	}

	//Операции сравнения 10
	case Comparasion: {
		Fraction first;
		Fraction second;
		ifile >> first;
		ifile >> second;
		print(ofile, first == second);
		print(ofile, first != second);
		print(ofile, first  > second);
		print(ofile, first >= second);
		print(ofile, first < second);
		print(ofile, first <= second);
		break;
	}

	}
}
