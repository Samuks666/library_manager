#ifndef GENRE_H
#define GENRE_H

#define MAX_GENRE 64

typedef struct {
    int id;
    char name[MAX_GENRE];
} Genre;

/// @brief Function to update a genre ID
/// @param genre Genre to be updated
/// @param new_id New ID to be assigned
/// @return 0 if Success | 1 if False
int update_genre_id(Genre *genre, const int new_id);

/// @brief Function to update a genre ID
/// @param genre Original Genre to update
/// @param name New Name to be assigned
/// @return 0 Success - 1 Fail
int update_genre_name(Genre *genre, const char *name);

#endif