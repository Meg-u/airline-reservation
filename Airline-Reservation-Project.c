// A simple C++ program for a flight ticket reservation system using structure and file
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// A structure to represent a flight
struct Flight {
    string flight_number; 
    string origin; 
    string destination; 
    int capacity; 
    int booked; 
    double price; 
};

// A structure to represent a user
struct User {
    string name; 
    string email; 
    string password; 
    vector<string> tickets; 
};

// A function to check if a user exists in the file
bool user_exists(string email) {
    ifstream file("users.txt"); 
    if (file.is_open()) { 
        string line; 
        while (getline(file, line)) { 
            if (line == email) { 
                file.close(); 
                return true; 
            }
        }
        file.close(); 
    }
    return false; 
}

// A function to create a new user and store it in the file
void create_user(string name, string email, string password) {
    ofstream file("users.txt", ios::app); 
    if (file.is_open()) { 
        file << email << endl; 
        file << name << endl; 
        file << password << endl;
        file << 0 << endl; 
        file.close(); 
    }
}

// A function to get the user details from the file
User get_user(string email) {
    User user; 
    ifstream file("users.txt"); 
    if (file.is_open()) { 
        string line; 
        while (getline(file, line)) {
            if (line == email) { 
                user.email = email;
                getline(file, user.name); 
                getline(file, user.password); 
                int n; 
                file >> n;
                file.ignore(); 
                for (int i = 0; i < n; i++) { 
                    string ticket;
                    getline(file, ticket);
                    user.tickets.push_back(ticket); 
                }
                break; 
            }
        }
        file.close();
    }
    return user; 
}

// A function to update the user details in the file
void update_user(User user) {
    ifstream file("users.txt"); 
    ofstream temp("temp.txt");
    if (file.is_open() && temp.is_open()) {
        string line; //
        while (getline(file, line)) { 
            if (line == user.email) {
                temp << user.email << endl; 
                temp << user.name << endl; 
                temp << user.password << endl;
                temp << user.tickets.size() << endl; 
                for (string ticket : user.tickets) { 
                    temp << ticket << endl; 
                }
            } else { 
                temp << line << endl; 
            }
        }
        file.close(); 
        temp.close(); 
        remove("users.txt"); 
        rename("temp.txt", "users.txt"); 
    }
}

// A function to display the available flights
void display_flights(vector<Flight> flights) {
    cout << "The available flights are:\n";
    cout << "Flight Number\tOrigin\tDestination\tCapacity\tBooked\tPrice\n";
    for (Flight flight : flights) { 
        cout << flight.flight_number << "\t" << flight.origin << "\t" << flight.destination << "\t" << flight.capacity << "\t" << flight.booked << "\t" << flight.price << endl;
    }
}

// A function to book a flight for a user
void book_flight(vector<Flight>& flights, User& user) {
    display_flights(flights);
    cout << "Enter the flight number you want to book: ";
    string fn;
    cin >> fn; 
    bool found = false; 
    for (Flight& flight : flights) {
        if (flight.flight_number == fn) { 
            found = true; 
            if (flight.booked < flight.capacity) { 
                flight.booked++; 
                user.tickets.push_back(fn);
                cout << "You have successfully booked the flight " << fn << ".\n";
                cout << "The ticket price is " << flight.price << ".\n";
                update_user(user); 
            } else { 
                cout << "Sorry, the flight " << fn << " is full.\n";
            }
            break; // Break the loop
        }
    }
    if (!found) { // If the flight is not found
        cout << "Sorry, the flight " << fn << " does not exist.\n";
    }
}

// A function to cancel a flight for a user
void cancel_flight(vector<Flight>& flights, User& user) {
    cout << "The flights you have booked are:\n";
    for (string ticket : user.tickets) { // For each ticket
        cout << ticket << endl;
    }
    cout << "Enter the flight number you want to cancel: ";
    string fn; 
    cin >> fn; 
    bool found = false;
    for (int i = 0; i < user.tickets.size(); i++) { 
        if (user.tickets[i] == fn) {
            found = true;
            user.tickets.erase(user.tickets.begin() + i);
            for (Flight& flight : flights) {
                if (flight.flight_number == fn) { 
                    flight.booked--; 
                    break;
                }
            }
            cout << "You have successfully canceled the flight " << fn << ".\n";
            update_user(user); // Update the user details in the file
            break; // Break the loop
        }
    }
    if (!found) { // If the ticket is not found
        cout << "Sorry, you have not booked the flight " << fn << ".\n";
    }
}

// A function to display the menu for a user
void display_menu(vector<Flight>& flights, User& user) {
    cout << "Welcome, " << user.name << ".\n";
    cout << "What do you want to do?\n";
    cout << "1. Book a flight\n";
    cout << "2. Cancel a flight\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
    int choice; // A variable to store the user choice
    cin >> choice; // Read the user choice
    switch (choice) { // Switch on the user choice
        case 1: // If the user chooses to book a flight
            book_flight(flights, user); // Call the book flight function
            break; // Break the switch
        case 2: // If the user
                    //case 2: // If the user chooses to cancel a flight
            cancel_flight(flights, user); // Call the cancel flight function
            break; // Break the switch
        case 3: // If the user chooses to exit
            cout << "Thank you for using the flight ticket reservation system. Goodbye.\n";
            return; // Return from the function
        default: // If the user enters an invalid choice
            cout << "Invalid choice. Please try again.\n";
            break; // Break the switch
    }
    display_menu(flights, user); // Recursively call the display menu function
}

// The main function
int main() {
    // A vector to store the available flights
    vector<Flight> flights;
    // Add some flights to the vector
    flights.push_back(Flight("AA101", "New York", "London", 100, 50, 500.0));
    flights.push_back(Flight("BB202", "Paris", "Tokyo", 80, 40, 600.0));
    flights.push_back(Flight("CC303", "Delhi", "Sydney", 120, 60, 700.0));

    cout << "Welcome to the flight ticket reservation system.\n";
    cout << "Do you have an account?\n";
    cout << "1. Yes\n";
    cout << "2. No\n";
    cout << "Enter your choice: ";
    int choice; 
    
    cin >> choice; 
    switch (choice) {
        case 1:
            cout << "Enter your email: ";
            string email;
            cin >> email;
            if (user_exists(email)) { 
                cout << "Enter your password: ";
                string password; 
                cin >> password; 
                User user = get_user(email); 
                if (user.password == password) {
                    display_menu(flights, user); 
                } else { 
                    cout << "Wrong password. Please try again.\n";
                }
            } else { 
                cout << "No account found with this email. Please create an account.\n";
            }
            break; 
       // case 2: // If the user does not have an account
            cout << "Enter your name: ";
            string name; 
            cin >> name; 
            cout << "Enter your email: ";
           /
            cin >> email; 
            if (!user_exists(email)) {
                cout << "Enter your password: ";
                string password; 
                cin >> password; 
                create_user(name, email, password); 
                User user = get_user(email); 
                display_menu(flights, user); 
            } else { 
                cout << "An account already exists with this email. Please login.\n";
            }
            break;
        /
            cout << "Invalid choice. Please try again.\n";
            break; 
    }
    return 0; 
}

