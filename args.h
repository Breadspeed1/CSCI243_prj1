#include "simulation.h"

#ifndef ARGS_H
#define ARGS_H

/**
 * Updates the provided settings with the supplied command line arguments.
 *
 * Error messages will be printed by this function.
 *
 * @param settings a pointer to the initial settings
 * @param argc number of args
 * @param argv arguments
 *
 * @return whether all arguments were parsed properly
 */
int update_settings(SimulationSettings *settings, int argc, char *argv[]);

#endif
