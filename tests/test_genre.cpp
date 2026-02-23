#include <gtest/gtest.h>
#include <cstring>
#include <vector>
#include "../include/core/library.h"

class GenreTest : public ::testing::Test {
protected:
    Library lib;
    
    void SetUp() override {
        ASSERT_EQ(lb_init(&lib), 0);
    }
    
    void TearDown() override {
        lb_free(&lib);
    }
};

// ========== Genre Addition Tests ==========

TEST_F(GenreTest, AddSingleGenre) {
    EXPECT_EQ(lb_add_genre(&lib, "Ficção Científica"), 0);
    EXPECT_EQ(lib.genre_count, 1);
    EXPECT_STREQ(lib.genres[0].name, "Ficção Científica");
    EXPECT_EQ(lib.genres[0].id, 1);
}

TEST_F(GenreTest, AddMultipleGenres) {
    const char *genres[] = {
        "Ficção Científica",
        "Mistério",
        "Romance",
        "Fantasia",
        "Terror"
    };
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(lb_add_genre(&lib, genres[i]), 0);
    }
    
    EXPECT_EQ(lib.genre_count, 5);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_STREQ(lib.genres[i].name, genres[i]);
        EXPECT_EQ(lib.genres[i].id, i + 1);
    }
}

TEST_F(GenreTest, AddGenreNullLibrary) {
    EXPECT_EQ(lb_add_genre(nullptr, "Test"), 1);
}

TEST_F(GenreTest, AddGenreNullName) {
    EXPECT_EQ(lb_add_genre(&lib, nullptr), 1);
}

// ========== Genre Capacity Tests ==========

TEST_F(GenreTest, GenreCapacityExpansion) {
    int initial_capacity = lib.genre_capacity;
    
    for (int i = 0; i < initial_capacity + 3; i++) {
        char genre_name[100];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        EXPECT_EQ(lb_add_genre(&lib, genre_name), 0);
    }
    
    EXPECT_GT(lib.genre_capacity, initial_capacity);
    EXPECT_EQ(lib.genre_count, initial_capacity + 3);
}

TEST_F(GenreTest, MultipleCapacityExpansions) {
    int initial_capacity = lib.genre_capacity;
    
    for (int i = 0; i < initial_capacity * 4; i++) {
        char genre_name[100];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        EXPECT_EQ(lb_add_genre(&lib, genre_name), 0);
    }
    
    EXPECT_EQ(lib.genre_count, initial_capacity * 4);
    EXPECT_GE(lib.genre_capacity, initial_capacity * 4);
}

// ========== Genre ID Assignment Tests ==========

TEST_F(GenreTest, GenreIDsAreSequential) {
    for (int i = 0; i < 10; i++) {
        char genre_name[100];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        lb_add_genre(&lib, genre_name);
    }
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(lib.genres[i].id, i + 1);
    }
}

TEST_F(GenreTest, GenreIDsAreUnique) {
    for (int i = 0; i < 20; i++) {
        char genre_name[100];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        lb_add_genre(&lib, genre_name);
    }
    
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            EXPECT_NE(lib.genres[i].id, lib.genres[j].id);
        }
    }
}

// ========== Genre Name Tests ==========

TEST_F(GenreTest, GenreNameStorage) {
    const char *test_name = "Ficção Científica e Fantasia";
    EXPECT_EQ(lb_add_genre(&lib, test_name), 0);
    EXPECT_STREQ(lib.genres[0].name, test_name);
}

TEST_F(GenreTest, GenreNameMaxLength) {
    char long_name[256];
    memset(long_name, 'A', 255);
    long_name[255] = '\0';
    
    // Should succeed but truncate if needed
    lb_add_genre(&lib, long_name);
    EXPECT_GT(strlen(lib.genres[0].name), 0);
}

TEST_F(GenreTest, EmptyGenreName) {
    // Implementation defines behavior for empty string
    // For now, it should be accepted but result might vary
    lb_add_genre(&lib, "");
    EXPECT_EQ(lib.genre_count, 1);
}

// ========== Genre Lookup Tests ==========

TEST_F(GenreTest, FindGenreByID) {
    lb_add_genre(&lib, "Ficção Científica");
    lb_add_genre(&lib, "Mistério");
    lb_add_genre(&lib, "Romance");
    
    int found_index = -1;
    for (int i = 0; i < lib.genre_count; i++) {
        if (lib.genres[i].id == 2) {
            found_index = i;
            break;
        }
    }
    
    EXPECT_NE(found_index, -1);
    EXPECT_STREQ(lib.genres[found_index].name, "Mistério");
}

TEST_F(GenreTest, FindGenreByName) {
    lb_add_genre(&lib, "Ficção Científica");
    lb_add_genre(&lib, "Mistério");
    lb_add_genre(&lib, "Romance");
    
    int found_index = -1;
    for (int i = 0; i < lib.genre_count; i++) {
        if (strcmp(lib.genres[i].name, "Mistério") == 0) {
            found_index = i;
            break;
        }
    }
    
    EXPECT_NE(found_index, -1);
    EXPECT_EQ(lib.genres[found_index].id, 2);
}

// ========== Genre Clear Tests ==========

TEST_F(GenreTest, ClearLibraryGenres) {
    for (int i = 0; i < 5; i++) {
        char genre_name[100];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        lb_add_genre(&lib, genre_name);
    }
    
    EXPECT_EQ(lib.genre_count, 5);
    
    lb_clear(&lib);
    EXPECT_EQ(lib.genre_count, 0);
}

// ========== Genre Update Tests ==========

TEST_F(GenreTest, UpdateGenreName) {
    lb_add_genre(&lib, "Ficção Científica");
    
    const char *new_name = "Sci-Fi";
    EXPECT_EQ(update_genre_name(&lib.genres[0], new_name), 0);
    EXPECT_STREQ(lib.genres[0].name, new_name);
}

TEST_F(GenreTest, UpdateGenreNameNullPointer) {
    lb_add_genre(&lib, "Ficção Científica");
    
    EXPECT_EQ(update_genre_name(nullptr, "New Name"), 1);
    EXPECT_EQ(update_genre_name(&lib.genres[0], nullptr), 1);
}

// ========== Stress Tests ==========

TEST_F(GenreTest, AddManyGenres) {
    const int num_genres = 1000;
    
    for (int i = 0; i < num_genres; i++) {
        char genre_name[100];
        snprintf(genre_name, sizeof(genre_name), "Genre_%d", i);
        EXPECT_EQ(lb_add_genre(&lib, genre_name), 0);
    }
    
    EXPECT_EQ(lib.genre_count, num_genres);
}

TEST_F(GenreTest, GenreDataIntegrity) {
    std::vector<std::string> genre_names = {
        "Ficção Científica",
        "Mistério",
        "Romance",
        "Fantasia",
        "Terror",
        "Aventura",
        "Drama",
        "Comédia"
    };
    
    for (const auto &name : genre_names) {
        lb_add_genre(&lib, name.c_str());
    }
    
    for (size_t i = 0; i < genre_names.size(); i++) {
        EXPECT_STREQ(lib.genres[i].name, genre_names[i].c_str());
        EXPECT_EQ(lib.genres[i].id, (int)i + 1);
    }
}
