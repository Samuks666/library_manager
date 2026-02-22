#ifndef AUTHOR_H
#define AUTHOR_H

#define MAX_AUTHOR_NAME 128

typedef struct {
    int id;
    char name[MAX_AUTHOR_NAME];
} Author;

/// @brief Function to update an author ID
/// @param author Autor to be updated
/// @param new_id New ID to be assigned
/// @return 0 if Success | 1 if False
int author_update_id(Author *author, const int new_id);

/// @brief Function to update author NAME
/// @param author Autor to be updated
/// @param new_id New NAME to be assigned
/// @return 0 if Success | 1 if False
int author_update_name(Author *author, const char *name);

#endif