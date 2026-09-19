# Task 3 - File-Based Database System

A C++ file-based database with persistent storage, CRUD operations, binary file I/O, record indexing, search, serialization, and basic concurrent-access protection.

## Features
- Create, read, update and delete records
- Persistent binary-file storage
- Record serialization/deserialization
- In-memory ID-to-file-offset index
- Fast indexed search
- Delete by marking records inactive
- Basic lock-file mechanism for write operations
- Unit tests
- Usage documentation

## Structure
```text
task3-file-based-database/
├── include/database.h
├── src/database.cpp
├── src/main.cpp
├── tests/test_database.cpp
└── README.md
```

## Compile
```bash
g++ -std=c++11 src/database.cpp src/main.cpp -Iinclude -o database
g++ -std=c++11 src/database.cpp tests/test_database.cpp -Iinclude -o tests
```

## Run
```bash
./database
./tests
```

In Dev-C++, create a console project and add `include/database.h`, `src/database.cpp`, and `src/main.cpp`. Add `tests/test_database.cpp` to a separate test project.

## Example
```text
1. Create record
2. View all records
3. Search record
4. Update record
5. Delete record
6. Exit

Enter ID: 101
Enter name: Ananya
Enter course: AIML
Enter GPA: 8.5
Record created successfully.
```

## Complexity
| Operation | Expected Time | Extra Space |
|---|---:|---:|
| Create | O(1) append | O(1) |
| Search | O(1) average with index | O(1) |
| Update | O(1) average with index | O(1) |
| Delete | O(1) average with index | O(1) |
| Read all | O(n) | O(1) |
| Build index | O(n) | O(n) |

The in-memory index requires O(n) additional space.

## Concurrency
Write operations use a simple `.lock` file. If the lock exists, another writer is considered active and the operation is rejected. For production software, OS-level file locks or a transactional database engine should be used.

## Serialization
Records use a fixed-size binary representation containing ID, fixed-size name, fixed-size course, GPA and active flag. This allows records to be written and read directly with binary file I/O.

## Memory and File Safety
Files are closed after operations. The lock file is removed after write operations. Deleted records are marked inactive so remaining records do not need to be shifted.
