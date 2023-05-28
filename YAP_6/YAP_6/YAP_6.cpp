#include<iostream>
#include<fstream>
#include<string>
#include<ctime>

using ll = long long;
using namespace std;

//Класс дробей
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
	string to_rational() const {
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
	Fraction operator+(Fraction another) {
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

//Класс узла для списка
class Node {	//Класс узла
public:
	Fraction fract;
	Node* next_node;
	Node* previous_node;

public:
	Node(Fraction new_value) {	//	Конструктор, принимающий значение для узла
		this->fract = new_value;
		this->next_node = nullptr;
		this->previous_node = nullptr;
	}
	Node() {	//	Конструктор
		this->fract = Fraction();
		this->next_node = nullptr;
		this->previous_node = nullptr;
	}
};

//Класс двусвязного списка
class DoubleLinkedList {
private:
	Node* head_pointer;
	Node* tail_pointer;
	int size_of_linked_list;

public:
	//Конструктор
	DoubleLinkedList() {	//	Конструктор, изначально голова и хвост пусты
		this->head_pointer = nullptr;
		this->tail_pointer = nullptr;
		size_of_linked_list = 0;
	}

	//Деструктор
	~DoubleLinkedList() {	//Деструктор
		Node* current = head_pointer;
		while (current != nullptr) {
			Node* next = current->next_node;
			delete current;
			current = next;
		}
	}

	//Метод получения указателя на начало
	Node* get_head_pointer() {	//Геттер на голову
		return this->head_pointer;
	}

	//Метод получения указателя на конец
	Node* get_tail_pointer() {	//Геттер на хвост
		return this->tail_pointer;
	}

	//Метод получения размера
	int get_size_of_linked_list() {	//Геттер на размер
		return this->size_of_linked_list;
	}

	//Метод добавление в начало
	Node* append_left(Fraction new_value) {	//Добавление элемента слева
		Node* new_created_node = new Node(new_value);
		new_created_node->next_node = head_pointer;	//Создаем необходимые связи
		if (head_pointer != nullptr) {
			head_pointer->previous_node = new_created_node;
		}
		head_pointer = new_created_node;
		++size_of_linked_list;
		return new_created_node;
	}

	//Метод добавление в конец
	Node* append_right(Fraction new_value) {	//Добавление справа
		Node* new_created_node = new Node(new_value);
		new_created_node->previous_node = tail_pointer;
		if (tail_pointer != nullptr) {	//Добавляем необходимые связи
			tail_pointer->next_node = new_created_node;
		}
		if (head_pointer == nullptr) {
			head_pointer = new_created_node;
		}
		tail_pointer = new_created_node;
		++size_of_linked_list;
		return new_created_node;
	}

	//Метод удаления первого элемента
	void delete_left() {	//Удаление слева
		if (head_pointer == nullptr) return;
		Node* next_node = head_pointer->next_node;
		if (next_node != nullptr)
			next_node->previous_node = nullptr;
		else
			tail_pointer = nullptr;
		delete head_pointer;
		--size_of_linked_list;
		head_pointer = next_node;
	}

	//Метод удаления последнего элемента
	void delete_right() {	//Удаление справа
		if (tail_pointer == nullptr) return;
		Node* previous_node = tail_pointer->previous_node;
		if (previous_node != nullptr)
			previous_node->next_node = nullptr;
		else
			head_pointer = nullptr;
		delete tail_pointer;
		--size_of_linked_list;
		tail_pointer = previous_node;
	}

	//Метод вывода списка в консоль и в файл
	void print(ofstream& ofile) {	//Вывод в файл и консоль
		Node* temp_head_pointer = head_pointer;
		while (temp_head_pointer != nullptr) {
			cout << temp_head_pointer->fract.to_rational() << ' ';
			temp_head_pointer = temp_head_pointer->next_node;
		}
		cout << "\n" << "\n";
	}

	//Меотод случайного заполнения списка
	void random_fill(ll lenght) {
		srand(time(0));
		for (ll i = 0; i < lenght; ++i) {
			ll numerator = rand();
			ll tmp = rand();
			ll denominator = tmp ? tmp : 1;	//исключение нуля в знаменателе
			Fraction fract = Fraction(numerator, denominator);
			append_right(fract);
		}
	}

	//Проверка на отсортированность
	bool is_sorted() {
		Node* current = head_pointer;
		while (current->next_node != nullptr){
			Node* tmp = current;
			current = current->next_node;
			if (tmp->fract > current->fract)
				return 0;
		}
		return 1;
	}

	//Метод вывода в файл количества элементов в списке
	void file_output(ofstream& ofile) {
		ofile << size_of_linked_list << endl;
	}

	//Метод сортировки вставками
	void insertion_sort() {	//Сортировка вставками
		//Алгоритм
		//Элемент массива сравнивается с предыдущими уже отсортированными
		//и встает на свое место(перебор до нужного места)
		if (head_pointer == nullptr || head_pointer->next_node == nullptr) {
			return;	//Если список из одного элемента или пуст то ничего не делаем
		}
		Node* current = head_pointer->next_node;	//Задаем текущую ноду
		while (current != nullptr) {		//Пока нода есть
			Node* temp = current->previous_node;	//Создаем Вложенную предыдущую ноду
			while (temp != nullptr && temp->fract > current->fract) {
				swap(temp, current);	//Если хранилище непусто и числа стоят в неправильном порядке
				temp = current->previous_node;	//Меняем и движемся в начало
			}
			//Как только вложенный цикл достигает начала, двигаем невложенный цикл дальше
			current = current->next_node;
		}
	}

	//Метод свапа двух узлов списка
	void swap(Node* first_node, Node* second_node) {	//Обмен указателями двух нодов
		if (first_node == nullptr || second_node == nullptr) {
			return;
		}	//Меняем местами две соседние ноды
			//6 связей меняем
		if (first_node->next_node == second_node) {
			if (first_node->previous_node != nullptr)
				first_node->previous_node->next_node = second_node;
			else {
				head_pointer = second_node;
				head_pointer->previous_node = nullptr;
			}
			if (second_node->next_node != nullptr)
				second_node->next_node->previous_node = first_node;
			else {
				tail_pointer = first_node;
				tail_pointer->next_node = nullptr;
			}
			first_node->next_node = second_node->next_node;
			second_node->previous_node = first_node->previous_node;
			second_node->next_node = first_node;
			first_node->previous_node = second_node;
		}
		else if (second_node->next_node == first_node) {
			if (second_node->previous_node != nullptr)
				second_node->previous_node->next_node = first_node;
			else {
				head_pointer = first_node;
				head_pointer->previous_node = nullptr;
			}
			if (first_node->next_node != nullptr)
				first_node->next_node->previous_node = second_node;
			else {
				tail_pointer = second_node;
				tail_pointer->next_node = nullptr;
			}
			second_node->next_node = first_node->next_node;
			first_node->previous_node = second_node->previous_node;
			first_node->next_node = second_node;
			second_node->previous_node = first_node;
		}
		else {	//Меняем местами две несоседние ноды местами
				//Меняем 8 связей
			Node* temp_prev = first_node->previous_node;
			Node* temp_next = first_node->next_node;
			first_node->previous_node = second_node->previous_node;
			first_node->next_node = second_node->next_node;
			second_node->previous_node = temp_prev;
			second_node->next_node = temp_next;
			if (first_node->previous_node != nullptr) {
				first_node->previous_node->next_node = first_node;
			}
			else {
				head_pointer = first_node;
				head_pointer->previous_node = nullptr;
			}
			if (first_node->next_node != nullptr) {
				first_node->next_node->previous_node = first_node;
			}
			else {
				tail_pointer = first_node;
				tail_pointer->next_node = nullptr;
			}
			if (second_node->previous_node != nullptr) {
				second_node->previous_node->next_node = second_node;
			}
			else {
				head_pointer = second_node;
				head_pointer->previous_node = nullptr;
			}
			if (second_node->next_node != nullptr) {
				second_node->next_node->previous_node = second_node;
			}
			else {
				tail_pointer = second_node;
				tail_pointer->next_node = nullptr;
			}
		}
	}
};

//Перегрузка оператора вывода в консоль
ostream& operator<<(ostream& out, const Fraction& fraction) {
	out << fraction.numerator << ' ' << fraction.denominator << ' ';
	return out;
}

//Перегрузка оператора вывода в файл
ofstream& operator<<(ofstream& out, const Fraction& fraction) {
	out << fraction.numerator << ' ' << fraction.denominator << ' ';
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
ll get_lenght(ifstream& ifile) {
	ll lenght;
	ifile >> lenght;
	return lenght;
}

//Функция подтверждения отсортированности списка
void console_confirm(DoubleLinkedList& List) {
	cout << "\n" << (List.is_sorted() ? 1 : 0) << endl;
}


int main() {
	DoubleLinkedList List;
	ifstream ifile("input.txt");
	ofstream ofile("output.txt");
	//Извлечение количества элементов, которые нужно сгенерировать
	ll lenght = get_lenght(ifile);
	//Генерация lenght количества элементов в списке
	List.random_fill(lenght);
	List.print(ofile);
	//Сортировка списка
	List.insertion_sort();
	List.print(ofile);
	//Проверка на отсортированность
	console_confirm(List);
	//Вывод в файл числа отсортированных элементов
	List.file_output(ofile);
	return 0;
}