#include <gtest/gtest.h>

// Test "dymny": sprawdza tylko, czy GoogleTest się kompiluje i uruchamia.
TEST(Smoke, GoogleTestWorks) {
    EXPECT_EQ(2 + 2, 4);
}
