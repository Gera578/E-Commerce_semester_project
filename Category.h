#pragma once
#ifndef CAREGORY_H
#define CATEGORY_H

#include <iostream>
#include <string>
#include "product.h"

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
            Product** newCapacity = new Product*[capacity];

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


	//function to remove a product
	bool remove(int id) {
		for (int i = 0; i < size; i++) {
			if (product[i]->getID() == id) {
				delete product[i];

				for (int j = i; j < size - 1; j++) {
					product[j] = product[j - 1]; //reduce the size of the inventory
				}

				size--;
				cout << "Product with id " << id << " removed\n";
				return true;
			}

		}
		cout << "Product with id " << id << " not found\n";
		return false;

	}

	//function to search a product
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

	//export to a file the catgeories
	void exportToFile(const string& filename) {
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

	//function to remove ALL products in a category
	void clearAllProducts() {
		for (int i = 0; i < size; i++) {
			delete product[i];
		}
		size = 0;
		cout << "All products removed from category.\n";
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
