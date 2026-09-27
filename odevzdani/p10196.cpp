#include <array>
#include <iostream>
#include <string>
#include <tuple>

namespace {
using Board = std::array<std::string, 8>;
using Cords = std::tuple<int, int>;

constexpr std::array<int, 2> PAWN_CAPTURES{-1, 1};
constexpr std::array<Cords, 8> KNIGH_R_CAPTURES = {
  {{2, 1}, {2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}},
};
constexpr std::array<Cords, 4> BISH_DIR
    = {{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};

constexpr std::array<Cords, 4> ROOK_DIR = {{{1, 0}, {0, 1}, {0, -1}, {-1, 0}}};

inline auto boundcheck(int x, int y) -> bool {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

inline auto in_check(const Board& board, Cords king, bool is_white) -> bool {
    const auto king_r = std::get<0>(king);
    const auto king_c = std::get<1>(king);

    // Pawn
    {
        char pawn = (is_white ? 'P' : 'p');
        int pawn_row = king_r + (is_white ? 1 : -1);
        for (int capture : PAWN_CAPTURES) {
            int pawn_col = king_c + capture;

            if (boundcheck(pawn_row, pawn_col)) {
                auto r = static_cast<std::size_t>(pawn_row);
                auto c = static_cast<std::size_t>(pawn_col);

                if (board.at(r).at(c) == pawn) {
                    return true;
                }
            }
        }
    }

    // Knight
    {
        char knight = (is_white ? 'N' : 'n');
        for (const auto& move : KNIGH_R_CAPTURES) {
            int knight_r = king_r + std::get<0>(move);
            int knight_c = king_c + std::get<1>(move);

            if (boundcheck(knight_r, knight_c)) {
                auto r = static_cast<std::size_t>(knight_r);
                auto c = static_cast<std::size_t>(knight_c);

                if (board.at(r).at(c) == knight) {
                    return true;
                }
            }
        }
    }

    // Bishop + Queen
    {
        char bish = (is_white ? 'B' : 'b');
        char queen = (is_white ? 'Q' : 'q');

        for (const auto& dir : BISH_DIR) {
            int dir_r = std::get<0>(dir);
            int dir_c = std::get<1>(dir);

            int tr = king_r + dir_r;
            int tc = king_c + dir_c;

            while (boundcheck(tr, tc)) {
                auto r = static_cast<std::size_t>(tr);
                auto c = static_cast<std::size_t>(tc);

                char pos = board.at(r).at(c);
                if (pos != '.') {
                    if (pos == bish || pos == queen) {
                        return true;
                    }
                    break;
                }

                tr += dir_r;
                tc += dir_c;
            }
        }
    }

    // Rook + Qeen
    {
        char rook = (is_white ? 'R' : 'r');
        char queen = (is_white ? 'Q' : 'q');

        for (const auto& dir : ROOK_DIR) {
            int dir_r = std::get<0>(dir);
            int dir_c = std::get<1>(dir);

            int tr = king_r + dir_r;
            int tc = king_c + dir_c;

            while (boundcheck(tr, tc)) {
                auto r = static_cast<std::size_t>(tr);
                auto c = static_cast<std::size_t>(tc);

                char pos = board.at(r).at(c);
                if (pos != '.') {
                    if (pos == rook || pos == queen) {
                        return true;
                    }
                    break;
                }

                tr += dir_r;
                tc += dir_c;
            }
        }
    }

    return false;
}

inline auto solve(std::istream& in, std::ostream& out) -> void {
    Board board;
    size_t game_number = 0;

    while (true) {
        bool all_empty = true;
        for (auto& row : board) {
            if (!(in >> row)) {
                return;
            }
            if (row.find_first_not_of('.') != std::string::npos) {
                all_empty = false;
            }
        }

        if (all_empty) {
            return;
        }

        Cords white_king{};
        Cords black_king{};

        // Find kings
        for (std::size_t r = 0; r < 8; ++r) {
            const std::string& row = board.at(r);

            for (std::size_t c = 0; c < 8; ++c) {
                if (row.at(c) == 'K') {
                    white_king = std::make_tuple(r, c);
                } else if (row.at(c) == 'k') {
                    black_king = std::make_tuple(r, c);
                }
            }
        }

        ++game_number;
        if (in_check(board, white_king, false)) {
            out << "Game #" << game_number << ": white king is in check.\n";
        } else if (in_check(board, black_king, true)) {
            out << "Game #" << game_number << ": black king is in check.\n";
        } else {
            out << "Game #" << game_number << ": no king is in check.\n";
        }
    }
}

} // namespace

auto main() -> int {
    solve(std::cin, std::cout);
    return 0;
}
