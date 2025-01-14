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
3. Install vcpkg and integrate it with your system. Follow the instructions on the vcpkg GitHub repository to install vcpkg and set it up.
4. Add a `vcpkg.json` file to the root of your project to specify the dependencies. For example, to include Snappy and Google Test, your `vcpkg.json` file should look like this:
   ```json
   {
     "name": "columnar-db",
     "version": "1.0.0",
     "dependencies": [
       "snappy",
       "gtest"
     ]
   }
   ```
5. Modify your `CMakeLists.txt` file to use vcpkg and find the required packages. Add the following lines to the top of your `CMakeLists.txt` file:
   ```cmake
   cmake_minimum_required(VERSION 3.15)
   set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
   ```
6. Update your `CMakeLists.txt` file to find and link the required packages. For example, to find and link Snappy and Google Test, add the following lines:
   ```cmake
   find_package(Snappy REQUIRED)
   find_package(GTest REQUIRED)

   target_link_libraries(columnar_db PRIVATE Snappy::snappy)
   target_link_libraries(test_columnar_db PRIVATE GTest::gtest GTest::gtest_main)
   ```
7. Build your project using CMake:
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
