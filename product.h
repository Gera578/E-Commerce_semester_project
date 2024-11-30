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

public:
	Product() {
		productId = 0;
		product = "";
		this->price = 0;
		quantity = 0;
	}

	Product(string pro, int id,  double price, int qty) {
		productId = id;
		product = pro;
		this->price = price;
		quantity = qty;
	}
	//gets
	int getID() {
		return productId;
	}

	string getProduct() {
		return product; //the name of the product
	}

	double getPrice() {
		return price;
	}

	int getQuantity() {
		return quantity;
	}

	void setQuantity(int qty) {
		quantity = qty;
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
private:
	double discount;

public:

	SpecialProduct() {
		productId = 0;
		product = "";
		this->price = 0;
		quantity = 0;
		discount = 0.0;
	}
	SpecialProduct( string pro,int id, double price, int qty, double disc)
		: Product(pro,id,  price, qty), // add the previous class
		discount(disc) {} //discount = disc

	void display() override {
		Product::display(); // re-use display function with override
		cout << "Discount: " << discount * 100 << "% off\n";
		cout << "Final price: " << fixed <<setprecision(2)<<getFinalPrice() << endl;
		
	}

	double getFinalPrice() {
		return price * (1 - discount);
	}


};

class Sort {
public:

	//functions to sort the products
	void merge(Product s[], int left, int mid, int right, int option) {
		int lsize = mid - left + 1; //left size
		int rsize = right - mid; //right size

		//temporary subarrays
		Product* leftArray = new Product[lsize];
		Product* rightArray = new Product[rsize];

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


	void mergeSort(Product s[], int left, int right, int option) {
		int mid;
		if (left < right) {
			mid = left + (right - left) / 2;
			mergeSort(s, left, mid, option);
			mergeSort(s, mid + 1, right, option);
			merge(s, left, mid, right, option);
		}
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