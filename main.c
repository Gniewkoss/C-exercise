#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    int id;
    int parent_id;
    struct User* parent;
    struct User* next;
    int* movies;
    int movie_count;
    int movie_capacity;
} User;

User* user_list = NULL;

int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

User* find_user(int user_id) {
    User* current = user_list;
    while (current) {
        if (current->id == user_id)
            return current;
        current = current->next;
    }
    return NULL;
}

void add_user(int parent_id, int user_id) {
    if (find_user(user_id) || (!find_user(parent_id) && parent_id != 0)) {
        printf("ERROR\n");
        return;
    }

    User* new_user = (User*)malloc(sizeof(User));
    if (!new_user) {
        printf("ERROR\n");
        return;
    }

    new_user->id = user_id;
    new_user->parent_id = parent_id;
    new_user->parent = find_user(parent_id);
    new_user->next = user_list;
    new_user->movies = NULL;
    new_user->movie_count = 0;
    new_user->movie_capacity = 0;
    user_list = new_user;

    printf("OK\n");
}

void delete_user(int user_id) {
    if (user_id == 0 || !find_user(user_id)) {
        printf("ERROR\n");
        return;
    }

    User* to_delete = NULL;
    User* prev = NULL;
    User* current = user_list;

    while (current) {
        if (current->id == user_id) {
            to_delete = current;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (!to_delete) {
        printf("ERROR\n");
        return;
    }

    User* parent = to_delete->parent;
    current = user_list;
    while (current) {
        if (current->parent_id == user_id) {
            current->parent_id = parent ? parent->id : -1;
            current->parent = parent;
        }
        current = current->next;
    }

    if (prev)
        prev->next = to_delete->next;
    else
        user_list = to_delete->next;

    free(to_delete->movies);
    free(to_delete);
    printf("OK\n");
}

void add_movie(int user_id, int movie_id) {
    User* user = find_user(user_id);
    if (!user) {
        printf("ERROR\n");
        return;
    }

    for (int i = 0; i < user->movie_count; i++) {
        if (user->movies[i] == movie_id) {
            printf("ERROR\n");
            return;
        }
    }

    if (user->movie_count == user->movie_capacity) {
        user->movie_capacity = (user->movie_capacity == 0) ? 4 : user->movie_capacity * 2;
        int* new_movies = (int*)realloc(user->movies, user->movie_capacity * sizeof(int));
        if (!new_movies) {
            printf("ERROR\n");
            return;
        }
        user->movies = new_movies;
    }

    user->movies[user->movie_count++] = movie_id;
    printf("OK\n");
}

void delete_movie(int user_id, int movie_id) {
    User* user = find_user(user_id);
    if (!user) {
        printf("ERROR\n");
        return;
    }

    for (int i = 0; i < user->movie_count; i++) {
        if (user->movies[i] == movie_id) {
            user->movies[i] = user->movies[--user->movie_count];
            printf("OK\n");
            return;
        }
    }

    printf("ERROR\n");
}

void print_user_movies(int user_id) {
    User* user = find_user(user_id);
    if (!user) {
        printf("ERROR\n");
        return;
    }

    int total_count = user->movie_count + (user->parent ? user->parent->movie_count : 0);
    int* all_movies = (int*)malloc(total_count * sizeof(int));
    if (!all_movies) {
        printf("ERROR\n");
        return;
    }

    int index = 0;

    for (int i = 0; i < user->movie_count; i++)
        all_movies[index++] = user->movies[i];

    if (user->parent) {
        for (int i = 0; i < user->parent->movie_count; i++)
            all_movies[index++] = user->parent->movies[i];
    }

    qsort(all_movies, total_count, sizeof(int), compare_ints);

    for (int i = 0; i < total_count; i++) {
        if (i == 0 || all_movies[i] != all_movies[i - 1])
            printf("%d ", all_movies[i]);
    }

    printf("\n");
    free(all_movies);
}

void free_users() {
    User* current = user_list;
    while (current) {
        User* next = current->next;
        free(current->movies);
        free(current);
        current = next;
    }
}

int main() {
    FILE* input_file = fopen("C:\\Users\\gniew\\CLionProjects\\untitled\\test.in", "r");


    if (!input_file) {
        printf("ERROR: Unable to open file test.in\n");
        return 1;
    }

    char command[50];
    while (fscanf(input_file, "%s", command) != EOF) {
        if (strcmp(command, "addUser") == 0) {
            char buffer1[20], buffer2[20];
            fscanf(input_file, "%s %s", buffer1, buffer2);
            char* endptr;
            int parent_id = strtol(buffer1, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            int user_id = strtol(buffer2, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            add_user(parent_id, user_id);
        } else if (strcmp(command, "delUser") == 0) {
            char buffer[20];
            fscanf(input_file, "%s", buffer);
            char* endptr;
            int user_id = strtol(buffer, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            delete_user(user_id);
        } else if (strcmp(command, "addMovie") == 0) {
            char buffer1[20], buffer2[20];
            fscanf(input_file, "%s %s", buffer1, buffer2);
            char* endptr;
            int user_id = strtol(buffer1, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            int movie_id = strtol(buffer2, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            add_movie(user_id, movie_id);
        } else if (strcmp(command, "delMovie") == 0) {
            char buffer1[20], buffer2[20];
            fscanf(input_file, "%s %s", buffer1, buffer2);
            char* endptr;
            int user_id = strtol(buffer1, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            int movie_id = strtol(buffer2, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            delete_movie(user_id, movie_id);
        } else if (strcmp(command, "printUserMovies") == 0) {
            char buffer[20];
            fscanf(input_file, "%s", buffer);
            char* endptr;
            int user_id = strtol(buffer, &endptr, 10);
            if (*endptr != '\0') {
                printf("ERROR: Invalid integer input\n");
                continue;
            }
            print_user_movies(user_id);
        }
    }

    fclose(input_file);
    free_users();
    return 0;
}
