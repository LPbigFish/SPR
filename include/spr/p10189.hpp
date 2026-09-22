#include <algorithm>
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace spr {
namespace p10189 {

inline void solve(std::istream& in, std::ostream& out) {
    // 1st line 2 nums 0 < n, m <= 100
    // 4 2 VALID
    int32_t n{};
    int32_t m{};

    while (in >> n >> m) {
        if (n == 0 && m == 0) {
            break;
        }

        std::vector<std::vector<int8_t>> rows(static_cast<size_t>(n));

        for (std::vector<std::vector<int8_t>>::size_type i = 0; i < rows.size();
             ++i) {
            rows[i].reserve(static_cast<size_t>(m));
            std::string row{};
            in >> row;

            for (std::string::size_type j = 0; j < row.size(); ++j) {
                rows[i].push_back(row[j]);
            }
        }

        for (std::vector<std::vector<int8_t>>::const_iterator row
             = rows.begin();
             row != rows.end();
             ++row) {
            for (std::vector<int8_t>::const_iterator c = row->begin();
                 c != row->end();
                 ++c) {
                out << static_cast<char>(*c) << '\n';
            }
        }
    }
}

inline auto mines(std::vector<std::vector<int8_t>>& field)
    -> std::vector<std::vector<int8_t>> {
    return field;
}
} // namespace p10189
} // namespace spr
