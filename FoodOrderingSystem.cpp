#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <sstream>
#include<cstdlib>

using namespace std;

void setTextColor()
 {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int randomColor = 1 + rand() % 15; // Random number between 1 and 15
    SetConsoleTextAttribute(hConsole, randomColor);
}




struct MenuItem {
    string name;
    int price;
    int quantity;
};

class OrderQueue {
    MenuItem items[100];
    int capacity;
    int topIndex;

public:
    OrderQueue() {
        capacity = 100;
        topIndex = -1;
    }

    void addItem(MenuItem item) {
        topIndex++;
        items[topIndex] = item;
    }

    MenuItem removeItem() {
        MenuItem firstItem = items[0];
        for (int i = 0; i < topIndex; i++) {
            items[i] = items[i + 1];
        }
        topIndex--;
        return firstItem;
    }

    bool hasItems() {
        return topIndex != -1;
    }
};

class MenuNode {
public:
    string name;
    int price;
    int revenue;
    MenuNode* right;
    MenuNode* left;

    MenuNode(string itemName, int itemPrice) {
        this->name = itemName;
        this->price = itemPrice;
        this->right = NULL;
        this->left = NULL;
        revenue = 0;
    }
};

class MenuTree {
public:
    MenuNode* createMenuItem(string name, int price) {
        MenuNode* newItem = new MenuNode(name, price);
        return newItem;
    }

    MenuNode* insertMenuItem(MenuNode* root, string name, int price) {
        if (root == NULL) {
            return createMenuItem(name, price);
        }

        if (name < root->name) {
            root->left = insertMenuItem(root->left, name, price);
        }
        else if (name > root->name) {
            root->right = insertMenuItem(root->right, name, price);
        }
        return root;
    }

    MenuNode* deleteMenuItem(MenuNode* root, string name) {
        if (root == NULL)
            return NULL;
            
        else if (name < root->name)
            root->left = deleteMenuItem(root->left, name);
            
        else if (name > root->name)
            root->right = deleteMenuItem(root->right, name);
            
        else {
            if (root->left == NULL) {
                MenuNode* temp = root->right;
                delete root;
                return temp;
            }
            
            else if (root->right == NULL) {
                MenuNode* temp = root->left;
                delete root;
                return temp;
            }
            
            else {
                MenuNode* temp = findMinValueNode(root->right);
                root->name = temp->name;
                root->right = deleteMenuItem(root->right, temp->name);
            }
        }
        return root;
    }

    MenuNode* findMinValueNode(MenuNode* node) {
        MenuNode* current = node;
        
        while (current->left != NULL)
            current = current->left;
            
        return current;
    }

    void displayMenu(MenuNode* root) {
        if (root == NULL) {
            return;
        }
        displayMenu(root->left);
        cout << "* " << root->name << " " << root->price << endl;
        displayMenu(root->right);
    }

    void saveMenuToFile(MenuNode* root) {
        if (root == NULL) {
            return;
        }
        saveMenuToFile(root->left);
        fstream file("Fooditem-details.txt", ios::app);
        file << "* " << root->name << ":\nPrice: " << root->price << "\nAmount Earned: " << root->revenue << endl << endl;
        saveMenuToFile(root->right);
    }

    void displayRevenue(MenuNode* root) {
        if (root == NULL) {
            return;
        }
        displayRevenue(root->left);
        cout << "* " << root->name << " " << root->revenue << endl;
        displayRevenue(root->right);
    }

    MenuNode* searchMenuItem(MenuNode* root, string name) {
        if (root == NULL)
            return NULL;
        
        else if (name == root->name)
            return root;
        
        else if (name < root->name)
            return searchMenuItem(root->left, name);
        
        else if (name > root->name)
            return searchMenuItem(root->right, name);
    }

