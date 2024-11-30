#pragma once
#ifndef STORE_H
#define STORE_H

#include <iostream>
#include "product.h"
#include "costumer.h"
#include <fstream>
#include <string>

using namespace std;
class store {
private:
	Costumer** costumers;
	Product** inventory; //dynamic array
	int size; //current size of the inventory
	int capacity; //capacity of inventory of certain product
	double earnings;

public:

	// Default constructor
	store() {
		costumers = new Costumer*[10];
		inventory = new Product*[20]; 
		size = 0;
		capacity = 0;
		earnings = 0.0;
	}

	// Parameterized constructor
	store(int size, int initialCapacity = 20) {
		costumers = new Costumer * [10];
		this->size = size;
		capacity = initialCapacity;
		earnings = 0.0;
		inventory = new Product * [capacity];  // Allocate memory for the inventory

	}




	//function to re size the capacity of inventory
	void reSize(Product* p) {
		capacity *= 2;
		Product** newCapacity = new Product * [capacity];

		for (int i = 0; i < size; i++) {
			newCapacity[i] = inventory[i];
		}
		delete[] inventory;
		inventory = newCapacity;
	}

	//function to add a product 
	void addProduct(Product* newProduct) {
		if (size == capacity) {
			reSize(newProduct);
		}
		inventory[size++] = newProduct;
	}

	//function to re stock a product in the store
	void reStock(int id, int quantity) {
		Product* p = search(id);

		if (p) { //if the product exists
			int newQauntity = p->getQuantity() + quantity;
			p->setQuantity(newQauntity);
			cout << "Product has been re-stocked, new quantity: " << newQauntity << endl;
		}
		else {
			cout << "Product " << id << " not found\n";
		}
	}

	//function to remove a product
	void remove(int id) {
		for (int i = 0; i < size; i++) {
			if (inventory[i]->getID() == id) {
				delete inventory[i];

				for (int j = i; j < size - 1; j++) {
					inventory[j] = inventory[j - 1]; //reduce the size of the inventory
				}

				size--;
				cout << "Product with id " << id << " removed\n";
				return;
			}

		}
		cout << "Product with id " << id << " not found\n";

	}

	//function to search a product
	Product* search(int productId) {
		for (int i = 0; i < size; i++) {
			if (inventory[i]->getID() == productId)//we can access to the function getID from Product
			{
				return inventory[i];
			}
		}

		return NULL;
	}

	//Verifies product availability 
	bool purchase(int id, int quantity) {
		Product* product = search(id);
		if (product) {
			if (product->getQuantity() >= quantity) {
				double totalPrice = product->getPrice() * quantity;
				product->setQuantity(product->getQuantity() - quantity);
				earnings += totalPrice;
				cout << "Purchase successful! Total: $" << totalPrice << endl;
				return true;
			}
			else {
				cout << "Not enough stock available." << endl;
			}
		}
		else {
			cout << "Product not found." << endl;
		}
		return false;
	}

	void processPurchase(int customerId, int productId, int quantity) {
		// Find the customer by ID
		Costumer* customer = nullptr;
		for (int i = 0; i < capacity; i++) {
			if (costumers[i]->getCostumerId() == customerId) {
				customer = costumers[i];
				break;
			}
		}

		if (!customer) {
			cout << "Customer with ID " << customerId << " not found." << endl;
			return;
		}

		// Find the product by ID
		Product* product = nullptr;
		for (int i = 0; i < size; i++) {
			if (inventory[i]->getID() == productId) {
				product = inventory[i];
				break;
			}
		}

		if (!product) {
			cout << "Product with ID " << productId << " not found." << endl;
			return;
		}

		// Check product availability
		if (product->getQuantity() < quantity) {
			cout << "Not enough stock for product: " << product->getProduct() << endl;
			return;
		}

		// Calculate total price
		double totalPrice = product->getPrice() * quantity;

		// Check customer balance
		if (!customer->makePurchase(totalPrice)) {
			cout << "Customer " << customer->getName() << " does not have enough balance." << endl;
			return;
		}

		// Process the purchase
		product->setQuantity(product->getQuantity() - quantity);
        string item = product->getProduct() + " x" + to_string(quantity);
        customer->addPurchase(item);
		earnings += totalPrice;

		cout << "Purchase successful! " << customer->getName()
			<< " bought " << quantity << " of " << product->getProduct() << endl;
	}

	//function to sell a product, it checks if there's enough stock, updates the stock
	void sellProduct(int productId, int quantity) {
		for (int i = 0; i < size; i++) {
			if (inventory[i]->getID() == productId) {
				if (inventory[i]->getQuantity() >= quantity) {
					inventory[i]->setQuantity(inventory[i]->getQuantity() - quantity);
					earnings += inventory[i]->getPrice() * quantity;
					cout << "Sold " << quantity << " of " << inventory[i]->getProduct() << endl;
				}
				else {
					cout << "Not enough stock to sell " << quantity << " of " << inventory[i]->getProduct() << endl;
				}
				return; // Product found, no need to continue searching
			}
		}
		cout << "Product with ID " << productId << " not found in inventory." << endl;
	}


	//function to display elements vailable
	void displayAvailable() {
		for (int i = 0; i < size; i++) {
			inventory[i]->display();
			cout << "------------------\n";
		}
	}

	//function to get information from a file
	void loadInventory(string& filename) {
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
			if (line.find("Product:") != string::npos) {
				// Extract product name
				productName = line.substr(line.find(":") + 2);

				// Read and parse subsequent lines
				getline(file, line); // Price
				price = stod(line.substr(line.find("$") + 1));

				getline(file, line); // ID
				id = stoi(line.substr(line.find(":") + 2));

				getline(file, line); // Quantity
				quantity = stoi(line.substr(line.find(":") + 2));

				// Add the product to inventory
				addProduct(new Product(productName, id, price, quantity));
			}
		}

		file.close();
	}

	//function to save the infromation in a file
	void saveInventory(string& filename) {
		ofstream file(filename);

		for (int i = 0; i < size; i++) {
			file << "Product: " << inventory[i]->getProduct() << endl
				<< "Price: $" << inventory[i]->getPrice() << endl
				<< "ID: " << inventory[i]->getID() << endl
				<< "In stock: " << inventory[i]->getQuantity() << endl;
		}

		file.close();
	}

	//function to write on a file the sales report
	void salesReport(string& filename) {
		ofstream file(filename);
		file << "Total Revenue: $" << earnings << endl;
		file.close();
	}

};
#endif // !STORE_H