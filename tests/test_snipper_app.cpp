#include <gtest/gtest.h>
#include <core/SnippetStore.h>
#include <models/Snippet.h>

#include <fstream>
#include <cstdio>
#include <cstdlib>

#ifndef DEFAULT_PROJECT_ROOT
#error "DEFAULT_PROJECT_ROOT must be defined!"
#endif

namespace {
std::string getTestDbPath() {
    return std::string(DEFAULT_PROJECT_ROOT) + "/data/test-snippets.db";
}

void cleanTestDb() {
    std::remove(getTestDbPath().c_str());
}
}  // namespace

class SnipperAppTest : public ::testing::Test {
protected:
    core::SnippetStore* store;

    void SetUp() override {
        cleanTestDb();
        store = new core::SnippetStore(getTestDbPath());
    }

    void TearDown() override {
        delete store;
        cleanTestDb();
    }

    model::Snippet createTestSnippet(const std::string& id,
                                     const std::string& title,
                                     const std::string& content,
                                     const std::vector<std::string>& tags) {
        model::Snippet s;
        s.id = id;
        s.title = title;
        s.content = content;
        s.tags = tags;
        s.createdAt = "2023-01-01T00:00:00Z";
        s.updatedAt = "2023-01-01T00:00:00Z";
        return s;
    }
};

TEST_F(SnipperAppTest, AddAndGetSnippetById) {
    auto s = createTestSnippet("100", "Test Snippet", "some code", {"cpp", "test"});
    store->addSnippet(s);

    auto fetched = store->getSnippetById("100");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->title, "Test Snippet");
    EXPECT_EQ(fetched->tags.size(), 2);
    EXPECT_EQ(fetched->tags[0], "cpp");
}

TEST_F(SnipperAppTest, RemoveSnippet) {
    auto s = createTestSnippet("200", "To Remove", "rm code", {});
    store->addSnippet(s);
    EXPECT_TRUE(store->getSnippetById("200").has_value());

    store->removeSnippet("200");
    EXPECT_FALSE(store->getSnippetById("200").has_value());
}

TEST_F(SnipperAppTest, RenameSnippet) {
    auto s = createTestSnippet("300", "RenameMe", "code", {});
    store->addSnippet(s);

    store->renameSnippet("300", "301");
    auto renamed = store->getSnippetById("301");
    auto old = store->getSnippetById("300");

    EXPECT_TRUE(renamed.has_value());
    EXPECT_FALSE(old.has_value());
    EXPECT_EQ(renamed->id, "301");
    EXPECT_EQ(renamed->title, "RenameMe");
}

TEST_F(SnipperAppTest, GetAllSnippets) {
    store->addSnippet(createTestSnippet("a", "1", "x", {}));
    store->addSnippet(createTestSnippet("b", "2", "y", {}));
    auto all = store->getAllSnippets();
    EXPECT_EQ(all.size(), 2);
}

TEST_F(SnipperAppTest, ClearSnippets) {
    store->addSnippet(createTestSnippet("x1", "ClearMe", "z", {}));
    EXPECT_EQ(store->getAllSnippets().size(), 1);

    store->clearAll();
    EXPECT_EQ(store->getAllSnippets().size(), 0);
}
