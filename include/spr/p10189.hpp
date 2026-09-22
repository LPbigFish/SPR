#include <algorithm>
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>
#include <ranges>
#include <string>
#include <vector>

namespace spr::p10189 {

inline void solve(std::istream &in, std::ostream &out) {
  // 1st line 2 nums 0 < n, m <= 100
  // 4 2 VALID
  int32_t n{}, m{};

  while (in >> n >> m) {
    if (n == 0 && m == 0) {
      break;
    }

    std::vector<std::string> rows(static_cast<std::size_t>(n));

    for (auto const i : std::views::iota(0, n)) {
      in >> rows[static_cast<std::size_t>(i)];
    }

    std::ranges::for_each(rows, [&out](std::string const &row) { out << row << '\n'; });
  }
}

template <std::size_t X, std::size_t Y>
inline std::array<int8_t, X * Y> mines(std::array<int8_t, X * Y> field) {
  return field;
}
} // namespace spr::p10189