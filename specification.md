# Columnar Database File Format Specification

## Schema

### Data Types

The columnar database file format supports the following data types:

- `INT`: 32-bit signed integer
- `FLOAT`: 32-bit floating-point number
- `DOUBLE`: 64-bit floating-point number
- `STRING`: Variable-length string
- `BOOL`: Boolean value
- `DATE`: Date value in the format `YYYY-MM-DD`
- `TIMESTAMP`: Timestamp value in the format `YYYY-MM-DD HH:MM:SS`

### Column Names

Column names are stored as variable-length strings and must be unique within a table. They are case-sensitive and can contain alphanumeric characters and underscores (`_`).

### Metadata

Metadata is stored at the beginning of the file and includes the following information:

- File version
- Number of columns
- Column names and data types
- Number of rows
- Compression method

### Primary Key

A primary key is used to give the sort order for the rows. The primary key column must be unique and is used to determine the order in which rows are stored and retrieved.

## Data Storage

### Columnar Storage

Data is stored in a columnar format, where each column is stored separately. This allows for better compression and faster query performance.

### Compression Techniques

The columnar database file format uses Snappy compression to reduce file size. Each column is compressed separately to allow for efficient access to individual columns.

### Encoding Methods

Data is encoded using the following methods:

- `INT`, `FLOAT`, `DOUBLE`, `BOOL`: Stored as binary values
- `STRING`: Stored as a length-prefixed binary value
- `DATE`, `TIMESTAMP`: Stored as strings in the specified format

### Granules

Data is grouped into "granules" that are 8192 rows each. This allows for efficient data access and better compression.

## File Format Structure

The file format is structured as follows:

1. **Header**: Contains metadata about the file, including the file version, number of columns, column names and data types, number of rows, and compression method.
2. **Column Data**: Each column is stored separately, with the data for each column compressed using Snappy compression.
3. **Indexes**: Contains the indexing structure for faster search operations.
4. **Footer**: Contains a checksum to verify the integrity of the file.

### Header

The header is stored at the beginning of the file and includes the following fields:

- `File Version` (4 bytes): A 32-bit integer representing the file version.
- `Number of Columns` (4 bytes): A 32-bit integer representing the number of columns in the table.
- `Column Names and Data Types` (variable length): A list of column names and their corresponding data types.
- `Number of Rows` (8 bytes): A 64-bit integer representing the number of rows in the table.
- `Compression Method` (variable length): A string representing the compression method used (e.g., "Snappy").
- `Primary Key` (variable length): The name of the primary key column.

### Column Data

Each column is stored separately and compressed using Snappy compression. The data for each column is stored in the following format:

- `Column Name` (variable length): The name of the column.
- `Data Type` (variable length): The data type of the column.
- `Compressed Data` (variable length): The compressed data for the column.

### Indexes

Indexes are stored to allow for faster search operations. The indexing structure is stored in the following format:

- `Number of Indexes` (4 bytes): A 32-bit integer representing the number of indexes.
- `Index Entries` (variable length): A list of index entries, each containing:
  - `Column Name` (variable length): The name of the indexed column.
  - `Number of Entries` (4 bytes): A 32-bit integer representing the number of entries in the index.
  - `Index Data` (variable length): A list of index data, each containing:
    - `Value` (variable length): The value being indexed.
    - `Number of Positions` (4 bytes): A 32-bit integer representing the number of positions for the value.
    - `Positions` (variable length): A list of positions (row indices) where the value is found.

### Footer

The footer is stored at the end of the file and includes the following field:

- `Checksum` (4 bytes): A 32-bit integer representing the checksum of the file, used to verify the integrity of the file.
