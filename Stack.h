#pragma once
template <typename T>//used to male it dynamic as in project file it stated input can be of any type

struct StackNode { // simple node struct with a pointer to next node

	T value;
	StackNode* next;

	StackNode() {
		value = {};
		next = NULL;
	}
};

template <class T>
class LinkedListStack { // a templatized node based stack
	StackNode<T>* head;
public:
	LinkedListStack() {
		head = NULL;
	}
	StackNode<T>* getHead() {
		return head;
	}
	void push(T data) {
		if (!head) { // pushes a value into the stack
			StackNode<T>* toPush = new StackNode<T>;
			toPush->value = data;
			head = toPush;
			toPush =NULL;
			return;
		}
		StackNode<T>* toPush = new StackNode<T>;
		toPush->value = data;
		toPush->next = head;
		head = toPush;
		toPush =NULL;
	}
	bool isEmpty() {
		return !(head);
	}
	T peek() {
		return head->value;
	}
	T pop() {
		// pops value from the top of stack and returns it
		if (!head) {
			return {};
		}

		StackNode<T>* toDelete = head;
		head = head->next;
		T toReturn = toDelete->value;
		delete toDelete;

		return toReturn;
	}
	void emptyStack() {
		while (!isEmpty()) {
			static_cast<void>(pop());
		}
	}
	int getListSize() {
		int size = 0;
		StackNode<T>* temp = head;
		while (temp) {
			size++;
			temp = temp->next;
		}
		return size;
	}
	~LinkedListStack() {
		emptyStack();
	}
};