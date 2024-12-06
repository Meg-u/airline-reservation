// A simple C++ program for a flight ticket reservation system using structure and file
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// A structure to represent a flight
struct Flight {
    string flight_number; // The flight number
    string origin; // The origin of the flight
    string destination; // The destination of the flight
    int capacity; // The maximum number of passengers
    int booked; // The number of passengers already booked
    double price; // The price of the ticket
};

// A structure to represent a user
struct User {
    string name; // The name of the user
    string email; // The email of the user
    string password; // The password of the user
    vector<string> tickets; // The tickets booked by the user
};

// A function to check if a user exists in the file
bool user_exists(string email) {
    ifstream file("users.txt"); // Open the file for reading
    if (file.is_open()) { // If the file is opened successfully
        string line; // A variable to store each line
        while (getline(file, line)) { // While there are lines to read
            if (line == email) { // If the line matches the email
                file.close(); // Close the file
                return true; // Return true
            }
        }
        file.close(); // Close the file
    }
    return false; // Return false
}

// A function to create a new user and store it in the file
void create_user(string name, string email, string password) {
    ofstream file("users.txt", ios::app); // Open the file for appending
    if (file.is_open()) { // If the file is opened successfully
        file << email << endl; // Write the email to the file
        file << name << endl; // Write the name to the file
        file << password << endl; // Write the password to the file
        file << 0 << endl; // Write the number of tickets to the file
        file.close(); // Close the file
    }
}

// A function to get the user details from the file
User get_user(string email) {
    User user; // A variable to store the user details
    ifstream file("users.txt"); // Open the file for reading
    if (file.is_open()) { // If the file is opened successfully
        string line; // A variable to store each line
        while (getline(file, line)) { // While there are lines to read
            if (line == email) { // If the line matches the email
                user.email = email; // Set the user email
                getline(file, user.name); // Get the user name
                getline(file, user.password); // Get the user password
                int n; // A variable to store the number of tickets
                file >> n; // Read the number of tickets
                file.ignore(); // Ignore the newline character
                for (int i = 0; i < n; i++) { // For each ticket
                    string ticket; // A variable to store the ticket
                    getline(file, ticket); // Get the ticket
                    user.tickets.push_back(ticket); // Add the ticket to the user tickets
                }
                break; // Break the loop
            }
        }
        file.close(); // Close the file
    }
    return user; // Return the user
}

// A function to update the user details in the file
void update_user(User user) {
    ifstream file("users.txt"); // Open the file for reading
    ofstream temp("temp.txt"); // Open a temporary file for writing
    if (file.is_open() && temp.is_open()) { // If both files are opened successfully
        string line; // A variable to store each line
        while (getline(file, line)) { // While there are lines to read
            if (line == user.email) { // If the line matches the user email
                temp << user.email << endl; // Write the user email to the temp file
                temp << user.name << endl; // Write the user name to the temp file
                temp << user.password << endl; // Write the user password to the temp file
                temp << user.tickets.size() << endl; // Write the number of tickets to the temp file
                for (string ticket : user.tickets) { // For each ticket
                    temp << ticket << endl; // Write the ticket to the temp file
                }
            } else { // If the line does not match the user email
                temp << line << endl; // Write the line to the temp file
            }
        }
        file.close(); // Close the file
        temp.close(); // Close the temp file
        remove("users.txt"); // Remove the original file
        rename("temp.txt", "users.txt"); // Rename the temp file to the original file
    }
}

// A function to display the available flights
void display_flights(vector<Flight> flights) {
    cout << "The available flights are:\n";
    cout << "Flight Number\tOrigin\tDestination\tCapacity\tBooked\tPrice\n";
    for (Flight flight : flights) { // For each flight
        cout << flight.flight_number << "\t" << flight.origin << "\t" << flight.destination << "\t" << flight.capacity << "\t" << flight.booked << "\t" << flight.price << endl;
    }
}

// A function to book a flight for a user
void book_flight(vector<Flight>& flights, User& user) {
    display_flights(flights); // Display the available flights
    cout << "Enter the flight number you want to book: ";
    string fn; // A variable to store the flight number
    cin >> fn; // Read the flight number
    bool found = false; // A variable to indicate if the flight is found
    for (Flight& flight : flights) { // For each flight
        if (flight.flight_number == fn) { // If the flight number matches
            found = true; // Set found to true
            if (flight.booked < flight.capacity) { // If the flight is not full
                flight.booked++; // Increment the booked count
                user.tickets.push_back(fn); // Add the flight number to the user tickets
                cout << "You have successfully booked the flight " << fn << ".\n";
                cout << "The ticket price is " << flight.price << ".\n";
                update_user(user); // Update the user details in the file
            } else { // If the flight is full
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
    string fn; // A variable to store the flight number
    cin >> fn; // Read the flight number
    bool found = false; // A variable to indicate if the ticket is found
    for (int i = 0; i < user.tickets.size(); i++) { // For each ticket
        if (user.tickets[i] == fn) { // If the ticket matches the flight number
            found = true; // Set found to true
            user.tickets.erase(user.tickets.begin() + i); // Erase the ticket from the user tickets
            for (Flight& flight : flights) { // For each flight
                if (flight.flight_number == fn) { // If the flight number matches
                    flight.booked--; // Decrement the booked count
                    break; // Break the loop
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
    int choice; // A variable to store the user choice
    cin >> choice; // Read the user choice
    switch (choice) { // Switch on the user choice
        case 1: // If the user has an account
            cout << "Enter your email: ";
            string email; // A variable to store the user email
            cin >> email; // Read the user email
            if (user_exists(email)) { // If the user exists in the file
                cout << "Enter your password: ";
                string password; // A variable to store the user password
                cin >> password; // Read the user password
                User user = get_user(email); // Get the user details from the file
                if (user.password == password) { // If the password matches
                    display_menu(flights, user); // Call the display menu function
                } else { // If the password does not match
                    cout << "Wrong password. Please try again.\n";
                }
            } else { // If the user does not exist in the file
                cout << "No account found with this email. Please create an account.\n";
            }
            break; // Break the switch
       // case 2: // If the user does not have an account
            cout << "Enter your name: ";
            string name; // A variable to store the user name
            cin >> name; // Read the user name
            cout << "Enter your email: ";
           // string email; // A variable to store the user email
            cin >> email; // Read the user email
            if (!user_exists(email)) { // If the user does not exist in the file
                cout << "Enter your password: ";
                string password; // A variable to store the user password
                cin >> password; // Read the user password
                create_user(name, email, password); // Create a new user and store it in the file
                User user = get_user(email); // Get the user details from the file
                display_menu(flights, user); // Call the display menu function
            } else { // If the user exists in the file
                cout << "An account already exists with this email. Please login.\n";
            }
            break; // Break the switch
        //default: // If the user enters an invalid choice
            cout << "Invalid choice. Please try again.\n";
            break; // Break the switch
    }
    return 0; // Return from the main function
}

