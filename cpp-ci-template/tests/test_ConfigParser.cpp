#include <gtest/gtest.h>
#include "ConfigParser.hpp"
#include <fstream>
#include <filesystem>

// ─── parseLine Tests ─────────────────────────────────────────────

TEST(ConfigParserLineTest, ParsesSimpleKeyValue) {
    auto [key, value] = ConfigParser::parseLine("APP_ENV=production");
    EXPECT_EQ(key,   "APP_ENV");
    EXPECT_EQ(value, "production");
}

TEST(ConfigParserLineTest, TrimsWhitespace) {
    auto [key, value] = ConfigParser::parseLine("  DB_HOST  =  localhost  ");
    EXPECT_EQ(key,   "DB_HOST");
    EXPECT_EQ(value, "localhost");
}

TEST(ConfigParserLineTest, SkipsCommentLines) {
    auto [key, value] = ConfigParser::parseLine("# This is a comment");
    EXPECT_TRUE(key.empty());
}

TEST(ConfigParserLineTest, SkipsEmptyLines) {
    auto [key, value] = ConfigParser::parseLine("   ");
    EXPECT_TRUE(key.empty());
}

TEST(ConfigParserLineTest, StripsDoubleQuotes) {
    auto [key, value] = ConfigParser::parseLine("APP_NAME=\"My App\"");
    EXPECT_EQ(value, "My App");
}

TEST(ConfigParserLineTest, StripsSingleQuotes) {
    auto [key, value] = ConfigParser::parseLine("LOG_LEVEL='debug'");
    EXPECT_EQ(value, "debug");
}

TEST(ConfigParserLineTest, StripsInlineComment) {
    auto [key, value] = ConfigParser::parseLine("DB_PORT=5432 # default postgres port");
    EXPECT_EQ(key,   "DB_PORT");
    EXPECT_EQ(value, "5432");
}

TEST(ConfigParserLineTest, HandlesEmptyValue) {
    auto [key, value] = ConfigParser::parseLine("OPTIONAL_KEY=");
    EXPECT_EQ(key,   "OPTIONAL_KEY");
    EXPECT_EQ(value, "");
}

TEST(ConfigParserLineTest, SkipsLineWithNoEquals) {
    auto [key, value] = ConfigParser::parseLine("NOT_A_VALID_LINE");
    EXPECT_TRUE(key.empty());
}

// ─── parseFile Tests ─────────────────────────────────────────────

class ConfigParserFileTest : public ::testing::Test {
protected:
    std::string tmpFile_;

    void SetUp() override {
        tmpFile_ = "/tmp/test_config.conf";
        std::ofstream f(tmpFile_);
        f << "# Test config\n"
          << "APP_ENV=testing\n"
          << "DB_HOST=127.0.0.1\n"
          << "DB_PORT=5432\n"
          << "APP_SECRET=supersecret\n"
          << "\n"
          << "LOG_LEVEL=\"info\"\n";
    }

    void TearDown() override {
        std::filesystem::remove(tmpFile_);
    }
};

TEST_F(ConfigParserFileTest, LoadsCorrectNumberOfKeys) {
    auto cfg = ConfigParser::parseFile(tmpFile_);
    EXPECT_EQ(cfg.size(), 5u);
}

TEST_F(ConfigParserFileTest, LoadsCorrectValues) {
    auto cfg = ConfigParser::parseFile(tmpFile_);
    EXPECT_EQ(cfg.at("APP_ENV"),  "testing");
    EXPECT_EQ(cfg.at("DB_HOST"), "127.0.0.1");
    EXPECT_EQ(cfg.at("DB_PORT"), "5432");
    EXPECT_EQ(cfg.at("LOG_LEVEL"), "info");
}

TEST_F(ConfigParserFileTest, ThrowsOnMissingFile) {
    EXPECT_THROW(
        ConfigParser::parseFile("/nonexistent/path.conf"),
        std::runtime_error
    );
}
