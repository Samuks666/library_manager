#include <gtest/gtest.h>
#include <cstring>
#include <cstdlib>
#include "../include/core/library.h"

class BookTest : public ::testing::Test {
protected:
    Book book;
    
    void SetUp() override {
        book.id = 1;
        strcpy(book.title, "Test Book");
        strcpy(book.isbn, "978-0000000000");
        book.publication_year = 2020;
        strcpy(book.description, "A test book");
        book.genre_ids = (int*)malloc(2 * sizeof(int));
        book.author_ids = (int*)malloc(2 * sizeof(int));
        book.genre_capacity = 2;
        book.author_capacity = 2;
        book.genre_count = 0;
        book.author_count = 0;
    }
    
    void TearDown() override {
        free(book.genre_ids);
        free(book.author_ids);
    }
};

// ========== Book Creation Tests ==========

TEST_F(BookTest, BookInitialization) {
    EXPECT_EQ(book.id, 1);
    EXPECT_STREQ(book.title, "Test Book");
    EXPECT_STREQ(book.isbn, "978-0000000000");
    EXPECT_EQ(book.publication_year, 2020);
    EXPECT_EQ(book.genre_count, 0);
    EXPECT_EQ(book.author_count, 0);
}

// ========== Genre Management Tests ==========

TEST_F(BookTest, AddGenreSuccess) {
    EXPECT_EQ(book_add_genre(&book, 1), 0);
    EXPECT_EQ(book.genre_count, 1);
    EXPECT_EQ(book.genre_ids[0], 1);
}

TEST_F(BookTest, AddMultipleGenres) {
    EXPECT_EQ(book_add_genre(&book, 1), 0);
    EXPECT_EQ(book_add_genre(&book, 2), 0);
    EXPECT_EQ(book.genre_count, 2);
    EXPECT_EQ(book.genre_ids[0], 1);
    EXPECT_EQ(book.genre_ids[1], 2);
}

TEST_F(BookTest, AddGenreInvalidID) {
    EXPECT_EQ(book_add_genre(&book, -1), 1);
    EXPECT_EQ(book_add_genre(&book, 0), 1);
}

TEST_F(BookTest, AddGenreCapacityExpansion) {
    EXPECT_EQ(book_add_genre(&book, 1), 0);
    EXPECT_EQ(book_add_genre(&book, 2), 0);
    
    int old_capacity = book.genre_capacity;
    EXPECT_EQ(book_add_genre(&book, 3), 0);
    
    EXPECT_GT(book.genre_capacity, old_capacity);
    EXPECT_EQ(book.genre_count, 3);
}

TEST_F(BookTest, RemoveGenreSuccess) {
    book_add_genre(&book, 1);
    book_add_genre(&book, 2);
    
    EXPECT_EQ(book_remove_genre(&book, 1), 0);
    EXPECT_EQ(book.genre_count, 1);
    EXPECT_EQ(book.genre_ids[0], 2);
}

TEST_F(BookTest, RemoveGenreNotFound) {
    book_add_genre(&book, 1);
    EXPECT_EQ(book_remove_genre(&book, 999), 1);
}

TEST_F(BookTest, RemoveGenreFromEmpty) {
    EXPECT_EQ(book_remove_genre(&book, 1), 1);
}

TEST_F(BookTest, AddGenreNullPointer) {
    EXPECT_EQ(book_add_genre(nullptr, 1), 1);
}

// ========== Author Management Tests ==========

TEST_F(BookTest, AddAuthorSuccess) {
    EXPECT_EQ(book_add_author(&book, 1), 0);
    EXPECT_EQ(book.author_count, 1);
    EXPECT_EQ(book.author_ids[0], 1);
}

TEST_F(BookTest, AddMultipleAuthors) {
    EXPECT_EQ(book_add_author(&book, 1), 0);
    EXPECT_EQ(book_add_author(&book, 2), 0);
    EXPECT_EQ(book.author_count, 2);
    EXPECT_EQ(book.author_ids[0], 1);
    EXPECT_EQ(book.author_ids[1], 2);
}

TEST_F(BookTest, AddAuthorInvalidID) {
    EXPECT_EQ(book_add_author(&book, -1), 1);
    EXPECT_EQ(book_add_author(&book, 0), 1);
}

