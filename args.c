#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "args.h"

#define VALID_OPTIONS ("Hb:c:d:n:p:s:")

/**
 * Print the usage of the program to standard error.
 */
static void usage() {
    fprintf(
        stderr,
        "usage: wildfire [options]\n"
        "By default, the simulation runs in overlay display mode.\n"
        "The -pN option makes the simulation run in print mode for up to N "
        "states.\n\n"
        "Simulation Configuration Options:\n"
        " -H  # View simulation options and quit.\n"
        " -bN # proportion of trees that are already burning. 0 < N < 101.\n"
        " -cN # probability that a tree will catch fire. 0 < N < 101.\n"
        " -dN # density: the proportion of trees in the grid. 0 < N < 101.\n"
        " -nN # proportion of neighbors that influence a tree catching fire. "
        " -1 < N < 101.\n"
        " -pN # number of states to print before quitting. -1 < N < ...\n"
        " -sN # simulation grid size. 4 < N < 41.");
}

int update_settings(SimulationSettings *settings, int argc, char *argv[]) {
    int val;
    int opt;

    while ((opt = getopt(argc, argv, VALID_OPTIONS)) != -1) {
        switch (opt) {
            case 'H':
                usage();
                return 1;
            case 'b':
                val = atoi(optarg);

                if (val > 0 && val < 101) {
                    settings->initial_burning = val;
                } else {
                    fprintf(stderr,
                            "(-bN) proportion already burning must be an "
                            "integer in [1...100].\n");
                    usage();

                    return 0;
                }
                break;
            case 'c':
                val = atoi(optarg);

                if (val > 0 && val < 101) {
                    settings->fire_probability = val;
                } else {
                    fprintf(
                        stderr,
                        "(-cN) probability a tree will catch fire must be an "
                        "integer in [1...100].\n");
                    usage();
                    return 0;
                }
                break;
            case 'd':
                val = atoi(optarg);

                if (val > 0 && val < 101) {
                    settings->forest_density = val;
                } else {
                    fprintf(stderr,
                            "(-dN) density of trees in the grid must be an "
                            "integer in [1...100].\n");
                    usage();
                    return 0;
                }
                break;
            case 'n':
                val = atoi(optarg);

                if (val >= 0 && val < 101) {
                    // find out the minimum number of neighbors required to be
                    // burning integer ceiling division
                    settings->neighbors_required = (val * 8 + 100 - 1) / 100;
                } else {
                    // goofy percent format to get rid of warnings
                    fprintf(
                        stderr,
                        "(-nN) %cneighbors influence catching fire must be an "
                        "integer in [0...100].\n",
                        '%');
                    usage();
                    return 0;
                }
                break;
            case 'p':
                val = atoi(optarg);

                if (val >= 0 && val <= 10000) {
                    settings->print_mode = PRINT;
                    settings->max_steps = val;
                } else {
                    fprintf(stderr,
                            "(-pN) number of states to print must be an "
                            "integer in [0...10000].\n");
                    usage();
                    return 0;
                }
                break;
            case 's':
                val = atoi(optarg);

                if (val > 4 && val < 41) {
                    settings->grid_size = val;
                } else {
                    fprintf(stderr, "(-sN) simulation grid size must be an "
                                    "integer in [5...40].\n");
                    usage();
                    return 0;
                }
                break;
            default:
                /* getopt already prints an error message for unrecognized
                 * options; show usage and return error. */
                usage();
                return 0;
        }
    }
    return 1;
}
