#ifndef SIM_H
#define SIM_H

typedef enum { PRINT, OVERLAY } DisplayMode;

typedef struct {
    int initial_burning;
    int fire_probability;
    int forest_density;
    int neighbor_effect;
    int grid_size;
    int max_steps;
    DisplayMode print_mode;
} SimulationSettings;

/**
 * Make a copy of the default simulation settingss
 *
 * @return the generated settings
 */
SimulationSettings make_default();

#endif