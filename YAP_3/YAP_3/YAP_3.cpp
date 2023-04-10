#include <iostream>
#include<fstream>
#include<ctime>
//	14. Пирамидальная сортировка (Heap) и Сортировка вставками (Insertion). Двусвязный нециклический список.

class Node {	//Класс узла
public:
	double value_of_node;
	Node* next_node;
	Node* previous_node;

public:
	Node(double new_value) {	//	Конструктор, принимающий значение для узла
		this->value_of_node = new_value;
		this->next_node = nullptr;
		this->previous_node = nullptr;
	}
	Node() {	//	Конструктор
		this->value_of_node = 0;
		this->next_node = nullptr;
		this->previous_node = nullptr;
	}
};


class double_linked_list {	//Класс списка
private:
	Node* head_pointer;
	Node* tail_pointer;
	int size_of_linked_list;

public:
	double_linked_list() {	//	Конструктор, изначально голова и хвост пусты
		this->head_pointer = nullptr;
		this->tail_pointer = nullptr;
		size_of_linked_list = 0;
	}

	~double_linked_list() {	//Деструктор
		Node* current = head_pointer;
		while (current != nullptr) {
			Node* next = current->next_node;
			delete current;
			current = next;
		}
	}

	Node* get_head_pointer() {	//Геттер на голову
		return this->head_pointer;
	}

	Node* get_tail_pointer() {	//Геттер на хвост
		return this->tail_pointer;
	}
	
	int get_size_of_linked_list() {	//Геттер на размер
		return this->size_of_linked_list;
	}

	Node* append_left(double new_value) {	//Добавление элемента слева
		Node* new_created_node = new Node(new_value);
		new_created_node->next_node = head_pointer;	//Создаем необходимые связи
		if (head_pointer != nullptr){
			head_pointer->previous_node = new_created_node;
		}
		head_pointer = new_created_node;
		++size_of_linked_list;
		return new_created_node;
	}

	Node* append_right(double new_value) {	//Добавление справа
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
	
	void read_from_file(std::ifstream& ifile) {	//Чтение из файла
		double new_element;
		while (ifile >> new_element)
			this->append_right(new_element);
	}

	void print(std::ofstream& ofile) {	//Вывод в файл и консоль
		Node* temp_head_pointer = head_pointer;
		std::cout << this->size_of_linked_list << ' ';
		ofile << this->size_of_linked_list << ' ';
		while (temp_head_pointer != nullptr){
			std::cout << temp_head_pointer->value_of_node << ' ';
			ofile << temp_head_pointer->value_of_node << ' ';
			temp_head_pointer = temp_head_pointer->next_node;
		}
		std::cout << std::endl;
	}

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
			while (temp != nullptr && temp->value_of_node > current->value_of_node) {
				swap(temp, current);	//Если хранилище непусто и числа стоят в неправильном порядке
				temp = current->previous_node;	//Меняем и движемся в начало
			}
			//Как только вложенный цикл достигает начала, двигаем невложенный цикл дальше
			current = current->next_node;
		}
	}

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

	void heap_sort() {
		int n = this->size_of_linked_list;
		Node* tempppppp = head_pointer;
		//Загружаем в кучу элементы
		for (int kk = n / 2 - 1; kk >= 0; kk--)
			get_heappp(tempppppp, n, kk);
		//Меняем Максимальный элемент с элементом i-1
		//То есть он встает на свое место
		//Пересобираем кучу
		//Повторяем до полной сортировки
		for (int kk = n - 1; kk >= 0; kk--)
		{
			heap_swap(0, kk, tempppppp);
			get_heappp(tempppppp, kk, 0);
		}
	}

	void get_heappp(Node* temptemp, int n, int i) {
		// Инициализация большего элемента, как корня
		int largestlargest = i;
		int leftleft = 2 * i + 1;
		int rightright = 2 * i + 2;
		Node* largesti = temptemp;
		//Находим большую ноду по индексу
		for (int k = 0; k < largestlargest; k++) {
			largesti = largesti->next_node;
		}
		Node* li = temptemp;
		//Находим левую ноду по индексу
		for (int k = 0; k < leftleft; k++) {
			if (li == nullptr)
				break;
			li = li->next_node;
		}
		//Нахожим правую ноду по индексу
		Node* ri = nullptr;
		if (li != nullptr) ri = li->next_node;

		// Если левый потомок больше корня
		if (li != nullptr)
			if (leftleft < n && li->value_of_node > largesti->value_of_node) {
				largestlargest = leftleft;
				largesti = li;
			}
		// Если правый потомок больше корня
		if (ri != nullptr)
			if (rightright < n && ri->value_of_node > largesti->value_of_node) {
				largestlargest = rightright;
				largesti = ri;
			}

		// Если largest не является корнем
		if (largestlargest != i)
		{
			heap_swap(i, largestlargest, temptemp);

			// Формируем дерево
			get_heappp(temptemp, n, largestlargest);
		}
	}


	void heap_swap(int i, int k, Node* swapList) {	
		Node* temp1 = swapList;
		Node* temp2 = swapList;
		for (int j = 0; j < i; j++) {
			temp1 = temp1->next_node;
		}
		for (int z = 0; z < k; z++) {
			temp2 = temp2->next_node;
		}

		/*swap(temp1, temp2);*/
		int number = temp1->value_of_node;
		temp1->value_of_node = temp2->value_of_node;

		temp2->value_of_node = number;

	}
};


bool sort_choose(std::ifstream& ifile) {
	bool choise;
	ifile >> choise;
	return choise;
}


int main() {
	double_linked_list List;
	std::ifstream ifile("input.txt");
	std::ofstream ofile("output.txt");
	bool choise = sort_choose(ifile);
	List.read_from_file(ifile);
	if (choise) {	// Выбираем сортировку
		List.insertion_sort();	//Сортируем
		List.print(ofile);	//выводим
	}
	else {
		List.heap_sort();	//Сортируем
		List.print(ofile);	//Выводим
	}
	ofile.close();
	return 0;
}
