#include "cli.h"

void cli_main_loop(Library *lib) {
    bool is_looping = true;
    int active_window = 0;

    cli_init_ncurses();
    noecho();  // Garantir que echo está desativado
    cbreak();  // Modo cbreak para getch() funcionar corretamente

    const char *main_menu[] = {
        "Adicionar Livro",
        "Remover Livro",
        "Atualizar Dados",
        "Listar Livros",
        "Buscar Livro",
        "Estatísticas",
        "Sair"
    };
    
    const char *update_menu[] = {
        "Atualizar Livro",
        "Atualizar Autor",
        "Atualizar Gênero"
    };
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    MenuWindow *menu1 = cli_create_window(main_menu, 7, 0);
    MenuWindow *menu2 = cli_create_window(update_menu, 3, max_x);
    
    int c;
    int frames = 30;
    
    while (is_looping) {
        clear();
        refresh();
        
        cli_draw_menu(menu1);
        if (active_window >= 1) cli_draw_menu(menu2);
        
        refresh();
        
        c = getch();
        
        if (active_window == 0) {
            switch(c) {
                case KEY_UP:
                    if(menu1->highlight > 0) menu1->highlight--;
                    break;
                case KEY_DOWN:
                    if(menu1->highlight < menu1->item_count - 1) menu1->highlight++;
                    break;
                case KEY_RIGHT:
                    // Só permite entrar no submenu se for "Atualizar Dados" (opção 3)
                    if (menu1->highlight == 2) {  // Índice 2 = opção 3
                        // Menu1 sai para esquerda, Menu2 entra da direita
                        for (int i = 0; i < frames; i++) {
                            clear();
                            int offset = (max_x * i) / frames;
                            
                            mvwin(menu1->win, 0, -offset);
                            cli_draw_menu(menu1);
                            
                            mvwin(menu2->win, 0, max_x - offset);
                            cli_draw_menu(menu2);
                            
                            refresh();
                            napms(10);
                        }
                        mvwin(menu1->win, 0, -max_x);
                        mvwin(menu2->win, 0, 0);
                        menu1->window_x = -max_x;
                        menu2->window_x = 0;
                        menu2->highlight = 0;
                        active_window = 1;
                    }
                    break;
                case 10:  // ENTER
                case '\r':  // Também suportar \r
                {
                    int choice = menu1->highlight + 1;
                    if (choice == 7) {  // Sair
                        is_looping = false;
                    } else if (choice == 3) {  // Atualizar Dados
                        // Entrar no submenu
                        for (int i = 0; i < frames; i++) {
                            clear();
                            int offset = (max_x * i) / frames;
                            
                            mvwin(menu1->win, 0, -offset);
                            cli_draw_menu(menu1);
                            
                            mvwin(menu2->win, 0, max_x - offset);
                            cli_draw_menu(menu2);
                            
                            refresh();
                            napms(10);
                        }
                        mvwin(menu1->win, 0, -max_x);
                        mvwin(menu2->win, 0, 0);
                        menu1->window_x = -max_x;
                        menu2->window_x = 0;
                        menu2->highlight = 0;
                        active_window = 1;
                    } else if (choice == 1) {  // Adicionar Livro
                        cli_add_book(lib);
                        noecho();  // Reativar noecho após retornar
                    } else if (choice == 2) {  // Remover Livro
                        cli_remove_book(lib);
                        noecho();  // Reativar noecho após retornar
                    } else if (choice == 4) {  // Listar Livros
                        cli_list_books(lib);
                        noecho();  // Reativar noecho após retornar
                    } else if (choice == 5) {  // Buscar Livro
                        cli_search_book(lib);
                        noecho();  // Reativar noecho após retornar
                    } else if (choice == 6) {  // Estatísticas
                        cli_statistics(lib);
                        noecho();  // Reativar noecho após retornar
                    }
                    break;
                }
            }
        }
        else if (active_window == 1) {
            switch(c) {
                case KEY_UP:
                    if(menu2->highlight > 0) menu2->highlight--;
                    break;
                case KEY_DOWN:
                    if(menu2->highlight < menu2->item_count - 1) menu2->highlight++;
                    break;
                case KEY_LEFT:
                    // Menu2 sai para direita, Menu1 entra da esquerda
                    for (int i = 0; i < frames; i++) {
                        clear();
                        int offset = (max_x * i) / frames;
                        
                        mvwin(menu2->win, 0, offset);
                        cli_draw_menu(menu2);
                        
                        mvwin(menu1->win, 0, -max_x + offset);
                        cli_draw_menu(menu1);
                        
                        refresh();
                        napms(10);
                    }
                    mvwin(menu2->win, 0, max_x);
                    mvwin(menu1->win, 0, 0);
                    menu1->window_x = 0;
                    menu2->window_x = max_x;
                    active_window = 0;
                    break;
                case 10:  // ENTER
                case '\r':  // Também suportar \r
                {
                    int choice = menu2->highlight + 1;
                    
                    if (choice == 1) {  // Atualizar Livro
                        cli_update_book(lib);
                        noecho();  // Reativar noecho após retornar
                    } else if (choice == 2) {  // Atualizar Autor
                        cli_update_author(lib);
                        noecho();  // Reativar noecho após retornar
                    } else if (choice == 3) {  // Atualizar Gênero
                        cli_update_genre(lib);
                        noecho();  // Reativar noecho após retornar
                    }
                    break;
                }
            }
        }
    }
    
    cli_destroy_window(menu1);
    cli_destroy_window(menu2);

    endwin();
}

