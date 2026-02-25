#ifndef CLI_H
#define CLI_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ncurses.h>

#include"library.h"
#include"log.h"

typedef struct {
    WINDOW *win;
    char **items;
    int item_count;
    int highlight;
    int window_x;
    int target_x;
} MenuWindow;

void cli_init_ncurses();
void cli_main_loop(Library *lib);
void cli_add_book(Library *lib);
void cli_remove_book(Library *lib);
void cli_update_book(Library *lib);
void cli_update_author(Library *lib);
void cli_update_genre(Library *lib);
void cli_list_books(Library *lib);
void cli_search_book(Library *lib);
void cli_statistics(Library *lib);

MenuWindow *cli_create_window(const char **items, int count, int start_x);
void cli_destroy_window(MenuWindow *menu);
void cli_draw_menu(MenuWindow *menu);
void cli_move_menu(MenuWindow *menu, int new_x);

#endif 