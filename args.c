#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "args.h"

#define VALID_OPTIONS ("Hb:c:d:n:p:s:")

static void usage() {
    printf("usage: wildfire [options]\n"
           "By default, the simulation runs in overlay display mode.\n"
           "The -pN option makes the simulation run in print mode for up to N "
           "states.\n\n"
           "Simulation Configuration Options:\n"
           "-H  # View simulation options and quit.\n"
           "-bN # proportion of trees that are already burning. 0 < N < 101.\n"
           "-cN # probability that a tree will catch fire. 0 < N < 101.\n"
           "-dN # density: the proportion of trees in the grid. 0 < N < 101.\n"
           "-nN # proportion of neighbors that influence a tree catching fire. "
           "-1 < N "
           "< 101.\n"
           "-pN # number of states to print before quitting. -1 < N < ...\n"
           "-sN # simulation grid size. 4 < N < 41.\n");
}

int update_settings(SimulationSettings *settings, int argc, char *argv[]) {
    int opt;
}
