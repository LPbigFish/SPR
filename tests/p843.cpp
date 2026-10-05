#include <gtest/gtest.h>

#include <spr/p843.hpp>

#include <map>
#include <set>
#include <sstream>
#include <string>

namespace {

auto run(const std::string& input) -> std::string {
    std::istringstream in(input);
    std::ostringstream out;

    spr::p843::solve(in, out);

    return out.str();
}

auto is_lowercase_letter(char c) -> bool {
    return c >= 'a' && c <= 'z';
}

auto is_valid_decryption(
    const std::string& encrypted,
    const std::string& decrypted,
    const std::set<std::string>& dictionary
) -> bool {
    if (encrypted.size() != decrypted.size()) {
        return false;
    }

    std::map<char, char> encrypted_to_plain;
    std::map<char, char> plain_to_encrypted;

    for (std::size_t i = 0; i < encrypted.size(); ++i) {
        const char encrypted_char = encrypted.at(i);
        const char decrypted_char = decrypted.at(i);

        if (encrypted_char == ' ') {
            if (decrypted_char != ' ') {
                return false;
            }
            continue;
        }

        if (!is_lowercase_letter(encrypted_char)
            || !is_lowercase_letter(decrypted_char)) {
            return false;
        }

        const auto encrypted_mapping = encrypted_to_plain.find(encrypted_char);
        if (encrypted_mapping != encrypted_to_plain.end()
            && encrypted_mapping->second != decrypted_char) {
            return false;
        }

        const auto plain_mapping = plain_to_encrypted.find(decrypted_char);
        if (plain_mapping != plain_to_encrypted.end()
            && plain_mapping->second != encrypted_char) {
            return false;
        }

        encrypted_to_plain[encrypted_char] = decrypted_char;
        plain_to_encrypted[decrypted_char] = encrypted_char;
    }

    std::istringstream words(decrypted);
    std::string word;
    while (words >> word) {
        if (dictionary.find(word) == dictionary.end()) {
            return false;
        }
    }

    return true;
}

} // namespace

TEST(p843, sampleInput) {
    const std::string input = "6\n"
                              "and\n"
                              "dick\n"
                              "jane\n"
                              "puff\n"
                              "spot\n"
                              "yertle\n"
                              "bjvg xsb hxsn xsb qymm xsb rqat xsb pnetfn\n"
                              "xxxx yyy zzzz www yyyy aaa bbbb ccc dddddd\n";

    const std::string expected
        = "dick and jane and puff and spot and yertle\n"
          "**** *** **** *** **** *** **** *** ******\n";

    EXPECT_EQ(run(input), expected);
}

TEST(p843, appliesOneToOneMappingAcrossAllWords) {
    const std::string input = "2\n"
                              "ab\n"
                              "bc\n"
                              "xy yz\n";

    EXPECT_EQ(run(input), "ab bc\n");
}

TEST(p843, rejectsConflictingMappingsBetweenWords) {
    const std::string input = "2\n"
                              "ab\n"
                              "cd\n"
                              "xy xz\n";

    EXPECT_EQ(run(input), "** **\n");
}

TEST(p843, masksAnImpossibleLineButPreservesItsSpaces) {
    const std::string input = "1\n"
                              "ab\n"
                              "aa  aaa\n";

    EXPECT_EQ(run(input), "**  ***\n");
}

TEST(p843, preservesSpacesAndLineBoundaries) {
    const std::string input = "1\n"
                              "a\n"
                              "a  a\n"
                              "a a\n";

    EXPECT_EQ(run(input), "a  a\n"
                          "a a\n");
}

TEST(p843, startsWithFreshMappingAfterAnImpossibleLine) {
    const std::string input = "1\n"
                              "ab\n"
                              "aaa\n"
                              "xy\n";

    EXPECT_EQ(run(input), "***\n"
                          "ab\n");
}

TEST(p843, acceptsAnyValidDecryptionWhenSeveralExist) {
    const std::string input = "2\n"
                              "no\n"
                              "on\n"
                              "xy yx\n";

    const std::set<std::string> dictionary{"no", "on"};
    std::istringstream output(run(input));
    std::string decrypted;

    ASSERT_TRUE(std::getline(output, decrypted));
    EXPECT_TRUE(is_valid_decryption("xy yx", decrypted, dictionary));
    EXPECT_FALSE(std::getline(output, decrypted));
}

TEST(p843, emptyInputAndDictionaryProduceNoOutput) {
    EXPECT_EQ(run(""), "");
    EXPECT_EQ(run("0\n"), "");
}