void cli_init_ncurses () {
    static int initialized = 0;
    
    if (initialized) return;
    
    initscr();
    start_color();
    use_default_colors();

    if (COLORS >= 256) {
        init_pair(1, 196, -1);   // Vermelho intenso
        init_pair(2, 46, -1);    // Verde brilhante
        init_pair(3, 226, -1);   // Amarelo brilhante
        init_pair(4, 21, -1);    // Azul
        init_pair(5, 201, -1);   // Magenta
    } else if (COLORS >= 8) {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    }

    clear();
    noecho();
    cbreak();
    nonl();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    initialized = 1;
}



void cli_add_book(Library *lib) {
    char title[256], isbn[20], description[512];
    char author_name[256], genre_name[256];
    int year, num_authors, num_genres;
    int *author_ids, *genre_ids;
    Book book = {0};
    
    clear();
    mvprintw(1, 5, "=== Adicionar Livro ===");
    
    mvprintw(3, 5, "Nome do Livro: ");
    refresh();
    echo();
    scanw("%255s", title);
    noecho();
    strcpy(book.title, title);
    
    mvprintw(4, 5, "ISBN: ");
    refresh();
    echo();
    scanw("%19s", isbn);
    noecho();
    strcpy(book.isbn, isbn);
    
    mvprintw(5, 5, "Ano de Publicação: ");
    refresh();
    echo();
    scanw("%d", &year);
    noecho();
    book.publication_year = year;
    
    mvprintw(6, 5, "Descrição: ");
    refresh();
    echo();
    scanw("%511s", description);
    noecho();
    strcpy(book.description, description);
    
    mvprintw(7, 5, "Número de autores: ");
    refresh();
    echo();
    scanw("%d", &num_authors);
    noecho();
    
    author_ids = (int *)malloc(num_authors * sizeof(int));
    if (!author_ids) {
        LOG_ERROR(ALLOCATION_ERROR);
        return;
    }
    
    for (int i = 0; i < num_authors; i++) {
        mvprintw(8 + i, 5, "Nome do autor %d: ", i + 1);
        refresh();
        echo();
        scanw("%128s", author_name);
        noecho();
        
        if (lb_add_author(lib, author_name) == 0) {
            author_ids[i] = lib->authors[lib->author_count - 1].id;
        }
    }
    
    int input_line = 8 + num_authors;
    mvprintw(input_line, 5, "Número de gêneros: ");
    refresh();
    echo();
    scanw("%d", &num_genres);
    noecho();
    
    genre_ids = (int *)malloc(num_genres * sizeof(int));
    if (!genre_ids) {
        LOG_ERROR(ALLOCATION_ERROR);
        free(author_ids);
        return;
    }
    
    for (int i = 0; i < num_genres; i++) {
        mvprintw(input_line + 1 + i, 5, "Nome do gênero %d: ", i + 1);
        refresh();
        echo();
        scanw("%64s", genre_name);
        noecho();
        
        if (lb_add_genre(lib, genre_name) == 0) {
            genre_ids[i] = lib->genres[lib->genre_count - 1].id;
        }
    }
    
    for (int i = 0; i < num_authors; i++) {
        book_add_author(&book, author_ids[i]);
    }
    
    for (int i = 0; i < num_genres; i++) {
        book_add_genre(&book, genre_ids[i]);
    }
    
    lb_add_book(lib, &book);
    
    free(author_ids);
    free(genre_ids);
    
    clear();
    mvprintw(10, 10, "Livro adicionado com sucesso!");
    mvprintw(11, 10, "Pressione qualquer tecla para continuar...");
    refresh();
    noecho();
    getch();
}

