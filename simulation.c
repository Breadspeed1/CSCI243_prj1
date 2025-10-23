#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "simulation.h"

static int translations[8][2] = {{0, 1}, {1, 0},   {0, -1}, {-1, 0},
                                 {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

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
    settings.neighbors_required = 2;
    settings.grid_size = 10;
    settings.print_mode = OVERLAY;
    settings.max_steps = -1;

    return settings;
}

void display_state(SimulationState *state, SimulationSettings *settings) {
    if (settings->print_mode == OVERLAY) {
        set_cur_pos(1, 1);
    }

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
        (double)settings->neighbors_required / 8.0);

    printf("cycle %d, current changes %d, cumulative changes %d.\n",
           state->step, state->last_changes, state->total_changes);
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

/**
 * Stage the next type for a particular cell based on if it should catch fire
 * and the simulation update rules.
 *
 * @param cell the cell to update
 * @param should_catch whether the simulation has indicated this cell should
 *                     catch on fire or not
 */
static void stage_cell(CellState *cell, int should_catch) {
    if (should_catch && cell->current_type == LIVE_TREE) {
        cell->next_type = BURNING_TREE;
    } else if (cell->current_type == BURNING_TREE && cell->time_burning < 2) {
        cell->time_burning++;
    } else if (cell->current_type == BURNING_TREE && cell->time_burning >= 2) {
        cell->next_type = DEAD_TREE;
    }
}

/**
 * Apply the staged changes to this cell
 *
 * @param cell the cell to apply the changes to
 *
 * @return whether or not the cell has changed
 */
static int update_cell(CellState *cell) {
    int changed = cell->current_type != cell->next_type;
    cell->current_type = cell->next_type;

    return changed;
}

/**
 * Check whether a particular cell location should catch fire based on the
 * simulation settings and the cell's neighbors
 *
 * @param row the row of the cell
 * @param col the column of the cell
 * @param settings the simulation settings
 * @param state the current state of the simulation
 *
 * @return whether or not the cell should catch fire
 */
static int should_catch(int row, int col, SimulationSettings *settings,
                        SimulationState *state) {
    //
    int translations_c = sizeof(translations) / sizeof(translations[0]);
    int num_burning = 0;

    for (int i = 0; i < translations_c; i++) {
        int new_row = row + translations[i][0];
        int new_col = col + translations[i][1];

        int row_valid = new_row >= 0 && new_row < settings->grid_size;
        int col_valid = new_col >= 0 && new_col < settings->grid_size;

        if (row_valid && col_valid &&
            (state->grid[new_row * settings->grid_size + new_col]
                 .current_type == BURNING_TREE)) {
            num_burning++;
        }
    }

    if (num_burning >= settings->neighbors_required) {
        return (random() % 100) < settings->fire_probability;
    }

    return 0;
}

int update(SimulationState *state, SimulationSettings *settings) {
    state->step++;
    state->last_changes = 0;
    int any_burning = 0;

    for (int row = 0; row < settings->grid_size; row++) {
        for (int col = 0; col < settings->grid_size; col++) {
            int catch = should_catch(row, col, settings, state);

            stage_cell(&state->grid[row * settings->grid_size + col], catch);
        }
    }

    for (int row = 0; row < settings->grid_size; row++) {
        for (int col = 0; col < settings->grid_size; col++) {
            int changed =
                update_cell(&state->grid[row * settings->grid_size + col]);

            if (state->grid[row * settings->grid_size + col].current_type ==
                BURNING_TREE) {
                any_burning = 1;
            }

            if (changed) {
                state->last_changes++;
            }
        }
    }

    state->total_changes += state->last_changes;

    return !any_burning;
}