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

typedef struct {
    CellType current_type;
    CellType next_type;
    int time_burning;
} CellState;

typedef enum {
    EMPTY = ' ',
    LIVE_TREE = 'Y',
    BURNING_TREE = '*',
    DEAD_TREE = '.'
} CellType;

typedef struct {
    CellState **grid;
    int active;
} SimulationState;

/**
 * Make a copy of the default simulation settingss
 *
 * @return the generated settings
 */
SimulationSettings make_default();

/**
 * Initializes the state based on the provided settings
 *
 * @param state the state to initialize
 * @param settings the settings to initialize according to
 */
void initialize(SimulationState *state, SimulationSettings *settings);

/**
 * Updates the state based on the provided settings
 *
 * @param state the state to update
 * @param settings the settings to update according to
 *
 * @return the number of cells whos state changed
 */
int update(SimulationState *state, SimulationSettings *settings);

#endif