void cli_remove_book(Library *lib) {
    char isbn[20];
    
    clear();
    mvprintw(1, 5, "=== Remover Livro ===");
    mvprintw(5, 5, "ISBN do livro a remover: ");
    refresh();
    
    echo();
    scanw("%19s", isbn);
    noecho();
    
    if (lb_remove_book(lib, isbn) == 0) {
        clear();
        mvprintw(10, 10, "Livro removido com sucesso!");
    } else {
        clear();
        mvprintw(10, 10, "Erro ao remover livro!");
    }
    
    mvprintw(11, 10, "Pressione qualquer tecla para continuar...");
    refresh();
    getch();
}



MenuWindow *cli_create_window(const char **items, int count, int start_x) {
    int max_y, max_x;
    MenuWindow *window = malloc(sizeof(MenuWindow));

    if (!window){
        LOG_ERROR(ALLOCATION_ERROR);
        return NULL;
    }
    
    window->items = malloc(count * sizeof(char*));
    if (!window->items) {
        LOG_ERROR(ALLOCATION_ERROR);
        free(window);
        return NULL;
    }

    window->item_count = count;
    window->highlight = 0;
    window->window_x = start_x;   
    window->target_x = start_x;

    for(int i = 0; i < count; i++) {
        window->items[i] = malloc(strlen(items[i]) + 1);
    
        if (!window->items[i]) {
            LOG_ERROR(ALLOCATION_ERROR);
            for(int j = 0; j < i; j++) {
                free(window->items[j]);
            }
            free(window->items);
            free(window);
            return NULL;
        }
        strcpy(window->items[i], items[i]);
    }

    getmaxyx(stdscr, max_y, max_x);
    int height = max_y;
    int min_width = max_x / 3;

    window->win = newwin(height, min_width, 0, start_x);
    
    if (!window->win) {
        LOG_ERROR("Erro ao criar janela ncurses");
        for(int i = 0; i < count; i++) {
            free(window->items[i]);
        }
        free(window->items);
        free(window);
        return NULL;
    }

    return window;
}

void cli_destroy_window(MenuWindow *menu){ 
    if (!menu) {
        LOG_ERROR(NULL_ERROR);
        return;
    }

    for (int i = 0; i < menu->item_count; i++) {
        free(menu->items[i]);
    }

    free(menu->items);

    delwin(menu->win);

    free(menu);
}

void cli_draw_menu(MenuWindow *menu) {
    if (!menu) {
        LOG_ERROR(NULL_ERROR);
        return;
    }

    werase(menu->win);
    box(menu->win, 0, 0);
    
    for (int i = 0; i < menu->item_count; i++) {
        if (i == menu->highlight) {
            if (COLORS >= 8) {
                wattron(menu->win, COLOR_PAIR(1) | A_REVERSE);
            } else {
                wattron(menu->win, A_REVERSE);
            }
        }
        mvwprintw(menu->win, i + 1, 1, "%s", menu->items[i]);
        if (i == menu->highlight) {
            if (COLORS >= 8) {
                wattroff(menu->win, COLOR_PAIR(1) | A_REVERSE);
            } else {
                wattroff(menu->win, A_REVERSE);
            }
        }
    }
    
    wrefresh(menu->win);
}

