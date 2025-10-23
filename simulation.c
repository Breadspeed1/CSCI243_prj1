#include "simulation.h"
#include "display.h"

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