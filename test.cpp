#include <gtest>
#include "Category.h"
#include "costumer.h"
#include "Order.h"
#include "product.h"
#include "Store.h"

// Test for Product class
TEST(ProductTest, DefaultConstructor) {
    Product p;
    EXPECT_EQ(p.getID(), 0);
    EXPECT_EQ(p.getProduct(), "");
    EXPECT_EQ(p.getPrice(), 0.0);
    EXPECT_EQ(p.getQuantity(), 0);
    EXPECT_EQ(p.getPointsRequired(), 0);
}

TEST(ProductTest, ParameterizedConstructor) {
    Product p("Laptop", 1, 999.99, 10);
    EXPECT_EQ(p.getID(), 1);
    EXPECT_EQ(p.getProduct(), "Laptop");
    EXPECT_EQ(p.getPrice(), 999.99);
    EXPECT_EQ(p.getQuantity(), 10);
}

TEST(ProductTest, SetAndGetPrice) {
    Product p;
    p.setPrice(499.99);
    EXPECT_EQ(p.getPrice(), 499.99);
}

TEST(ProductTest, SetAndGetQuantity) {
    Product p;
    p.setQuantity(20);
    EXPECT_EQ(p.getQuantity(), 20);
}

// Test for SpecialProduct class
TEST(SpecialProductTest, DefaultConstructor) {
    SpecialProduct sp;
    EXPECT_EQ(sp.getDiscount(), 0.0);
}

TEST(SpecialProductTest, ParameterizedConstructor) {
    SpecialProduct sp("Smartphone", 2, 799.99, 5, 0.1);
    EXPECT_EQ(sp.getID(), 2);
    EXPECT_EQ(sp.getProduct(), "Smartphone");
    EXPECT_EQ(sp.getPrice(), 799.99);
    EXPECT_EQ(sp.getQuantity(), 5);
    EXPECT_EQ(sp.getDiscount(), 0.1);
}

TEST(SpecialProductTest, GetFinalPrice) {
    SpecialProduct sp("Smartphone", 2, 799.99, 5, 0.1);
    EXPECT_NEAR(sp.getFinalPrice(), 719.991, 0.001);
}

// Test for Costumer class
TEST(CostumerTest, DefaultConstructor) {
    Costumer c;
    EXPECT_EQ(c.getName(), "");
    EXPECT_EQ(c.getWallet(), 0.0);
    EXPECT_EQ(c.getCostumerId(), 0);
    EXPECT_EQ(c.getLoyaltyPoints(), 0);
    EXPECT_EQ(c.getPurchaseCount(), 0);
}

TEST(CostumerTest, ParameterizedConstructor) {
    Costumer c("John", 1000.0, 1);
    EXPECT_EQ(c.getName(), "John");
    EXPECT_EQ(c.getWallet(), 1000.0);
    EXPECT_EQ(c.getCostumerId(), 1);
}

TEST(CostumerTest, AddBalance) {
    Costumer c("John", 1000.0, 1);
    c.addBalance(500.0);
    EXPECT_EQ(c.getWallet(), 1500.0);
}

TEST(CostumerTest, MakePurchase) {
    Costumer c("John", 1000.0, 1);
    EXPECT_TRUE(c.makePurchase(500.0));
    EXPECT_EQ(c.getWallet(), 500.0);
    EXPECT_FALSE(c.makePurchase(600.0));
}

// Test for Store class
TEST(StoreTest, DefaultConstructor) {
    Store s;
    EXPECT_EQ(s.getInventory(), nullptr);
}

TEST(StoreTest, AddProduct) {
    Store s;
    Product p("Laptop", 1, 999.99, 10);
    s.addProduct(&p);
    EXPECT_EQ(s.getInventory()[0]->getID(), 1);
}

TEST(StoreTest, SearchProduct) {
    Store s;
    Product p("Laptop", 1, 999.99, 10);
    s.addProduct(&p);
    Product* found = s.search(1);
    EXPECT_EQ(found->getID(), 1);
    EXPECT_EQ(found->getProduct(), "Laptop");
}

TEST(StoreTest, PurchaseProduct) {
    Store s;
    Product p("Laptop", 1, 999.99, 10);
    s.addProduct(&p);
    Costumer c("John", 1000.0, 1);
    EXPECT_TRUE(s.purchase(1, 1, c));
    EXPECT_EQ(c.getWallet(), 0.01);
    EXPECT_EQ(s.search(1)->getQuantity(), 9);
}

// Test for Category class
TEST(CategoryTest, DefaultConstructor) {
    Category c;
    EXPECT_EQ(c.getName(), "");
    EXPECT_EQ(c.getSize(), 0);
}

TEST(CategoryTest, AddProduct) {
    Category c("Electronics");
    Product p("Laptop", 1, 999.99, 10);
    c.addProduct(p);
    EXPECT_EQ(c.getSize(), 1);
}

TEST(CategoryTest, SearchProductById) {
    Category c("Electronics");
    Product p("Laptop", 1, 999.99, 10);
    c.addProduct(p);
    Product* found = c.searchId(1);
    EXPECT_EQ(found->getID(), 1);
    EXPECT_EQ(found->getProduct(), "Laptop");
}

TEST(CategoryTest, SearchProductByName) {
    Category c("Electronics");
    Product p("Laptop", 1, 999.99, 10);
    c.addProduct(p);
    string name = "Laptop";
    Product* found = c.search(name);
    EXPECT_EQ(found->getProduct(), "Laptop");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