void cli_move_menu(MenuWindow *menu, int new_x) {
    if (!menu) {
        LOG_ERROR(NULL_ERROR);
        return;
    }
    
    mvwin(menu->win, 0, new_x);
    menu->window_x = new_x;
    wrefresh(menu->win);
}

// ============================================================
// FUNÇÕES DE ATUALIZAÇÃO
// ============================================================

void cli_update_book(Library *lib) {
    char isbn[20];
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    attron(A_BOLD);
    mvprintw(1, 5, "=== Atualizar Livro ===");
    attroff(A_BOLD);
    
    mvprintw(3, 5, "ISBN do livro: ");
    refresh();
    
    noecho();
    cbreak();
    echo();
    scanw("%19s", isbn);
    noecho();
    
    // Procurar livro
    int book_idx = -1;
    for (int i = 0; i < lib->book_count; i++) {
        if (strcmp(lib->books[i].isbn, isbn) == 0) {
            book_idx = i;
            break;
        }
    }
    
    if (book_idx == -1) {
        clear();
        mvprintw(5, 5, "Livro não encontrado!");
        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
        refresh();
        noecho();
        getch();
        echo();
        return;
    }
    
    // Submenu de atualização
    const char *update_options[] = {
        "Atualizar Título",
        "Atualizar Ano",
        "Atualizar Descrição",
        "Voltar"
    };
    
    MenuWindow *submenu = cli_create_window(update_options, 4, max_x);
    
    int c;
    int frames = 30;
    int in_submenu = 0;
    
    while (1) {
        clear();
        attron(A_BOLD);
        mvprintw(1, 5, "Livro: %s", lib->books[book_idx].title);
        attroff(A_BOLD);
        
        if (in_submenu) {
            cli_draw_menu(submenu);
        }
        
        refresh();
        c = getch();
        
        if (!in_submenu) {
            if (c == KEY_RIGHT) {
                for (int i = 0; i < frames; i++) {
                    clear();
                    int offset = (max_x * i) / frames;
                    mvprintw(1, 5, "Livro: %s", lib->books[book_idx].title);
                    mvwin(submenu->win, 0, max_x - offset);
                    cli_draw_menu(submenu);
                    refresh();
                    napms(10);
                }
                mvwin(submenu->win, 0, 0);
                in_submenu = 1;
            } else if (c == 10) {  // ENTER
                break;
            }
        } else {
            switch(c) {
                case KEY_UP:
                    if(submenu->highlight > 0) submenu->highlight--;
                    break;
                case KEY_DOWN:
                    if(submenu->highlight < submenu->item_count - 1) submenu->highlight++;
                    break;
                case KEY_LEFT:
                    for (int i = 0; i < frames; i++) {
                        clear();
                        int offset = (max_x * i) / frames;
                        mvprintw(1, 5, "Livro: %s", lib->books[book_idx].title);
                        mvwin(submenu->win, 0, offset);
                        cli_draw_menu(submenu);
                        refresh();
                        napms(10);
                    }
                    mvwin(submenu->win, 0, max_x);
                    submenu->highlight = 0;
                    in_submenu = 0;
                    break;
                case 10:  // ENTER
                {
                    int choice = submenu->highlight + 1;
                    
                    if (choice == 1) {  // Título
                        clear();
                        mvprintw(5, 5, "Novo título: ");
                        refresh();
                        char new_title[256];
                        echo();
                        scanw("%255s", new_title);
                        noecho();
                        book_update_title(&lib->books[book_idx], new_title);
                        
                        clear();
                        mvprintw(5, 5, "Título atualizado!");
                        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
                        refresh();
                        getch();
                    } else if (choice == 2) {  // Ano
                        clear();
                        mvprintw(5, 5, "Novo ano: ");
                        refresh();
                        int new_year;
                        echo();
                        scanw("%d", &new_year);
                        noecho();
                        book_update_publication_year(&lib->books[book_idx], new_year);
                        
                        clear();
                        mvprintw(5, 5, "Ano atualizado!");
                        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
                        refresh();
                        getch();
                    } else if (choice == 3) {  // Descrição
                        clear();
                        mvprintw(5, 5, "Nova descrição: ");
                        refresh();
                        char new_desc[512];
                        echo();
                        scanw("%511s", new_desc);
                        noecho();
                        book_update_description(&lib->books[book_idx], new_desc);
                        
                        clear();
                        mvprintw(5, 5, "Descrição atualizada!");
                        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
                        refresh();
                        getch();
                    } else if (choice == 4) {  // Voltar
                        for (int i = 0; i < frames; i++) {
                            clear();
                            int offset = (max_x * i) / frames;
                            mvprintw(1, 5, "Livro: %s", lib->books[book_idx].title);
                            mvwin(submenu->win, 0, offset);
                            cli_draw_menu(submenu);
                            refresh();
                            napms(10);
                        }
                        mvwin(submenu->win, 0, max_x);
                        submenu->highlight = 0;
                        in_submenu = 0;
                        break;
                    }
                    submenu->highlight = 0;
                    break;
                }
            }
        }
    }
    
    cli_destroy_window(submenu);
}

