//Product class where the products are defined and used.
//This file also includes the mergeSort algorithm to sort products by: cheapest, expensive, name, less quantity, id

#pragma once
#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <iomanip> //for set precision to round to fixed decimals

using namespace std;

class Product {
protected:
	int productId;
	string product;
	double price;
	int quantity;

	int pointsRequired;

public:
	Product() {
		productId = 0;
		product = "";
		price = 0.0;
		quantity = 0;
		pointsRequired = 0;
	}


	Product(string pro, int id,  double pr, int qty) {
		productId = id;
		product = pro;
		price = pr;
		quantity = qty;
		pointsRequired = 0;
	}

	//gets
	int getID() {
		return productId;
	}

	string getProduct()const {
		return product; //the name of the product
	}

	double getPrice() const {
		return price;
	}

	int getQuantity() const {
		return quantity;
	}

	int getPointsRequired() {
		return pointsRequired;
	}

	void setQuantity(int qty) {
		quantity = qty;
	}

	//sets
	void setPrice(double pr) {
		price = pr; 
	}

	void displayProductsForPoints(Product** products, int size, int userPoints) {
		cout << "Products you can purchase with your points (" << userPoints << " points available):\n";
		for (int i = 0; i < size; ++i) {
			if (products[i]->getPointsRequired() <= userPoints && products[i]->getQuantity() > 0) {
				cout << products[i]->getID() << ". " << products[i]->getProduct()
					<< " - " << products[i]->getPointsRequired() << " points"
					<< " (" << products[i]->getQuantity() << " in stock)\n";
			}
		}
	}

	void purchaseWithPoints(Product** products, int size, int& userPoints) {
		displayProductsForPoints(products, size, userPoints);

		cout << "Enter the ID of the product you want to purchase: ";
		int productId;
		cin >> productId;

		for (int i = 0; i < size; ++i) {
			if (products[i]->getID() == productId) {
				if (products[i]->getQuantity() > 0 && products[i]->getPointsRequired() <= userPoints) {
					userPoints -= products[i]->getPointsRequired();
					products[i]->setQuantity(products[i]->getQuantity() - 1);
					cout << "You purchased " << products[i]->getProduct() << "! Remaining points: " << userPoints << endl;
				}
				else if (products[i]->getQuantity() == 0) {
					cout << "Sorry, " << products[i]->getProduct() << " is out of stock.\n";
				}
				else {
					cout << "You do not have enough points for " << products[i]->getProduct() << ".\n";
				}
				return;
			}
		}
		cout << "Invalid product ID.\n";
	}


	//for polymorphism, we are going to use it with the class special product
	virtual void display() {
		cout << "Product: " << product << endl
			<< "ID: " << productId << endl
			<< "Price: " << price << endl
			<< "Quantity in stock: " << quantity << endl;
	}

	virtual ~Product() {};

};

//class for prodducts that contain a discount
class SpecialProduct : public Product{ //inheritance to the class Product
protected:
	double discount;

public:

	SpecialProduct() {
		productId = 0;
		product = "";
		price = 0.0;
		quantity = 0;
		discount = 0.0;
	}
	SpecialProduct( string pro,int id, double price, int qty, double disc)
		: Product(pro,id,  price, qty), // add the previous class
		discount(disc) {} //discount = disc

	double getDiscount() {
		return discount;
	}

	void setDiscount(double disc) {
		discount = disc;
	}

	void display() override {
		Product::display(); // re-use display function with override
		cout << "Discount: " << discount * 100 << "% off\n";
		cout << "Final price: " << fixed <<setprecision(2)<<getFinalPrice() << endl;
		
	}

	double getFinalPrice() {
		return price * (price * discount / 100.00);
	}

};

class Sort {
public:

