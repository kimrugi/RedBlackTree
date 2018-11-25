#include <iostream>
#include <assert.h>
#include <string>
#include <random>
using namespace std;
#define RED 0
#define BLACK 1
#define Node node
class node
{
public:
	int key;
	int color;
	node *left, *right, *parent;
};
node *root;
node *NILL;
void error(const char* c) {
	cout << "error: " << c;
}

class Queue
{
	Node** buffer;
	const int size;
	int front;
	int rear;

public:
	Queue(int size) :size(size)
	{
		buffer = new Node*[size];
		front = rear = 0;
	}
	~Queue()
	{
		delete[] buffer;
	}
	void increase_size() 
	{
		int tmp_size = size * 2;
		Node** tmp = new Node*[tmp_size];
		for (int i = front; i < rear; ++i) {
			tmp[i] = buffer[i];
		}
		delete[] buffer;
		buffer = tmp;
	}
	bool enqueue(Node* data)
	{
		if (Is_full())
		{
			increase_size();
		}
		buffer[rear] = data;
		rear = Next(rear);
		return true;
	}
	Node* pop()
	{
		if (Is_empty())
		{
			error("queue is empty");
			return nullptr;
		}
		Node* re = buffer[front];
		front = Next(front);
		return re;
	}
	bool Is_full()
	{
		return Next(rear) == front;
	}
	bool Is_empty()
	{
		return front == rear;
	}
	int Next(int now)
	{
		return (now + 1) % size;
	}
};


void print_tree();
Node* insert_tree(Node*& R, int data);
void check_tree_recursion(node *p, int B_sofar, int B_total);
void check_tree(node *root);
Node* search(Node* root, int data);
void del_all(node* n);
bool is_in(int* arr, int len, int d);

//삽입
//-----------------------------------------------------------
void insert_red_black_tree(int data);
void case_root(Node* n);
void case_parent_black(Node*);
void case_parent_red(Node*);
Node* uncle(Node* n);
void case_uncle_red(Node* n);
void case_uncle_black(Node* n);
//-----------------------------------------------------------
//삭제
//-----------------------------------------------------------
void left_rotation(node* n);
void right_rotation(node* n);
node* minimum_tree(node* n);
node* find_successor(node* n);
void delete_tree(node* n);
void delete_fix(node* n);
void change_node(node*, node*);
void delete_red_black_tree(int data);
//-----------------------------------------------------------
//삽입동작:				O
//삭제동작:				O
//check_tree() 통과:	O



int main()
{
	NILL = new node;
	NILL->key = -9999;
	NILL->color = BLACK;
	root = NILL;

	int randseed = 0;
	int in = -1;

	cout << "랜덤 시드" << endl;
	cin >> randseed;
	//cout << "삽입할 노드 수" << endl;
	//cin >> in;
	in = 1000;
	srand(randseed);
	int* arr = new int[in];
	int tmp_random = rand() % 1000;
	for (int i = 0; i < in; ++i) {
		while(is_in(arr, i, tmp_random))
			tmp_random = rand() % 1000;	
		arr[i] = tmp_random;
	}

	for (int i = 0; i < in; ++i) {
		cout << arr[i] << ", ";
	}
	cout << endl;
	
	for(int i = 0; i < in; ++i){
		insert_red_black_tree(arr[i]);
		check_tree(root);
	}
	cout << "삽입 끝" << endl;
	for (int i = 0; i < in; ++i) {
		delete_red_black_tree(arr[i]);
		check_tree(root);
	}
	cout << "삭제 끝" << endl;
	system("pause");
	if (root != NILL) {
		del_all(root);
		delete root;
	}
	delete NILL;
}


Node* insert_tree(Node*& R, int data) {
	Node* result = NILL;
	if (R == NILL) {
		R = new Node;
		R->parent = nullptr;
		R->left = R->right = NILL;
		R->color = RED;
		R->key = data;
		return R;
	}
	else if (R->key > data) {
		result = insert_tree(R->left, data);
		R->left->parent = R;
		return result;
	}
	else {
		result = insert_tree(R->right, data);
		R->right->parent = R;
		return result;
	}
}

