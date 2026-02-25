#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"
#include "cli.h"
#include "log.h"

int main(int argc, char *argv[]) {
/*
    if (argc < 2) {
        printf("Uso: %s [opções]\n", argv[0]);
        printf("Use -h ou --help para mais informações\n");
        return 1;
    }
*/
    Library MyLib;
    lb_init(&MyLib);
/*
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Sistema de Gerenciamento de Biblioteca\n\n");
            printf("Uso: %s [opções]\n\n", argv[0]);
            printf("Opções:\n");
            printf("  -h, --help       Mostra esta mensagem de ajuda\n");
            printf("  -c, --cli        Inicia o modo interativo CLI\n");
            return 0;
        }
        else if (strcmp(argv[i], "--cli") == 0 || strcmp(argv[i], "-c") == 0) {
            cli_main_loop(&MyLib);
            lb_free(&MyLib);
            return 0;
        }
        else {
            fprintf(stderr, "Opção desconhecida: %s\n", argv[i]);
            fprintf(stderr, "Use -h ou --help para ver as opções disponíveis\n");
            return 1;
        }
    }
*/
    cli_main_loop(&MyLib);
    lb_free(&MyLib);

    return 0;
}