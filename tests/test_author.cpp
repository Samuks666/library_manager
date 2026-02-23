#include <gtest/gtest.h>
#include <cstring>
#include <vector>
#include "../include/core/library.h"

class AuthorTest : public ::testing::Test {
protected:
    Library lib;
    
    void SetUp() override {
        ASSERT_EQ(lb_init(&lib), 0);
    }
    
    void TearDown() override {
        lb_free(&lib);
    }
};

// ========== Author Addition Tests ==========

TEST_F(AuthorTest, AddSingleAuthor) {
    EXPECT_EQ(lb_add_author(&lib, "Isaac Asimov"), 0);
    EXPECT_EQ(lib.author_count, 1);
    EXPECT_STREQ(lib.authors[0].name, "Isaac Asimov");
    EXPECT_EQ(lib.authors[0].id, 1);
}

TEST_F(AuthorTest, AddMultipleAuthors) {
    const char *authors[] = {
        "Isaac Asimov",
        "Agatha Christie",
        "J.K. Rowling",
        "George R.R. Martin",
        "Stephen King"
    };
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(lb_add_author(&lib, authors[i]), 0);
    }
    
    EXPECT_EQ(lib.author_count, 5);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_STREQ(lib.authors[i].name, authors[i]);
        EXPECT_EQ(lib.authors[i].id, i + 1);
    }
}

TEST_F(AuthorTest, AddAuthorNullLibrary) {
    EXPECT_EQ(lb_add_author(nullptr, "Test Author"), 1);
}

TEST_F(AuthorTest, AddAuthorNullName) {
    EXPECT_EQ(lb_add_author(&lib, nullptr), 1);
}

// ========== Author Capacity Tests ==========

TEST_F(AuthorTest, AuthorCapacityExpansion) {
    int initial_capacity = lib.author_capacity;
    
    for (int i = 0; i < initial_capacity + 3; i++) {
        char author_name[100];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        EXPECT_EQ(lb_add_author(&lib, author_name), 0);
    }
    
    EXPECT_GT(lib.author_capacity, initial_capacity);
    EXPECT_EQ(lib.author_count, initial_capacity + 3);
}

TEST_F(AuthorTest, MultipleCapacityExpansions) {
    int initial_capacity = lib.author_capacity;
    
    for (int i = 0; i < initial_capacity * 4; i++) {
        char author_name[100];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        EXPECT_EQ(lb_add_author(&lib, author_name), 0);
    }
    
    EXPECT_EQ(lib.author_count, initial_capacity * 4);
    EXPECT_GE(lib.author_capacity, initial_capacity * 4);
}

// ========== Author ID Assignment Tests ==========

TEST_F(AuthorTest, AuthorIDsAreSequential) {
    for (int i = 0; i < 10; i++) {
        char author_name[100];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        lb_add_author(&lib, author_name);
    }
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(lib.authors[i].id, i + 1);
    }
}

TEST_F(AuthorTest, AuthorIDsAreUnique) {
    for (int i = 0; i < 20; i++) {
        char author_name[100];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        lb_add_author(&lib, author_name);
    }
    
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            EXPECT_NE(lib.authors[i].id, lib.authors[j].id);
        }
    }
}

// ========== Author Name Tests ==========

TEST_F(AuthorTest, AuthorNameStorage) {
    const char *test_name = "Isaac Asimov";
    EXPECT_EQ(lb_add_author(&lib, test_name), 0);
    EXPECT_STREQ(lib.authors[0].name, test_name);
}

TEST_F(AuthorTest, AuthorNameMaxLength) {
    char long_name[256];
    memset(long_name, 'A', 255);
    long_name[255] = '\0';
    
    lb_add_author(&lib, long_name);
    EXPECT_GT(strlen(lib.authors[0].name), 0);
}

TEST_F(AuthorTest, EmptyAuthorName) {
    lb_add_author(&lib, "");
    EXPECT_EQ(lib.author_count, 1);
}

// ========== Author Lookup Tests ==========

TEST_F(AuthorTest, FindAuthorByID) {
    lb_add_author(&lib, "Isaac Asimov");
    lb_add_author(&lib, "Agatha Christie");
    lb_add_author(&lib, "J.K. Rowling");
    
    int found_index = -1;
    for (int i = 0; i < lib.author_count; i++) {
        if (lib.authors[i].id == 2) {
            found_index = i;
            break;
        }
    }
    
    EXPECT_NE(found_index, -1);
    EXPECT_STREQ(lib.authors[found_index].name, "Agatha Christie");
}

