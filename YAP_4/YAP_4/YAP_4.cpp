#include<iostream>
#include<fstream>
#include<deque>
//28. Сбалансированное дерево. Процедура: Удаление нечётных элементов. Обход дерева: Прямой.
using namespace std;
int extract_number(deque<int>& all_numbers);
bool is_odd(int value);

//Класс узла
class Btree_node {
	friend class BalancedTree;
public:
	Btree_node(int val) {
		value = val;
		left = nullptr;
		right = nullptr;
	}
private:
	int value;
	Btree_node* left;
	Btree_node* right;
};


//Класс сбалансированного дерева
class BalancedTree {
private:
	Btree_node* Balanced_tree_root;

public:
	//Конструктор
	BalancedTree() {
		Balanced_tree_root = nullptr;
	}

	//Деструктор
	~BalancedTree() {
		if (Balanced_tree_root != nullptr)
		{
			destruct_delete(Balanced_tree_root);
			Balanced_tree_root = nullptr;
			delete Balanced_tree_root;
		}
	}

	// Вспомогательная функция для деструктора
	void destruct_delete(Btree_node* current_node)
	{
		if (current_node != nullptr)
		{
			destruct_delete(current_node->left);
			destruct_delete(current_node->right);
			delete current_node;
		}
	}

	//Функция нужна для того, чтобы не передавать корень как аргумент в мейне
	void fill(deque<Btree_node*>& tree_deque, deque<int>& all_numbers) {
		Balanced_tree_root = new Btree_node(extract_number(all_numbers));
		tree_deque.push_back(Balanced_tree_root);
		t_fill(Balanced_tree_root, tree_deque, all_numbers);
	}

	//Заполнение дерева в ширину, приоритет - левый узел
	void t_fill(Btree_node* current_node, deque<Btree_node*>& tree_deque, deque<int>& all_numbers) {
		if (all_numbers.empty())
			return;
		if (!current_node->left) {
			current_node->left = new Btree_node(extract_number(all_numbers));
			//Добавляем дважды в очередь узел, тк у узла два дочерних элемента
			tree_deque.push_back(current_node->left);
			tree_deque.push_back(current_node->left);
		}
		else if (!current_node->right) {
			current_node->right = new Btree_node(extract_number(all_numbers));
			//Добавляем дважды в очередь узел, тк у узла два дочерних элемента
			tree_deque.push_back(current_node->right);
			tree_deque.push_back(current_node->right);
		}
		//Если числа есть, то обновляем очередь
		if (!all_numbers.empty()) {
			Btree_node* next = tree_deque.front();
			tree_deque.pop_front();
			t_fill(next, tree_deque, all_numbers);
		}
		//Если числа кончились, выходим из рекурсии
		else {
			all_numbers.clear();
			tree_deque.clear();
			return;
		}
	}

	//Функция нужна для того, чтобы не передавать корень как аргумент в мейне
	void print(ofstream& ofile) {
		t_print(Balanced_tree_root, ofile);
	}

	//Вывод дерева, прямой обход
	void t_print(Btree_node* current_node, ofstream& ofile)
	{
		if (!current_node)   // Базовый случай
			return;
		cout << current_node->value << " ";
		ofile << current_node->value << " ";
		t_print(current_node->left, ofile);   //рекурсивный вызов левого поддерева
		t_print(current_node->right, ofile);  //рекурсивный вызов правого поддерева
	}

	//Функция нужна для того, чтобы не передавать корень как аргумент в мейне
	void delete_odds() {
		Btree_node* parent_node = nullptr;
		t_delete_odds(Balanced_tree_root, parent_node);
		delete parent_node;
	}

	//Удаление нечетных узлов, прямой обход
	void t_delete_odds(Btree_node*& current_node, Btree_node *&parent_node) {
		if (!current_node)
			return;
		if (is_odd(current_node->value)) {
			delete_node(current_node, parent_node);
			t_delete_odds(current_node, parent_node);   //рекурсивный вызов левого поддерева
		}
		else {
			t_delete_odds(current_node->left, current_node);   //рекурсивный вызов левого поддерева
			t_delete_odds(current_node->right, current_node);  //рекурсивный вызов правого поддерева
		}
	}	