    int processOrder(MenuNode* root) {
        OrderQueue orderQueue;
        MenuItem item;
        int orderMore;
        string customerNames[5] = {"John", "Fatima", "Usman", "Asuka", "Akai"};
        srand(time(0));
        int randomIndex = rand() % 5;
        int quantity;
        string itemName;
        int totalBill = 0;

        do {
            cout << "Displaying Menu: \n";
            displayMenu(root);
            cout << "Enter the desired food item you would like to order" << endl;
            cin >> itemName;
            MenuNode* menuItem = searchMenuItem(root, itemName);
            if (menuItem == NULL) {
                cout << "Sorry, This item doesn't exist.\n";
            }
            else {
                item.name = itemName;
                item.price = menuItem->price;
                cout << "Enter the desired quantity" << endl;
                cin >> quantity;
                item.quantity = quantity;
                menuItem->revenue += menuItem->price * quantity;
                cout << "That would be " << menuItem->price * quantity << endl;
                orderQueue.addItem(item);
            }
            cout << "Do you want to order another item(1 for yes/0 for no)?" << endl;
            cin >> orderMore;  
        } while (orderMore != 0);

        ifstream receiptNumberFile("ReceiptNumber.txt");
        int receiptNumber;
        receiptNumberFile >> receiptNumber;
        fstream receiptFile("Receipt.txt", ios::app);
        cout << "----------------------      Displaying your Receipt: ------------------\n";
        receiptFile << "----------------------      Displaying Receipt#" << receiptNumber << ": ------------------\n";

        while (orderQueue.hasItems()) {
            item = orderQueue.removeItem();
            cout << item.name << "x" << item.quantity << "     " << item.price * item.quantity << endl;
            receiptFile << item.name << "x" << item.quantity << "     " << item.price * item.quantity << endl;
            totalBill += item.price * item.quantity;
        }

        cout << "Total Bill: " << totalBill << "\nYour food will be delivered to your address in 30 minutes by our driver " << customerNames[randomIndex] << " in 30 minutes!\n";
        receiptFile << "Total Bill: " << totalBill << "\nThe food was delivered by " << customerNames[randomIndex] << "\n";
        
        ofstream newReceiptNumberFile("ReceiptNumber.txt");
        receiptNumber++;
        newReceiptNumberFile << receiptNumber;
        return totalBill;
    }
};

struct CustomerData {
    string username, password, email;
    string address;
    int totalSpent;
};

class Customer {
    string username, password, email;
    string address;
    int totalSpent;

public:
    Customer* next = NULL;
    Customer* prev = NULL;
    
    Customer() {
        next = NULL;
        prev = NULL;
        username = "";
        password = "";
        email = "";
        address = "";
        totalSpent = 0;
    }

    Customer(string name, string pwd, string mail, string addr) {
        username = name;
        password = pwd;
        email = mail;
        address = addr;
        totalSpent = 0;
        next = NULL;
        prev = NULL;
    }

    Customer(string name, string pwd, string mail, string addr, int spent) {
        username = name;
        password = pwd;
        email = mail;
        address = addr;
        totalSpent = spent;
        next = NULL;
        prev = NULL;
    }
    
    bool isUsernameMatch(string inputName) {
        return username == inputName;
    }

    bool isPasswordMatch(string inputPassword) {
        return password == inputPassword;
    }

    void setUsername(string input) {
        username = input;
    }

    void setPassword(string input) {
        password = input;
    }

    void setEmail(string input) {
        email = input;
    }

    void setAddress(string input) {
        address = input;
    }

    void addToTotalSpent(int amount) {
        totalSpent += amount;
    }

    string getUsername() {
        return username;
    }

    string getPassword() {
        return password;
    }

    string getEmail() {
        return email;
    }

    string getAddress() {
        return address;
    }

    int getTotalSpent() {
        return totalSpent;
    }
};

