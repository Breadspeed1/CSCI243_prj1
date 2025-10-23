#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "simulation.h"

/**
 * Create a new CellState
 *
 * @param type the type to create the CellState with
 *
 * @return the created CellState
 */
static CellState make_cell_state(CellType type) {
    CellState state;

    state.time_burning = 0;
    state.current_type = type;
    state.next_type = type;

    return state;
}

/**
 * Insert a cell in a random spot in the grid
 *
 * This function assumes that there is at least one empty spot
 *
 * @param state the state containing the grid to insert the cell into
 * @param type the type of cell to insert
 */
static void insert_random(SimulationState *state, CellType type, int gridsize) {
    int row = random() % gridsize;
    int col = random() % gridsize;

    while (state->grid[row * gridsize + col].current_type != EMPTY) {
        row = random() % gridsize;
        col = random() % gridsize;
    }

    state->grid[row * gridsize + col] = make_cell_state(type);
}

SimulationSettings make_default() {
    SimulationSettings settings;

    settings.initial_burning = 10;
    settings.fire_probability = 30;
    settings.forest_density = 50;
    settings.neighbor_effect = 25;
    settings.grid_size = 10;
    settings.print_mode = OVERLAY;
    settings.max_steps = -1;

    return settings;
}

/**
 * Display a string with the specified method.
 *
 * @pre String is terminated by null character '\0'
 *
 * @param str string to print
 * @param mode the mode to print the string in
 */
static void display_string(char *str, int *cursor_row, int *cursor_col) {
    while (*str) {
        set_cur_pos(*cursor_row, *cursor_col);
        if (*str == '\n') {
            (*cursor_row)++;
            *cursor_col = 1;
        } else {
            put(*str);
            (*cursor_col)++;
        }
        str++;
    }
}

static void display_sequential(SimulationState *state,
                               SimulationSettings *settings) {
    for (int row = 0; row < settings->grid_size; row++) {
        for (int col = 0; col < settings->grid_size; col++) {
            printf("%c", (char)state->grid[row * settings->grid_size + col]
                             .current_type);
        }
        printf("\n");
    }

    printf(
        "size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f\n",
        settings->grid_size, (double)settings->fire_probability / 100.0,
        (double)settings->forest_density / 100.0,
        (double)settings->initial_burning / 100.0,
        (double)settings->neighbor_effect / 100.0);

    printf("cycle %d, current changes %d, cumulative changes %d.\n",
           state->step, state->last_changes, state->total_changes);
}

static void display_overlay(SimulationState *state,
                            SimulationSettings *settings) {
    char print_buffer[70] = {'\0'};

    int cursor_row = 1;
    int cursor_col = 1;

    for (; cursor_row - 1 < settings->grid_size; cursor_row++) {
        cursor_col = 1;
        for (; cursor_col - 1 < settings->grid_size; cursor_col++) {
            set_cur_pos(cursor_row, cursor_col);
            put((char)state
                    ->grid[(cursor_row - 1) * settings->grid_size + cursor_col -
                           1]
                    .current_type);
        }
    }

    cursor_row++;
    cursor_col = 1;

    sprintf(
        print_buffer,
        "size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor%.2f\n",
        settings->grid_size, (double)settings->fire_probability / 100.0,
        (double)settings->forest_density / 100.0,
        (double)settings->initial_burning / 100.0,
        (double)settings->neighbor_effect / 100.0);

    display_string(print_buffer, &cursor_row, &cursor_col);

    sprintf(print_buffer,
            "cycle %d, current changes %d, cumulative changes %d.\n",
            state->step, state->last_changes, state->total_changes);

    display_string(print_buffer, &cursor_row, &cursor_col);
}

void display_state(SimulationState *state, SimulationSettings *settings) {
    if (settings->print_mode == PRINT) {
        display_sequential(state, settings);
    } else if (settings->print_mode == OVERLAY) {
        clear();
        display_overlay(state, settings);
    }
}

void initialize(SimulationState *state, SimulationSettings *settings) {
    state->last_changes = 0;
    state->total_changes = 0;
    state->step = 0;

    for (int row = 0; row < settings->grid_size; row++) {
        for (int col = 0; col < settings->grid_size; col++) {
            state->grid[row * settings->grid_size + col] =
                make_cell_state(EMPTY);
        }
    }

    int total_spaces = settings->grid_size * settings->grid_size;
    int total_trees = (total_spaces * settings->forest_density) / 100;
    int already_burning = (total_trees * settings->initial_burning) / 100;
    int live_trees = total_trees - already_burning;

    for (int i = 0; i < already_burning; i++)
        insert_random(state, BURNING_TREE, settings->grid_size);

    for (int i = 0; i < live_trees; i++)
        insert_random(state, LIVE_TREE, settings->grid_size);
}