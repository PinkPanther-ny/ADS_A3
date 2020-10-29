#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "utils.h"
#include "utils.h"
#include "hashtable.h"


#define DEBUG 1
#define DEBUG_LOG "Better solution found, %3d pegs remain, %7d node expanded (%f SECS).\n", \
                    remainPeg, expanded_nodes, (double)(clock()-start)/CLOCKS_PER_SEC

void initialize_ai();

void find_solution( state_t* init_state );
void free_all_memory(node_t* main_branch, HashTable * table);

void free_node(node_t* node, node_t* until_reach);
bool is_seen_first_time(HashTable * table, node_t * node);
#endif
