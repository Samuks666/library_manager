#ifndef BOOK_H
#define BOOK_H

#ifdef __cplusplus
extern "C" {
#endif

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

/// @brief Function to update a book ID
/// @param book Book to be updated
/// @param new_id New ID to be assigned
/// @return 0 if Success | 1 if False
int book_update_id(Book *book, const int new_id);

/// @brief Function to update a book title
/// @param book Book to be updated
/// @param title New Title to be Assigned
/// @return 0 if Success | 1 if False
int book_update_title(Book *book, const char *title);

/// @brief Function to update a book ISBN
/// @param book Book to be updated
/// @param isbn New ISBN to be assigned 
/// @return 0 if Success | 1 if False
int book_update_isbn(Book *book, const char *isbn);

/// @brief Function to update a book publish year
/// @param book Book to be updated
/// @param new_year New Publish Year to be assigned 
/// @return 0 if Success | 1 if False
int book_update_publication_year(Book *book, const int new_year);

/// @brief Function to update a book description
/// @param book Book to be updated
/// @param description New description to be assigned 
/// @return 0 if Success | 1 if False
int book_update_description(Book *book, const char *description); 

/// @brief Function to add a genre to a book
/// @param book Book to be updated
/// @param genre_id Genre ID do be added 
/// @return 0 if Success | 1 if False
int book_add_genre(Book *book, const int genre_id);

/// @brief Function to remove a genre to a book
/// @param book Book to be updated
/// @param genre_id Genre ID to be removed
/// @return 0 if Success | 1 if False
int book_remove_genre(Book *book, const int genre_id);

/// @brief Function to add an author to a book
/// @param book Book to be updated
/// @param author_id Author ID to be added 
/// @return 0 if Success | 1 if False
int book_add_author(Book *book, const int author_id);

/// @brief Function to remove an author to a book
/// @param book Book to be updated
/// @param author_id Author ID to be removed
/// @return 0 if Success | 1 if False
int book_remove_author(Book *book, const int author_id);

/// @brief Function to expand genre capacity
/// @param book Book to be updated
/// @return 0 if Success | 1 if False
int book_reserve_genre(Book *book);

/// @brief Function to expand author capacity
/// @param book Book to be updated
/// @return 0 if Success | 1 if False
int book_reserve_author(Book *book);

/// @brief Function to get all genres ID's of a book
/// @param book Book to get genre ID's 
/// @return 0 if Success | 1 if False
int *book_get_genre_ids(Book *book);

/// @brief Function to get all authors ID's of a book
/// @param book Book to get authors ID's
/// @return 0 if Success | 1 if False
int *book_get_author_ids(Book *book);


#ifdef __cplusplus
}
#endif

#endif