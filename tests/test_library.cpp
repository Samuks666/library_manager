#include <gtest/gtest.h>
#include <cstring>
#include "../include/core/library.h"

class LibraryTest : public ::testing::Test {
protected:
    Library lib;
    
    void SetUp() override {
        ASSERT_EQ(lb_init(&lib), 0);
    }
    
    void TearDown() override {
        lb_free(&lib);
    }
};

// ========== Initialization Tests ==========

TEST_F(LibraryTest, InitializationSuccess) {
    EXPECT_EQ(lib.book_count, 0);
    EXPECT_EQ(lib.author_count, 0);
    EXPECT_EQ(lib.genre_count, 0);
    EXPECT_GT(lib.book_capacity, 0);
    EXPECT_GT(lib.author_capacity, 0);
    EXPECT_GT(lib.genre_capacity, 0);
}

TEST(LibraryInitTest, NullPointerHandling) {
    EXPECT_EQ(lb_init(nullptr), 1);
}

// ========== Genre Tests ==========

TEST_F(LibraryTest, AddGenreSuccess) {
    lb_add_genre(&lib, "Ficção Científica");
    EXPECT_EQ(lib.genre_count, 1);
    EXPECT_STREQ(lib.genres[0].name, "Ficção Científica");
    EXPECT_EQ(lib.genres[0].id, 1);
}

TEST_F(LibraryTest, AddMultipleGenres) {
    lb_add_genre(&lib, "Ficção Científica");
    lb_add_genre(&lib, "Mistério");
    lb_add_genre(&lib, "Romance");
    
    EXPECT_EQ(lib.genre_count, 3);
    EXPECT_STREQ(lib.genres[0].name, "Ficção Científica");
    EXPECT_STREQ(lib.genres[1].name, "Mistério");
    EXPECT_STREQ(lib.genres[2].name, "Romance");
}

TEST_F(LibraryTest, GenreCapacityExpansion) {
    int initial_capacity = lib.genre_capacity;
    
    for (int i = 0; i < initial_capacity + 2; i++) {
        char genre_name[50];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        lb_add_genre(&lib, genre_name);
    }
    
    EXPECT_GT(lib.genre_capacity, initial_capacity);
    EXPECT_EQ(lib.genre_count, initial_capacity + 2);
}

TEST_F(LibraryTest, AddGenreNullPointer) {
    EXPECT_EQ(lb_add_genre(nullptr, "Test"), 1);
    EXPECT_EQ(lb_add_genre(&lib, nullptr), 1);
}

// ========== Author Tests ==========

TEST_F(LibraryTest, AddAuthorSuccess) {
    lb_add_author(&lib, "Isaac Asimov");
    EXPECT_EQ(lib.author_count, 1);
    EXPECT_STREQ(lib.authors[0].name, "Isaac Asimov");
    EXPECT_EQ(lib.authors[0].id, 1);
}

TEST_F(LibraryTest, AddMultipleAuthors) {
    lb_add_author(&lib, "Isaac Asimov");
    lb_add_author(&lib, "Agatha Christie");
    lb_add_author(&lib, "J.K. Rowling");
    
    EXPECT_EQ(lib.author_count, 3);
    EXPECT_STREQ(lib.authors[0].name, "Isaac Asimov");
    EXPECT_STREQ(lib.authors[1].name, "Agatha Christie");
    EXPECT_STREQ(lib.authors[2].name, "J.K. Rowling");
}

TEST_F(LibraryTest, AuthorCapacityExpansion) {
    int initial_capacity = lib.author_capacity;
    
    for (int i = 0; i < initial_capacity + 2; i++) {
        char author_name[50];
        snprintf(author_name, sizeof(author_name), "Author_%d", i);
        lb_add_author(&lib, author_name);
    }
    
    EXPECT_GT(lib.author_capacity, initial_capacity);
    EXPECT_EQ(lib.author_count, initial_capacity + 2);
}

TEST_F(LibraryTest, AddAuthorNullPointer) {
    EXPECT_EQ(lb_add_author(nullptr, "Test"), 1);
    EXPECT_EQ(lb_add_author(&lib, nullptr), 1);
}

// ========== Book Tests ==========

