#pragma once
#ifndef STORE_H
#define STORE_H

#include "product.h"
#include "costumer.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class store {
protected:
	Costumer** costumers;
	Product** inventory; //dynamic array
	int size; //current size of the inventory
	int capacity; //capacity of inventory of certain product
	double earnings;

public:

	// Default constructor
	store() {
		capacity = 10;
		costumers = new Costumer*[10];
		inventory = new Product*[20]; 
		size = 0;
		earnings = 0.0;
	}

	// Parameterized constructor
	store(int size, int initialCapacity = 20) {
		this->size = size;
		capacity = initialCapacity;
		earnings = 0.0;
		costumers = new Costumer * [10];
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
	bool purchase(int id, int quantity, Costumer& costumer) {
		Product* product = search(id);
		if (product) {
			if (product->getQuantity() >= quantity) {
				double totalPrice = product->getPrice() * quantity;
				product->setQuantity(product->getQuantity() - quantity);
				earnings += totalPrice;
				cout << "Purchase successful! Total: $" << totalPrice << endl;
				if (costumer.makePurchase(totalPrice)) {
					cout << "Payment successful! Remaining balance: $" << costumer.getWallet() << endl;
					return true;
				}
				else {
					cout << "Insufficient funds in wallet!, try to add more money" << endl;

				}
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
	virtual void displayAvailable() {
		for (int i = 0; i < size; i++) {
			if (inventory[i] != nullptr) {
				cout << "Product: " << inventory[i]->getProduct() << "\n"
					<< "ID: " << inventory[i]->getID() << "\n"
					<< "Price: $" << inventory[i]->getPrice() << "\n"
					<< "Quantity: " << inventory[i]->getQuantity() << "\n"
					<< "---------------------------" << endl;

			}
			else {
				cout << "Inventory slot " << i << " is empty." << endl;
			}
		}
	}

	//function to get information from a file
	virtual void loadInventory(string& filename) {
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
	virtual void saveInventory(string& filename) {
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
	virtual void salesReport(string& filename) {
		ofstream file(filename);
		file << "Total Revenue: $" << earnings << endl;
		file.close();
	}

};

class bestBuy : public store {
private:
public:
	bestBuy() : store() {}

	// Apply a discount to a specific product by ID
	void applyDiscount(int productId, double discountPercentage) {
		for (int i = 0; i < size; i++) {
			if (inventory[i]->getID() == productId) {
				SpecialProduct* sp = dynamic_cast<SpecialProduct*>(inventory[i]);
				if (sp) {
					sp->setDiscount(discountPercentage / 100.0); // Convert to fractional form
					cout << "Discount of " << discountPercentage << "% applied to product ID " << productId << endl;
				}
				else {
					cout << "Product with ID " << productId << " is not a special product.\n";
				}
				return;
			}
		}
		cout << "Product with ID " << productId << " not found.\n";
	}


    void rewardLoyaltyPoints(Costumer* customer, double amountSpent) {
        if (customer) {
            int points = static_cast<int>(amountSpent / 10); // 1 point per $10 spent
            customer->addLoyaltyPoints(points);
            cout << "Rewarded " << points << " points to customer: " << customer->getName() << endl;
        }
        else {
            cout << "Customer not found!\n";
        }
    }


	// Display products on promotion
	void displayPromotions() {
		cout << "Promotional Products:\n";
		for (int i = 0; i < size; i++) {
			SpecialProduct* sp = dynamic_cast<SpecialProduct*>(inventory[i]);
			if (sp && sp->getDiscount() > 0.0) {
				sp->display();
				cout << "-----------------------\n";
			}
		}
	}

	void useStoreFeatures(string name, int id, double price) {
		// Inherited functions
		addProduct(new Product(name, 101, 599.99, 15));
		sellProduct(101, 2);
		string filename = "inventory.txt";
		saveInventory(filename);
		cout << "Inventory saved to file.\n";
	}

	// Overridden display function to show all inventory
	void displayAvailable() override {
		cout << "Best Buy Inventory:\n";
		for (int i = 0; i < size; i++) {
			if (inventory[i] != nullptr) {
				inventory[i]->display();
				cout << "-----------------------\n";
			}
		}
	}
};

class Gamestop : public store {
public:
    Gamestop() : store() {}

    // You can also add new methods specific to Gamestop, like adding promotions or discounts
    void applyDiscount(int productId, double discountPercentage) {
        Product* product = search(productId);
        if (product) {
            double discountedPrice = product->getPrice() * (1 - discountPercentage / 100);
            product->setPrice(discountedPrice);
            cout << "Discount applied! New price for " << product->getProduct() << ": $" << discountedPrice << endl;
        }
        else {
            cout << "Product not found." << endl;
        }
    }
};

#endif // !STORE_H