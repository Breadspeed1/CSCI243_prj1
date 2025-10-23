#define _DEFAULT_SOURCE

#include <stdlib.h>

#include "args.h"
#include "simulation.h"

int main(int argc, char *argv[]) {
    SimulationSettings settings = make_default();

    if (!update_settings(&settings, argc, argv)) {
        return EXIT_FAILURE;
    }

    CellState grid[settings.grid_size * settings.grid_size];

    SimulationState state;

    state.grid = grid;

    srandom(41);

    initialize(&state, &settings);

    display_state(&state, &settings);

    return EXIT_SUCCESS;
}