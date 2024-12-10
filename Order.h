#pragma once
#ifndef ORDER_H
#define ORDER_H

#include "product.h"
#include "Category.h"
#include <iostream>
#include <string>
#include <fstream>

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
	Product** products;

public:

	Order() {
		products = nullptr;
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
		delete[] products;  // Free the dynamically allocated memory for products
	}


	int getSize() {
		return size;
	}


	//similar to getProductById, but this one looks for the order
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

	//search a product in the order by name
	Product* searchProductByName(const string& productName) {
		Node<T>* current = head;
		while (current != nullptr) {
			if (current->order.getProduct() == productName) {
				return &current->order; // Return a pointer to the product
			}
			current = current->next;
		}
		cout << "Product not found." << endl;
		return nullptr;
	}


	Product* searchProductById(int id) {
		Node<T>* current = head;
		while (current != nullptr) {
			if (current->order.getID() == id) {
				return &current->order;
			}
			current = current->next;
		}
		cout << "Product not found." << endl;
		return nullptr;
	}

	//something like sorting, this function shows just the orders in a range of prices
	void filterByPrice(double min, double max) {
		cout << "Products in price range " << min << "-" << max << ": " << endl;
		Node<T>* current = head;
		while (current != nullptr) {
			if (current->order.getPrice() >= min && current->order.getPrice() <= max) {
				current->order.display();
				cout << "------------------" << endl;
			}
			current = current->next;
		}
	}

	//adds an order to the order
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

	//removes an order
	void removeOrder(int orderId) {
		//if the linked list is empty
		if (isEmpty()) {
			return;
		}

		//if the order to remove is in the first node (head)
		if (head->order.getID() == orderId) {
			Node<T>* ptr = head;

			head = head->next;
			delete ptr;
			size--;
			return;
		}

		//any other part of the list
		Node<T>* current = head;
		while (current->next && current->next->order.getID() != orderId) {
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

	//removes ALL orders
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


	void sortOrder(int option) {
		if (!head || size <= 1) {
			cerr << "No orders to sort or only one order exists!" << endl;
			return;
		}

		head = mergeSort(head, option); // Update the head pointer after sorting 
	}

	//returns the average price of the order
	double averagePrice() {
		if (head == nullptr || size == 0) {
			return 0.0; // Return 0 if there are no products
		}
		double totalPrice = 0;
		Node<T>* current = head;
		while (current != nullptr) {
			totalPrice += current->order.getPrice();
			current = current->next;
		}
		return totalPrice / size;
	}



	//function to get information from a file
	void loadInventory(const string& filename) {
		ifstream file(filename);
		if (!file) {
			cerr << "Error opening file!" << endl;
			return;
		}

		string line;
		string productName;
		int id = 0, quantity = 0;
		double price = 0.0;
		double discount = 0.0;

		while (getline(file, line)) {
			// If the line starts with "Product:", it means we've found a new product entry
			if (line.find("Product:") != string::npos) {
				// Extract product name (skip the "Product: " part)
				productName = line.substr(line.find(":") + 2);

				// Read the next line for price (expect "$" sign before price)
				getline(file, line); // Price
				price = stod(line.substr(line.find("$") + 1));

				// Read the next line for ID
				getline(file, line); // ID
				id = stoi(line.substr(line.find(":") + 2));

				// Read the next line for quantity
				getline(file, line); // Quantity
				quantity = stoi(line.substr(line.find(":") + 2));

				// Optional: Read discount (if available)
				// If there's a discount line, you can read it too, otherwise skip
				if (getline(file, line) && line.find("Discount:") != string::npos) {
					discount = stod(line.substr(line.find(":") + 2)); // If there's a discount, parse it
				}
				else {
					discount = 0.0;  // No discount if line is absent
				}

				// Add the product to the inventory (assuming addOrder is correct)
				Product newProduct(productName, id, price, quantity);
				addOrder(newProduct);  // Assuming addOrder takes Product objects (not pointers)
			}
		}

		file.close();
	}

	//function to export the order to a file
	void saveToFile( string& filename) {
		ofstream outFile(filename);
		if (!outFile) {
			cerr << "Error: Could not open file for saving: " << filename << endl;
			return;
		}

		// Write category name 

		// Write size
		outFile << "Number of elements in the file: " << size << endl << endl;

		// Write product details
		Node<T>* current = head;
		while (current != nullptr) {
			const T& product = current->order; // Get the product from the current node
			outFile << product.getProduct() << "\n$"
				<< product.getPrice() << "\n"
				<< product.getQuantity()<<" Elements in stock" << endl;
			outFile << "---------------\n";
			current = current->next;
		}

		if (!outFile) {
			cerr << "Error: Writing to file failed!" << endl;
		}
		else {
			cout << "Data successfully saved to " << filename << endl;
		}

		outFile.close();
	}


	//displays the whole order
	void displayOrder() { 
		Node<T>* ptr = head;
		cout << "Order List:" << endl;
		while (ptr) {
			cout << "Order ID: " << ptr->order.getID() << endl
				<< "Product: " << ptr->order.getProduct() << endl
				<< "Quantity: " << ptr->order.getQuantity() << endl
				<< "Price: " << ptr->order.getPrice() << endl;
			cout << "----------------------------\n";
			ptr = ptr->next;
		}
	}


	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		return false;
	}


	//functions to sort the products
	Node<T>* merge(Node<T>* left, Node<T>* right, int option) {
		if (!left) return right;
		if (!right) return left;

		Node<T>* result = nullptr;

		// Custom sorting condition based on the option
		bool condition = false;
		switch (option) {
		case 1: 
			condition = left->order.getPrice() <= right->order.getPrice();//cheapest
			break;
		case 2: 
			condition = left->order.getPrice() >= right->order.getPrice(); //more expensive
			break;
		case 3: 
			condition = left->order.getProduct() <= right->order.getProduct(); //name
			break;
		case 4: 
			condition = left->order.getQuantity() <= right->order.getQuantity(); //less quantity
			break;

		case 5:
			condition = left->order.getID() <= right->order.getID(); //lowest id
			break;

		default:
			cerr << "Invalid sorting option!" << endl;
			return nullptr;
		}

		if (condition) {
			result = left;
			result->next = merge(left->next, right, option);
		}
		else {
			result = right;
			result->next = merge(left, right->next, option);
		}
		return result;
	}

	Node<T>* mergeSort(Node<T>* head, int option) {
		if (!head || !head->next) {
			return head;
		}

		// Split the linked list into two halves
		Node<T>* mid = getMiddle(head);
		Node<T>* rightHalf = mid->next;
		mid->next = nullptr;

		// Sort both halves
		Node<T>* left = mergeSort(head, option);
		Node<T>* right = mergeSort(rightHalf, option);

		// Merge sorted halves
		return merge(left, right, option);
	}

	Node<T>* getMiddle(Node<T>* head) {
		if (!head) return head;

		Node<T>* slow = head;
		Node<T>* fast = head;

		while (fast->next && fast->next->next) {
			slow = slow->next;
			fast = fast->next->next;
		}
		return slow;
	}


};
#endif // !ORDER_H