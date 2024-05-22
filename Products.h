#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

using namespace std;
struct Product {
    int Id, Price;
    char Name[100]; // Assuming a maximum name length of 100 characters
    int nameOffset; // Offset of the product name in secondary file
};
const string ProductFile = "products.txt";
const string PrimaryFile = "primary.txt";
const string SecondaryFile = "secondary.txt";

class ProductClass {
private:
    Product P;// Offset of the product name in secondary file

public:
    ProductClass() {}
    int numDigits(int number);
    bool isProductIdExists(int productId);
    void insertProduct(); // -----------> 1
    void updateProduct(int productId); // -----------> 2
    void displayProductById(int productId);  // -----------> 7
    void displayProductByName(const string& productName); // -----------> 8
    void updateProductByName(const string& productName); // -----------> 3
    void deleteProduct(int productId, int x = 1); // -----------> ( one par , id only ) 4
    void Display(); // -----------> 9
    void deleteProductByName(const string& productName , int x = 1); // -----------> ( one par , string only ) 5
    int findProductIdByName(const string& productName);
    string findProductNameById(int productId);
    bool isProductNameExists(const string& productName);
    void DisplayTop5(); // -----------> 6
    // Function to insert a new product

};
// Function to calculate the number of digits in an integer
int ProductClass:: numDigits(int number) {
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
bool ProductClass:: isProductIdExists(int productId) {
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return false;
    }

    int idFromFile;
    while (primaryFile >> idFromFile) {
        if (idFromFile == productId) {
            primaryFile.close();
            return true; // Product ID already exists
        }
        // Skip the rest of the line (offset) as we're only checking IDs
        primaryFile.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    primaryFile.close();
    return false; // Product ID does not exist
}

// Function to insert a new product record
void ProductClass:: insertProduct() {
    Product p;

    cout << "Enter Product ID: ";
    cin >> p.Id;

    if (isProductIdExists(p.Id)) {
        cout << "Error: Product ID '" << p.Id << "' already exists." << endl;
        return;
    }

    cout << "Enter Product Name: ";
    cin.ignore(); // Ignore newline character left in the input buffer
    cin.getline(p.Name, sizeof(p.Name));

    cout << "Enter Product Price: ";
    cin >> p.Price;

    ofstream writeFile(ProductFile, ios::app | ios::binary);
    if (!writeFile.is_open()) {
        cout << "Failed to open file for writing." << endl;
        return;
    }

    // Get current write position (offset) in the products file
    int offset = writeFile.tellp();

    // Calculate sizes of each field
    int idSize = numDigits(p.Id);
    int nameSize = strlen(p.Name);
    int priceSize = numDigits(p.Price);

    // Write sizes followed by data
    writeFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    writeFile.write(reinterpret_cast<const char*>(&p.Id), sizeof(p.Id));

    writeFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    writeFile.write(p.Name, nameSize);

    writeFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    writeFile.write(reinterpret_cast<const char*>(&p.Price), sizeof(p.Price));

    // Write the delimiter at the end of the record
    char delimiter = '|';
    writeFile.write(reinterpret_cast<const char*>(&delimiter), sizeof(delimiter));

    writeFile.close();
    cout << "Product record inserted successfully." << endl;

    // Save product ID and offset to primary file
    ofstream primaryFile(PrimaryFile, ios::app);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for writing." << endl;
        return;
    }

    primaryFile << p.Id << " " << offset << endl;
    primaryFile.close();

    // Save product name and offset to secondary file
    ofstream secondaryFile(SecondaryFile, ios::app);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for writing." << endl;
        return;
    }

    secondaryFile << p.Name << " " << offset << endl;
    secondaryFile.close();
}

// Function to display all product records


