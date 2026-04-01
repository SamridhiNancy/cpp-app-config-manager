#include <gtest/gtest.h>
#include "ConfigManager.hpp"
#include <fstream>
#include <filesystem>

class ConfigManagerTest : public ::testing::Test {
protected:
    ConfigManager cfg_;
    std::string tmpFile_;

    void SetUp() override {
        tmpFile_ = "/tmp/test_manager.conf";
        std::ofstream f(tmpFile_);
        f << "APP_ENV=development\n"
          << "DB_HOST=localhost\n"
          << "DB_PORT=5432\n"
          << "APP_SECRET=mysecret\n";

        cfg_.loadFromFile(tmpFile_);
    }

    void TearDown() override {
        std::filesystem::remove(tmpFile_);
    }
};

// ─── get() Tests ──────────────────────────────────────────────────

TEST_F(ConfigManagerTest, GetReturnsCorrectValue) {
    EXPECT_EQ(cfg_.get("APP_ENV"), "development");
    EXPECT_EQ(cfg_.get("DB_HOST"), "localhost");
}

TEST_F(ConfigManagerTest, GetThrowsOnMissingKey) {
    EXPECT_THROW(cfg_.get("NONEXISTENT_KEY"), std::runtime_error);
}

// ─── getOptional() Tests ──────────────────────────────────────────

TEST_F(ConfigManagerTest, GetOptionalReturnsValueIfExists) {
    auto val = cfg_.getOptional("DB_PORT");
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val, "5432");
}

TEST_F(ConfigManagerTest, GetOptionalReturnsNulloptIfMissing) {
    auto val = cfg_.getOptional("MISSING_KEY");
    EXPECT_FALSE(val.has_value());
}

// ─── getOrDefault() Tests ─────────────────────────────────────────

TEST_F(ConfigManagerTest, GetOrDefaultReturnsValueIfExists) {
    EXPECT_EQ(cfg_.getOrDefault("APP_ENV", "production"), "development");
}

TEST_F(ConfigManagerTest, GetOrDefaultReturnsFallbackIfMissing) {
    EXPECT_EQ(cfg_.getOrDefault("LOG_LEVEL", "INFO"), "INFO");
}

// ─── has() Tests ──────────────────────────────────────────────────

TEST_F(ConfigManagerTest, HasReturnsTrueForExistingKey) {
    EXPECT_TRUE(cfg_.has("DB_HOST"));
}

TEST_F(ConfigManagerTest, HasReturnsFalseForMissingKey) {
    EXPECT_FALSE(cfg_.has("NONEXISTENT"));
}

// ─── set() Tests ──────────────────────────────────────────────────

TEST_F(ConfigManagerTest, SetAddsNewKey) {
    cfg_.set("NEW_KEY", "new_value");
    EXPECT_EQ(cfg_.get("NEW_KEY"), "new_value");
}

TEST_F(ConfigManagerTest, SetOverridesExistingKey) {
    cfg_.set("APP_ENV", "staging");
    EXPECT_EQ(cfg_.get("APP_ENV"), "staging");
}

// ─── size() Tests ─────────────────────────────────────────────────

TEST_F(ConfigManagerTest, SizeReturnsCorrectCount) {
    EXPECT_EQ(cfg_.size(), 4u);
}

TEST_F(ConfigManagerTest, SizeIncrementsOnSet) {
    cfg_.set("BRAND_NEW_KEY", "value");
    EXPECT_EQ(cfg_.size(), 5u);
}

// ─── dump() smoke test ────────────────────────────────────────────

TEST_F(ConfigManagerTest, DumpDoesNotThrow) {
    EXPECT_NO_THROW(cfg_.dump());
}
