/// @file library.c
/// @brief Implementation of Library.h

#ifndef LIBRARY_H
#define LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "book.h"
#include "author.h"
#include "genre.h"
#include "log.h"

typedef struct {
    Book *books;
    int book_count;
    int book_capacity;

    Author *authors;
    int author_count;
    int author_capacity;

    Genre *genres;
    int genre_count;
    int genre_capacity;

} Library;

// Core Functions

/// @brief Function to Initialize a Library
/// @param lib Library to be initialized
/// @return 0 if Success | 1 if False
int lb_init(Library *lib);

/// @brief Function to free library memory
/// @param lib Library to get freed
void lb_free(Library *lib);

/// @brief Function to clear library data maintaining capacity
/// @param lib Library to get cleared
void lb_clear(Library *lib);

// CRUD Functions
//books

/// @brief Function to add a book to library
/// @param lib Library to add the book
/// @param book Book to be added
/// @return 0 if Success | 1 if False
int lb_add_book(Library *lib, const Book *book);

/// @brief Function to remove a book from library
/// @param lib Library to remove the book
/// @param isbn ISBN of the book to be removed
/// @return 0 if Success | 1 if False
int lb_remove_book(Library *lib, const char *isbn);

//authors

/// @brief Function to add an author to a library
/// @param lib Library to add the author
/// @param author_name Name of the author to add
/// @return 0 if Success | 1 if False
int lb_add_author(Library *lib, const char *author_name);

//genres

/// @brief Function to add a genre to a library
/// @param lib Library to add the Genre
/// @param genre_name Genre to add 
/// @return 0 if Success | 1 if False
int lb_add_genre(Library *lib, const char *genre_name);

//Utils Functions

/// @brief Function to expand capacity of books from a library 
/// @param lib Library to expand the capacity of books
/// @return 0 if Success | 1 if False
int lb_reserve_books(Library *lib);

/// @brief Function to expand capacity of authors from a library
/// @param lib Library to expand the capacity of authors
/// @return 0 if Success | 1 if False
int lb_reserve_authors(Library *lib);

/// @brief Function to expand capacity of genres from a library
/// @param lib Library to expand the capacity of genres
/// @return 0 if Success | 1 if False
int lb_reserve_genres(Library *lib);

#ifdef __cplusplus
}
#endif

#endif