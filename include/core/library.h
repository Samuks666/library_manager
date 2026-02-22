#ifndef LIBRARY_H
#define LIBRARY_H

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
void lb_free(Library *lib);
void lb_clear(Library *lib);

// CRUD Functions
//books
int lb_add_book(Library *lib, const Book *book);
int lb_remove_book(Library *lib, const char *isbn);

//authors
int lb_add_author(Library *lib, const char *author_name);

//genres
int lb_add_genre(Library *lib, const char *genre_name);

//Utils
int lb_reserve_books(Library *lib);
int lb_reserve_authors(Library *lib);

#endif