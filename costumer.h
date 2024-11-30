#pragma once

#ifndef COSTUMER_H
#define COSTUMER_H


#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Costumer {
private:
	string name;
	double wallet;
	int costumerId;

	string* purchaseHistory; // Dynamic array for purchase history
	int purchaseCount;       // Number of purchases made
	int capacity;            // Maximum capacity of the array

public:
	Costumer(string nam, double balance, int id) {
		name = nam;
		wallet = balance;
		costumerId = id;

		purchaseCount = 0;
		capacity = 5;
		purchaseHistory = new string[capacity];
	}

	~Costumer() {
		delete[] purchaseHistory;
	}

	//gets
	string getName() {
		return name;
	}

	double getWallet() {
		return wallet;
	}

	int getCostumerId() {
		return costumerId;
	}

	int getPurchaseCount() {
		return purchaseCount;
	}

	int getCapacity() {
		return capacity;
	}


	//function to add balance to the account
	void addBalance(double amount) {
		if (amount <= 0) {
			cout << "The amount to add must be grater than 0\n";
		}
		else {
			cout << "Amount " << amount << " has been added to your wallet\n";
			wallet += amount;
			cout << "Current amount in wallet: " << wallet << endl;

		}
	}

	bool makePurchase(double amount) {
		if (wallet >= amount) {
			wallet -= amount;
			return true;
		}

		return false;
	}
	
	void addPurchase( string item) {
		// Resize if the array is full
		if (purchaseCount == capacity) {
			capacity *= 2;
			string* newHistory = new string[capacity];
			for (int i = 0; i < purchaseCount; i++) {
				newHistory[i] = purchaseHistory[i];
			}
			delete[] purchaseHistory;
			purchaseHistory = newHistory;
		}

		// Add the new purchase
		purchaseHistory[purchaseCount] = item;
		purchaseCount++;
	}

	void removePurchase(int index) {
		if (index < 0 or index >= purchaseCount) {
			cout << "Invalid item\n";
			return;
		}
		for (int i = 0; i < purchaseCount - 1; i++) {
			purchaseHistory[i] = purchaseHistory[i + 1];
		}
		purchaseCount--;
	}

	bool searchPurchase(string item) {
		for (int i = 0; i < purchaseCount; i++) {
			if (purchaseHistory[i] == item) {
				return true;
			}
		}
		return false;
	}

	void clearPurchaseHistory() {
		delete[] purchaseHistory;
		capacity = 5;
		purchaseCount = 0;
		purchaseHistory = new string[capacity];
		cout << "Purchase history cleared!" << endl;
	}

	// Overload the << operator for easy printing of customer information.
	friend ostream& operator<<(ostream& os, const Costumer& costumer) {
		os << "Costumer ID: " << costumer.costumerId
			<< ", Name: " << costumer.name
			<< ", Wallet Balance: $" << costumer.wallet;
		return os;
	}



	// Function to export the purchase history to a file
	void exportPurchaseHistory(const string& filename) {
		ofstream outFile(filename);
		if (!outFile) { // Check for file opening failure
			cout << "Error opening file: " << filename << endl;
			return;
		}
		outFile << "Purchase History for " << name << ":" << endl;
		for (int i = 0; i < purchaseCount; i++) {
			outFile << i + 1 << ". " << purchaseHistory[i] << endl;
		}
		outFile.close(); // Close the file
		cout << "Purchase history exported to " << filename << endl;
	}



	void displayPurchaseHistory() {
		cout << "Purchase History for " << name << ":" << endl;
		for (int i = 0; i < purchaseCount; i++) {
			cout << i + 1 << ". " << purchaseHistory[i] << endl;
		}
	}

};
#endif // !COSTUMER_H