class CustomerManager : public Customer, public MenuTree {
    Customer* head = NULL;
    Customer* tail = NULL;
    MenuNode* menuRoot = NULL;
    int totalRevenue;

public:
    CustomerManager() {
        totalRevenue = 0;
        insertCustomer("Usman", "Usman932", "k213225@nu.edu.pk", "-", 999);
        insertCustomer("Fatima", "Fati12389", "k213455@nu.edu.pk", "DE", 700);
        insertCustomer("Test-ID-A", "Test12", "TestA@.edu.pk", "-", 1200);
        insertCustomer("Test-ID-B", "Test34", "TestB@.edu.pk", "-", 400);
        insertCustomer("Test-ID-C", "Test56", "TestC@nu.edu.pk", "-", 9999);
        insertCustomer("Test-ID-D", "Test78", "TestD@nu.edu.pk", "-", 100);

        menuRoot = MenuTree::insertMenuItem(menuRoot, "Biryani", 150);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Burger", 200);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Chow-mein", 500);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Noodle-Soup", 290);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Fried-Rice", 650);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Wonton", 675);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Manchurian", 700);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Chilli-Chicken", 999);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Spaghetti", 895);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Dumplings", 750);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Nihari", 600);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Chicken-Karahi", 890);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Haleem", 250);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Cheese-Burger", 250);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Drink-Small", 60);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Drink-Large", 200);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Icecream", 150);
        menuRoot = MenuTree::insertMenuItem(menuRoot, "Chocolate-Cake", 500);
    }

    string getUniqueUsername() {
        bool isUnique = false;
        string name;
        Customer* temp = head;
        while (!isUnique) {
            cout << "Enter your username: ";
            cin >> name;
            if (temp == NULL) {
                break;
            }
            isUnique = true;
            while (temp != NULL) {
                if (temp->isUsernameMatch(name)) {
                    cout << "Username is already taken, please enter a different one\n";
                    isUnique = false;
                    temp = head;
                    break;
                }
                temp = temp->next;
            }
        }
        return name;
    }

  string getValidPassword() {
    int minLength = 8;
    bool hasUpperCase = false, hasNumber = false;
    string password;
    do {
        hasUpperCase = false;
        hasNumber = false;

        cout << "Enter the password, the password must have at least one uppercase letter and one number: ";
        cin >> password;

        cout << "Password entered: " << password << endl;

        for (char c : password) {
            if (isupper(c)) hasUpperCase = true;
            if (isdigit(c)) hasNumber = true;
        }

        cout << "Has uppercase: " << (hasUpperCase ? "Yes" : "No") << endl;
        cout << "Has number: " << (hasNumber ? "Yes" : "No") << endl;
        cout << "Length: " << password.length() << endl;

        if (password.length() < minLength) {
            cout << "Password must be at least " << minLength << " characters long." << endl;
        }

    } while (password.length() < minLength || !hasUpperCase || !hasNumber);

    return password;
}
       string getEmail() {
        string email;
        cout << "Enter your email address: ";
        cin >> email;
        return email;
    }

    string getAddress() {
        string address;
        cout << "Enter your address: ";
        cin.ignore();
        getline(cin, address);
        return address;
    }

    void insertCustomer(string name, string password, string email, string address, int totalSpent = 0) {
        Customer* newCustomer = new Customer(name, password, email, address, totalSpent);
        if (head == NULL) {
            head = tail = newCustomer;
        } else {
            tail->next = newCustomer;
            newCustomer->prev = tail;
            tail = newCustomer;
        }
    }

    void registerCustomer() {
        string name = getUniqueUsername();
        string password = getValidPassword();
        string email = getEmail();
        string address = getAddress();
        insertCustomer(name, password, email, address);
        cout << "Registration successful!\n";
    }

    Customer* loginCustomer() {
        string name, password;
        cout << "Enter your username: ";
        cin >> name;
        cout << "Enter your password: ";
        cin >> password;
        Customer* temp = head;
        while (temp != NULL) {
            if (temp->isUsernameMatch(name) && temp->isPasswordMatch(password)) {
                cout << "Login successful!\n";
                return temp;
            }
            temp = temp->next;
        }
        cout << "Invalid username or password.\n";
        return NULL;
    }

    void displayCustomers() {
        Customer* temp = head;
        while (temp != NULL) {
            cout << "Username: " << temp->getUsername() << ", Email: " << temp->getEmail() << ", Total Spent: $" << temp->getTotalSpent() << endl;
            temp = temp->next;
        }
    }

    void updateCustomerInfo(Customer* customer) {
        int choice;
        cout << "What would you like to update?\n";
        cout << "1. Password\n2. Email\n3. Address\n";
        cin >> choice;
        switch (choice) {
            case 1:
                customer->setPassword(getValidPassword());
                break;
            case 2:
                customer->setEmail(getEmail());
                break;
            case 3:
                customer->setAddress(getAddress());
                break;
            default:
                cout << "Invalid choice.\n";
        }
        cout << "Information updated successfully!\n";
    }

    void deleteCustomer(string username) {
        Customer* temp = head;
        while (temp != NULL) {
            if (temp->isUsernameMatch(username)) {
                if (temp == head) {
                    head = temp->next;
                    if (head != NULL) head->prev = NULL;
                } else if (temp == tail) {
                    tail = temp->prev;
                    tail->next = NULL;
                } else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }
                delete temp;
                cout << "Customer deleted successfully.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Customer not found.\n";
    }

    void saveCustomersToFile() {
        ofstream file("customers.txt");
        Customer* temp = head;
        while (temp != NULL) {
            file << temp->getUsername() << "," << temp->getPassword() << "," 
                 << temp->getEmail() << "," << temp->getAddress() << "," 
                 << temp->getTotalSpent() << endl;
            temp = temp->next;
        }
        file.close();
    }

    void loadCustomersFromFile() {
        ifstream file("customers.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, password, email, address;
            int totalSpent;
            getline(ss, name, ',');
            getline(ss, password, ',');
            getline(ss, email, ',');
            getline(ss, address, ',');
            ss >> totalSpent;
            insertCustomer(name, password, email, address, totalSpent);
        }
        file.close();
    }

  void saveOrderHistory(Customer* customer, const string& itemName, int quantity, int price) {
    string filename = customer->getUsername() + "_OrderHistory.txt";
    ofstream file(filename, ios::app);
    file << itemName << " x" << quantity << " - $" << price * quantity << "\n";
    file.close();
}

void viewOrderHistory(Customer* customer) {
    string filename = customer->getUsername() + "_OrderHistory.txt";
    ifstream file(filename);

    if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
        cout << "No order history available.\n";
        return;
    }

    cout << "\n--- Order History ---\n";
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void customerMenu(Customer* customer) {
     int choice;
    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. Place an order\n";
        cout << "2. View order history\n";
        cout << "3. Update personal information\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int orderTotal = processOrder(menuRoot);
                customer->addToTotalSpent(orderTotal);
                totalRevenue += orderTotal;
                break;
            }
            case 2:
                cout << "Your total spent is: " << customer->getTotalSpent() << endl;
                break;
            case 3:
                updateCustomerInfo(customer);
                break;
            case 4:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}


    void adminMenu() {
        int choice;
        do {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add new menu item\n";
            cout << "2. Remove menu item\n";
            cout << "3. Display menu\n";
            cout << "4. Display revenue\n";
            cout << "5. Display customers\n";
            cout << "6. Delete customer\n";
            cout << "7. Save data\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string name;
                    int price;
                    cout << "Enter item name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter item price: ";
                    cin >> price;
                    menuRoot = insertMenuItem(menuRoot, name, price);
                    cout << "Item added successfully.\n";
                    break;
                }
                case 2: {
                    string name;
                    cout << "Enter item name to remove: ";
                    cin.ignore();
                    getline(cin, name);
                    menuRoot = deleteMenuItem(menuRoot, name);
                    cout << "Item removed successfully.\n";
                    break;
                }
                case 3:
                    cout << "Current Menu:\n";
                    displayMenu(menuRoot);
                    break;
                case 4:
                    cout << "Revenue by Item:\n";
                    displayRevenue(menuRoot);
                    cout << "Total Revenue: $" << totalRevenue << endl;
                    break;
                case 5:
                    displayCustomers();
                    break;
                case 6: {
                    string username;
                    cout << "Enter username to delete: ";
                    cin >> username;
                    deleteCustomer(username);
                    break;
                }
                case 7:
                    saveCustomersToFile();
                    saveMenuToFile(menuRoot);
                    cout << "Data saved successfully.\n";
                    break;
                case 8:
                    cout << "Exiting admin menu...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 8);
    }
};

int main() 
{
	srand(time(0));
	setTextColor();
    CustomerManager manager;
    int choice;

    do {
        cout << "\n--- Welcome to the Food Ordering System ---\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.registerCustomer();
                break;
            case 2: {
                Customer* loggedInCustomer = manager.loginCustomer();
                if (loggedInCustomer != NULL) {
                    manager.customerMenu(loggedInCustomer);
                }
                break;
            }
            case 3: {
                string adminPassword;
                cout << "Enter admin password: ";
                cin >> adminPassword;
                if (adminPassword == "admin123") {  // Simple admin password for demonstration
                    manager.adminMenu();
                } else {
                    cout << "Invalid admin password.\n";
                }
                break;
            }
            case 4:
                cout << "Thank you for using the Food Ordering System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}