void cli_update_author(Library *lib) {
    int author_id;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    attron(A_BOLD);
    mvprintw(1, 5, "=== Atualizar Autor ===");
    attroff(A_BOLD);
    
    mvprintw(3, 5, "ID do autor: ");
    refresh();
    
    noecho();
    echo();
    scanw("%d", &author_id);
    noecho();
    
    // Procurar autor
    int author_idx = -1;
    for (int i = 0; i < lib->author_count; i++) {
        if (lib->authors[i].id == author_id) {
            author_idx = i;
            break;
        }
    }
    
    if (author_idx == -1) {
        clear();
        mvprintw(5, 5, "Autor não encontrado!");
        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
        refresh();
        getch();
        return;
    }
    
    const char *update_options[] = {
        "Atualizar Nome",
        "Voltar"
    };
    
    MenuWindow *submenu = cli_create_window(update_options, 2, max_x);
    
    int c;
    int frames = 30;
    int in_submenu = 0;
    
    while (1) {
        clear();
        attron(A_BOLD);
        mvprintw(1, 5, "Autor: %s", lib->authors[author_idx].name);
        attroff(A_BOLD);
        
        if (in_submenu) {
            cli_draw_menu(submenu);
        }
        
        refresh();
        c = getch();
        
        if (!in_submenu) {
            if (c == KEY_RIGHT) {
                for (int i = 0; i < frames; i++) {
                    clear();
                    int offset = (max_x * i) / frames;
                    mvprintw(1, 5, "Autor: %s", lib->authors[author_idx].name);
                    mvwin(submenu->win, 0, max_x - offset);
                    cli_draw_menu(submenu);
                    refresh();
                    napms(10);
                }
                mvwin(submenu->win, 0, 0);
                in_submenu = 1;
            } else if (c == 10) {
                break;
            }
        } else {
            switch(c) {
                case KEY_UP:
                    if(submenu->highlight > 0) submenu->highlight--;
                    break;
                case KEY_DOWN:
                    if(submenu->highlight < submenu->item_count - 1) submenu->highlight++;
                    break;
                case KEY_LEFT:
                    for (int i = 0; i < frames; i++) {
                        clear();
                        int offset = (max_x * i) / frames;
                        mvprintw(1, 5, "Autor: %s", lib->authors[author_idx].name);
                        mvwin(submenu->win, 0, offset);
                        cli_draw_menu(submenu);
                        refresh();
                        napms(10);
                    }
                    mvwin(submenu->win, 0, max_x);
                    submenu->highlight = 0;
                    in_submenu = 0;
                    break;
                case 10:  // ENTER
                {
                    int choice = submenu->highlight + 1;
                    
                    if (choice == 1) {  // Nome
                        clear();
                        mvprintw(5, 5, "Novo nome: ");
                        refresh();
                        char new_name[128];
                        echo();
                        scanw("%127s", new_name);
                        noecho();
                        author_update_name(&lib->authors[author_idx], new_name);
                        
                        clear();
                        mvprintw(5, 5, "Autor atualizado!");
                        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
                        refresh();
                        getch();
                    } else if (choice == 2) {  // Voltar
                        for (int i = 0; i < frames; i++) {
                            clear();
                            int offset = (max_x * i) / frames;
                            mvprintw(1, 5, "Autor: %s", lib->authors[author_idx].name);
                            mvwin(submenu->win, 0, offset);
                            cli_draw_menu(submenu);
                            refresh();
                            napms(10);
                        }
                        mvwin(submenu->win, 0, max_x);
                        submenu->highlight = 0;
                        in_submenu = 0;
                        break;
                    }
                    submenu->highlight = 0;
                    break;
                }
            }
        }
    }
    
    cli_destroy_window(submenu);
}

