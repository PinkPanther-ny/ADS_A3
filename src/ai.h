#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "utils.h"

#define DEBUG 1
#define DEBUG_LOG "Better solution found, %3d pegs remain, %7d node expanded (%f SECS).\n", \
                    remainPeg, expanded_nodes, (double)(clock()-start)/CLOCKS_PER_SEC

void initialize_ai();

void find_solution( state_t* init_state );
void free_memory(unsigned expanded_nodes);

#endif