void print_tree() {
	if (root == NILL)
		return;
	Queue queue(20);
	Node* tmp;
	queue.enqueue(root);
	int current_count = 1;
	int next_count = 0;
	int i = 1;
	cout << i << " ";
	while (!queue.Is_empty()) {
		tmp = queue.pop();
		current_count--;
		cout << "(" << tmp->color << ", " << tmp->key << ") ";
		if (tmp->left != NILL) {
			queue.enqueue(tmp->left);
			next_count++;
		}
		if (tmp->right != NILL) {
			queue.enqueue(tmp->right);
			next_count++;
		}
		if (current_count == 0) {
			cout << endl;
			i++;
			cout << i << " ";
			current_count = next_count;
			next_count = 0;
		}
	}
}




void check_tree_recursion(node *p, int B_sofar, int B_total)
{
	if (p == NILL)
	{
		assert(B_sofar == B_total);
		return;
	}

	check_tree_recursion(p->left, B_sofar + (p->left != NILL && p->left->color), B_total);
	check_tree_recursion(p->right, B_sofar + (p->right != NILL && p->right->color), B_total);
	if (p->left)
		assert(!(!p->left->color && !p->color));
	if (p->right)
		assert(!(!p->right->color && !p->color));
}

void check_tree(node *root)
{
	int cnt = 0;
	node *p;
	if (root == NILL)
		return;
	for (p = root; p != NILL; p = p->left)
		cnt += p->color;
	check_tree_recursion(root, p->color, cnt);
}

Node* uncle(Node* n) {
	Node* granpa = n->parent->parent;
	if (granpa->left == n->parent) {
		return granpa->right;
	}
	else {
		return granpa->left;
	}

}

void insert_red_black_tree(int data) {
	Node* n = insert_tree(root, data);
	case_root(n);
}
void case_root(Node* n) {
	if (n->parent == nullptr)
		n->color = BLACK;
	else
		case_parent_black(n);
}
void case_parent_black(Node* n) {
	if (n->parent->color == BLACK)
		return;
	else
		case_parent_red(n);
}
void case_parent_red(Node* n) {
	if (uncle(n) != NILL && uncle(n)->color == RED) {
		case_uncle_red(n);
		return;
	}
	else if (uncle(n) == NILL || uncle(n)->color == BLACK) {
		case_uncle_black(n);
		return;
	}
}
void case_uncle_red(Node* n) {
	n->parent->color = BLACK;
	uncle(n)->color = BLACK;
	n->parent->parent->color = RED;
	case_root(n->parent->parent);
}
void case_uncle_black(Node* n) {
	Node* parent = n->parent;
	Node* granpa = parent->parent;
	if (granpa->left == parent) {
		if (parent->right == n) {
			left_rotation(n->parent);
			case_uncle_black(parent);
		}
		else if (parent->left == n) {
			n->parent->color = BLACK;
			n->parent->parent->color = RED;
			right_rotation(n->parent->parent);
		}
	}
	else if (granpa->right == parent) {
		if (parent->right == n) {
			n->parent->color = BLACK;
			n->parent->parent->color = RED;
			left_rotation(n->parent->parent);
		}
		else if (parent->left == n) {
			right_rotation(n->parent);
			case_uncle_black(parent);
		}
	}
	
}
Node* search(Node* root, int data) {
	if (root == nullptr)
		return nullptr;
	if (root->key == data)
		return root;
	if (data < root->key)
		return search(root->left, data);
	else
		return search(root->right, data);
}
bool is_in(int* arr, int len, int d) {
	for (int i = 0; i < len; ++i) {
		if (arr[i] == d)
			return true;
	}
	return false;
}
void del_all(node* n) {
	if (n->left != NILL && left != nullptr) {
		del_all(n->left);
		delete n->left;
		n->left = NILL;
	}
	if (n->right != NILL && right != nullptr) {
		del_all(n->right);
		delete n->right;
		n->right = NILL;
	}
}
node* minimum_tree(node* n) {
	if (n->left != NILL) {
		return minimum_tree(n->left);
	}
	else return n;
}
node* find_successor(node* n) {
	if (n->right != NILL) {
		return minimum_tree(n->right);
	}
	node* me = n;
	node* p = me->parent;
	while (p != nullptr && me == p->right) {
		me = p;
		p = me->parent;
	}
	return p;
}

