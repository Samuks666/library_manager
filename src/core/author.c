#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# include "author.h"
#include "log.h"

int author_update_id(Author *author, const int new_id) {
    if (!author) {
        LOG_ERROR("Null Author");
        return 1;
    }

    if (new_id <= 0) {
        LOG_ERROR("Unsupported ID");
        return 1;
    }

    author->id = new_id;
    LOG_INFO("Updated Author - ID %d - %s", new_id,author->name);
    return 0;
}

int author_update_name(Author *author, const char *name){
    if (!author || !name) {
        LOG_ERROR("Null Author or name");
        return 1;
    }

    strcpy(author->name, name);
    LOG_INFO("Updated Author - ID %d - %s", author->id, name);
    return 0;
}