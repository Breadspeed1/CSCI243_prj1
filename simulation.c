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

void display_state(SimulationState *state, SimulationSettings *settings) {
    if (settings->print_mode == OVERLAY) {
        clear();
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
        (double)settings->neighbor_effect / 100.0);

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