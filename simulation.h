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

typedef enum {
    EMPTY = ' ',
    LIVE_TREE = 'Y',
    BURNING_TREE = '*',
    DEAD_TREE = '.'
} CellType;

typedef struct {
    CellType current_type;
    CellType next_type;
    int time_burning;
} CellState;

typedef struct {
    CellState *grid;
    int last_changes;
    int total_changes;
    int step;
} SimulationState;

/**
 * Display the state to the screen
 *
 * @param state the state to show
 * @param settings the settings of the simulation
 */
void display_state(SimulationState *state, SimulationSettings *settings);

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
 * @return whether or not the simulation has completed
 */
int update(SimulationState *state, SimulationSettings *settings);

#endif