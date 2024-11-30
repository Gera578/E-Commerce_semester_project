#include <iostream>
#include "Store.h"
#include "costumer.h"
#include "Order.h"
#include "product.h"
#include "Category.h"

using namespace std;

int main() {
    // Create a Costumer object
    Costumer customer("John Doe", 150.00, 101);

    // Add purchases

    customer.addPurchase("Laptop");
    customer.addPurchase("Phone");
    customer.addPurchase("Headphones");

    // Test the overloaded << operator
    cout << customer << endl;

    // Export the purchase history
    customer.exportPurchaseHistory("purchase_history.txt");

    return 0;
}
