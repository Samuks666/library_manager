#include <stdio.h>
#include <stdlib.h>

# include "genre.h"
#include "log.h"

int update_genre_id(Genre *genre, const int new_id) {
    if (!genre) {
        LOG_ERROR("Null Genre");
        return 1;
    }

    if (new_id <= 0) {
        LOG_ERROR("Unsupported ID");
        return 1;
    }

    genre->id = new_id;
    LOG_INFO("Updated Genre - ID %d - %s", new_id,genre->name);
    return 0;
}

int update_genre_name(Genre *genre, const char *name) {
    if (!genre || !name) {
        LOG_ERROR("Null Genre or name");
        return 1;
    }

    strcpy(genre->name, name);
    LOG_INFO("Updated Genre - ID %d - %s", genre->id, name);
    return 0;
}

