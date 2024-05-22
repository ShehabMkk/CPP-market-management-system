
#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

using namespace std;
struct customer {
    int Id, phonenum;
    char Name[100]; // Assuming a maximum name length of 100 characters
    int nameOffset; // Offset of the product name in secondary file
};
const string cus_file = "customers.txt";
const string cus_PrimaryFile = "customer_primary.txt";
const string cus_SecondaryFile = "customer_secondary.txt";

class customerclass {
private:
    customer P;// Offset of the product name in secondary file

public:
    customerclass() {}
    int numDigits(int number);
    bool iscustomerIdExists(int customerId);
    void insertcustomer(); // -----------> 1
    void updatecustomer(int customerid); // -----------> 2
    void displaycustomerById(int customerId);  // -----------> 7
    void displaycustomerByName(const string& customerName); // -----------> 8
    void updatecustomerByName(const string& customerName); // -----------> 3
    void deletecustomer(int customerId, int x = 1); // -----------> ( one par , id only ) 4
    void Display(); // -----------> 9
    void deletecustomerByName(const string& customerName , int x = 1); // -----------> ( one par , string only ) 5
    int findcustomerIdByName(const string& customerName);
    string findcustomerNameById(int customerId);
    bool iscustomerNameExists(const string& cusName);
    void DisplayTop5(); // -----------> 6
    // Function to insert a new product

};
// Function to calculate the number of digits in an integer
int customerclass:: numDigits(int number) {
    int count = 0;
    if (number == 0) {
        return 1; // Handle special case for zero
    }
    while (number != 0) {
        number /= 10;
        count++;
    }
    return count;
}

// Function to check if a product ID already exists in the primary file
bool customerclass:: iscustomerIdExists(int cusId) {
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return false;
    }

    int idFromFile;
    while (primaryFile >> idFromFile) {
        if (idFromFile == cusId) {
            primaryFile.close();
            return true; // product ID already exists
        }
        // Skip the rest of the line (offset) as we're only checking IDs
        primaryFile.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    primaryFile.close();
    return false; // product ID does not exist
}

// Function to insert a new product record
void customerclass:: insertcustomer() {
    customer p;

    cout << "Enter product ID: ";
    cin >> p.Id;

    if (iscustomerIdExists(p.Id)) {
        cout << "Error: customer ID '" << p.Id << "' already exists." << endl;
        return;
    }

    cout << "Enter customer Name: ";
    cin.ignore(); // Ignore newline character left in the input buffer
    cin.getline(p.Name, sizeof(p.Name));

    cout << "Enter customer phone number: ";
    cin >> p.phonenum;

    ofstream writeFile(cus_file, ios::app | ios::binary);
    if (!writeFile.is_open()) {
        cout << "Failed to open file for writing." << endl;
        return;
    }

    // Get current write position (offset) in the products file
    int offset = writeFile.tellp();

    // Calculate sizes of each field
    int idSize = numDigits(p.Id);
    int nameSize = strlen(p.Name);
    int priceSize = numDigits(p.phonenum);

    // Write sizes followed by data
    writeFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    writeFile.write(reinterpret_cast<const char*>(&p.Id), sizeof(p.Id));

    writeFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    writeFile.write(p.Name, nameSize);

    writeFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    writeFile.write(reinterpret_cast<const char*>(&p.phonenum), sizeof(p.phonenum));

    // Write the delimiter at the end of the record
    char delimiter = '|';
    writeFile.write(reinterpret_cast<const char*>(&delimiter), sizeof(delimiter));

    writeFile.close();
    cout << "customer record inserted successfully." << endl;

    // Save product ID and offset to primary file
    ofstream primaryFile(cus_PrimaryFile, ios::app);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for writing." << endl;
        return;
    }

    primaryFile << p.Id << " " << offset << endl;
    primaryFile.close();

    // Save product name and offset to secondary file
    ofstream secondaryFile(cus_SecondaryFile, ios::app);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for writing." << endl;
        return;
    }

    secondaryFile << p.Name << " " << offset << endl;
    secondaryFile.close();
}

// Function to display all product records


