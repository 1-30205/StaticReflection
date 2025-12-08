#include <gtest/gtest.h>
#include <sstream>
#include <type_traits>
#include <static_reflection.hpp>
#include <compile_time_str.hpp>

struct EmptyStruct
{
    REFL_CLASS(EmptyStruct);
};

struct Point
{
    int x;
    int y;
    REFL_CLASS(Point, x, y);
};

struct StructWithArrayField
{
    int x[3];
    REFL_CLASS(StructWithArrayField, x);
};

class PrivateField
{
public:
    PrivateField(int x) : x(x) {}

private:
    int x;

public:
    REFL_CLASS(PrivateField, x);
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

TEST(static_reflection_test, private_field_test)
{
    static_assert(std::is_same_v<Refl::field_type<PrivateField, 0>, int>);

    PrivateField pf(10);
    EXPECT_EQ(Refl::field_ref<0>(pf), 10);
    Refl::field_ref<0>(pf) = 20;
    EXPECT_EQ(Refl::field_ref<0>(pf), 20);

    PrivateField p(1);
    const PrivateField cp(2);
    PrivateField& pr = p;
    const PrivateField& cpr = cp;

    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(p)), int &>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(pr)), int &>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(cp)), const int &>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(cpr)), const int &>);

    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(Point{1, 1})), int &&>);

    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(std::move(p))), int &&>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(std::move(pr))), int &&>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(std::move(cp))), const int &&>);
    static_assert(std::is_same_v<decltype(Refl::field_ref<0>(std::move(cpr))), const int &&>);
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

TEST(compile_time_str_test, make_compile_string_test)
{
    using namespace Refl::StrUtil;
    static_assert(std::is_same_v<cts_pop_front_t<compile_time_str<'a'>>, empty_cts>);
    static_assert(std::is_same_v<cts_pop_front_t<compile_time_str<'a', 'b'>>, compile_time_str<'b'>>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<>, '0'>, empty_cts>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<>, 'a'>, empty_cts>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'1'>, '0'>, compile_time_str<'1'>>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<'1'>, '0'>, empty_cts>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'1','2','3'>, '0'>, compile_time_str<'1','2','3'>>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<'1','2','3'>, '0'>, empty_cts>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'1','2','3'>, '1'>, empty_cts>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<'1','2','3'>, '1'>, compile_time_str<'2','3'>>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'1','2','3'>, '2'>, compile_time_str<'1'>>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<'1','2','3'>, '2'>, compile_time_str<'3'>>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'1','2','3'>, '3'>, compile_time_str<'1','2'>>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<'1','2','3'>, '3'>, empty_cts>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'1','2','3'>, '4'>, compile_time_str<'1','2','3'>>);
    static_assert(std::is_same_v<cts_split_once_t2<compile_time_str<'1','2','3'>, '4'>, empty_cts>);

    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'h','e','l','l','o'>, '\0'>,
        compile_time_str<'h','e','l','l','o'>>);
    static_assert(std::is_same_v<cts_split_once_t1<compile_time_str<'h','e','l','l','o','\0','\0','\0','\0','\0'>, '\0'>,
        compile_time_str<'h','e','l','l','o'>>);

    static_assert(std::is_same_v<MKCTS(""), empty_cts>);
    static_assert(std::is_same_v<MkCts(""), empty_cts>); // bug

    static_assert(std::is_same_v<MKCTS("h"), compile_time_str<'h'>>);
    static_assert(std::is_same_v<MkCts("h"), compile_time_str<'h'>>);

    static_assert(std::is_same_v<MKCTS("hi"), compile_time_str<'h', 'i'>>);
    static_assert(std::is_same_v<MkCts("hi"), compile_time_str<'h', 'i'>>);

    static_assert(std::is_same_v<MKCTS("she"), compile_time_str<'s','h','e'>>);
    static_assert(std::is_same_v<MkCts("she"), MKCTS("she")>);

    static_assert(std::is_same_v<
        MKCTS("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"),
        compile_time_str<
            '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
            '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
            '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
            '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
        >>);
    static_assert(std::is_same_v<
        MkCts("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"),
        MKCTS("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef")
    >);

    auto helloworld = MkCtsObj("helloworld");
    static_assert(std::is_same_v<decltype(helloworld), compile_time_str<'h','e','l','l','o','w','o','r','l','d'>>);
    auto empty = MkCtsObj("");
    static_assert(std::is_same_v<decltype(empty), compile_time_str<>>);
}

TEST(compile_time_str_test, cts_size_test)
{
    using namespace Refl::StrUtil;
    static_assert(cts_size_v<MKCTS("")> == 0);

    using namespace Refl::StrUtil;
    static_assert(cts_size_v<MKCTS("a")> == 1);
}

int main(int argc, char **argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}