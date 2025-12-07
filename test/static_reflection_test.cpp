#include <gtest/gtest.h>
#include <sstream>
#include <type_traits>
#include <static_reflection.hpp>

struct EmptyStruct
{
    REFL_STRUCT(EmptyStruct);
};

struct Point
{
    int x;
    int y;
    REFL_STRUCT(Point, x, y);
};

struct StructWithArrayField
{
    int x[3];
    REFL_STRUCT(StructWithArrayField, x);
};

TEST(static_reflection_test, macro_test)
{
    static_assert(REFL_GET_ARGS_COUNT() == 0);
    static_assert(REFL_GET_ARGS_COUNT(a) == 1);
    static_assert(REFL_GET_ARGS_COUNT(a, b, c) == 3);
}

TEST(static_reflection_test, struct_name_test)
{
    EXPECT_STREQ(Refl::struct_name<EmptyStruct>, "EmptyStruct");
    EXPECT_STREQ(Refl::struct_name<Point>, "Point");
    EXPECT_STREQ(Refl::struct_name<StructWithArrayField>, "StructWithArrayField");
}

TEST(static_reflection_test, field_count_test)
{
    static_assert(Refl::field_count<EmptyStruct> == 0);
    static_assert(Refl::field_count<Point> == 2);
    static_assert(Refl::field_count<StructWithArrayField> == 1);
}

TEST(static_reflection_test, field_name_test)
{
    EXPECT_STREQ("x", (Refl::field_name<Point, 0>));
    EXPECT_STREQ("y", (Refl::field_name<Point, 1>));

    EXPECT_STREQ("x", (Refl::field_name<StructWithArrayField, 0>));
}

TEST(static_reflection_test, field_type_test)
{
    static_assert(std::is_same_v<Refl::field_type<Point, 0>, int>);
    static_assert(std::is_same_v<Refl::field_type<Point, 1>, int>);

    static_assert(std::is_same_v<Refl::field_type<StructWithArrayField, 0>, int[3]>);
}

TEST(static_reflection_test, field_ref_test)
{
    Point p = { 1, 1 };
    const Point cp = { 2, 2 };
    Point& pr = p;
    const Point& cpr = cp;

    EXPECT_EQ(Refl::field_ref<0>(p), 1);
    EXPECT_EQ(Refl::field_ref<0>(cp), 2);
    EXPECT_EQ(Refl::field_ref<1>(pr), 1);
    EXPECT_EQ(Refl::field_ref<1>(cpr), 2);

    Refl::field_ref<0>(p) = 3;
    EXPECT_EQ(Refl::field_ref<0>(p), 3);
    EXPECT_EQ(Refl::field_ref<0>(pr), 3);

    Refl::field_ref<1>(pr) = 4;
    EXPECT_EQ(Refl::field_ref<1>(p), 4);
    EXPECT_EQ(Refl::field_ref<1>(pr), 4);

    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(p)), int &>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(pr)), int &>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(cp)), const int &>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(cpr)), const int &>);

    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(Point{1, 1})), int &&>);

    static_assert(std::is_same_v<decltype(Refl::field_ref<1>(std::move(p))), int &&>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<1>(std::move(pr))), int &&>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<1>(std::move(cp))), const int &&>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<1>(std::move(cpr))), const int &&>);

    StructWithArrayField s = {
        {1, 2, 3}
    };
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(s)), int (&)[3]>);

    decltype(auto) x = Refl::field_ref<0>(s);
    EXPECT_EQ(x[0], 1);
    EXPECT_EQ(x[1], 2);
    EXPECT_EQ(x[2], 3);

    x[2] = 4;

    EXPECT_EQ(Refl::field_ref<0>(s)[2], 4);
}

TEST(static_reflection_test, for_each_field_test)
{
    Point p = { 10, 100 };

    std::stringstream ss;
    Refl::for_each_field(p,
        [&ss](std::size_t index, const char* name, auto&& ref) {
            // 可以这么接着往下转发 std::forward<decltype(field_ref)>(field_ref)
            ss << index << " " << name << " " << ref << std::endl;
        }
    );
    std::string result = ss.str();
    std::string expectResult =
    "0 x 10\n"
    "1 y 100\n";

    EXPECT_EQ(result, expectResult);
}

int main(int argc, char **argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}