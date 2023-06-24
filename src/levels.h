#ifndef DS_LEVELS_H
#define DS_LEVELS_H
#include <stdio.h>
#include <stdlib.h>

struct choice {
    char *data;
    int len;
    void *level;
    void *next_choice;
    void *previous_choice;
};

struct choice_level {
    struct choice *choices;
    int level_depth;
    void *next_level;
};

struct choice_level *levels;
struct choice_level *current_level;

int popout_choice(struct choice *ch);
int popin_choice(struct choice *ch, struct choice_level *dst);
struct choice *create_choice(char *data, int len);
int destroy_choice(struct choice *ch);

int migrate_choice(struct choice* ch, struct choice_level *dst);
struct choice_level *create_level();

#endif