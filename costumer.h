#pragma once

#ifndef COSTUMER_H
#define COSTUMER_H


#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Costumer {
protected:
	string name;
	double wallet;
	int costumerId;
	int loyaltyPoints;

	string* purchaseHistory; // Dynamic array for purchase history
	int purchaseCount;       // Number of purchases made
	int capacity;            // Maximum capacity of the array

public:

	Costumer() {
		name = "";
		wallet = 0.0;
		costumerId = 0;
		loyaltyPoints = 0;

		purchaseCount = 0;
		capacity = 5;
		purchaseHistory = new string[capacity];
	}

	Costumer(string nam, double balance, int id) {
		name = nam;
		wallet = balance;
		costumerId = id;
		loyaltyPoints = 0;

		purchaseCount = 0;
		capacity = 5;
		purchaseHistory = new string[capacity];
	}


	//gets
	string getName() {
		return name;
	}

	double getWallet() {
		return wallet;
	}

	int getCostumerId() const {
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

	void addLoyaltyPoints(int points) {
		if (points > 0) {
			loyaltyPoints += points;
			cout << points << " points added to " << name << "'s account.\n";
		}
		else {
			cout << "Invalid point value! Points must be greater than 0.\n";
		}
	}


	bool makePurchase(double amount) {

		if (this == nullptr) {
			cout << "Invalid customer object.\n";
			return false;
		}

		if (wallet >= amount) {
			wallet -= amount;
			return true;
		}

		return false;
	}
	
	void addPurchase( string item) {
		// Initialize purchaseHistory if it's null
		if (purchaseHistory == nullptr) {
			purchaseHistory = new string[capacity]; // Initial allocation if not done
		}

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
		if (index < 0 || index >= purchaseCount) {
			cout << "Invalid item\n";
			return;
		}
		// Move the last item to the 'deleted' index
		purchaseHistory[index] = purchaseHistory[purchaseCount - 1];
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
		purchaseHistory = new string[capacity];
		purchaseCount = 0;
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

	void exportCustomerInfo(const string& filename) {
		ofstream outFile(filename);
		if (!outFile) {
			cout << "Error opening file: " << filename << endl;
			return;
		}
		outFile << "Customer Information for " << name << endl;
		outFile << "Wallet Balance: $" << wallet << endl;
		outFile << "Total Purchases: " << purchaseCount << endl;
		outFile.close();
		cout << "Customer information exported to " << filename << endl;
	}

	void loadPurchaseHistory(const string& filename) {
		ifstream inFile(filename);
		if (!inFile) {
			cout << "Error opening file for loading!" << endl;
			return;
		}
		string item;
		while (getline(inFile, item)) {
			addPurchase(item);
		}
		inFile.close();
		cout << "Purchase history loaded from " << filename << endl;
	}

	void displayPurchaseHistory() {
		cout << "Purchase History for " << name << ":" << endl;
		for (int i = 0; i < purchaseCount; i++) {
			cout << i + 1 << ". " << purchaseHistory[i] << endl;
		}
	}

};
#endif // !COSTUMER_H