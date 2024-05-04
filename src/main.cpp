#include <iostream>
#include <static_reflection.hpp>
#include <type_traits>
#include <utility>

struct Color
{
    uint16_t red {0};
    uint16_t green {0};
    uint16_t blue {0};
    REFL_STRUCT(Color, red, green, blue);
};

struct NestedColor
{
    Color color;
    int x {0};
    REFL_STRUCT(NestedColor, color, x);
};

template<typename T>
void DumpObj(T&& obj, const char* fieldName = "", int depth = 0) {
    auto indent = [depth] {
        for (int i = 0; i < depth; ++i) {
            std::cout << "    ";
        }
    };

    if constexpr(Refl::is_reflected_v<std::decay_t<T>>) { // (1)
        indent();
        std::cout << fieldName << (*fieldName ? ": {" : "{") << std::endl;
        Refl::for_each_field(obj, [depth](std::size_t, const char* fieldName, auto&& value) {
            DumpObj(value, fieldName, depth + 1);
        });
        indent();
        std::cout << "}" << (depth == 0 ? "" : ",") << std::endl;
    } else { // (2)
        indent();
        std::cout << fieldName << ": " << obj << "," << std::endl;
    }
}

int main(int argc, const char* argv[])
{
    std::cout << "hello cpp static reflection" << std::endl;
    std::cout << std::endl;

    Color color = {
        0,
        127,
        255
    };
    NestedColor ncolor = {
        { 0, 127, 255 },
        185
    };
    DumpObj(ncolor);
    return 0;
}