TEST_F(AuthorTest, FindAuthorByName) {
    lb_add_author(&lib, "Isaac Asimov");
    lb_add_author(&lib, "Agatha Christie");
    lb_add_author(&lib, "J.K. Rowling");
    
    int found_index = -1;
    for (int i = 0; i < lib.author_count; i++) {
        if (strcmp(lib.authors[i].name, "Agatha Christie") == 0) {
            found_index = i;
            break;
        }
    }
    
    EXPECT_NE(found_index, -1);
    EXPECT_EQ(lib.authors[found_index].id, 2);
}

// ========== Author Clear Tests ==========

TEST_F(AuthorTest, ClearLibraryAuthors) {
    for (int i = 0; i < 5; i++) {
        char author_name[100];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        lb_add_author(&lib, author_name);
    }
    
    EXPECT_EQ(lib.author_count, 5);
    
    lb_clear(&lib);
    EXPECT_EQ(lib.author_count, 0);
}

// ========== Author Update Tests ==========

TEST_F(AuthorTest, UpdateAuthorName) {
    lb_add_author(&lib, "Isaac Asimov");
    
    const char *new_name = "Isaac Asimov (1920-1992)";
    EXPECT_EQ(author_update_name(&lib.authors[0], new_name), 0);
    EXPECT_STREQ(lib.authors[0].name, new_name);
}

TEST_F(AuthorTest, UpdateAuthorNameNullPointer) {
    lb_add_author(&lib, "Isaac Asimov");
    
    EXPECT_EQ(author_update_name(nullptr, "New Name"), 1);
    EXPECT_EQ(author_update_name(&lib.authors[0], nullptr), 1);
}

// ========== Realistic Author Tests ==========

TEST_F(AuthorTest, AddClassicAuthors) {
    std::vector<std::string> classic_authors = {
        "William Shakespeare",
        "Jane Austen",
        "Mark Twain",
        "Charles Dickens",
        "Leo Tolstoy"
    };
    
    for (const auto &author : classic_authors) {
        EXPECT_EQ(lb_add_author(&lib, author.c_str()), 0);
    }
    
    EXPECT_EQ(lib.author_count, 5);
}

TEST_F(AuthorTest, AddModernAuthors) {
    std::vector<std::string> modern_authors = {
        "George R.R. Martin",
        "J.K. Rowling",
        "Stephen King",
        "Neil Gaiman",
        "Patrick Rothfuss"
    };
    
    for (const auto &author : modern_authors) {
        EXPECT_EQ(lb_add_author(&lib, author.c_str()), 0);
    }
    
    EXPECT_EQ(lib.author_count, 5);
    
    // Verify first author
    EXPECT_STREQ(lib.authors[0].name, "George R.R. Martin");
    EXPECT_EQ(lib.authors[0].id, 1);
}

// ========== Stress Tests ==========

TEST_F(AuthorTest, AddManyAuthors) {
    const int num_authors = 1000;
    
    for (int i = 0; i < num_authors; i++) {
        char author_name[100];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        EXPECT_EQ(lb_add_author(&lib, author_name), 0);
    }
    
    EXPECT_EQ(lib.author_count, num_authors);
}

TEST_F(AuthorTest, AuthorDataIntegrity) {
    std::vector<std::string> author_names = {
        "Isaac Asimov",
        "Agatha Christie",
        "J.K. Rowling",
        "George R.R. Martin",
        "Stephen King",
        "Neil Gaiman",
        "Patrick Rothfuss",
        "Brandon Sanderson"
    };
    
    for (const auto &name : author_names) {
        lb_add_author(&lib, name.c_str());
    }
    
    for (size_t i = 0; i < author_names.size(); i++) {
        EXPECT_STREQ(lib.authors[i].name, author_names[i].c_str());
        EXPECT_EQ(lib.authors[i].id, (int)i + 1);
    }
}

// ========== Author-Book Association Tests ==========

TEST_F(AuthorTest, AuthorIDCanBeAssociatedWithBooks) {
    lb_add_author(&lib, "Isaac Asimov");
    
    Author &author = lib.authors[0];
    int author_id = author.id;
    
    EXPECT_EQ(author_id, 1);
    EXPECT_STREQ(author.name, "Isaac Asimov");
}

TEST_F(AuthorTest, MultipleAuthorsCanBeUsed) {
    lb_add_author(&lib, "Author 1");
    lb_add_author(&lib, "Author 2");
    lb_add_author(&lib, "Author 3");
    
    EXPECT_EQ(lib.authors[0].id, 1);
    EXPECT_EQ(lib.authors[1].id, 2);
    EXPECT_EQ(lib.authors[2].id, 3);
}