void customerclass::updatecustomer(int customerid) {
    // Open primary file to find offset
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int idFromFile;
    int offset = 0;
    bool found = false;

    // Read primary file to find the offset corresponding to customerid
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == customerid) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "customer with ID '" << customerid << "' not found." << endl;
        return;
    }

    // Open product file for updating
    fstream productFile(cus_file, ios::in | ios::out | ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for editing." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    customer customer;
    int idSize, nameSize, phonenumSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&customer.Id), sizeof(customer.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(customer.Name, nameSize);
    customer.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&phonenumSize), sizeof(phonenumSize));
    productFile.read(reinterpret_cast<char*>(&customer.phonenum), sizeof(customer.phonenum));

    // Prompt for new product price
    cout << "Enter new customer phone number: ";
    cin >> customer.phonenum;

    // Update the record in the file
    productFile.seekp(offset);

    // Calculate new price size after update
    phonenumSize = numDigits(customer.phonenum);

    // Write updated sizes followed by data
    productFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    productFile.write(reinterpret_cast<const char*>(&customer.Id), sizeof(customer.Id));
    productFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    productFile.write(customer.Name, nameSize);
    productFile.write(reinterpret_cast<const char*>(&phonenumSize), sizeof(phonenumSize));
    productFile.write(reinterpret_cast<const char*>(&customer.phonenum), sizeof(customer.phonenum));

    productFile.close();

    cout << "customer with ID '" << customerid << "' updated successfully." << endl;
}
void customerclass::displaycustomerById(int customerId) {
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file to find the offset corresponding to customerId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == customerId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "customer with ID '" << customerId << "' not found." << endl;
        return;
    }

    // Open product file and move to the correct position (offset) to read product details
    ifstream productFile(cus_file, ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for reading." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    customer customer;
    int idSize, nameSize, phonenumSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&customer.Id), sizeof(customer.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(customer.Name, nameSize);
    customer.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&phonenumSize), sizeof(phonenumSize));
    productFile.read(reinterpret_cast<char*>(&customer.phonenum), sizeof(customer.phonenum));

    productFile.close();

    // Output the product details
    cout << "customer ID: " << customer.Id << endl;
    cout << "customer Name: " << customer.Name << endl;
    cout << "customer phone number: " << customer.phonenum << endl;
    cout << "---------------------" << endl;
}
void customerclass::displaycustomerByName(const string& customerName) {
    ifstream secondaryFile(cus_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    string nameFromFile;
    int offset;
    bool found = false;

    // Read secondary file to find the offset corresponding to customerName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == customerName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "customer with name '" << customerName << "' not found." << endl;
        return;
    }

    // Open product file and move to the correct position (offset) to read product details
    ifstream productFile(cus_file, ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for reading." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    customer customer;
    int idSize, nameSize, phonenumSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&customer.Id), sizeof(customer.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(customer.Name, nameSize);
    customer.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&phonenumSize), sizeof(phonenumSize));
    productFile.read(reinterpret_cast<char*>(&customer.phonenum), sizeof(customer.phonenum));

    productFile.close();

    // Output the product details
    cout << "customer ID: " << customer.Id << endl;
    cout << "customerName: " << customer.Name << endl;
    cout << "customer phone number: " << customer.phonenum << endl;
    cout << "---------------------" << endl;
}
void customerclass::updatecustomerByName(const string& customerName) {
    ifstream secondaryFile(cus_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    string nameFromFile;
    int offset = -1; // Initialize offset to an invalid value
    bool found = false;

    // Read secondary file to find the offset corresponding to customerName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == customerName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "customer with name '" << customerName << "' not found." << endl;
        return;
    }

    // Open product file for updating
    fstream productFile(cus_file, ios::in | ios::out | ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for editing." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    customer customer;
    int idSize, nameSize, phonenumSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&customer.Id), sizeof(customer.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(customer.Name, nameSize);
    customer.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&phonenumSize), sizeof(phonenumSize));
    productFile.read(reinterpret_cast<char*>(&customer.phonenum), sizeof(customer.phonenum));

    // Prompt for new product price
    cout << "Enter new customer phone number: ";
    cin >> customer.phonenum;

    // Update the record in the file
    productFile.seekp(offset);

    // Calculate new price size after update
    phonenumSize = numDigits(customer.phonenum);

    // Write updated sizes followed by data
    productFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    productFile.write(reinterpret_cast<const char*>(&customer.Id), sizeof(customer.Id));
    productFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    productFile.write(customer.Name, nameSize);
    productFile.write(reinterpret_cast<const char*>(&phonenumSize), sizeof(phonenumSize));
    productFile.write(reinterpret_cast<const char*>(&customer.phonenum), sizeof(customer.phonenum));

    productFile.close();

    cout << "customer with name '" << customerName << "' updated successfully." << endl;
}
void customerclass::deletecustomer(int customerId, int x) {
    if(x) {
        deletecustomerByName(findcustomerNameById(customerId), 0);
    }
    // Open primary file to find offset
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    ofstream tempPrimaryFile("temp_primary.txt");
    if (!tempPrimaryFile.is_open()) {
        cout << "Failed to create a temporary primary file." << endl;
        primaryFile.close();
        return;
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file and copy entries except the one with the given customerId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile != customerId) {
            tempPrimaryFile << idFromFile << " " << offset << endl;
        } else {
            found = true;
        }
    }

    primaryFile.close();
    tempPrimaryFile.close();

    if (!found) {
        cout << "customer with ID '" << customerId << "' not found." << endl;
        remove("temp_primary.txt"); // Remove temporary file
        return;
    }
    remove(cus_PrimaryFile.c_str());
    // Rename temporary primary file to replace the original
    if (rename("temp_primary.txt", cus_PrimaryFile.c_str()) ){
        cout << "Failed to rename temporary primary file." << endl;
        return;
    }
    deletecustomerByName(findcustomerNameById(customerId));
    cout << "customer with ID '" << customerId << "' deleted successfully." << endl;
}
void customerclass::Display() {
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int customerId, offset;
    while (primaryFile >> customerId >> offset) {
        // Call displaycustomerById for each product ID
        displaycustomerById(customerId);
    }

    primaryFile.close();
}