void ProductClass::updateProduct(int productId) {
    // Open primary file to find offset
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int idFromFile;
    int offset = 0;
    bool found = false;

    // Read primary file to find the offset corresponding to productId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == productId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "Product with ID '" << productId << "' not found." << endl;
        return;
    }

    // Open product file for updating
    fstream productFile(ProductFile, ios::in | ios::out | ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for editing." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    Product product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.Price), sizeof(product.Price));

    // Prompt for new product price
    cout << "Enter new Product Price: ";
    cin >> product.Price;

    // Update the record in the file
    productFile.seekp(offset);

    // Calculate new price size after update
    priceSize = numDigits(product.Price);

    // Write updated sizes followed by data
    productFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    productFile.write(reinterpret_cast<const char*>(&product.Id), sizeof(product.Id));
    productFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    productFile.write(product.Name, nameSize);
    productFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    productFile.write(reinterpret_cast<const char*>(&product.Price), sizeof(product.Price));

    productFile.close();

    cout << "Product with ID '" << productId << "' updated successfully." << endl;
}
void ProductClass::displayProductById(int productId) {
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file to find the offset corresponding to productId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == productId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "Product with ID '" << productId << "' not found." << endl;
        return;
    }

    // Open product file and move to the correct position (offset) to read product details
    ifstream productFile(ProductFile, ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for reading." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    Product product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.Price), sizeof(product.Price));

    productFile.close();

    // Output the product details
    cout << "Product ID: " << product.Id << endl;
    cout << "Product Name: " << product.Name << endl;
    cout << "Product Price: " << product.Price << endl;
    cout << "---------------------" << endl;
}
void ProductClass::displayProductByName(const string& productName) {
    ifstream secondaryFile(SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    string nameFromFile;
    int offset;
    bool found = false;

    // Read secondary file to find the offset corresponding to productName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == productName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "Product with name '" << productName << "' not found." << endl;
        return;
    }

    // Open product file and move to the correct position (offset) to read product details
    ifstream productFile(ProductFile, ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for reading." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    Product product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.Price), sizeof(product.Price));

    productFile.close();

    // Output the product details
    cout << "Product ID: " << product.Id << endl;
    cout << "Product Name: " << product.Name << endl;
    cout << "Product Price: " << product.Price << endl;
    cout << "---------------------" << endl;
}void ProductClass::updateProductByName(const string& productName) {
    ifstream secondaryFile(SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    string nameFromFile;
    int offset = -1; // Initialize offset to an invalid value
    bool found = false;

    // Read secondary file to find the offset corresponding to productName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == productName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "Product with name '" << productName << "' not found." << endl;
        return;
    }

    // Open product file for updating
    fstream productFile(ProductFile, ios::in | ios::out | ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for editing." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    Product product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.Price), sizeof(product.Price));

    // Prompt for new product price
    cout << "Enter new Product Price: ";
    cin >> product.Price;

    // Update the record in the file
    productFile.seekp(offset);

    // Calculate new price size after update
    priceSize = numDigits(product.Price);

    // Write updated sizes followed by data
    productFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    productFile.write(reinterpret_cast<const char*>(&product.Id), sizeof(product.Id));
    productFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    productFile.write(product.Name, nameSize);
    productFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    productFile.write(reinterpret_cast<const char*>(&product.Price), sizeof(product.Price));

    productFile.close();

    cout << "Product with name '" << productName << "' updated successfully." << endl;
}
void ProductClass::deleteProduct(int productId, int x) {
    if(x) {
        deleteProductByName(findProductNameById(productId),0);
    }
    // Open primary file to find offset
    ifstream primaryFile(PrimaryFile);
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

    // Read primary file and copy entries except the one with the given productId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile != productId) {
            tempPrimaryFile << idFromFile << " " << offset << endl;
        } else {
            found = true;
        }
    }

    primaryFile.close();
    tempPrimaryFile.close();

    if (!found) {
        cout << "Product with ID '" << productId << "' not found." << endl;
        remove("temp_primary.txt"); // Remove temporary file
        return;
    }
    remove(PrimaryFile.c_str());
    // Rename temporary primary file to replace the original
    if (rename("temp_primary.txt", PrimaryFile.c_str()) ){
        cout << "Failed to rename temporary primary file." << endl;
        return;
    }
    deleteProductByName(findProductNameById(productId));
    cout << "Product with ID '" << productId << "' deleted successfully." << endl;
}
void ProductClass::Display() {
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int productId, offset;
    while (primaryFile >> productId >> offset) {
        // Call displayProductById for each product ID
        displayProductById(productId);
    }

    primaryFile.close();
}

