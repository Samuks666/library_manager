#ifndef BOOK_H
#define BOOK_H

#define MAX_TITLE 128
#define ISBN_SIZE 18
#define MAX_DESCRIPTION 512

typedef struct {
    int id;

    char title[MAX_TITLE];
    char isbn[ISBN_SIZE];

    int *genre_ids;
    int genre_count;
    int genre_capacity;
    
    int publication_year;

    int *author_ids;
    int author_count;
    int author_capacity;

    char description[MAX_DESCRIPTION];

} Book;


int book_update_id(Book *book, const int new_id);


int book_update_title(Book *book, const char *title);


int book_update_isbn(Book *book, const char *isbn);


int book_update_publication_year(Book *book, const int new_year);


int book_update_description(Book *book, const char *description); 


int book_add_genre(Book *book, const int genre_id);


int book_remove_genre(Book *book, const int genre_id);


int book_add_author(Book *book, const int author_id);


int book_remove_author(Book *book, const int author_id);


int book_reserve_genre(Book *book);


int book_reserve_author(Book *book);


int *book_get_genre_ids(Book *book);


int *book_get_author_ids(Book *book);


#endif