	//functions to sort the products
	void merge(Product products[], int left, int mid, int right, int sortOption) {
		int n1 = mid - left + 1;
		int n2 = right - mid;

		Product* leftArray = new Product[n1];
		Product* rightArray = new Product[n2];

		// Copy data into temporary arrays
		for (int i = 0; i < n1; i++) leftArray[i] = products[left + i];
		for (int i = 0; i < n2; i++) rightArray[i] = products[mid + 1 + i];

		int i = 0, j = 0, k = left;
		while (i < n1 && j < n2) {
			bool conditionMet = false;

			// Sort based on the chosen option
			switch (sortOption) {
			case 1: // Price Ascending
				conditionMet = leftArray[i].getPrice() <= rightArray[j].getPrice();
				break;
			case 2: // Price Descending
				conditionMet = leftArray[i].getPrice() >= rightArray[j].getPrice();
				break;
			case 3: // By Name
				conditionMet = leftArray[i].getProduct() <= rightArray[j].getProduct();
				break;
			case 4: // Quantity Ascending
				conditionMet = leftArray[i].getQuantity() <= rightArray[j].getQuantity();
				break;
			case 5: // ID Ascending
				conditionMet = leftArray[i].getQuantity() <= rightArray[j].getID();
				break;
			default:
				cout << "Invalid option\n";
				break;
			}

			if (conditionMet) {
				products[k] = leftArray[i];
				i++;
			}
			else {
				products[k] = rightArray[j];
				j++;
			}
			k++;
		}

		// Copy any remaining elements of leftArray[]
		while (i < n1) {
			products[k] = leftArray[i];
			i++;
			k++;
		}

		// Copy any remaining elements of rightArray[]
		while (j < n2) {
			products[k] = rightArray[j];
			j++;
			k++;
		}

		delete[] leftArray;
		delete[] rightArray;
	}

	void mergeSort(Product products[], int left, int right, int sortOption) {
		if (left >= right) return;  // Base case for recursion

		int mid = left + (right - left) / 2;

		mergeSort(products, left, mid, sortOption);
		mergeSort(products, mid + 1, right, sortOption);

		merge(products, left, mid, right, sortOption);
	}

	//function to sort the special products:
	 void merge(SpecialProduct s[], int left, int mid, int right, int option) {
		int lsize = mid - left + 1; //left size
		int rsize = right - mid; //right size

		//temporary subarrays
		SpecialProduct* leftArray = new SpecialProduct[lsize];
		SpecialProduct* rightArray = new SpecialProduct[rsize];

		//we copy the data to the temporary arrays:

		//left array
		for (int i = 0; i < lsize; i++) {
			leftArray[i] = s[left + i];
		}

		//right array
		for (int j = 0; j < rsize; j++) {
			rightArray[j] = s[mid + 1 + j];
		}

		//i and j keeps track of the two subarrays
		int i = 0; //i for the left
		int j = 0; //j for the right
		int k = left; //original array (s)

		while (i < lsize && j < rsize) {
			bool conidtion = false;
			switch (option) {
			case 1:
				conidtion = leftArray[i].getPrice() <= rightArray[j].getPrice();
				break;
			case 2:
				conidtion = leftArray[i].getPrice() >= rightArray[j].getPrice();
				break;
			case 3:
				conidtion = leftArray[i].getProduct() <= rightArray[j].getProduct();
				break;
			case 4:
				conidtion = leftArray[i].getQuantity() <= rightArray[j].getQuantity();
				break;
			default:
				cout << "invalid option";
				break;
			}
			if (conidtion) {

				s[k] = leftArray[i];
				i++; //let's go to next element on the left side...
			}
			else {
				s[k] = rightArray[j];
				j++; //let's go to next element on the right side...
			}
			k++; //let's go to next element on the original array...
		}

		// Copy the remaining elements of leftArray, 
		// if there are any
		while (i < lsize) {
			s[k] = leftArray[i];
			i++;
			k++;
		}

		// Copy the remaining elements of rightArray, 
		// if there are any
		while (j < rsize) {
			s[k] = rightArray[j];
			j++;
			k++;
		}

		//Free allocated memory
		delete[] leftArray;
		delete[] rightArray;
	}

	void mergeSort(SpecialProduct s[], int left, int right, int option) {
		int mid;
		if (left < right) {
			mid = left + (right - left) / 2;
			mergeSort(s, left, mid, option);
			mergeSort(s, mid + 1, right, option);
			merge(s, left, mid, right, option);
		}
	}


};

#endif // !PRODUCT_H