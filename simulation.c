#include "simulation.h"
#include "display.h"

SimulationSettings make_default() {
    SimulationSettings settings;

    settings.initial_burning = 0.1;
    settings.fire_probability = 0.3;
    settings.forest_density = 0.5;
    settings.neighbor_effect = 0.25;
    settings.grid_size = 10;
    settings.print_mode = OVERLAY;
    settings.max_steps = -1;

    return settings;
}