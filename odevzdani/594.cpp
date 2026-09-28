#include <cstdint>
#include <iostream>

namespace {
inline auto solve(std::istream& in, std::ostream& out) -> void {
    int32_t number{};

    while (in >> number) {
        auto bits{static_cast<std::uint32_t>(number)};

        std::uint32_t endian
            = ((bits & 0x000000FF) << 24) | ((bits & 0x0000FF00) << 8)
            | ((bits & 0x00FF0000) >> 8) | ((bits & 0xFF000000) >> 24);

        std::int64_t result{endian};
        if (result >= (std::int64_t{1} << 31)) {
            result -= (std::int64_t{1} << 32);
        }

        out << number << " converts to " << result << '\n';
    }
}
} // namespace

auto main() -> int {
    solve(std::cin, std::cout);
    return 0;
}