TEST_F(BookTest, AddAuthorCapacityExpansion) {
    EXPECT_EQ(book_add_author(&book, 1), 0);
    EXPECT_EQ(book_add_author(&book, 2), 0);
    
    int old_capacity = book.author_capacity;
    EXPECT_EQ(book_add_author(&book, 3), 0);
    
    EXPECT_GT(book.author_capacity, old_capacity);
    EXPECT_EQ(book.author_count, 3);
}

TEST_F(BookTest, RemoveAuthorSuccess) {
    book_add_author(&book, 1);
    book_add_author(&book, 2);
    
    EXPECT_EQ(book_remove_author(&book, 1), 0);
    EXPECT_EQ(book.author_count, 1);
    EXPECT_EQ(book.author_ids[0], 2);
}

TEST_F(BookTest, RemoveAuthorNotFound) {
    book_add_author(&book, 1);
    EXPECT_EQ(book_remove_author(&book, 999), 1);
}

TEST_F(BookTest, RemoveAuthorFromEmpty) {
    EXPECT_EQ(book_remove_author(&book, 1), 1);
}

TEST_F(BookTest, AddAuthorNullPointer) {
    EXPECT_EQ(book_add_author(nullptr, 1), 1);
}

// ========== Book Update Tests ==========

TEST_F(BookTest, UpdateTitleSuccess) {
    EXPECT_EQ(book_update_title(&book, "New Title"), 0);
    EXPECT_STREQ(book.title, "New Title");
}

TEST_F(BookTest, UpdateTitleNullPointer) {
    EXPECT_EQ(book_update_title(nullptr, "New Title"), 1);
    EXPECT_EQ(book_update_title(&book, nullptr), 1);
}

TEST_F(BookTest, UpdateISBNSuccess) {
    EXPECT_EQ(book_update_isbn(&book, "978-1111111111"), 0);
    EXPECT_STREQ(book.isbn, "978-1111111111");
}

TEST_F(BookTest, UpdateISBNNullPointer) {
    EXPECT_EQ(book_update_isbn(nullptr, "978-1111111111"), 1);
    EXPECT_EQ(book_update_isbn(&book, nullptr), 1);
}

TEST_F(BookTest, UpdatePublicationYearSuccess) {
    EXPECT_EQ(book_update_publication_year(&book, 2021), 0);
}

TEST_F(BookTest, UpdatePublicationYearInvalid) {
    EXPECT_EQ(book_update_publication_year(&book, -1), 1);
    EXPECT_EQ(book_update_publication_year(&book, 0), 1);
}

TEST_F(BookTest, UpdatePublicationYearNullPointer) {
    EXPECT_EQ(book_update_publication_year(nullptr, 2021), 1);
}

TEST_F(BookTest, UpdateDescriptionSuccess) {
    EXPECT_EQ(book_update_description(&book, "New description"), 0);
    EXPECT_STREQ(book.description, "New description");
}

TEST_F(BookTest, UpdateDescriptionNullPointer) {
    EXPECT_EQ(book_update_description(nullptr, "New description"), 1);
    EXPECT_EQ(book_update_description(&book, nullptr), 1);
}

// ========== Getter Tests ==========

TEST_F(BookTest, GetGenreIDs) {
    book_add_genre(&book, 1);
    book_add_genre(&book, 2);
    
    int *genre_ids = book_get_genre_ids(&book);
    EXPECT_EQ(genre_ids[0], 1);
    EXPECT_EQ(genre_ids[1], 2);
}

TEST_F(BookTest, GetAuthorIDs) {
    book_add_author(&book, 1);
    book_add_author(&book, 2);
    
    int *author_ids = book_get_author_ids(&book);
    EXPECT_EQ(author_ids[0], 1);
    EXPECT_EQ(author_ids[1], 2);
}

// ========== Complex Workflow Tests ==========

TEST_F(BookTest, CompleteBookWorkflow) {
    // Add genres
    EXPECT_EQ(book_add_genre(&book, 1), 0);
    EXPECT_EQ(book_add_genre(&book, 2), 0);
    
    // Add authors
    EXPECT_EQ(book_add_author(&book, 1), 0);
    EXPECT_EQ(book_add_author(&book, 2), 0);
    
    // Update info
    EXPECT_EQ(book_update_title(&book, "Updated Title"), 0);
    
    // Remove one of each
    EXPECT_EQ(book_remove_genre(&book, 1), 0);
    EXPECT_EQ(book_remove_author(&book, 2), 0);
    
    // Verify final state
    EXPECT_EQ(book.genre_count, 1);
    EXPECT_EQ(book.author_count, 1);
    EXPECT_STREQ(book.title, "Updated Title");
}
