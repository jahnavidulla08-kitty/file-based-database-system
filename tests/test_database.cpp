#include "../include/database.h"
#include <cassert>
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

int main() {
    const char* file = "test_records.dat";
    remove(file);
    remove("test_records.dat.lock");

    FileDatabase db(file);
    Record record;

    // Create + duplicate check
    assert(db.createRecord(101, "Ananya", "AIML", 8.5));
    assert(db.createRecord(102, "Rahul", "CSE", 9.0));
    assert(!db.createRecord(101, "Duplicate", "AIML", 7.0));

    // Search
    assert(db.searchRecord(101, record));
    assert(record.id == 101);
    assert(string(record.name) == "Ananya");

    // Update
    assert(db.updateRecord(101, "Ananya Updated", "AIML", 9.2));
    assert(db.searchRecord(101, record));
    assert(string(record.name) == "Ananya Updated");
    assert(record.gpa == 9.2);

    // Delete
    assert(db.deleteRecord(102));
    assert(!db.searchRecord(102, record));

    // Persistence: rebuild index from the same file
    FileDatabase reopened(file);
    assert(reopened.searchRecord(101, record));
    assert(string(record.name) == "Ananya Updated");

    // Missing record checks
    assert(!reopened.searchRecord(999, record));
    assert(!reopened.updateRecord(999, "Nobody", "NA", 0.0));
    assert(!reopened.deleteRecord(999));

    reopened.clearDatabase();
    remove(file);
    remove("test_records.dat.lock");

    cout << "All File-Based Database unit tests passed successfully!" << endl;
    return 0;
}