void customerclass::deletecustomerByName(const string& productName, int x) {
    if(x)
    {
        deletecustomer(findcustomerIdByName(productName), 0);
    }
    ifstream secondaryFile(cus_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    ofstream tempSecondaryFile("temp_secondary.txt");
    if (!tempSecondaryFile.is_open()) {
        cout << "Failed to create a temporary secondary file." << endl;
        secondaryFile.close();
        return;
    }

    string nameFromFile;
    int offset;
    bool found = false;

    // Read secondary file and copy entries except the one with the given productName
    while (secondaryFile >> nameFromFile >> offset) {
        if (nameFromFile != productName) {
            tempSecondaryFile << nameFromFile << " " << offset << endl;
        } else {
            found = true;
        }
    }

    secondaryFile.close();
    tempSecondaryFile.flush();
    tempSecondaryFile.close();

    if (!found) {
        cout << "customer with name '" << productName << "' not found." << endl;
        remove("temp_secondary.txt"); // Remove temporary file
        return;
    }

    remove(cus_SecondaryFile.c_str());

    // Rename temporary secondary file to replace the original
    if (rename("temp_secondary.txt", cus_SecondaryFile.c_str()) != 0) {
        cout << "Failed to rename temporary secondary file." << endl;
        return;
    }

    // Now that the secondary file is updated, delete the corresponding entry from the primary file
    ifstream tempSecondaryFileReader(cus_SecondaryFile);
    if (!tempSecondaryFileReader.is_open()) {
        cout << "Failed to open updated secondary file for reading." << endl;
        return;
    }

    // Read updated secondary file to find corresponding product IDs


    cout << "customer with name '" << productName << "' deleted successfully." << endl;
}

int customerclass::findcustomerIdByName(const string& customerName) {
    ifstream secondaryFile(cus_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return -1; // Return -1 to indicate failure
    }

    string nameFromFile;
    int offset;
    bool found = false;

    // Read secondary file to find the offset corresponding to productName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == customerName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "customer with name '" << customerName << "' not found." << endl;
        return -1; // Return -1 to indicate failure
    }

    // Open primary file to find the product ID corresponding to the offset
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return -1; // Return -1 to indicate failure
    }

    int idFromFile, offsetFromFile, customerId = -1;

    // Read primary file to find the ID corresponding to the offset
    while (primaryFile >> idFromFile >> offsetFromFile) {
        if (offsetFromFile == offset) {
            customerId = idFromFile;
            break;
        }
    }

    primaryFile.close();

    if (customerId == -1) {
        cout << "Failed to find customer ID for name '" << customerName << "'." << endl;
    }

    return customerId;
}

string customerclass::findcustomerNameById(int customerId) {
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return ""; // Return empty string to indicate failure
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file to find the offset corresponding to customerId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == customerId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "customer with ID '" << customerId << "' not found." << endl;
        return ""; // Return empty string to indicate failure
    }

    // Open secondary file to find the product name corresponding to the offset
    ifstream secondaryFile(cus_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return ""; // Return empty string to indicate failure
    }

    string nameFromFile;
    string customerName = "";

    // Read secondary file to find the name corresponding to the offset
    while (secondaryFile >> nameFromFile >> offset) {
        if (offset == offset) {
            customerName = nameFromFile;
            break;
        }
    }

    secondaryFile.close();

    if (customerName.empty()) {
        cout << "Failed to find customer name for ID '" << customerId << "'." << endl;
    }

    return customerName;
}

bool customerclass::iscustomerNameExists(const string& cusName) {
    ifstream secondaryFile(cus_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return false; // Return false to indicate failure
    }

    string nameFromFile;
    int offset;

    // Read secondary file to find if cusName exists
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-sensitive comparison)
        if (nameFromFile == cusName) {
            secondaryFile.close();
            return true; // product name exists
        }
    }

    secondaryFile.close();
    return false; // product name does not exist
}
void customerclass::DisplayTop5() {
    ifstream primaryFile(cus_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int productId, offset;
    int recordCount = 0;
    const int maxRecords = 5; // Display only the first 5 records

    while (primaryFile >> productId >> offset) {
        // Call displaycustomerById for each product ID
        displaycustomerById(productId);

        recordCount++;
        if (recordCount >= maxRecords) {
            break; // Stop after displaying the first 5 records
        }
    }

    primaryFile.close();
}