#include <unity.h>
#include <polygon.h>

void test_polygon_creation()
{
    Polygon poly{{0,0},{1,0},{0,1}};
}

void test_polygon_drawing()
{
    Polygon poly{{0,0},{1,0},{0,1}};
}

int main()
{
    UNITY_BEGIN();
    // RUN_TEST(test_polygon_creation);
    UNITY_END();
}