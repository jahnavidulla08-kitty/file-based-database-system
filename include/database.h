#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <cstdint>

struct Record {
    int32_t id;
    char name[50];
    char course[30];
    double gpa;
    uint8_t active;
};

class FileDatabase {
private:
    std::string dataFile;
    std::string lockFile;
    std::unordered_map<int32_t, std::streamoff> index;

    bool acquireLock();
    void releaseLock();
    bool readRecordAt(std::streamoff offset, Record& record);
    bool writeRecordAt(std::streamoff offset, const Record& record);

public:
    explicit FileDatabase(const std::string& fileName = "records.dat");

    bool createRecord(int32_t id, const std::string& name,
                      const std::string& course, double gpa);
    bool readAll(std::ostream& out);
    bool searchRecord(int32_t id, Record& result);
    bool updateRecord(int32_t id, const std::string& name,
                      const std::string& course, double gpa);
    bool deleteRecord(int32_t id);
    void buildIndex();
    void clearDatabase();

    static void printRecord(std::ostream& out, const Record& record);
};

#endif
