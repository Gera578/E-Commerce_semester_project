#pragma once
#ifndef CAREGORY_H
#define CATEGORY_H

#include <iostream>
#include <string>
#include "product.h"
#include <fstream>

using namespace std;

//class to organize produccts by their category
class Category{
private:
	string name;	//name of the category
	Product** product;		//array
	int size;		//current number of products in the category
	int capacity;	//maximum capacity

public:

	//default constructor
	Category() {
		name = "";
		capacity = 10;
		product = new Product * [capacity]; //initial capacity of 10
		size = 0;
	}

	Category(string categoryName, int cap = 10) {
		name = categoryName;
		capacity = cap;
		product = new Product * [cap];
		size = 0;
	}

	// Destructor
	~Category() {
		for (int i = 0; i < size; i++) {
			delete product[i];
		}
		delete[] product;
	}

	void addProduct(const Product& newProduct) {
		if (size == capacity) {
			// Double the capacity of the array
			capacity *= 2;

			// Create a new array with the increased capacity
			Product** newCapacity = new Product * [capacity];

			// Copy the existing products to the new array
			for (int i = 0; i < size; i++) {
				newCapacity[i] = product[i];  // Copy the Product pointer
			}

			// Free the old array
			delete[] product;

			// Point the product array to the new, larger array
			product = newCapacity;
		}

		// Add the new product at the end of the array
		product[size] = new Product(newProduct);  // Create a new Product object
		size++;
	}



	//function to remove a product by id
	bool remove(int id) {
		for (int i = 0; i < size; i++) {
			if (product[i]->getID() == id) {
				delete product[i];

				for (int j = i; j < size - 1; j++) {
					product[j] = product[j + 1]; // Shift elements to the left
				}

				cout << "Product with id " << id << " removed\n";
				size--;
				return true;
			}
		}
		cout << "Product with id " << id << " not found\n";
		return false;
	}


	//function to remove ALL products in a category
	void clearAllProducts() {
		for (int i = 0; i < size; i++) {
			if (product[i] != nullptr) {
				delete product[i];  // Delete each product if not nullptr
				product[i] = nullptr; // Set the pointer to null after deletion to avoid dangling pointer
			}
		}
		delete[] product;  // Delete the array of pointers itself
		product = nullptr;  // Set the array pointer to null for safety
		size = 0;
		cout << "All products removed from category.\n";
	}



	//function to search a product by id
	Product* searchId(int productId) {
		for (int i = 0; i < size; i++) {
			if (product[i]->getID() == productId)//we can access to the function getID from Product
			{
				return product[i];
			}
		}
		cout << "Product with the id '" << productId << "' not found\n";
		return NULL;
	}

	//similar than search by ID, but with name
	Product* search(string& name) {
		for (int i = 0; i < size; i++) {
			if (product[i]->getProduct() == name)//we can access to the function getID from Product
			{
				return product[i];
			}
		}

		cout << "Product with the name '" << name << "' not found\n";
		return NULL;
	}

	//returns the average price of the category
	double averagePrice() {
		if (size == 0) return 0.0;

		double totalPrice = 0;
		for (int i = 0; i < size; i++) {
			totalPrice += product[i]->getPrice();
		}
		return totalPrice / size;
	}

	//find the most expesive product
	Product* mostExpensiveProduct() {
		if (size == 0) return nullptr;

		Product* maxProduct = product[0];
		for (int i = 1; i < size; i++) {
			if (product[i]->getPrice() > maxProduct->getPrice()) {
				maxProduct = product[i];
			}
		}
		return maxProduct;
	}

	//find the cheapest product
	Product* leastExpensiveProduct() {
		if (size == 0) return nullptr;

		Product* minProduct = product[0];
		for (int i = 1; i < size; i++) {
			if (product[i]->getPrice() < minProduct->getPrice()) {
				minProduct = product[i];
			}
		}
		return minProduct;
	}

	// Overload the << operator for easy printing of customer information.
	friend ostream& operator<<(ostream& os, Category& category) {
		os << "Category: " << category.name << endl;
		for (int i = 0; i < category.size; i++) {
			os << i + 1 << ". " << category.product[i]->getProduct() << " (ID: "
				<< category.product[i]->getID() << ", Price: $"
				<< category.product[i]->getPrice() << ")\n";
		}
		return os;
	}

	//function to get information from a file
	void loadFromFile(string& filename) {
		ifstream inFile(filename);
		if (!inFile) {
			cout << "Error opening file for loading!" << endl;
			return;
		}

		getline(inFile, name); // Load category name
		inFile >> size; // Load number of products
		inFile.ignore(); // Ignore the remaining newline

		for (int i = 0; i < size; i++) {
			string productName;
			double price;
			int quantity;

			getline(inFile, productName, ',');
			inFile >> price;
			inFile.ignore();
			inFile >> quantity;
			inFile.ignore();

			addProduct(Product(productName, i + 1, price, quantity)); // Adjust as needed
		}
		inFile.close();
		cout << "Category loaded from " << filename << endl;
	}

	//export to a file the catgeories
	void exportToFile( string& filename) {
		ofstream outFile(filename);
		if (!outFile) {
			cout << "Error opening file for export.\n";
			return;
		}
		outFile << "Category: " << name << endl;
		for (int i = 0; i < size; i++) {
			outFile << "Product ID: " << product[i]->getID()
				<< ", Name: " << product[i]->getProduct()
				<< ", Price: " << product[i]->getPrice() << endl;
		}
		outFile.close();
		cout << "Category exported to " << filename << endl;
	}

	//function to return the total value of all the products in certain category
	double getTotalValue() {
		double total = 0.0;
		for (int i = 0; i < size; i++) {
			total += product[i]->getPrice() * product[i]->getQuantity();
		}
		return total;
	}


	void displayCategory() {
		cout << "Category: " << name << endl;
		for (int i = 0; i < size; i++) {
			product[i]->display();
			cout << endl;
		}
	}

	string getName() {
		return name;
	}

	int getSize() {
		return size;
	}
};

#endif // !CAREGORY_H
