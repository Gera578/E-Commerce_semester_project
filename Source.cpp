#include <iostream>
#include <string>
#include "Category.h"
#include "costumer.h"
#include "Order.h"
#include "product.h"
#include "Store.h"

using namespace std;

int main() {
    //objects creation
    Order<Product> order;
    Category category;

    Category consoleCategory("Consoles");
    Category pcConsoleCategory("Products for console and PC");
    Category videogamesCategory("Videogames");

    string name;
    double wallet = 1000.00; ///initial balance of a thousand dollars
    Costumer costumer("Gera", wallet, 1);

    Product* foundProduct = nullptr;
    Store store;

    const int max = 100; //define the max size of the array

    // Allocate arrays on the heap
    Product* product = new Product[max];
    SpecialProduct* sp = new SpecialProduct[max];
    SpecialProduct* gsSp = new SpecialProduct[max];
    Category* gsCategory = new Category[max];
    Product* gsProduct = new Product[max];

    bestBuy bestbuy;

    int product_count = 0; //to keep track of what is in the array

    Gamestop gs;
    Sort sort;
    ///////////////////////////////////////////

    //variables
    double amount = 0.0; //to tell how much to pay in best buy

    int choice; //for the switch to select what store
    bool admin = true; //to know in which mode the program will run
    string bestFile = "products.txt"; //load inventory for best buy
    string gameFile = "gamestop_inventory.txt"; //load inventory for game stop
    string categoryFile = "bestCategory.txt";

    string costumerFile = "costumer_purchase_information.txt";
    string costumerInfo = "costumer_info.txt";
    string orderFile = "order.txt"; 


    string orderCategory; //to order by category the order
    int bestID = 0; //to find a product by its id with best buy

    int gsID = 0; //to find products in gamestop

    bool bought = true; //to know if the user have bought or not a product in best buy

    int purchase = 0; //to know if the user will purchase or not in best buy

    int addBalanceOrNot = 0; //to know if the user want to add balance or not
    int toSort = 0; //to know if the user wants to sort the products or not
    int sortOption = 0;

    int size = 0;

    int visited = 0; //checks if the user have bought in best buy
    int visited2 = 0; //checks if the user have bought in game stop

    //variables from below are being used on case 3
    int removeOrNot = 0; //case 3, to know if the order will be deleted or not

    int index = 0;
    int quantity = 0;
    ///////////////////////////////////////////
    int role = 0;//to know if the test will run as costumer or admin
    int adminChoice = 0;

    while (true) {
        system("cls");
        cout << "[1] Costumer\n[2] Admin\n[3] Exit\n";
        cin >> role;
        switch (role) {
        case 1:
            cout << "Welcome! We have a couple of stores for now: Best Buy for electronics and GameStop for videogames\n";
            cout << "Menu\n";
            cout << "Best Buy [1]\nGameStop [2]\Summary [3]\n";
            cin >> choice;
            switch (choice)
            {
            case 1:
                bestbuy.loadInventory(bestFile, product, product_count, max);
                cout << "Welcome to Best Buy!, These are our products available: \n";

                for (int i = 0; i < product_count; i++) {
                    product[i].display();
                    cout << "---------------------------------\n";
                }

                bought = false;
                while (bought == false) {

                    //if the user enters any other number that is not 1, it will continue and display it unsorted
                    cout << "Do you want to sort the products? yes[1] no[2]\n";
                    cin >> toSort;

                    if (toSort == 1) {
                        cout << "Options to sort:\n[1] Cheapest firsts\n[2] Expensive first\n[3] By name\n[4] By less quantity\n[5] By ID\n";
                        cin >> sortOption;

                        switch (sortOption) {
                        case 1:
                            sort.mergeSort(product, 0, product_count - 1, 1);
                            cout << "Sorted in price Ascending order\n";
                            break;

                        case 2:
                            sort.mergeSort(product, 0, product_count - 1, 2);
                            cout << "Sorted in price Descending order\n";
                            break;

                        case 3:
                            sort.mergeSort(product, 0, product_count - 1, 3);
                            cout << "Sorted by name\n";
                            break;

                        case 4:
                            sort.mergeSort(product, 0, product_count - 1, 4);
                            cout << "Sorted by less quantity\n";
                            break;

                        case 5:
                            sort.mergeSort(product, 0, product_count - 1, 5);
                            cout << "Sorted by id\n";
                            break;

                        default:
                            cout << "Invalid sort option!\n";
                            break;
                        }

                    }
                    for (int i = 0; i < product_count; i++) {
                        bestbuy.addProduct(new Product(product[i]));
                    }
                    bestbuy.applyDiscount(101, .24);
                    bestbuy.applyDiscount(110, .30);

                    bestbuy.displayAvailable();
                    cout << endl;

                    for (int i = 0; i < product_count; i++) {
                        bestbuy.addProduct(new SpecialProduct(sp[i]));
                    }
                    bestbuy.displayPromotions();

                    cout << "Enter the id of a product to buy\n";
                    cin >> bestID;

                    // Call search and get the product
                    foundProduct = bestbuy.search(bestID);

                    // Check if the product was found
                    if (foundProduct != NULL) {
                        cout << "Product found:" << endl << endl;
                        foundProduct->display();

                        // Re-check if the wallet has enough after adding funds
                        if (costumer.getWallet() >= foundProduct->getPrice() || costumer.getLoyaltyPoints() >= foundProduct->getPointsRequired()) {
                            cout << "You have enough funds or loyalty points to purchase this product!\n";
                            cout << "Choose your payment method:\n1. Wallet\n2. Loyalty Points\nEnter your choice: ";
                            int paymentChoice;
                            cin >> paymentChoice;

                            if (paymentChoice == 1) { // Wallet purchase
                                if (costumer.getWallet() >= foundProduct->getPrice()) {
                                    // Proceed with wallet purchase
                                    cout << "Enter the quantity of the products to buy: "; cin >> quantity;
                                    bestbuy.sellProduct(foundProduct->getID(), quantity);
                                    if (bestbuy.purchase(foundProduct->getID(), quantity, costumer)) {
                                        cout << "Great! Your order has been saved.\n";
                                        cout << "This is your current wallet balance: $" << costumer.getWallet() << endl;
                                        visited++;

                                        if (visited >= 3) {
                                            bestbuy.rewardLoyaltyPoints(&costumer, foundProduct->getPrice());
                                        }
                                    }
                                }
                                else {
                                    cout << "Insufficient funds, add more: \n";
                                    cout << "Current balance: $" << costumer.getWallet() << endl;

                                    double wallet;
                                    cout << "$";
                                    cin >> wallet;
                                    costumer.addBalance(wallet);
                                }
                            }
                            else if (paymentChoice == 2) { // Points-based purchase
                                if (costumer.getLoyaltyPoints() >= foundProduct->getPointsRequired()) {
                                    int points = foundProduct->getPointsRequired();
                                    costumer.deductLoyaltyPoints(points);
                                    cout << "You have purchased the product using your loyalty points!\n";
                                    cout << "Remaining loyalty points: " << costumer.getLoyaltyPoints() << endl;
                                }
                                else {
                                    cout << "You do not have enough loyalty points.\n";
                                }
                            }
                            else {
                                cout << "Invalid payment choice.\n";
                            }

                            // Record the purchase in the history
                            string purchasedItem = foundProduct->getProduct();
                            costumer.addPurchase(purchasedItem);

                            order.addOrder(*foundProduct);
                            bought = true;

                        }
                        else {
                            cout << "Purchase did not proceed.\n";
                            bought = false;
                        }

                    }
                    else {
                        cout << "Product with id " << bestID << " not found" << endl;
                    }
                }

                cout << "\n\nPress Any key to continue\n";
                cin.ignore();
                cin.get();
                break;

                ////////////////////////////////////////////////////////////////////////////////////////////////

            case 2://FOR GAME STOP
                cout << "Welcome to Game Stop!, These are our products available: \n";
                gs.loadInventory(gameFile, gsProduct, product_count, max);
                //category.loadFromFile(categoryFile);

                for (int i = 0; i < product_count; i++) {
                    gsProduct[i].display();
                    cout << "---------------------------------\n";
                }

                // Display products in each category
                bought = false;
                while (bought == false) {

                    //if the user enters any other number that is not 1, it will continue and display it unsorted
                    cout << "Do you want to sort the products? yes[1] no[2]\n";
                    cin >> toSort;

                    if (toSort == 1) {
                        cout << "Options to sort:\n[1] Cheapest firsts\n[2] Expensive first\n[3] By name\n[4] By less quantity\n[5] By ID\n";
                        cin >> sortOption;

                        switch (sortOption) {
                        case 1:
                            sort.mergeSort(gsProduct, 0, product_count - 1, 1);
                            cout << "Sorted in price Ascending order\n";
                            break;

                        case 2:
                            sort.mergeSort(gsProduct, 0, product_count - 1, 2);
                            cout << "Sorted in price Descending order\n";
                            break;

                        case 3:
                            sort.mergeSort(gsProduct, 0, product_count - 1, 3);
                            cout << "Sorted by name\n";
                            break;

                        case 4:
                            sort.mergeSort(gsProduct, 0, product_count - 1, 4);
                            cout << "Sorted by less quantity\n";
                            break;

                        case 5:
                            sort.mergeSort(gsProduct, 0, product_count - 1, 5);
                            cout << "Sorted by id\n";
                            break;

                        default:
                            cout << "Invalid sort option!\n";
                            break;
                        }

                    }
                    for (int i = 0; i < product_count; i++) {
                        gs.addProduct(new Product(gsProduct[i]));
                    }
                    for (int i = 0; i < product_count; i++) {
                        // Assign products to categories based on their names or other properties
                        if (gsProduct[i].getProduct().find("Xbox") != string::npos || gsProduct[i].getProduct().find("PlayStation") != string::npos || gsProduct[i].getProduct().find("Nintendo") != string::npos) {
                            consoleCategory.addProduct(gsProduct[i]);
                        }
                        else if (gsProduct[i].getProduct().find("Mouse") != string::npos || gsProduct[i].getProduct().find("Keyboard") != string::npos) {
                            pcConsoleCategory.addProduct(gsProduct[i]);
                        }
                        else if (gsProduct[i].getProduct().find("Cyberpunk") != string::npos || gsProduct[i].getProduct().find("Last of Us") != string::npos || gsProduct[i].getProduct().find("Call of Duty") != string::npos) {
                            videogamesCategory.addProduct(gsProduct[i]);

                        }
                    }
                    gs.applyDiscount(101, .24);
                    gs.applyDiscount(110, .30);


                    //gs.displayAvailable();
                    cout << endl;

                    for (int i = 0; i < product_count; i++) {
                        gs.addProduct(new SpecialProduct(gsSp[i]));
                    }
                    gs.displayPromotions();

                    cout << endl << endl;
                    consoleCategory.displayCategory();
                    pcConsoleCategory.displayCategory();
                    videogamesCategory.displayCategory();

                    cout << "Enter the id of a product to buy: "; cin >> gsID;
                    // Call search and get the product
                    foundProduct = gs.search(gsID);

                    // Check if the product was found
                    if (foundProduct != NULL) {
                        cout << "Product found:" << endl << endl;
                        foundProduct->display();

                        // Re-check if the wallet has enough after adding funds
                        if (costumer.getWallet() >= foundProduct->getPrice() || costumer.getLoyaltyPoints() >= foundProduct->getPointsRequired()) {
                            cout << "You have enough funds or loyalty points to purchase this product!\n";
                            cout << "Choose your payment method:\n1. Wallet\n2. Loyalty Points\nEnter your choice: ";
                            int paymentChoice;
                            cin >> paymentChoice;

                            if (paymentChoice == 1) { // Wallet purchase
                                if (costumer.getWallet() >= foundProduct->getPrice()) {
                                    // Proceed with wallet purchase
                                    cout << "Enter the quantity of the products to buy: "; cin >> quantity;
                                    gs.sellProduct(foundProduct->getID(), quantity);
                                    if (gs.purchase(foundProduct->getID(), quantity, costumer)) {
                                        cout << "Great! Your order has been saved.\n";
                                        cout << "This is your current wallet balance: $" << costumer.getWallet() << endl;
                                        // Record the purchase in the history
                                        string purchasedItem = foundProduct->getProduct();
                                        costumer.addPurchase(purchasedItem);

                                        order.addOrder(*foundProduct);
                                        bought = true;
                                        
                                        visited++;

                                        if (visited >= 3) {
                                            gs.rewardLoyaltyPoints(&costumer, foundProduct->getPrice());
                                        }
                                    }
                                }
                                else {
                                    cout << "Insufficient funds, add more: \n";
                                    cout << "Current balance: $" << costumer.getWallet() << endl;

                                    cout << "$";
                                    cin >> wallet;
                                    costumer.addBalance(wallet);
                                }
                            }
                            else if (paymentChoice == 2) { // Points-based purchase
                                if (costumer.getLoyaltyPoints() >= foundProduct->getPointsRequired()) {
                                    int points = foundProduct->getPointsRequired();
                                    costumer.deductLoyaltyPoints(points);
                                    cout << "You have purchased the product using your loyalty points!\n";
                                    cout << "Remaining loyalty points: " << costumer.getLoyaltyPoints() << endl;

                                    // Record the purchase in the history
                                    string purchasedItem = foundProduct->getProduct();
                                    costumer.addPurchase(purchasedItem);

                                    order.addOrder(*foundProduct);
                                    bought = true;
                                }
                                else {
                                    cout << "You do not have enough loyalty points.\n";
                                }
                            }
                            else {
                                cout << "Invalid payment choice.\n";
                            }

                        }
                        else {
                            cout << "Purchase did not proceed.\n";
                            bought = false;
                        }

                    }
                    else {
                        cout << "Product not found\n";
                    }
                }

                cout << "\n\nPress Any key to continue\n";
                cin.ignore();
                cin.get();
                break;
            case 3:
                while (true) {
                    cout << "This is your purchase history: \n\n";
                    costumer.displayPurchaseHistory();//it will display only the name of the products bought, information will come with order.displayOrder();

                    order.displayOrder();

                    cout << "\n Average money spent for the order: " << order.averagePrice() << endl;


                    cout << "Menu:\n[1] Delete a purchase history\n[2] Clear the purchase history\n[3] See information for an order\n[4] Export the history to a file\n[0] Exit\n";
                    cin >> removeOrNot;

                    if (removeOrNot == 1) {
                        cout << "Enter the number of the order you want to delete\n";
                        cin >> index;

                        costumer.removePurchase(index - 1);
                        costumer.displayPurchaseHistory();
                        break;
                    }

                    else if (removeOrNot == 2) {
                        cout << "Are you sure? Enter 'yes' if you are sure\n";
                        string verification;
                        cin >> verification;

                        if (verification == "yes" || verification == "YES") { //make sure the user enter yes
                            costumer.clearPurchaseHistory();
                        }
                        else {
                            cout << "Invalid answer. Your history was not erased.\n";
                        }
                        break;
                    }
                    else if (removeOrNot == 3) {
                        cout << "Enter the id of the product to see its information\n";
                        int id = 0;
                        cin >> id;
                        foundProduct = order.searchProductById(id);
                        foundProduct->display();
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    else if (removeOrNot == 4) {

                        cout << "Do you want to sort your order?\n[1] yes\n[2] no\n";

                        cin >> toSort;

                        if (toSort == 1) {
                            while (true) {
                                cout << "Options to sort:\n[1] Cheapest firsts\n[2] Expensive first\n[3] By name\n[4] By less quantity\n[5] By ID\n";
                                cin >> sortOption;

                                if (sortOption == 1) {
                                    order.sortOrder(1);
                                    break;
                                }
                                else if (sortOption == 2) {
                                    order.sortOrder(2);
                                    break;
                                }
                                else if (sortOption == 3) {
                                    order.sortOrder(3);
                                    break;
                                }
                                else if (sortOption == 4) {
                                    order.sortOrder(4);
                                    break;
                                }
                                else if (sortOption == 5) {
                                    order.sortOrder(5);
                                    break;
                                    return false;
                                }
                                else {
                                    cout << "Invalid option, please try again.\n";
                                }
                            }
                        }

                        costumer.exportCustomerInfo(costumerInfo);
                        costumer.exportPurchaseHistory(costumerFile);
                        order.saveToFile(orderFile);
                        break;

                    }
                    else if (removeOrNot == 0) {
                        break; // Exit the loop
                    }
                    else {
                        cout << "Invalid option. Try again.\n";
                    }

                }
                cout << "Press any key to continue\n";
                cin.ignore();
                cin.get();
                break;
            default:
                break;
            }
            break;

        case 2:
            bestbuy.loadInventory(bestFile, product, product_count, max);
            gs.loadInventory(gameFile, gsProduct, product_count, max);
            for (int i = 0; i < product_count; i++) {
                bestbuy.addProduct(new Product(product[i]));
            }
            for (int i = 0; i < product_count; i++) {
                gs.addProduct(new Product(gsProduct[i]));
            }

            cout << "Admin Menu:\n[1] re-stock a product\n[2] See data in categories for game stop\n";
            cin >> adminChoice;

            if (adminChoice == 1) {

                bestbuy.applyDiscount(101, .24);
                bestbuy.applyDiscount(110, .30);

                bestbuy.displayAvailable();
                gs.displayAvailable();

                cout << "Enter the number of products to re-stock: ";
                cin >> bestID;

                foundProduct = bestbuy.search(bestID);
                bestbuy.reStock(foundProduct->getID(), 10);
                foundProduct->display();
            }
            else if (adminChoice == 2) {
                cout << "This is the information for the different categories at game stop\n";

                cout << "\nTotal value for the category: "<< consoleCategory.getTotalValue() << endl;

                cout << "Average: " << consoleCategory.averagePrice() << endl;

                cout << "\nTotal value for the category: " << pcConsoleCategory.getName() << pcConsoleCategory.getTotalValue() << endl;

                cout << "Average: " << pcConsoleCategory.averagePrice() << endl;

                cout << "\nTotal value for the category: " << videogamesCategory.getName() << " "<< videogamesCategory.getTotalValue() << endl;

                cout << "Average: " << videogamesCategory.averagePrice() << endl;

            }

            cout << "Enter any key to continue\n";
            cin.ignore();
            cin.get();

            break;
        case 3:
            return false;
            break;
        default:
            break;
        }

    }




    return 0;
}

