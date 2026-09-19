#include "../include/database.h"
#include <cstring>
#include <cstdio>

FileDatabase::FileDatabase(const std::string& fileName)
    : dataFile(fileName), lockFile(fileName + ".lock") {
    buildIndex();
}

bool FileDatabase::acquireLock() {
    std::ifstream check(lockFile.c_str());
    if (check.good()) {
        check.close();
        return false;
    }
    check.close();

    std::ofstream lock(lockFile.c_str());
    if (!lock) return false;
    lock << "WRITE_LOCK";
    lock.close();
    return true;
}

void FileDatabase::releaseLock() {
    std::remove(lockFile.c_str());
}

void FileDatabase::buildIndex() {
    index.clear();
    std::ifstream file(dataFile.c_str(), std::ios::binary);
    if (!file) return;

    Record record;
    while (true) {
        std::streamoff offset = file.tellg();
        if (!file.read(reinterpret_cast<char*>(&record), sizeof(record)))
            break;
        if (record.active)
            index[record.id] = offset;
    }
    file.close();
}

bool FileDatabase::readRecordAt(std::streamoff offset, Record& record) {
    std::ifstream file(dataFile.c_str(), std::ios::binary);
    if (!file) return false;
    file.seekg(offset);
    bool ok = static_cast<bool>(
        file.read(reinterpret_cast<char*>(&record), sizeof(record))
    );
    file.close();
    return ok && record.active;
}

bool FileDatabase::writeRecordAt(std::streamoff offset, const Record& record) {
    std::fstream file(dataFile.c_str(),
                      std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;
    file.seekp(offset);
    bool ok = static_cast<bool>(
        file.write(reinterpret_cast<const char*>(&record), sizeof(record))
    );
    file.flush();
    file.close();
    return ok;
}

bool FileDatabase::createRecord(int32_t id, const std::string& name,
                                const std::string& course, double gpa) {
    if (index.find(id) != index.end()) return false;
    if (!acquireLock()) return false;

    Record record;
    std::memset(&record, 0, sizeof(record));
    record.id = id;
    std::strncpy(record.name, name.c_str(), sizeof(record.name) - 1);
    std::strncpy(record.course, course.c_str(), sizeof(record.course) - 1);
    record.gpa = gpa;
    record.active = 1;

    std::ofstream file(dataFile.c_str(), std::ios::binary | std::ios::app);
    if (!file) {
        releaseLock();
        return false;
    }

    std::streamoff offset = file.tellp();
    file.write(reinterpret_cast<const char*>(&record), sizeof(record));
    bool ok = static_cast<bool>(file);
    file.close();

    if (ok) index[id] = offset;
    releaseLock();
    return ok;
}

bool FileDatabase::readAll(std::ostream& out) {
    std::ifstream file(dataFile.c_str(), std::ios::binary);
    if (!file) return false;

    Record record;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        if (record.active) {
            printRecord(out, record);
            found = true;
        }
    }
    file.close();
    return found;
}

bool FileDatabase::searchRecord(int32_t id, Record& result) {
    std::unordered_map<int32_t, std::streamoff>::iterator it = index.find(id);
    if (it == index.end()) return false;
    return readRecordAt(it->second, result);
}

bool FileDatabase::updateRecord(int32_t id, const std::string& name,
                                const std::string& course, double gpa) {
    std::unordered_map<int32_t, std::streamoff>::iterator it = index.find(id);
    if (it == index.end()) return false;
    if (!acquireLock()) return false;

    Record record;
    if (!readRecordAt(it->second, record)) {
        releaseLock();
        return false;
    }

    std::memset(record.name, 0, sizeof(record.name));
    std::memset(record.course, 0, sizeof(record.course));
    std::strncpy(record.name, name.c_str(), sizeof(record.name) - 1);
    std::strncpy(record.course, course.c_str(), sizeof(record.course) - 1);
    record.gpa = gpa;

    bool ok = writeRecordAt(it->second, record);
    releaseLock();
    return ok;
}

bool FileDatabase::deleteRecord(int32_t id) {
    std::unordered_map<int32_t, std::streamoff>::iterator it = index.find(id);
    if (it == index.end()) return false;
    if (!acquireLock()) return false;

    Record record;
    if (!readRecordAt(it->second, record)) {
        releaseLock();
        return false;
    }

    record.active = 0;
    bool ok = writeRecordAt(it->second, record);
    if (ok) index.erase(it);

    releaseLock();
    return ok;
}

void FileDatabase::clearDatabase() {
    if (!acquireLock()) return;
    std::ofstream file(dataFile.c_str(), std::ios::binary | std::ios::trunc);
    file.close();
    index.clear();
    releaseLock();
}

void FileDatabase::printRecord(std::ostream& out, const Record& record) {
    out << "ID: " << record.id
        << " | Name: " << record.name
        << " | Course: " << record.course
        << " | GPA: " << record.gpa << std::endl;
}
