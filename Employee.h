#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

using namespace std;

const string Employee_file = "employees.txt";
const string Employee_PrimaryFile = "employee_primary.txt";
const string Employee_SecondaryFile = "employee_secondary.txt";

class Employeeclass {
private:
    struct employee {
        int Id, salary;
        char Name[100]; // Assuming a maximum name length of 100 characters
        int nameOffset; // Offset of the product name in secondary file
    };

public:
    Employeeclass() {}
    int numDigits(int number);
    bool isEmployeeIdExists(int EmpId);
    void insertEmp(); // -----------> 1
    void updateEmp(int EmpId); // -----------> 2
    void displayEmpById(int EmpId);  // -----------> 7
    void displayEmpByName(const string& EmpName); // -----------> 8
    void updateEmpByName(const string& EmpName); // -----------> 3
    void deleteEmp(int EmpId, int x = 1); // -----------> ( one par , id only ) 4
    void Display(); // -----------> 9
    void deleteEmpByName(const string& EmpName , int x = 1); // -----------> ( one par , string only ) 5
    int findEmpIdByName(const string& EmpName);
    string findEmpNameById(int EmpId);
    bool isEmpNameExists(const string& Empname);
    void DisplayTop5(); // -----------> 6
    // Function to insert a new product

};
// Function to calculate the number of digits in an integer
int Employeeclass:: numDigits(int number) {
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
bool Employeeclass::isEmployeeIdExists(int EmpId)  {
    ifstream primaryFile(Employee_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return false;
    }

    int idFromFile;
    while (primaryFile >> idFromFile) {
        if (idFromFile == EmpId) {
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
void Employeeclass:: insertEmp() {
    employee p;

    cout << "Enter employee ID: ";
    cin >> p.Id;

    if (isEmployeeIdExists(p.Id)) {
        cout << "Error: employee ID '" << p.Id << "' already exists." << endl;
        return;
    }

    cout << "Enter employee Name: ";
    cin.ignore(); // Ignore newline character left in the input buffer
    cin.getline(p.Name, sizeof(p.Name));

    cout << "Enter employee price: ";
    cin >> p.salary;

    ofstream writeFile(Employee_file, ios::app | ios::binary);
    if (!writeFile.is_open()) {
        cout << "Failed to open file for writing." << endl;
        return;
    }

    // Get current write position (offset) in the products file
    int offset = writeFile.tellp();

    // Calculate sizes of each field
    int idSize = numDigits(p.Id);
    int nameSize = strlen(p.Name);
    int priceSize = numDigits(p.salary);

    // Write sizes followed by data
    writeFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    writeFile.write(reinterpret_cast<const char*>(&p.Id), sizeof(p.Id));

    writeFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    writeFile.write(p.Name, nameSize);

    writeFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    writeFile.write(reinterpret_cast<const char*>(&p.salary), sizeof(p.salary));

    // Write the delimiter at the end of the record
    char delimiter = '|';
    writeFile.write(reinterpret_cast<const char*>(&delimiter), sizeof(delimiter));

    writeFile.close();
    cout << "employee record inserted successfully." << endl;

    // Save product ID and offset to primary file
    ofstream primaryFile(Employee_PrimaryFile, ios::app);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for writing." << endl;
        return;
    }

    primaryFile << p.Id << " " << offset << endl;
    primaryFile.close();

    // Save product name and offset to secondary file
    ofstream secondaryFile(Employee_SecondaryFile, ios::app);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for writing." << endl;
        return;
    }

    secondaryFile << p.Name << " " << offset << endl;
    secondaryFile.close();
}

// Function to display all product records


void Employeeclass::updateEmp(int EmpId) {
    // Open primary file to find offset
    ifstream primaryFile(Employee_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int idFromFile;
    int offset = 0;
    bool found = false;

    // Read primary file to find the offset corresponding to EmpId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == EmpId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "employee with ID '" << EmpId << "' not found." << endl;
        return;
    }

    // Open product file for updating
    fstream productFile(Employee_file, ios::in | ios::out | ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for editing." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    employee product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.salary), sizeof(product.salary));

    // Prompt for new product price
    cout << "Enter new employee salary: ";
    cin >> product.salary;

    // Update the record in the file
    productFile.seekp(offset);

    // Calculate new price size after update
    priceSize = numDigits(product.salary);

    // Write updated sizes followed by data
    productFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    productFile.write(reinterpret_cast<const char*>(&product.Id), sizeof(product.Id));
    productFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    productFile.write(product.Name, nameSize);
    productFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    productFile.write(reinterpret_cast<const char*>(&product.salary), sizeof(product.salary));

    productFile.close();

    cout << "employee with ID '" << EmpId << "' updated successfully." << endl;
}
void Employeeclass::displayEmpById(int EmpId) {
    ifstream primaryFile(Employee_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file to find the offset corresponding to EmpId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == EmpId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "Employee with ID '" << EmpId << "' not found." << endl;
        return;
    }

    // Open product file and move to the correct position (offset) to read product details
    ifstream productFile(Employee_file, ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open Employee file for reading." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    employee product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.salary), sizeof(product.salary));

    productFile.close();

    // Output the product details
    cout << "Employee ID: " << product.Id << endl;
    cout << "Employee Name: " << product.Name << endl;
    cout << "Employee salary: " << product.salary << endl;
    cout << "---------------------" << endl;
}
void Employeeclass::displayEmpByName(const string& EmpName) {
    ifstream secondaryFile(Employee_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    string nameFromFile;
    int offset;
    bool found = false;

    // Read secondary file to find the offset corresponding to EmpName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == EmpName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "Employee with name '" << EmpName << "' not found." << endl;
        return;
    }

    // Open product file and move to the correct position (offset) to read product details
    ifstream productFile(Employee_file, ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for reading." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    employee product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.salary), sizeof(product.salary));

    productFile.close();

    // Output the product details
    cout << "Employee ID: " << product.Id << endl;
    cout << "Employee Name: " << product.Name << endl;
    cout << "Employee salary: " << product.salary << endl;
    cout << "---------------------" << endl;
}
void Employeeclass::updateEmpByName(const string& EmpName) {
    ifstream secondaryFile(Employee_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return;
    }

    string nameFromFile;
    int offset = -1; // Initialize offset to an invalid value
    bool found = false;

    // Read secondary file to find the offset corresponding to EmpName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == EmpName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "Employee with name '" << EmpName << "' not found." << endl;
        return;
    }

    // Open product file for updating
    fstream productFile(Employee_file, ios::in | ios::out | ios::binary);
    if (!productFile.is_open()) {
        cout << "Failed to open product file for editing." << endl;
        return;
    }

    // Move to the correct position (offset) in the product file
    productFile.seekg(offset);

    // Read product data
    employee product;
    int idSize, nameSize, priceSize;

    productFile.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    productFile.read(reinterpret_cast<char*>(&product.Id), sizeof(product.Id));

    productFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    productFile.read(product.Name, nameSize);
    product.Name[nameSize] = '\0';

    productFile.read(reinterpret_cast<char*>(&priceSize), sizeof(priceSize));
    productFile.read(reinterpret_cast<char*>(&product.salary), sizeof(product.salary));

    // Prompt for new product price
    cout << "Enter new Employee salary: ";
    cin >> product.salary;

    // Update the record in the file
    productFile.seekp(offset);

    // Calculate new price size after update
    priceSize = numDigits(product.salary);

    // Write updated sizes followed by data
    productFile.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    productFile.write(reinterpret_cast<const char*>(&product.Id), sizeof(product.Id));
    productFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    productFile.write(product.Name, nameSize);
    productFile.write(reinterpret_cast<const char*>(&priceSize), sizeof(priceSize));
    productFile.write(reinterpret_cast<const char*>(&product.salary), sizeof(product.salary));

    productFile.close();

    cout << "Employee with name '" << EmpName << "' updated successfully." << endl;
}
void Employeeclass::deleteEmp(int EmpId, int x) {
    if(x) {
        deleteEmpByName(findEmpNameById(EmpId), 0);
    }
    // Open primary file to find offset
    ifstream primaryFile(Employee_PrimaryFile);
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

    // Read primary file and copy entries except the one with the given EmpId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile != EmpId) {
            tempPrimaryFile << idFromFile << " " << offset << endl;
        } else {
            found = true;
        }
    }

    primaryFile.close();
    tempPrimaryFile.close();

    if (!found) {
        cout << "Employee with ID '" << EmpId << "' not found." << endl;
        remove("temp_primary.txt"); // Remove temporary file
        return;
    }
    remove(Employee_PrimaryFile.c_str());
    // Rename temporary primary file to replace the original
    if (rename("temp_primary.txt", Employee_PrimaryFile.c_str()) ){
        cout << "Failed to rename temporary primary file." << endl;
        return;
    }
    deleteEmpByName(findEmpNameById(EmpId));
    cout << "Employee with ID '" << EmpId << "' deleted successfully." << endl;
}
void Employeeclass::Display() {
    ifstream primaryFile(Employee_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int productId, offset;
    while (primaryFile >> productId >> offset) {
        // Call displaycustomerById for each product ID
        displayEmpById(productId);
    }

    primaryFile.close();
}

void Employeeclass::deleteEmpByName(const string& EmpName, int x) {
    if(x)
    {
        deleteEmp(findEmpIdByName(EmpName), 0);
    }
    ifstream secondaryFile(Employee_SecondaryFile);
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

    // Read secondary file and copy entries except the one with the given EmpName
    while (secondaryFile >> nameFromFile >> offset) {
        if (nameFromFile != EmpName) {
            tempSecondaryFile << nameFromFile << " " << offset << endl;
        } else {
            found = true;
        }
    }

    secondaryFile.close();
    tempSecondaryFile.flush();
    tempSecondaryFile.close();

    if (!found) {
        cout << "Employee with name '" << EmpName << "' not found." << endl;
        remove("temp_secondary.txt"); // Remove temporary file
        return;
    }

    remove(Employee_SecondaryFile.c_str());

    // Rename temporary secondary file to replace the original
    if (rename("temp_secondary.txt", Employee_SecondaryFile.c_str()) != 0) {
        cout << "Failed to rename temporary secondary file." << endl;
        return;
    }

    // Now that the secondary file is updated, delete the corresponding entry from the primary file
    ifstream tempSecondaryFileReader(Employee_SecondaryFile);
    if (!tempSecondaryFileReader.is_open()) {
        cout << "Failed to open updated secondary file for reading." << endl;
        return;
    }

    // Read updated secondary file to find corresponding product IDs


    cout << "Employee with name '" << EmpName << "' deleted successfully." << endl;
}

int Employeeclass::findEmpIdByName(const string& EmpName) {
    ifstream secondaryFile(Employee_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return -1; // Return -1 to indicate failure
    }

    string nameFromFile;
    int offset;
    bool found = false;

    // Read secondary file to find the offset corresponding to EmpName
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-insensitive comparison)
        if (nameFromFile == EmpName) {
            found = true;
            break;
        }
    }

    secondaryFile.close();

    if (!found) {
        cout << "Employee with name '" << EmpName << "' not found." << endl;
        return -1; // Return -1 to indicate failure
    }

    // Open primary file to find the product ID corresponding to the offset
    ifstream primaryFile(Employee_PrimaryFile);
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
        cout << "Failed to find Employee ID for name '" << EmpName << "'." << endl;
    }

    return productId;
}

