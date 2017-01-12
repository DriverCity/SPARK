#include <gtest/gtest.h>
#include "VerifyParking.h"
#include "ParkingDatabase/ParkingDatabaseMock.h"
#include "CloudService/CloudServiceMock.h"

TEST(VerifyParkingTest, constructorTest){
    spark::VerifyParking testInstance;

    EXPECT_EQ(nullptr,testInstance.getCloudService());
    //EXPECT_EQ(nullptr,testInstance.getDbPointer());

}
