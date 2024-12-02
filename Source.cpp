#include <iostream>
#include <string>
#include "Category.h"
#include "costumer.h"
#include "Order.h"
#include "product.h"
#include "Store.h"

using namespace std;

int main() {
    Order<Product> order1;

    // Load products from a file
    string filename = "gamestop_inventory.txt"; // Make sure this file exists and has valid data
    order1.loadInventory(filename); // This will load products into the order

    // Display the orders in the list after loading
    cout << "Displaying all orders:" << endl;
    order1.displayOrder();

    // Sort products by price in ascending order
    cout << "\nSorting products by price (ascending)..." << endl;
    order1.sortOrder(1);  // Call the sortByPrice method

    // Display the sorted orders
    cout << "Displaying sorted orders:" << endl;
    order1.displayOrder();

    // Save sorted orders to a file
    string outputFilename = "order_sorted.txt";
    order1.saveToFile(outputFilename);

    // Load orders from the saved file
    order1.loadInventory(outputFilename);

    // Display the loaded orders
    cout << "\nDisplaying orders after loading from file:" << endl;
    order1.displayOrder();

    return 0;
}
