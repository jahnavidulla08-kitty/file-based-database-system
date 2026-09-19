#include "../include/database.h"
#include <iostream>
#include <limits>
using namespace std;

int main() {
    FileDatabase db("records.dat");
    int choice = 0;

    do {
        cout << "\n===== FILE-BASED DATABASE =====\n";
        cout << "1. Create record\n";
        cout << "2. View all records\n";
        cout << "3. Search record\n";
        cout << "4. Update record\n";
        cout << "5. Delete record\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id; string name, course; double gpa;
            cout << "Enter ID: "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter name: "; getline(cin, name);
            cout << "Enter course: "; getline(cin, course);
            cout << "Enter GPA: "; cin >> gpa;

            cout << (db.createRecord(id, name, course, gpa)
                ? "Record created successfully.\n"
                : "Could not create record.\n");
        }
        else if (choice == 2) {
            if (!db.readAll(cout)) cout << "No records found.\n";
        }
        else if (choice == 3) {
            int id; Record record;
            cout << "Enter ID to search: "; cin >> id;
            if (db.searchRecord(id, record))
                FileDatabase::printRecord(cout, record);
            else
                cout << "Record not found.\n";
        }
        else if (choice == 4) {
            int id; string name, course; double gpa;
            cout << "Enter ID to update: "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter new name: "; getline(cin, name);
            cout << "Enter new course: "; getline(cin, course);
            cout << "Enter new GPA: "; cin >> gpa;

            cout << (db.updateRecord(id, name, course, gpa)
                ? "Record updated successfully.\n"
                : "Could not update record.\n");
        }
        else if (choice == 5) {
            int id;
            cout << "Enter ID to delete: "; cin >> id;
            cout << (db.deleteRecord(id)
                ? "Record deleted successfully.\n"
                : "Could not delete record.\n");
        }
        else if (choice == 6) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
