#include "Products.h"
#include "customers.h"
#include "Employee.h"
#include <iostream>
#include <string>
using namespace std;

void displayMenu() {
    cout << "Welcome to the Management System!" << endl;
    cout << "1. Product Management" << endl;
    cout << "2. Customer Management" << endl;
    cout << "3. Employee Management" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayProductMenu() {
    cout << "Product Management Menu" << endl;
    cout << "1. Insert Product" << endl;
    cout << "2. Update Product" << endl;
    cout << "3. Delete Product" << endl;
    cout << "4. Display Products" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayCustomerMenu() {
    cout << "Customer Management Menu" << endl;
    cout << "1. Insert Customer" << endl;
    cout << "2. Update Customer" << endl;
    cout << "3. Delete Customer" << endl;
    cout << "4. Display Customers" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayEmployeeMenu() {
    cout << "Employee Management Menu" << endl;
    cout << "1. Add Employee" << endl;
    cout << "2. Update Employee" << endl;
    cout << "3. Delete Employee" << endl;
    cout << "4. Display Employees" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

void productManagementMenu(ProductClass& productManager) {
    int choice;
    do {
        displayProductMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                productManager.insertProduct();
                break;
            case 2:
                int updateChoice;
                cout << "How do you want to update the product?" << endl;
                cout << "1. Update by ID" << endl;
                cout << "2. Update by Name" << endl;
                cout << "3. Back to Product Management Menu" << endl;
                cout << "Enter your choice: ";
                cin >> updateChoice;

                if (updateChoice == 1) {
                    int productIdToUpdate;
                    cout << "Enter the ID of the product you want to update: ";
                    cin >> productIdToUpdate;
                    productManager.updateProduct(productIdToUpdate);
                } else if (updateChoice == 2) {
                    string productNameToUpdate;
                    cout << "Enter the name of the product you want to update: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, productNameToUpdate);
                    productManager.updateProductByName(productNameToUpdate);
                } else if (updateChoice == 3) {
                    cout << "Returning to Product Management Menu..." << endl;
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }

            case 3:
                int deleteChoice;
                cout << "How do you want to delete the product?" << endl;
                cout << "1. Delete by ID" << endl;
                cout << "2. Delete by Name" << endl;
                cout << "Enter your choice: ";
                cin >> deleteChoice;

                if (deleteChoice == 1) {
                    int productIdToDelete;
                    cout << "Enter the ID of the product you want to delete: ";
                    cin >> productIdToDelete;
                    productManager.deleteProduct(productIdToDelete);
                } else if (deleteChoice == 2) {
                    string productNameToDelete;
                    cout << "Enter the name of the product you want to delete: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, productNameToDelete);
                    productManager.deleteProductByName(productNameToDelete);
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 4:
                int displayChoice;
                cout << "How do you want to display the products?" << endl;
                cout << "1. Display by ID" << endl;
                cout << "2. Display by Name" << endl;
                cout << "3. Display Top 5 Products" << endl;
                cout << "Enter your choice: ";
                cin >> displayChoice;

                if (displayChoice == 1) {
                    int productIdToDisplay;
                    cout << "Enter the ID of the product you want to display: ";
                    cin >> productIdToDisplay;
                    productManager.displayProductById(productIdToDisplay);
                } else if (displayChoice == 2) {
                    string productNameToDisplay;
                    cout << "Enter the name of the product you want to display: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, productNameToDisplay);
                    productManager.displayProductByName(productNameToDisplay);
                } else if (displayChoice == 3) {
                    productManager.DisplayTop5();
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 5:
                cout << "Exiting Product Management..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break; // Added break statement for the default case
        }
    } while (choice != 5);
}

void customerManagementMenu(customerclass& customerManager) {
    int choice;
    do {
        displayCustomerMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                customerManager.insertcustomer();
                break;
            case 2:
                int updateChoice;
                cout << "How do you want to update the customer data?" << endl;
                cout << "1. Update by ID" << endl;
                cout << "2. Update by Name" << endl;
                cout << "3. Back to customer Management Menu" << endl;
                cout << "Enter your choice: ";
                cin >> updateChoice;

                if (choice == 1) {
                    customerManager.insertcustomer();
                } else if (choice == 2) {
                    int customerIdToUpdate;
                    cout << "Enter the ID of the customer you want to update: ";
                    cin >> customerIdToUpdate;
                    customerManager.updatecustomer(customerIdToUpdate);
                } else if (choice == 3) {
                    int customerIdToDisplay;
                    cout << "Enter the ID of the customer you want to display: ";
                    cin >> customerIdToDisplay;
                    customerManager.displaycustomerById(customerIdToDisplay);
                } else if (choice == 4) {
                    customerManager.Display();
                } else if (choice == 5) {
                    cout << "Exiting Customer Management..." << endl;
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }


            case 3:
                int deleteChoice;
                cout << "How do you want to delete the customer?" << endl;
                cout << "1. Delete by ID" << endl;
                cout << "2. Delete by Name" << endl;
                cout << "Enter your choice: ";
                cin >> deleteChoice;

                if (deleteChoice == 1) {
                    int customerIdToDelete;
                    cout << "Enter the ID of the customer you want to delete: ";
                    cin >> customerIdToDelete;
                    customerManager.deletecustomer(customerIdToDelete);
                } else if (deleteChoice == 2) {
                    string customerNameToDelete;
                    cout << "Enter the name of the customer you want to delete: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, customerNameToDelete);
                    customerManager.deletecustomerByName(customerNameToDelete);
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 4:
                int displayChoice;
                cout << "How do you want to display the customers?" << endl;
                cout << "1. Display by ID" << endl;
                cout << "2. Display by Name" << endl;
                cout << "3. Display Top 5 Customers" << endl;
                cout << "Enter your choice: ";
                cin >> displayChoice;

                if (displayChoice == 1) {
                    int customerIdToDisplay;
                    cout << "Enter the ID of the customer you want to display: ";
                    cin >> customerIdToDisplay;
                    customerManager.displaycustomerById(customerIdToDisplay);
                } else if (displayChoice == 2) {
                    string customerNameToDisplay;
                    cout << "Enter the name of the customer you want to display: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, customerNameToDisplay);
                    customerManager.displaycustomerByName(customerNameToDisplay);
                } else if (displayChoice == 3) {
                   customerManager.DisplayTop5();
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 5:
                cout << "Exiting Customer Management..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);
}

void employeeManagementMenu(Employeeclass& employeeManager) {
    int choice;
    do {
        displayEmployeeMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                employeeManager.insertEmp();
                break;
            case 2:
                int updateChoice;
                cout << "How do you want to update the employee?" << endl;
                cout << "1. Update by ID" << endl;
                cout << "2. Update by Name" << endl;
                cout << "3. Back to Employee Management Menu" << endl;
                cout << "Enter your choice: ";
                cin >> updateChoice;

                if (updateChoice == 1) {
                    int employeeIdToUpdate;
                    cout << "Enter the ID of the employee you want to update: ";
                    cin >> employeeIdToUpdate;
                    employeeManager.updateEmp(employeeIdToUpdate);
                } else if (updateChoice == 2) {
                    string employeeNameToUpdate;
                    cout << "Enter the name of the employee you want to update: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, employeeNameToUpdate);
                    employeeManager.updateEmpByName(employeeNameToUpdate);
                } else if (updateChoice == 3) {
                    cout << "Returning to Employee Management Menu..." << endl;
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 3:
                int deleteChoice;
                cout << "How do you want to delete the employee?" << endl;
                cout << "1. Delete by ID" << endl;
                cout << "2. Delete by Name" << endl;
                cout << "Enter your choice: ";
                cin >> deleteChoice;

                if (deleteChoice == 1) {
                    int employeeIdToDelete;
                    cout << "Enter the ID of the employee you want to delete: ";
                    cin >> employeeIdToDelete;
                    employeeManager.deleteEmp(employeeIdToDelete);
                } else if (deleteChoice == 2) {
                    string employeeNameToDelete;
                    cout << "Enter the name of the employee you want to delete: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, employeeNameToDelete);
                    employeeManager.deleteEmpByName(employeeNameToDelete);
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 4:
                int displayChoice;
                cout << "How do you want to display the employees?" << endl;
                cout << "1. Display by ID" << endl;
                cout << "2. Display by Name" << endl;
                cout << "3. Display Top 5 Employees" << endl;
                cout << "Enter your choice: ";
                cin >> displayChoice;

                if (displayChoice == 1) {
                    int employeeIdToDisplay;
                    cout << "Enter the ID of the employee you want to display: ";
                    cin >> employeeIdToDisplay;
                    employeeManager.displayEmpById(employeeIdToDisplay);
                } else if (displayChoice == 2) {
                    string employeeNameToDisplay;
                    cout << "Enter the name of the employee you want to display: ";
                    cin.ignore(); // Ignore newline character left in the input buffer
                    getline(cin, employeeNameToDisplay);
                    employeeManager.deleteEmpByName(employeeNameToDisplay);
                } else if (displayChoice == 3) {
                    employeeManager.DisplayTop5();
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;

            case 5:
                cout << "Exiting Employee Management..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);
}

bool authenticateEmployee() {
    string password;
    cout << "Enter the employee password: ";
    cin >> password;
    return password == "1234";
}

int main() {
    ProductClass productManager;
    customerclass customerManager;
    Employeeclass employeeManager;

    cout << "Welcome to the Supermarket Management System!" << endl;
    cout << "Are you an employee or a customer?" << endl;
    cout << "1. Employee" << endl;
    cout << "2. Customer" << endl;
    cout << "Enter your choice: ";
    int userType;
    cin >> userType;

    if (userType == 1) {
        if (authenticateEmployee()) {
            cout << "Welcome Employee!" << endl;
            cout << "What do you want to manage?" << endl;
            cout << "1. Employee Management" << endl;
            cout << "2. Product Management" << endl;
            cout << "Enter your choice: ";
            int employeeChoice;
            cin >> employeeChoice;

            if (employeeChoice == 1) {
                employeeManagementMenu(employeeManager);
            } else if (employeeChoice == 2) {
                productManagementMenu(productManager);
            } else {
                cout << "Invalid choice. Exiting..." << endl;
            }
        } else {
            cout << "Invalid password. Exiting..." << endl;
        }
    } else if (userType == 2) {
        cout << "Welcome Customer!" << endl;
        // You can add customer-related functionality here if needed
    } else {
        cout << "Invalid choice. Exiting..." << endl;
    }

    return 0;
}