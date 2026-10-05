#include <array>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace spr {
namespace p110 {

struct State {
    std::ostream* out;
    int n;

    State(std::ostream& out, int n): out{&out}, n{n} {}
};

inline auto generate(
    const State& state,
    std::int32_t next_var,
    const std::vector<char>& order,
    const std::string& tabs
) -> void;

inline auto insert_var(
    const State& state,
    std::int32_t next_var,
    const std::vector<char>& order,
    std::int32_t pos,
    const std::string& tabs,
    bool is_else_if = false
) -> void {
    const char c = static_cast<char>('a' + next_var);

    if (is_else_if) {
        *state.out << tabs << "else if "
                   << order.at(static_cast<std::size_t>(pos)) << " < " << c
                   << " then\n";
    } else {
        *state.out << tabs << "if " << order.at(static_cast<std::size_t>(pos))
                   << " < " << c << " then\n";
    }

    std::vector<char> new_order = order;
    new_order.insert(new_order.begin() + pos + 1, c);
    generate(state, next_var + 1, new_order, tabs + "  ");

    if (pos == 0) {
        *state.out << tabs << "else\n";

        new_order = order;
        new_order.insert(new_order.begin(), c);

        generate(state, next_var + 1, new_order, tabs + "  ");
    } else {
        insert_var(state, next_var, order, pos - 1, tabs, true);
    }
}

inline auto generate(
    const State& state,
    std::int32_t next_var,
    const std::vector<char>& order,
    const std::string& tabs
) -> void {
    if (state.n == next_var) {
        *state.out << tabs << "writeln(";

        for (std::size_t i = 0; i < static_cast<std::size_t>(state.n); ++i) {
            if (i) {
                *state.out << ",";
            }
            *state.out << order.at(i);
        }

        *state.out << ")\n";
        return;
    }

    insert_var(
        state,
        next_var,
        order,
        static_cast<std::int32_t>(order.size()) - 1,
        tabs
    );
}

inline auto solve(std::istream& in, std::ostream& out) -> void {
    size_t m{};
    if (!(in >> m)) {
        return;
    }

    for (size_t k = 0; k < m; ++k) {
        int32_t n{};
        if (!(in >> n)) {
            return;
        }

        if (1 > n || n > 8) {
            return;
        }

        out << "program sort(input,output);\n";
        out << "var\n";

        for (int i = 0; i < n; ++i) {
            if (i) {
                out << ",";
            }
            out << static_cast<char>('a' + i);
        }

        out << " : integer;\n";
        out << "begin\n";
        out << "  readln(";

        for (int i = 0; i < n; ++i) {
            if (i) {
                out << ",";
            }
            out << static_cast<char>('a' + i);
        }

        out << ");\n";

        std::vector<char> order;
        order.push_back('a');

        generate(State{out, n}, 1, order, "  ");

        out << "end.\n";

        if (k + 1 < m) {
            out << '\n';
        }
    }
}

} // namespace p110
} // namespace spr
