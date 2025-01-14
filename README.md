# Columnar Database File Format

This project aims to create a columnar database file format with C++ and Snappy compression. The file format is designed to efficiently store and query large datasets by organizing data in columns, allowing for better compression and faster query performance.

## Features

- Columnar storage format
- Snappy compression for reduced file size
- Support for various data types
- Efficient data serialization and deserialization
- Handling of edge cases, such as null values and large datasets

## Installation

To install the library, follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/githubnext/workspace-blank.git
   ```
2. Navigate to the project directory:
   ```
   cd workspace-blank
   ```
3. Build the library using CMake:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

## Usage

To use the library in your project, include the header files and link against the compiled library. Here is an example of how to use the library:

```cpp
#include "columnar_db.h"

int main() {
    // Create a new columnar database
    ColumnarDB db;

    // Add columns and data
    db.addColumn("column1", DataType::INT);
    db.addColumn("column2", DataType::STRING);
    db.addRow({1, "data1"});
    db.addRow({2, "data2"});

    // Serialize the database to a file
    db.serialize("data.cdb");

    // Deserialize the database from a file
    ColumnarDB db2;
    db2.deserialize("data.cdb");

    return 0;
}
```

For more detailed information on the file format and library usage, please refer to the [specification document](specification.md).
