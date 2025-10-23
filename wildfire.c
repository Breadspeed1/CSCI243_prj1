#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "args.h"
#include "display.h"
#include "simulation.h"

#define FRAME_DELAY (750000)

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

    clear();

    display_state(&state, &settings);

    int done = update(&state, &settings);

    while (!done) {
        done = update(&state, &settings);

        usleep(FRAME_DELAY);

        display_state(&state, &settings);
    }

    printf("Fires are out.\n");

    return EXIT_SUCCESS;
}