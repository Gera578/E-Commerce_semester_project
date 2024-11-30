#pragma once
#ifndef ORDER_H
#define ORDER_H

#include "product.h"
#include <iostream>
#include <string>

using namespace std;

template <class T>

struct Node {
	T order;
	Node* next;

	Node(T& data, Node* next = NULL) {
		order = data;
		this->next = next;
	}
};

template <class T>
class Order {
private:
	Node<T>* head;
	Node<T>* tail;
	int size;

public:

	Order() {
		head = nullptr; // Start with an empty list
		tail = nullptr;
		size = 0;
	}

	// Destructor to avoid memory leaks
	~Order() {
		while (head) {
			Node<T>* temp = head;
			head = head->next;
			delete temp;
		}
	}

	int getSize() {
		return size;
	}

	int getOrderId() {
		if (isEmpty() == false) {
			return head->order.getOrderId();
		}
		return -1;
	}

	int getOrderByIndex(int index) {
		if (index < 0 || index >= size) {
			return -1; // Return -1 for invalid index
		}
		Node<T>* current = head; // Declare current and initialize it to head
		for (int i = 0; i < index; i++) {
			current = current->next;
		}
		return current->order.getOrderId();
	}



	void addOrder(T& data) {
		Node<T>* newNode = new Node<T>(data);

		if (!head) {
			head = tail = newNode;
		}
		else {
			tail->next = newNode;
			tail = newNode;
		}
		size++;
	}

	void removeOrder(int orderId) {
		//if the linked list is empty
		if (isEmpty()) {
			return;
		}

		//if the order to remove is in the first node (head)
		if (head->order.getOrderId() == orderId) {
			Node<T>* ptr = head;

			head = head->next;
			delete ptr;
			size--;
			return;
		}

		//any other part of the list
		Node<T>* current = head;
		while (current->next && current->next->order.getOrderId() != orderId) {
			current = current->next;
		}

		// If order not found
		if (!current->next) return;

		Node<T>* temp = current->next;
		current->next = current->next->next;
		if (current->next == nullptr) {
			tail = current; // Update tail if necessary
		}
		delete temp;
		size--;
	}


	bool updateOrder(int orderId, T& data) {
		Node<T>* ptr = head;

		while (ptr) {
			if (ptr->order.getOrderId() == orderId) {
				ptr->next = data;
				return true;
			}

			ptr = ptr->next;
		}

		return false; //order not found
	}

	void removeOrders() {
		while (head)
		{
			Node<T>* ptr = head;
			head = head->next;
			delete ptr;
		}

		tail = NULL;
		size = 0;
	}

	void displayOrder() { 
		Node<T>* ptr = head;
		cout << "Order List:" << endl;
		while (ptr) {
			cout << "Order ID: " << ptr->order.getID() << ", Product: "
				<< ptr->order.getProduct() << ", Quantity: "
				<< ptr->order.getQuantity() << ", Price: "
				<< ptr->order.getPrice() << endl;
			ptr = ptr->next;
		}
	}


	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		return false;
	}
};
#endif // !ORDER_H