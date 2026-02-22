#include <stdlib.h>
#include <stdio.h>
#include "library.h"

#include <string.h>

// Core Functions

int lb_init(Library *lib) {
    if (!lib){
        LOG_ERROR(NULL_ERROR);
        return 1;
    } 

    memset(lib, 0, sizeof(*lib));

    lib->book_capacity = 2;
    lib->author_capacity = 2;

    lib->books = malloc(lib->book_capacity * sizeof(Book));

    if (!lib->books) {
        LOG_ERROR(ALLOCATION_ERROR);
        return 1;
    }

    lib->authors = malloc(lib->author_capacity * sizeof(Author));
    if (!lib->authors) {
        free(lib->books);
        memset(lib, 0, sizeof(*lib));
        LOG_ERROR(ALLOCATION_ERROR);
        return 1;
    }

    lib->genres = malloc(lib->genre_capacity * sizeof(Genre));
    if (!lib->genres) {
        free(lib->books);
        free(lib->authors);
        memset(lib, 0, sizeof(*lib));
        LOG_ERROR(ALLOCATION_ERROR);
        return 1;
    }

    LOG_INFO("Init Successfully");
    return 0;
}

void lb_free(Library *lib) {
    if(!lib) {
        LOG_ERROR(NULL_ERROR);
        return;
    }

    free(lib->books);
    lib->books = NULL;

    free(lib->authors);
    lib->authors = NULL;

    free(lib->genres);
    lib->genres = NULL;

    lib->book_count = 0;
    lib->book_capacity = 0;
    
    lib->author_count = 0;
    lib->author_capacity = 0;

    lib->genre_count = 0;
    lib->genre_capacity = 0;

    memset(lib, 0, sizeof(*lib));

    LOG_INFO("Freed library");
}

void lb_clear(Library *lib) {
    if(!lib) {
        LOG_ERROR(NULL_ERROR);
        return;
    }

    free(lib->books);
    lib->books = NULL;

    free(lib->authors);
    lib->authors = NULL;

    free(lib->genres);
    lib->genres = NULL;

    lib->book_count = 0;
    lib->author_count = 0;
    lib->genre_count = 0;

    LOG_INFO("Library Cleared");
}

// CRUD Functions

//books
int lb_add_book(Library *lib, const Book *book) {
    if (!lib || !book) {
        LOG_ERROR("Null Library or Null Book");
        return 1;
    }

    if (book->isbn[0] == '\0'){
        LOG_ERROR("Invalid ISBN");
        return 1;
    }

    if (!lb_reserve_books(lib)) {
        LOG_ERROR("Expand Capacity");
        return 1;
    }

    lib->books[lib->book_count] = *book;
    lib->book_count++;
    LOG_INFO("Book Added - %s", book->isbn);

    return 0;
}

int lb_remove_book(Library *lib, const char *isbn){
    if (!lib || !isbn) {
        LOG_ERROR("Null Library or Null ISBN");
        return 1;
    }

    int rm_index = -1;
    for (int i = 0; i < lib->book_count; i++) {
        if (strcmp(lib->books[i].isbn, isbn) == 0) {
            rm_index = i;
            break;
        }
    }

    if (rm_index < 0) {
        LOG_ERROR("Book to be removed not Found");
        return 1;
    }

    for (int i = rm_index; i < lib->book_count - 1; i++) {
        lib->books[i] = lib->books[i + 1];
    }

    lib->book_count--;
    memset(&lib->books[lib->book_count], 0, sizeof(Book));

    LOG_INFO("Book Removed - %s", isbn);

    return 0;
}

//authors
int lb_add_author(Library *lib, const char *author_name) {
    if (!lib || !author_name) {
        LOG_ERROR("Null Library or Author Name");
        return 1;
    }

    if (!lb_reserve_authors(lib)) {
        LOG_ERROR("Expand Capacity");
        return 1;
    }

    int index = lib->author_count;
    lib->authors[index].id = index + 1;
    strncpy(lib->authors[index].name,author_name,MAX_AUTHOR_NAME - 1);
    lib->authors[index].name[MAX_AUTHOR_NAME - 1] = '\0';

    lib->author_count++;

    LOG_INFO("Author added: %s", author_name);
    return 0;
}

//genres
int lb_add_genre(Library *lib, const char *genre_name) {
    if (!lib || !genre_name) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    if (!lb_reserve_authors(lib)) {
        LOG_ERROR(EXPAND_CAPACITY);
        return 1;
    }

    int index = lib->genre_count;
    lib->genres[index].id = index + 1;
    strncpy(lib->genres[index].name,genre_name,MAX_GENRE - 1);
    lib->genres[index].name[MAX_GENRE - 1] = '\0';

    lib->genre_count++;

    LOG_INFO("Genre added: %s", genre_name);
    return 0;
}

// Utils
int lb_reserve_books(Library *lib) {
    if (lib->book_count >= lib->book_capacity) {
        int new_cap = lib->book_capacity ? lib->book_capacity * 2 : 2;
        Book *tmp = realloc(lib->books, new_cap * sizeof(Book));

        if (!tmp) {
            LOG_ERROR("Realloc Failed");
            return 1;
        }

        lib->books = tmp;
        lib->book_capacity = new_cap;
    }

    LOG_INFO("Capacity Expanded - Books");
    return 0;
}

int lb_reserve_authors(Library *lib) {
    if (lib->author_count >= lib->author_capacity) {
        int new_cap = lib->author_capacity ? lib->author_capacity * 2 : 2;
        Author *tmp = realloc(lib->authors,new_cap * sizeof(Author));

        if (!tmp) {
            LOG_ERROR("Realloc failed");
            return 1;
        }

        lib->authors = tmp;
        lib->author_capacity = new_cap;
    }

    LOG_INFO("Capacity Expanded - Authors");
    return 0;
}