void ProductClass::deleteProductByName(const string& productName,int x) {
    if(x)
    {
        deleteProduct(findProductIdByName(productName),0);
    }
    ifstream secondaryFile(SecondaryFile);
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
        cout << "Product with name '" << productName << "' not found." << endl;
        remove("temp_secondary.txt"); // Remove temporary file
        return;
    }

    remove(SecondaryFile.c_str());

    // Rename temporary secondary file to replace the original
    if (rename("temp_secondary.txt", SecondaryFile.c_str()) != 0) {
        cout << "Failed to rename temporary secondary file." << endl;
        return;
    }

    // Now that the secondary file is updated, delete the corresponding entry from the primary file
    ifstream tempSecondaryFileReader(SecondaryFile);
    if (!tempSecondaryFileReader.is_open()) {
        cout << "Failed to open updated secondary file for reading." << endl;
        return;
    }

    // Read updated secondary file to find corresponding product IDs


    cout << "Product with name '" << productName << "' deleted successfully." << endl;
}

int ProductClass::findProductIdByName(const string& productName) {
    ifstream secondaryFile(SecondaryFile);
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
        if (nameFromFile == productName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "Product with name '" << productName << "' not found." << endl;
        return -1; // Return -1 to indicate failure
    }

    // Open primary file to find the product ID corresponding to the offset
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return -1; // Return -1 to indicate failure
    }

    int idFromFile, offsetFromFile, productId = -1;

    // Read primary file to find the ID corresponding to the offset
    while (primaryFile >> idFromFile >> offsetFromFile) {
        if (offsetFromFile == offset) {
            productId = idFromFile;
            break;
        }
    }

    primaryFile.close();

    if (productId == -1) {
        cout << "Failed to find product ID for name '" << productName << "'." << endl;
    }

    return productId;
}

string ProductClass::findProductNameById(int productId) {
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return ""; // Return empty string to indicate failure
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file to find the offset corresponding to productId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == productId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "Product with ID '" << productId << "' not found." << endl;
        return ""; // Return empty string to indicate failure
    }

    // Open secondary file to find the product name corresponding to the offset
    ifstream secondaryFile(SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return ""; // Return empty string to indicate failure
    }

    string nameFromFile;
    string productName = "";

    // Read secondary file to find the name corresponding to the offset
    while (secondaryFile >> nameFromFile >> offset) {
        if (offset == offset) {
            productName = nameFromFile;
            break;
        }
    }

    secondaryFile.close();

    if (productName.empty()) {
        cout << "Failed to find product name for ID '" << productId << "'." << endl;
    }

    return productName;
}

bool ProductClass::isProductNameExists(const string& productName) {
    ifstream secondaryFile(SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return false; // Return false to indicate failure
    }

    string nameFromFile;
    int offset;

    // Read secondary file to find if productName exists
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-sensitive comparison)
        if (nameFromFile == productName) {
            secondaryFile.close();
            return true; // Product name exists
        }
    }

    secondaryFile.close();
    return false; // Product name does not exist
}
void ProductClass::DisplayTop5() {
    ifstream primaryFile(PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int productId, offset;
    int recordCount = 0;
    const int maxRecords = 5; // Display only the first 5 records

    while (primaryFile >> productId >> offset) {
        // Call displayProductById for each product ID
        displayProductById(productId);

        recordCount++;
        if (recordCount >= maxRecords) {
            break; // Stop after displaying the first 5 records
        }
    }

    primaryFile.close();
}