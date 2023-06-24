#include "levels.h"


int popout_choice(struct choice *ch) {
    if (ch == NULL) {
        return 1;
    }

    if (ch->previous_choice == NULL) {
        return 1;
    }

    if (ch->next_choice == NULL) {
        return 1;
    }

    if (ch->next_choice != ch && ch->previous_choice != ch) {
        struct choice* previous_choice = ch->previous_choice;
        previous_choice->next_choice = ch->next_choice;

        struct choice* next_choice = ch->next_choice;
        next_choice->previous_choice = ch->previous_choice;
    }

    ch->previous_choice = NULL;
    ch->next_choice = NULL;
    ch->level = NULL;

    return 0;
}

int popin_choice(struct choice *ch, struct choice_level *dst) {
    if (ch == NULL) {
        return 1;
    }

    if (dst == NULL) {
        return 1;
    }

    ch->level = dst;

    struct choice *first_choice;
    struct choice *last_choice;

    if (dst->choices == NULL) {
        dst->choices = ch;
        first_choice = ch;
        last_choice = ch;
    } else {
        first_choice = dst->choices;
        last_choice = first_choice->previous_choice;
    }

    ch->previous_choice = last_choice;
    ch->next_choice = first_choice;

    last_choice->next_choice = ch;
    first_choice->previous_choice = ch;

    return 0;
}

struct choice *create_choice(char *data, int len) {
    if (data == NULL) {
        return NULL;
    }

    if (len == 0) {
        return NULL;
    }

    struct choice *new_choice = (struct choice *) malloc(sizeof(struct choice));

    new_choice->data = data;
    new_choice->len = len;
    new_choice->next_choice = NULL;
    new_choice->previous_choice = NULL;
    new_choice->level = NULL;

    return new_choice;
}

int destroy_choice(struct choice *ch) {
    if (popout_choice(ch) == 1) {
        return 1;
    }

    free(ch->data);
    free(ch);

    return 0;
}

struct choice_level *create_level() {
    struct choice_level *new_level = (struct choice_level*) malloc(sizeof(struct choice_level));
    new_level->choices = NULL;

    if (levels == NULL) {
        new_level->level_depth = 0;
        levels = new_level;
    } else {
        new_level->level_depth = ++current_level->level_depth;
        current_level->next_level = new_level;
    }

    current_level = new_level;

    return new_level;
}

int migrate_choice(struct choice* ch, struct choice_level *dst) {
    if (popout_choice(ch) == 1) {
        return 1;
    }

    if (popin_choice(ch, dst) == 1) {
        return 1;
    }

    return 0;
}