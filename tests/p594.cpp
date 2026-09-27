#include <gtest/gtest.h>

#include <spr/p594.hpp>

#include <cstdint>
#include <limits>
#include <sstream>
#include <string>

namespace {

auto run(const std::string& input) -> std::string {
    std::istringstream in(input);
    std::ostringstream out;

    spr::p594::solve(in, out);

    return out.str();
}

} // namespace

TEST(p594, sampleInput) {
    const std::string input = "123456789\n"
                              "-123456789\n"
                              "1\n"
                              "16777216\n"
                              "20034556\n";

    const std::string expected = "123456789 converts to 365779719\n"
                                 "-123456789 converts to -349002504\n"
                                 "1 converts to 16777216\n"
                                 "16777216 converts to 1\n"
                                 "20034556 converts to -55365375\n";

    EXPECT_EQ(run(input), expected);
}