TEST_F(LibraryTest, AddBookSuccess) {
    Book book;
    book.id = 1;
    strcpy(book.title, "Foundation");
    strcpy(book.isbn, "978-8532511010");
    book.publication_year = 1951;
    strcpy(book.description, "A masterpiece of science fiction");
    book.genre_ids = nullptr;
    book.author_ids = nullptr;
    book.genre_count = 0;
    book.author_count = 0;
    book.genre_capacity = 0;
    book.author_capacity = 0;
    
    EXPECT_EQ(lb_add_book(&lib, &book), 0);
    EXPECT_EQ(lib.book_count, 1);
    EXPECT_STREQ(lib.books[0].title, "Foundation");
    EXPECT_STREQ(lib.books[0].isbn, "978-8532511010");
}

TEST_F(LibraryTest, AddBookNullPointer) {
    EXPECT_EQ(lb_add_book(nullptr, nullptr), 1);
}

TEST_F(LibraryTest, AddBookInvalidISBN) {
    Book book;
    book.isbn[0] = '\0';
    EXPECT_EQ(lb_add_book(&lib, &book), 1);
}

TEST_F(LibraryTest, RemoveBookSuccess) {
    lb_add_genre(&lib, "Sci-Fi");
    
    Book book;
    book.id = 1;
    strcpy(book.title, "Foundation");
    strcpy(book.isbn, "978-8532511010");
    book.publication_year = 1951;
    strcpy(book.description, "Test");
    book.genre_ids = (int*)malloc(2 * sizeof(int));
    book.author_ids = (int*)malloc(2 * sizeof(int));
    book.genre_capacity = 2;
    book.author_capacity = 2;
    book.genre_count = 0;
    book.author_count = 0;
    
    lb_add_book(&lib, &book);
    EXPECT_EQ(lib.book_count, 1);
    
    EXPECT_EQ(lb_remove_book(&lib, "978-8532511010"), 0);
    EXPECT_EQ(lib.book_count, 0);
}

TEST_F(LibraryTest, RemoveBookNotFound) {
    EXPECT_EQ(lb_remove_book(&lib, "nonexistent-isbn"), 1);
}

TEST_F(LibraryTest, BookCapacityExpansion) {
    int initial_capacity = lib.book_capacity;
    
    for (int i = 0; i < initial_capacity + 2; i++) {
        Book book;
        book.id = i + 1;
        snprintf(book.title, sizeof(book.title), "Book_%d", i);
        snprintf(book.isbn, sizeof(book.isbn), "978-%d", 1000000 + i);
        book.publication_year = 2020 + i;
        strcpy(book.description, "Test book");
        book.genre_ids = nullptr;
        book.author_ids = nullptr;
        book.genre_count = 0;
        book.author_count = 0;
        book.genre_capacity = 0;
        book.author_capacity = 0;
        
        lb_add_book(&lib, &book);
    }
    
    EXPECT_GT(lib.book_capacity, initial_capacity);
    EXPECT_EQ(lib.book_count, initial_capacity + 2);
}

// ========== Library State Tests ==========

TEST_F(LibraryTest, CompleteWorkflow) {
    // Add genres
    lb_add_genre(&lib, "Ficção Científica");
    lb_add_genre(&lib, "Mistério");
    
    // Add authors
    lb_add_author(&lib, "Isaac Asimov");
    lb_add_author(&lib, "Agatha Christie");
    
    // Add book
    Book book;
    book.id = 1;
    strcpy(book.title, "Foundation");
    strcpy(book.isbn, "978-8532511010");
    book.publication_year = 1951;
    strcpy(book.description, "Masterpiece");
    book.genre_ids = (int*)malloc(2 * sizeof(int));
    book.author_ids = (int*)malloc(2 * sizeof(int));
    book.genre_capacity = 2;
    book.author_capacity = 2;
    book.genre_count = 0;
    book.author_count = 0;
    
    lb_add_book(&lib, &book);
    
    // Verify state
    EXPECT_EQ(lib.genre_count, 2);
    EXPECT_EQ(lib.author_count, 2);
    EXPECT_EQ(lib.book_count, 1);
    
    free(book.genre_ids);
    free(book.author_ids);
}

// ========== Memory Management Tests ==========

TEST(LibraryMemoryTest, InitAndFree) {
    Library lib;
    EXPECT_EQ(lb_init(&lib), 0);
    
    // Add some data
    lb_add_genre(&lib, "Test");
    lb_add_author(&lib, "Test Author");
    
    // Free should not crash
    lb_free(&lib);
    
    // State after free
    EXPECT_EQ(lib.book_count, 0);
    EXPECT_EQ(lib.author_count, 0);
    EXPECT_EQ(lib.genre_count, 0);
}