	//Функция удаления узла
	void delete_node(Btree_node *&current_node, Btree_node*& parent_node) {
		if (current_node == Balanced_tree_root) {	//Если удаляем корень
			if (!current_node->left && !current_node->right) {
				Balanced_tree_root = nullptr;
				delete Balanced_tree_root;
				delete current_node;
			}
			else if (current_node->left && !current_node->right) {
				Balanced_tree_root = current_node->left;
			}
			else if (current_node->right && !current_node->left) {
				Balanced_tree_root = current_node->right;
			}
			//Случай когда есть оба потомка
			else if (current_node->right && current_node->left) {
				Btree_node* tmp = Balanced_tree_root;
				Balanced_tree_root = Balanced_tree_root->left;
				retraction(tmp->left, tmp->right);
				Balanced_tree_root->right = tmp->right;
				delete tmp;

			}
		}
		else {	//Если удаляем не корень
			if (!current_node->left && !current_node->right) {
				if (current_node == parent_node->left) {
					parent_node->left = nullptr;
					delete parent_node->left;
				}
				else if (current_node == parent_node->right) {
					parent_node->right = nullptr;
					delete parent_node->right;
				}
			}
			else if (current_node->left && !current_node->right) {
				if (current_node == parent_node->left) {
					parent_node->left = current_node->left;
				}
				else if (current_node == parent_node->right) {
					parent_node->right = current_node->left;
				}
			}
			else if (current_node->right && !current_node->left) {
				if (current_node == parent_node->left) {
					parent_node->left = current_node->right;
				}
				else if (current_node == parent_node->right) {
					parent_node->right = current_node->right;
				}
			}
			//Случай когда есть оба потомка
			else if (current_node->right && current_node->left) {
				if (current_node == parent_node->left) {
					Btree_node* tmp = parent_node->left;
					parent_node->left = parent_node->left->left;
					retraction(parent_node->left, tmp->right);
					delete tmp;
				}
				else if (current_node == parent_node->right) {
					Btree_node* tmp = parent_node->right;
					parent_node->right = parent_node->right->left;
					retraction(parent_node->right, tmp->right);
					delete tmp;
				}

			}

		}
	}

	//Функция подтягивания левого элемента на место удаленного родителя
	void retraction(Btree_node *&left_node, Btree_node*& right_node) {
	//Алгоритм:
	//На вход подается левая и правая нода
	// На место левого потомка левой ноды ставим правую ноду
	//повторяем пока левая нода не пуста
	//Если она пуста, правую ноду переносим влево
		if (left_node) {
			Btree_node* tmp = left_node->right;
			left_node->right = right_node;
			retraction(left_node->left, tmp);
		}
		else if (right_node && !left_node) {
			left_node = right_node;
			right_node = nullptr;
			delete right_node;
		}
	}
};


//Функция, которая считывает файл в очередь
deque<int> file_read(ifstream& ifile) {
	int new_deque_element;
	deque<int> all_numbers;
	while (ifile >> new_deque_element)
		all_numbers.push_back(new_deque_element);
	return all_numbers;
}


//Функция, которая берет элемент из очереди
int extract_number(deque<int>& all_numbers) {
	int number_value = all_numbers.front();
	all_numbers.pop_front();
	return number_value;
}


//Функция для определения нечетности числа
//Возвращает 1, если число нечетное.
bool is_odd(int value) {
		return (value & 1) == 1 ? 1 : 0;
}


int main() {
	ifstream ifile("input.txt");
	ofstream ofile("output.txt");	//Объявление переменных
	deque<int> all_numbers;
	deque<Btree_node*> tree_deque;
	BalancedTree balanced_tree;
	all_numbers = file_read(ifile);		//Читаем файл в очередь
	balanced_tree.fill(tree_deque, all_numbers);		//Заполняем дерево
	balanced_tree.delete_odds();		//Удаляем нечетные элементы
	balanced_tree.print(ofile);	//Выводим дерево
	ofile.close();
	ifile.close();
	return 0;	//Очищаем грехи
}