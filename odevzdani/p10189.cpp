#include <iostream>
#include <tuple>
#include <vector>

using Bomb = std::tuple<std::size_t, std::size_t>;
using Field = std::vector<std::vector<char>>;

namespace {

inline auto mine(Field& field, const Bomb& bomb) -> void {
    const auto x = std::get<0>(bomb);
    const auto y = std::get<1>(bomb);

    const auto x0 = std::max<size_t>(0, x > 0 ? x - 1 : 0);
    const auto x1 = std::min(field.front().size(), x + 2);
    const auto y0 = std::max<size_t>(0, y > 0 ? y - 1 : 0);
    const auto y1 = std::min(field.size(), y + 2);

    for (std::size_t yy = y0; yy < y1; ++yy) {
        for (std::size_t xx = x0; xx < x1; ++xx) {
            if (xx == x && yy == y) {
                continue;
            }
            if (field.at(yy).at(xx) != '*') {
                field.at(yy).at(xx)++;
            }
        }
    }
}

inline auto solve(std::istream& in, std::ostream& out) -> void {
    // 1st line 2 nums 0 < n, m <= 100
    // 4 2 VALID
    std::size_t n{};
    std::size_t m{};
    std::size_t field_n{};

    while (in >> n >> m) {
        if (n == 0 && m == 0) {
            break;
        }
        ++field_n;

        if (field_n > 1) {
            out << '\n';
        }

        Field field(n);
        std::vector<Bomb> bombs{};

        for (std::size_t y = 0; y < n; ++y) {
            auto& row = field.at(y);
            std::string s_row{};
            in >> s_row;

            for (std::size_t x = 0; x < m; ++x) {
                char filed = s_row.at(x);
                if (filed == '*') {
                    bombs.emplace_back(x, y);
                    row.emplace_back('*');
                } else {
                    row.emplace_back('0');
                }
            }
        }

        for (const Bomb& bomb : bombs) {
            mine(field, bomb);
        }

        out << "Field #" << field_n << ":\n";
        for (auto& row : field) {
            for (const auto& c : row) {
                out << c;
            }
            out << '\n';
        }
    }
}

}; // namespace

auto main() -> int {
    solve(std::cin, std::cout);
    return 0;
}
