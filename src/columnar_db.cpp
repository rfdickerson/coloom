#include "columnar_db.h"
#include <snappy.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>

enum class DataType {
    INT,
    FLOAT,
    DOUBLE,
    STRING,
    BOOL,
    DATE,
    TIMESTAMP
};

class ColumnarDB {
public:
    ColumnarDB() = default;

    void addColumn(const std::string& name, DataType type) {
        columns[name] = type;
        data[name] = std::vector<std::string>();
    }

    void setPrimaryKey(const std::string& name) {
        if (columns.find(name) == columns.end()) {
            throw std::runtime_error("Primary key column does not exist");
        }
        primaryKey = name;
    }

    void addRow(const std::vector<std::string>& row) {
        if (row.size() != columns.size()) {
            throw std::runtime_error("Row size does not match number of columns");
        }

        int i = 0;
        for (const auto& col : columns) {
            data[col.first].push_back(row[i]);
            ++i;
        }

        if (data.begin()->second.size() % 8192 == 0) {
            granules.push_back(data);
            for (auto& col : data) {
                col.second.clear();
            }
        }
    }

    void serialize(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write header
        writeHeader(file);

        // Write column data
        for (const auto& col : columns) {
            writeColumnData(file, col.first, col.second);
        }

        // Write footer
        writeFooter(file);

        file.close();
    }

    void deserialize(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for reading");
        }

        // Read header
        readHeader(file);

        // Read column data
        for (const auto& col : columns) {
            readColumnData(file, col.first, col.second);
        }

        // Read footer
        readFooter(file);

        file.close();
    }

private:
    std::unordered_map<std::string, DataType> columns;
    std::unordered_map<std::string, std::vector<std::string>> data;
    std::vector<std::unordered_map<std::string, std::vector<std::string>>> granules;
    std::string primaryKey;

    void writeHeader(std::ofstream& file) {
        // Write file version
        int32_t version = 1;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));

        // Write number of columns
        int32_t numColumns = columns.size();
        file.write(reinterpret_cast<const char*>(&numColumns), sizeof(numColumns));

        // Write column names and data types
        for (const auto& col : columns) {
            writeString(file, col.first);
            file.write(reinterpret_cast<const char*>(&col.second), sizeof(col.second));
        }

        // Write number of rows
        int64_t numRows = data.begin()->second.size();
        file.write(reinterpret_cast<const char*>(&numRows), sizeof(numRows));

        // Write compression method
        writeString(file, "Snappy");

        // Write primary key
        writeString(file, primaryKey);
    }

    void writeColumnData(std::ofstream& file, const std::string& name, DataType type) {
        // Write column name
        writeString(file, name);

        // Write data type
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));

        // Write compressed data
        std::string uncompressedData;
        for (const auto& value : data[name]) {
            uncompressedData += value + "\n";
        }

        std::string compressedData;
        snappy::Compress(uncompressedData.data(), uncompressedData.size(), &compressedData);
        writeString(file, compressedData);
    }

    void writeFooter(std::ofstream& file) {
        // Write checksum (dummy value for now)
        int32_t checksum = 0;
        file.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
    }

    void readHeader(std::ifstream& file) {
        // Read file version
        int32_t version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));

        // Read number of columns
        int32_t numColumns;
        file.read(reinterpret_cast<char*>(&numColumns), sizeof(numColumns));

        // Read column names and data types
        for (int i = 0; i < numColumns; ++i) {
            std::string name = readString(file);
            DataType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(type));
            columns[name] = type;
            data[name] = std::vector<std::string>();
        }

        // Read number of rows
        int64_t numRows;
        file.read(reinterpret_cast<char*>(&numRows), sizeof(numRows));

        // Read compression method
        std::string compressionMethod = readString(file);
        if (compressionMethod != "Snappy") {
            throw std::runtime_error("Unsupported compression method");
        }

        // Read primary key
        primaryKey = readString(file);
    }

    void readColumnData(std::ifstream& file, const std::string& name, DataType type) {
        // Read column name
        std::string columnName = readString(file);

        // Read data type
        DataType columnType;
        file.read(reinterpret_cast<char*>(&columnType), sizeof(columnType));

        // Read compressed data
        std::string compressedData = readString(file);

        // Decompress data
        std::string uncompressedData;
        snappy::Uncompress(compressedData.data(), compressedData.size(), &uncompressedData);

        // Parse data
        std::istringstream iss(uncompressedData);
        std::string value;
        while (std::getline(iss, value)) {
            data[name].push_back(value);
        }
    }

    void readFooter(std::ifstream& file) {
        // Read checksum (dummy value for now)
        int32_t checksum;
        file.read(reinterpret_cast<char*>(&checksum), sizeof(checksum));
    }

    void writeString(std::ofstream& file, const std::string& str) {
        int32_t length = str.size();
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(str.data(), length);
    }

    std::string readString(std::ifstream& file) {
        int32_t length;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        std::string str(length, '\0');
        file.read(&str[0], length);
        return str;
    }
};
