#include <stdlib.h>

#include "args.h"
#include "simulation.h"

int main(int argc, char *argv[]) {
    SimulationSettings settings = make_default();

    if (!update_settings(&settings, argc, argv)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}