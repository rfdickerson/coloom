#include "columnar_db.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <fstream>

class ColumnarDBTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a new columnar database
        db.addColumn("column1", DataType::INT);
        db.addColumn("column2", DataType::STRING);
        db.addRow({"1", "data1"});
        db.addRow({"2", "data2"});
    }

    ColumnarDB db;
};

TEST_F(ColumnarDBTest, SerializeAndDeserialize) {
    // Serialize the database to a file
    db.serialize("test_data.cdb");

    // Deserialize the database from the file
    ColumnarDB db2;
    db2.deserialize("test_data.cdb");

    // Check that the data matches
    EXPECT_EQ(db2.getData("column1"), std::vector<std::string>({"1", "2"}));
    EXPECT_EQ(db2.getData("column2"), std::vector<std::string>({"data1", "data2"}));
}

TEST_F(ColumnarDBTest, HandleVariousDataTypes) {
    // Add columns with different data types
    db.addColumn("column3", DataType::FLOAT);
    db.addColumn("column4", DataType::BOOL);
    db.addRow({"3", "data3", "3.14", "true"});
    db.addRow({"4", "data4", "2.71", "false"});

    // Serialize and deserialize the database
    db.serialize("test_data_types.cdb");
    ColumnarDB db2;
    db2.deserialize("test_data_types.cdb");

    // Check that the data matches
    EXPECT_EQ(db2.getData("column3"), std::vector<std::string>({"3.14", "2.71"}));
    EXPECT_EQ(db2.getData("column4"), std::vector<std::string>({"true", "false"}));
}

TEST_F(ColumnarDBTest, HandleLargeDatasets) {
    // Add a large number of rows
    for (int i = 0; i < 1000000; ++i) {
        db.addRow({std::to_string(i), "data" + std::to_string(i)});
    }

    // Serialize and deserialize the database
    db.serialize("test_large_dataset.cdb");
    ColumnarDB db2;
    db2.deserialize("test_large_dataset.cdb");

    // Check that the data matches
    for (int i = 0; i < 1000000; ++i) {
        EXPECT_EQ(db2.getData("column1")[i], std::to_string(i));
        EXPECT_EQ(db2.getData("column2")[i], "data" + std::to_string(i));
    }
}

TEST_F(ColumnarDBTest, PrimaryKeyFunctionality) {
    // Set primary key
    db.setPrimaryKey("column1");

    // Add rows
    db.addRow({"3", "data3"});
    db.addRow({"1", "data1"});
    db.addRow({"2", "data2"});

    // Serialize and deserialize the database
    db.serialize("test_primary_key.cdb");
    ColumnarDB db2;
    db2.deserialize("test_primary_key.cdb");

    // Check that the data is sorted by primary key
    EXPECT_EQ(db2.getData("column1"), std::vector<std::string>({"1", "2", "3"}));
    EXPECT_EQ(db2.getData("column2"), std::vector<std::string>({"data1", "data2", "data3"}));
}

TEST_F(ColumnarDBTest, GranuleFunctionality) {
    // Add a large number of rows to create multiple granules
    for (int i = 0; i < 16384; ++i) {
        db.addRow({std::to_string(i), "data" + std::to_string(i)});
    }

    // Serialize and deserialize the database
    db.serialize("test_granules.cdb");
    ColumnarDB db2;
    db2.deserialize("test_granules.cdb");

    // Check that the data matches
    for (int i = 0; i < 16384; ++i) {
        EXPECT_EQ(db2.getData("column1")[i], std::to_string(i));
        EXPECT_EQ(db2.getData("column2")[i], "data" + std::to_string(i));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