void delete_tree(node* n) {
	node* me;
	node* tmp;
	if (n->left == NILL || n->right == NILL)
		tmp = n;
	else
		tmp = find_successor(n);
	
	if (tmp->left != NILL) {
		me = tmp->left;
	}
	else {
		me = tmp->right;
	}
	me->parent = tmp->parent;
	if (tmp->parent == nullptr)
		root = me;
	else if (tmp == tmp->parent->left)
		tmp->parent->left = me;
	else
		tmp->parent->right = me;
	if (n != tmp) {
		n->key = tmp->key;
	}
	if (tmp->color == BLACK) {
		delete_fix(me);
	}
	if (n != tmp) {
		delete tmp;
	}
	else delete n;
	
	return;
}

void left_rotation(node* n) {
	node* tmp = n->right;
	n->right = tmp->left;
	tmp->left->parent = n;
	tmp->parent = n->parent;
	if (n->parent == nullptr)
		root = tmp;
	else if (n == n->parent->left)
		n->parent->left = tmp;
	else
		n->parent->right = tmp;
	tmp->left = n;
	n->parent = tmp;
}
void right_rotation(node* n) {
	node* tmp = n->left;
	n->left = tmp->right;
	tmp->right->parent = n;
	tmp->parent = n->parent;
	if (n->parent == nullptr)
		root = tmp;
	else if (n == n->parent->left)
		n->parent->left = tmp;
	else
		n->parent->right = tmp;
	tmp->right = n;
	n->parent = tmp;
}





void delete_fix(node* n) {
	node* tmp;
	while (n != root && n->color == BLACK) {
		if (n == n->parent->left) {
			tmp = n->parent->right;
			if (tmp->color == RED) {
				tmp->color = BLACK;
				n->parent->color = RED;
				node* parent = n->parent;
				left_rotation(n->parent);
				NILL->parent = n->parent;
				tmp = n->parent->right;
			}
			if (tmp->left->color == BLACK && tmp->right->color == BLACK) {
				tmp->color = RED;
				n = n->parent;
			}
			else if (tmp->right->color == BLACK) {
				tmp->left->color = BLACK;
				tmp->color = RED;
				node* parent = n->parent;
				right_rotation(tmp);
				NILL->parent = parent;
				tmp = n->parent->right;
			}
			if (tmp->color == BLACK && tmp->left->color == RED && tmp->right->color == RED) {
				node* parent = n->parent;
				left_rotation(tmp->parent);
				NILL->parent = parent;
				tmp->color = tmp->left->color;
				tmp->left->color = BLACK;
				tmp->right->color = BLACK;
				break;
			}
			if (tmp->color == BLACK && tmp->right->color == RED) {
				tmp->color = n->parent->color;
				n->parent->color = BLACK;
				tmp->right->color = BLACK;
				node* parent = n->parent;
				left_rotation(n->parent);
				NILL->parent = parent;
				n = root;
			}
		}
		else {
			tmp = n->parent->left;
			if (tmp->color == RED) {
				tmp->color = BLACK;
				n->parent->color = RED;
				node* parent = n->parent;
				right_rotation(n->parent);
				NILL->parent = parent;
				tmp = n->parent->left;
			}
			if (tmp->left->color == BLACK && tmp->right->color == BLACK) {
				tmp->color = RED;
				n = n->parent;
			}
			else if (tmp->left->color == BLACK) {
				tmp->right->color = BLACK;
				tmp->color = RED;
				node* parent = n->parent;
				left_rotation(tmp);
				NILL->parent = parent;
				tmp = n->parent->left;
			}
			if (tmp->color == BLACK && tmp->left->color == RED && tmp->right->color == RED) {
				node* parent = n->parent;
				right_rotation(tmp->parent);
				NILL->parent = parent;
				tmp->color = tmp->right->color;
				tmp->left->color = BLACK;
				tmp->right->color = BLACK;
				break;
			}
			if (tmp->color == BLACK && tmp->left->color == RED) {
				tmp->color = n->parent->color;
				n->parent->color = BLACK;
				tmp->left->color = BLACK;
				node* parent = n->parent;
				right_rotation(n->parent);
				NILL->parent = parent;
				n = root;
			}
		}
	}
	n->color = BLACK;
	return;
}
void delete_red_black_tree(int data) {
	node* to_delete_node = search(root, data);
	delete_tree(to_delete_node);
}
