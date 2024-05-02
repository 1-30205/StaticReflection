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

TEST(static_reflection_test, type_name_test)
{
    EXPECT_STREQ(Refl::type_name<EmptyStruct>, "EmptyStruct");
    EXPECT_STREQ(Refl::type_name<Point>, "Point");
    EXPECT_STREQ(Refl::type_name<StructWithArrayField>, "StructWithArrayField");
}

TEST(static_reflection_test, field_count_test)
{
    static_assert(Refl::field_count<EmptyStruct> == 0);
    static_assert(Refl::get_field_count<EmptyStruct>() == 0);

    static_assert(Refl::field_count<Point> == 2);
    static_assert(Refl::get_field_count<Point>() == 2);

    static_assert(Refl::field_count<StructWithArrayField> == 1);
    static_assert(Refl::get_field_count<StructWithArrayField>() == 1);
}

TEST(static_reflection_test, field_name_test)
{
    EXPECT_STREQ("x", (Refl::field<Point, 0>::name()));
    EXPECT_STREQ("y", (Refl::field<Point, 1>::name()));

    EXPECT_STREQ("x", (Refl::get_field_name<Point, 0>()));
    EXPECT_STREQ("x", (Refl::field_name<Point, 0>));

    EXPECT_STREQ("x", (Refl::field_name<StructWithArrayField, 0>));
}

TEST(static_reflection_test, field_type_test)
{
    static_assert(std::is_same_v<Refl::field<Point, 0>::type, int>);
    static_assert(std::is_same_v<Refl::field_t<Point, 0>, int>);

    static_assert(std::is_same_v<Refl::field_t<StructWithArrayField, 0>, int[3]>);
}

TEST(static_reflection_test, field_ref_test)
{
    Point p = {
        .x = 1,
        .y = 1
    };
    const Point cp = {
        .x = 2,
        .y = 2
    };
    Point& pr = p;
    const Point& cpr = cp;

    EXPECT_EQ(Refl::get_field_ref<0>(p), 1);
    EXPECT_EQ(Refl::get_field_ref<0>(cp), 2);
    EXPECT_EQ(Refl::get_field_ref<1>(pr), 1);
    EXPECT_EQ(Refl::get_field_ref<1>(cpr), 2);

    Refl::get_field_ref<0>(p) = 3;
    EXPECT_EQ(Refl::get_field_ref<0>(p), 3);
    EXPECT_EQ(Refl::get_field_ref<0>(pr), 3);

    Refl::get_field_ref<1>(pr) = 4;
    EXPECT_EQ(Refl::get_field_ref<1>(p), 4);
    EXPECT_EQ(Refl::get_field_ref<1>(pr), 4);

    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(p)), int &>);
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(pr)), int &>);
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(cp)), const int &>);
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(cpr)), const int &>);

    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(Point{1, 1})), int &&>);

    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(std::move(p))), int &&>);
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(std::move(pr))), int &&>);
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(std::move(cp))), const int &&>);
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(std::move(cpr))), const int &&>);

    StructWithArrayField s = {
        {1, 2, 3}
    };
    static_assert(std::is_same_v<decltype(Refl::get_field_ref<0>(s)), int (&)[3]>);

    decltype(auto) x = Refl::get_field_ref<0>(s);
    EXPECT_EQ(x[0], 1);
    EXPECT_EQ(x[1], 2);
    EXPECT_EQ(x[2], 3);

    x[2] = 4;

    EXPECT_EQ(Refl::get_field_ref<0>(s)[2], 4);
}

TEST(static_reflection_test, for_each_field_test)
{
    Point p = {
        .x = 10,
        .y = 100
    };

    std::stringstream ss;
    Refl::for_each_field(p,
        [&ss](std::size_t index, const char* name, auto&& field_ref) {
            // 可以这么接着往下转发 std::forward<decltype(field_ref)>(field_ref)
            ss << index << " " << name << " " << field_ref << std::endl;
        }
    );
    std::string result = ss.str();
    std::string expectResult =
    "0 x 10\n"
    "1 y 100\n";

    EXPECT_EQ(result, expectResult);
}