void cli_update_genre(Library *lib) {
    int genre_id;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    attron(A_BOLD);
    mvprintw(1, 5, "=== Atualizar Gênero ===");
    attroff(A_BOLD);
    
    mvprintw(3, 5, "ID do gênero: ");
    refresh();
    
    noecho();
    echo();
    scanw("%d", &genre_id);
    noecho();
    
    // Procurar gênero
    int genre_idx = -1;
    for (int i = 0; i < lib->genre_count; i++) {
        if (lib->genres[i].id == genre_id) {
            genre_idx = i;
            break;
        }
    }
    
    if (genre_idx == -1) {
        clear();
        mvprintw(5, 5, "Gênero não encontrado!");
        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
        refresh();
        getch();
        return;
    }
    
    const char *update_options[] = {
        "Atualizar Nome",
        "Voltar"
    };
    
    MenuWindow *submenu = cli_create_window(update_options, 2, max_x);
    
    int c;
    int frames = 30;
    int in_submenu = 0;
    
    while (1) {
        clear();
        attron(A_BOLD);
        mvprintw(1, 5, "Gênero: %s", lib->genres[genre_idx].name);
        attroff(A_BOLD);
        
        if (in_submenu) {
            cli_draw_menu(submenu);
        }
        
        refresh();
        c = getch();
        
        if (!in_submenu) {
            if (c == KEY_RIGHT) {
                for (int i = 0; i < frames; i++) {
                    clear();
                    int offset = (max_x * i) / frames;
                    mvprintw(1, 5, "Gênero: %s", lib->genres[genre_idx].name);
                    mvwin(submenu->win, 0, max_x - offset);
                    cli_draw_menu(submenu);
                    refresh();
                    napms(10);
                }
                mvwin(submenu->win, 0, 0);
                in_submenu = 1;
            } else if (c == 10) {
                break;
            }
        } else {
            switch(c) {
                case KEY_UP:
                    if(submenu->highlight > 0) submenu->highlight--;
                    break;
                case KEY_DOWN:
                    if(submenu->highlight < submenu->item_count - 1) submenu->highlight++;
                    break;
                case KEY_LEFT:
                    for (int i = 0; i < frames; i++) {
                        clear();
                        int offset = (max_x * i) / frames;
                        mvprintw(1, 5, "Gênero: %s", lib->genres[genre_idx].name);
                        mvwin(submenu->win, 0, offset);
                        cli_draw_menu(submenu);
                        refresh();
                        napms(10);
                    }
                    mvwin(submenu->win, 0, max_x);
                    submenu->highlight = 0;
                    in_submenu = 0;
                    break;
                case 10:  // ENTER
                {
                    int choice = submenu->highlight + 1;
                    
                    if (choice == 1) {  // Nome
                        clear();
                        mvprintw(5, 5, "Novo nome: ");
                        refresh();
                        char new_name[64];
                        echo();
                        scanw("%63s", new_name);
                        noecho();
                        update_genre_name(&lib->genres[genre_idx], new_name);
                        
                        clear();
                        mvprintw(5, 5, "Gênero atualizado!");
                        mvprintw(6, 5, "Pressione qualquer tecla para continuar...");
                        refresh();
                        getch();
                    } else if (choice == 2) {  // Voltar
                        for (int i = 0; i < frames; i++) {
                            clear();
                            int offset = (max_x * i) / frames;
                            mvprintw(1, 5, "Gênero: %s", lib->genres[genre_idx].name);
                            mvwin(submenu->win, 0, offset);
                            cli_draw_menu(submenu);
                            refresh();
                            napms(10);
                        }
                        mvwin(submenu->win, 0, max_x);
                        submenu->highlight = 0;
                        in_submenu = 0;
                        break;
                    }
                    submenu->highlight = 0;
                    break;
                }
            }
        }
    }
    
    cli_destroy_window(submenu);
}