string Employeeclass::findEmpNameById(int EmpId) {
    ifstream primaryFile(Employee_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return ""; // Return empty string to indicate failure
    }

    int idFromFile, offset;
    bool found = false;

    // Read primary file to find the offset corresponding to EmpId
    while (primaryFile >> idFromFile >> offset) {
        if (idFromFile == EmpId) {
            found = true;
            break;
        }
    }

    primaryFile.close();

    if (!found) {
        cout << "Employee with ID '" << EmpId << "' not found." << endl;
        return ""; // Return empty string to indicate failure
    }

    // Open secondary file to find the product name corresponding to the offset
    ifstream secondaryFile(Employee_SecondaryFile);
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
        cout << "Failed to find Employee name for ID '" << EmpId << "'." << endl;
    }

    return productName;
}

bool Employeeclass::isEmpNameExists(const string& Empname ) {
    ifstream secondaryFile(Employee_SecondaryFile);
    if (!secondaryFile.is_open()) {
        cout << "Failed to open secondary file for reading." << endl;
        return false; // Return false to indicate failure
    }

    string nameFromFile;
    int offset;

    // Read secondary file to find if productEmp exists
    while (secondaryFile >> nameFromFile >> offset) {
        // Check if the name matches (case-sensitive comparison)
        if (nameFromFile == Empname) {
            secondaryFile.close();
            return true; // product name exists
        }
    }

    secondaryFile.close();
    return false; // product name does not exist
}
void Employeeclass::DisplayTop5() {
    ifstream primaryFile(Employee_PrimaryFile);
    if (!primaryFile.is_open()) {
        cout << "Failed to open primary file for reading." << endl;
        return;
    }

    int EmpId, offset;
    int recordCount = 0;
    const int maxRecords = 5; // Display only the first 5 records

    while (primaryFile >> EmpId >> offset) {
        // Call displayempById for each product ID
        displayEmpById(EmpId);

        recordCount++;
        if (recordCount >= maxRecords) {
            break; // Stop after displaying the first 5 records
        }
    }

    primaryFile.close();
}