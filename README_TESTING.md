# Library Manager - Build & Test Guide

## Estrutura do Projeto

```
library_manager/
├── src/
│   ├── core/          # Lógica principal (book, genre, author, library)
│   ├── utils/         # Utilidades (logging)
│   └── main.c         # Apenas uma demo simples
├── tests/             # Testes unitários com Google Test
│   ├── test_library.cpp
│   ├── test_book.cpp
│   ├── test_genre.cpp
│   └── test_author.cpp
├── include/           # Headers públicos
├── CMakeLists.txt     # Build configuration
└── README_TESTING.md  # Este arquivo
```

## Build Modes

### Debug Mode (Com Testes + AddressSanitizer)

Use para desenvolvimento. Inclui:
- Testes unitários (Google Test)
- AddressSanitizer (deteta memory leaks)
- Símbolos de debug
- Otimização desabilitada (mais lento)

```bash
cd library_manager
rm -rf build
mkdir build
cd build

# Configure para Debug (padrão)
cmake ..

# Compile
make -j4

# Execute todos os testes
ctest -V

# Ou execute um teste específico
ctest -V -R test_book
ctest -V -R test_library
ctest -V -R test_genre
ctest -V -R test_author
```

### Release Mode (Sem Testes, Otimizado)

Use para produção. Inclui:
- Código otimizado (-O3)
- Sem AddressSanitizer (overhead removido)
- Sem testes
- Executável mais rápido

```bash
cd library_manager
rm -rf build
mkdir build
cd build

# Configure para Release
cmake -DCMAKE_BUILD_TYPE=Release ..

# Compile
make -j4

# Execute apenas o programa
./test_app
```

## Comandos Completos

### Desenvolvimento (Debug + Testes)

```bash
# Build de zero
rm -rf build && mkdir build && cd build
cmake ..
make -j4
ctest -V

# Ou com sanitizer verbose
ASAN_OPTIONS=verbosity=1 ctest -V
```

### Produção (Release)

```bash
# Build de zero
rm -rf build && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=OFF ..
make -j4
./test_app
```

## Entender os Testes

### Estrutura de um Teste

```cpp
class LibraryTest : public ::testing::Test {
protected:
    Library lib;
    
    void SetUp() override {
        // Executado ANTES de cada teste
        ASSERT_EQ(lb_init(&lib), 0);
    }
    
    void TearDown() override {
        // Executado DEPOIS de cada teste
        lb_free(&lib);
    }
};

TEST_F(LibraryTest, AddGenreSuccess) {
    lb_add_genre(&lib, "Ficção Científica");
    EXPECT_EQ(lib.genre_count, 1);
    EXPECT_STREQ(lib.genres[0].name, "Ficção Científica");
}
```

### Assertions Comuns

```cpp
EXPECT_EQ(5, 5)          // Igual
EXPECT_NE(5, 3)          // Diferente
EXPECT_GT(10, 5)         // Maior que
EXPECT_LT(5, 10)         // Menor que
EXPECT_STREQ("a", "a")   // String igual
EXPECT_NULL(ptr)         // Ponteiro nulo
```

## O que foi testado?

### test_library.cpp
- ✅ Inicialização da biblioteca
- ✅ Adição de gêneros, autores, livros
- ✅ Expansão de capacidade
- ✅ Remoção de livros
- ✅ Cases nulos/inválidos
- ✅ Limpeza de memória

### test_book.cpp
- ✅ Criação e atualização de livros
- ✅ Adicionar/remover gêneros de livro
- ✅ Adicionar/remover autores de livro
- ✅ Atualizar título, ISBN, descrição
- ✅ Expansão de capacidade (gêneros/autores)
- ✅ Getters para IDs

### test_genre.cpp
- ✅ Adição de gêneros
- ✅ Atualização de nomes
- ✅ Expansão de capacidade
- ✅ Lookup por ID/nome
- ✅ Stress tests (1000 gêneros)
- ✅ Integridade de dados

### test_author.cpp
- ✅ Adição de autores
- ✅ Atualização de nomes
- ✅ Expansão de capacidade
- ✅ Lookup por ID/nome
- ✅ Stress tests (1000 autores)
- ✅ Integridade de dados

## Memory Leak Detection

### Em Debug Mode

Memory leaks são **automaticamente detectados** pelo AddressSanitizer:

```bash
cd build
./test_app

# Se houver leak, verá:
# ==pid==ERROR: LeakSanitizer: detected memory leaks

# Se não houver, o programa sai normalmente
```

### Com Valgrind (Manual)

Se quiser usar Valgrind ao invés de AddressSanitizer:

```bash
# Desabilitar sanitizer
cmake -DENABLE_SANITIZERS=OFF ..
make

# Rodar com Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./test_app
```

## Verificar Cobertura

```bash
# Compilar com coverage
cmake -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_C_FLAGS="--coverage" ..
make

# Rodar testes
ctest

# Gerar relatório (requer lcov)
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
open coverage_report/index.html
```

## Troubleshooting

### Erro: "gtest not found"
```bash
# Google Test é baixado automaticamente via FetchContent
# Verifique conexão internet durante o build
rm -rf build && mkdir build && cd build
cmake ..
```

### Erro: "sanitizer não funciona"
```bash
# Verifique se está em Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Testes falhando
```bash
# Execute um teste individualmente para ver detalhes
./test_library
./test_book
./test_genre
./test_author

# Ou com output detalhado
ctest -V --output-on-failure
```

## Resumo Rápido

| Objetivo | Comando |
|----------|---------|
| Desenvolver com testes | `cd build && cmake .. && make && ctest -V` |
| Build release otimizado | `cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make` |
| Rodar um teste específico | `ctest -V -R test_book` |
| Verificar memory leaks | `ASAN_OPTIONS=verbosity=1 ./test_app` |
| Limpar build | `rm -rf build` |
| Rebuild paralelo | `make -j$(nproc)` |

## Próximos Passos

1. **Execute os testes**: `ctest -V`
2. **Verifique memory leaks**: `ASAN_OPTIONS=verbosity=1 ./test_app`
3. **Adicione seus próprios testes** em `tests/`
4. **Compile Release** quando quiser otimização: `cmake -DCMAKE_BUILD_TYPE=Release ..`