void cli_list_books(Library *lib) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    attron(A_BOLD);
    mvprintw(0, 0, "=== Lista de Livros ===");
    attroff(A_BOLD);
    
    if (lib->book_count == 0) {
        mvprintw(2, 0, "Nenhum livro registrado!");
        mvprintw(max_y - 1, 0, "Pressione qualquer tecla para continuar...");
        refresh();
        getch();
        return;
    }
    
    int line = 2;
    for (int i = 0; i < lib->book_count && line < max_y - 1; i++) {
        mvprintw(line, 0, "[%d] %s (ISBN: %s)", 
                 lib->books[i].id, 
                 lib->books[i].title, 
                 lib->books[i].isbn);
        line++;
    }
    
    mvprintw(max_y - 1, 0, "Pressione qualquer tecla para continuar...");
    refresh();
    getch();
}

void cli_search_book(Library *lib) {
    char search_term[256];
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    attron(A_BOLD);
    mvprintw(1, 5, "=== Buscar Livro ===");
    attroff(A_BOLD);
    
    mvprintw(3, 5, "Título ou ISBN: ");
    refresh();
    echo();
    scanw("%255s", search_term);
    noecho();
    
    clear();
    attron(A_BOLD);
    mvprintw(1, 5, "=== Resultados da Busca ===");
    attroff(A_BOLD);
    
    int found = 0;
    int line = 3;
    
    for (int i = 0; i < lib->book_count && line < max_y - 1; i++) {
        if (strstr(lib->books[i].title, search_term) != NULL || 
            strstr(lib->books[i].isbn, search_term) != NULL) {
            mvprintw(line, 5, "[%d] %s (ISBN: %s, Ano: %d)",
                     lib->books[i].id,
                     lib->books[i].title,
                     lib->books[i].isbn,
                     lib->books[i].publication_year);
            line++;
            found++;
        }
    }
    
    if (found == 0) {
        mvprintw(3, 5, "Nenhum livro encontrado!");
    }
    
    mvprintw(max_y - 1, 5, "Pressione qualquer tecla para continuar...");
    refresh();
    getch();
}

void cli_statistics(Library *lib) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    attron(A_BOLD);
    mvprintw(1, 5, "=== Estatísticas da Biblioteca ===");
    attroff(A_BOLD);
    
    mvprintw(3, 5, "Total de Livros: %d", lib->book_count);
    mvprintw(4, 5, "Total de Autores: %d", lib->author_count);
    mvprintw(5, 5, "Total de Gêneros: %d", lib->genre_count);
    
    // Calcular livros por autor
    if (lib->author_count > 0) {
        int author_books = 0;
        for (int i = 0; i < lib->book_count; i++) {
            author_books += lib->books[i].author_count;
        }
        mvprintw(7, 5, "Total de Relacionamentos Livro-Autor: %d", author_books);
    }
    
    // Calcular livros por gênero
    if (lib->genre_count > 0) {
        int genre_books = 0;
        for (int i = 0; i < lib->book_count; i++) {
            genre_books += lib->books[i].genre_count;
        }
        mvprintw(8, 5, "Total de Relacionamentos Livro-Gênero: %d", genre_books);
    }
    
    mvprintw(max_y - 1, 5, "Pressione qualquer tecla para continuar...");
    refresh();
    getch();
}