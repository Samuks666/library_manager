#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# include "book.h"
#include "log.h"

int book_update_id(Book *book, const int new_id) {
    if (!book) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    if (new_id <= 0) {
        LOG_ERROR("Unsupported ID");
        return 1;
    }

    book->id = new_id;
    LOG_INFO("Updated Book ID - %d - %s", new_id,book->title);
    return 0;
}

int book_update_title(Book *book, const char *title) {
    if (!book || !title) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    strcpy(book->title, title);
    LOG_INFO("Updated Book Title - %s - %s", book->isbn, title);
    return 0;
}

int book_update_isbn(Book *book, const char *isbn){ 
    if (!book || !isbn) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    strcpy(book->isbn, isbn);
    LOG_INFO("Updated Book ISBN - %s - %s", book->isbn, book->title);
    return 0;
}

int book_update_publication_year(Book *book, const int new_year) {
    if (!book) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    if (new_year <= 0) {
        LOG_ERROR("Unsupported Year");
        return 1;
    }

    book->id = new_year;
    LOG_INFO("Updated Book Year - %d - %s", new_year,book->title);
    return 0;
}

int book_update_description(Book *book, const char *description) { 
    if (!book || !description) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    strcpy(book->description, description);
    LOG_INFO("Updated Book Description - ISBN %s ", book->isbn);
    return 0;
}

int book_add_genre(Book *book, const int genre_id) {
    if (!book) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    if (genre_id <= 0) {
        LOG_ERROR("Unsupported Genre ID");
        return 1;
    }

    if (book_reserve_genre(book) != 0) {
        LOG_ERROR(EXPAND_CAPACITY);
        return 1;
    }

    book->genre_ids[book->genre_count] = genre_id;
    book->genre_count++;

    LOG_INFO("Book Genre Added - %d", genre_id);
    return 0;
}

int book_remove_genre(Book *book, const int genre_id) {
        if (!book) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    int rm_index = -1;
    for (int i = 0; i < book->genre_count; i++) {
        if (book->genre_ids[i] == genre_id) {
            rm_index = i;
            break;
        }
    }

    if (rm_index < 0) {
        LOG_ERROR("Genre to be removed from Book not Found");
        return 1;
    }

    for (int i = rm_index; i < book->genre_count - 1; i++) {
        book->genre_ids[i] = book->genre_ids[i + 1];
    }

    book->genre_count--;
    memset(&book->genre_ids[book->genre_count], 0, sizeof(int));
    
    LOG_INFO("Book Genre Removed - %d", genre_id);

    return 0;
}

int book_add_author(Book *book, const int author_id){
        if (!book) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    if (author_id <= 0) {
        LOG_ERROR("Unsupported Genre ID");
        return 1;
    }

    if (book_reserve_author(book) != 0) {
        LOG_ERROR(EXPAND_CAPACITY);
        return 1;
    }

    book->author_ids[book->author_count] = author_id;
    book->author_count++;

    LOG_INFO("Book Author Added - %d", author_id);
    return 0;
}

int book_remove_author(Book *book, const int author_id) {
    if (!book) {
        LOG_ERROR(NULL_ERROR);
        return 1;
    }

    int rm_index = -1;
    for (int i = 0; i < book->author_count; i++) {
        if (book->author_ids[i] == author_id) {
            rm_index = i;
            break;
        }
    }

    if (rm_index < 0) {
        LOG_ERROR("Genre to be removed from Book not Found");
        return 1;
    }

    for (int i = rm_index; i < book->author_count - 1; i++) {
        book->author_ids[i] = book->author_ids[i + 1];
    }

    book->author_count--;
    memset(&book->author_ids[book->author_count], 0, sizeof(int));

    LOG_INFO("Book Author Removed - %d", author_id);

    return 0;
}

int book_reserve_genre(Book *book) {
    if (book->genre_count >= book->genre_capacity) {
        int new_cap = book->genre_capacity ? book->genre_capacity * 2 : 2;
        int *tmp = realloc(book->genre_ids, new_cap * sizeof(int));

        if (!tmp) {
            LOG_ERROR(REALLOCATION_ERROR);
            return 1;
        }

        book->genre_ids = tmp;
        book->genre_capacity = new_cap;
        LOG_INFO("Capacity Expanded - Genres");
    }

    return 0;
}

int book_reserve_author(Book *book) {
    if (book->author_count >= book->author_capacity) {
        int new_cap = book->author_capacity ? book->author_capacity * 2 : 2;
        int *tmp = realloc(book->author_ids, new_cap * sizeof(int));

        if (!tmp) {
            LOG_ERROR(REALLOCATION_ERROR);
            return 1;
        }

        book->author_ids = tmp;
        book->author_capacity = new_cap;
        LOG_INFO("Capacity Expanded - Authors");
    }

    return 0;
}

int *book_get_genre_ids(Book *book) {
    return book->genre_ids;
}

int *book_get_author_ids(Book *book) {
    return book->author_ids;
}

