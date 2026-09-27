#include <gtest/gtest.h>

#include <spr/p110.hpp>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace {

auto run(const std::string& input) -> std::string {
    std::istringstream in(input);
    std::ostringstream out;

    spr::p110::solve(in, out);

    return out.str();
}
} // namespace

TEST(p110, sampleInput) {
    const std::string input = "1\n\n3\n";

    const std::string expected = "program sort(input,output);\n"
                                 "var\n"
                                 "a,b,c : integer;\n"
                                 "begin\n"
                                 "  readln(a,b,c);\n"
                                 "  if a < b then\n"
                                 "    if b < c then\n"
                                 "      writeln(a,b,c)\n"
                                 "    else if a < c then\n"
                                 "      writeln(a,c,b)\n"
                                 "    else\n"
                                 "      writeln(c,a,b)\n"
                                 "  else\n"
                                 "    if a < c then\n"
                                 "      writeln(b,a,c)\n"
                                 "    else if b < c then\n"
                                 "      writeln(b,c,a)\n"
                                 "    else\n"
                                 "      writeln(c,b,a)\n"
                                 "end.\n";

    EXPECT_EQ(run(input), expected);
}
