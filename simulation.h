#ifndef SIM_H
#define SIM_H

typedef enum { PRINT, OVERLAY } DisplayMode;

typedef struct {
  float initial_burning;
  float fire_probability;
  float forest_density;
  float neighbor_effect;
  int grid_size;
  DisplayMode print_mode;
} SimulationSettings;

/**
 * Make a copy of the default simulation settingss
 *
 * @return the generated settings
 */
SimulationSettings make_default();

#endif