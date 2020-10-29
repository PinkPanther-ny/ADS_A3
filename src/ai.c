#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"


void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;

	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;
		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);

	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action ){

    node_t* new_node = create_init_node(&(n->state));

    new_node->depth = n->depth + 1;
    new_node->parent = n;

    new_node->state.cursor.x = (*selected_peg).x;
    new_node->state.cursor.y = (*selected_peg).y;

    execute_move_t( &(new_node->state), selected_peg, action );
    new_node->move = action;
	return new_node;

}

/**
 * Find a solution path as per algorithm description in the handout
 */
void find_solution( state_t* init_state  ){

	HashTable table;

	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE, sizeof(int8_t) * SIZE * SIZE, 16769023);

	// Initialize Stack
    //DFS GRAPH ALGORITHM BY USING STACK
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );
    node_t* new_node = NULL;

    stack_push(n);
    ht_insert(&table, &n->state.field, n);
    int remainPeg = num_pegs(&n->state);

    bool isDead;
    position_s curPos;

    // Debug use
    clock_t start = clock();
    printf("\n");
    while (!is_stack_empty()){

        n = stack_top();
        stack_pop();
        expanded_nodes++;

        if(num_pegs(&n->state) < remainPeg){
            save_solution(n);
            remainPeg = num_pegs(&n->state);
            if(DEBUG){ printf(DEBUG_LOG); }
        }

        // If no action can be taken at current board, it's a dead branch
        isDead = true;
        for(curPos.x=0;curPos.x<SIZE;curPos.x++){
            for(curPos.y=0;curPos.y<SIZE;curPos.y++){
                for(int jump=left;jump<=down;jump++){

                    if( can_apply(&n->state, &curPos, jump) ){
                        new_node = applyAction(n, &curPos, jump);
                        generated_nodes++;

                        if( won(&new_node->state) ){
                            remainPeg = num_pegs(&new_node->state);
                            save_solution(new_node);
                            if(DEBUG){ printf(DEBUG_LOG); }

                            free_all_memory(new_node, &table);
                            return;
                        }

                        // Cutting unnecessary subtrees
                        if( is_seen_first_time(&table, new_node) ){

                            isDead = false;
                            stack_push(new_node);
                            ht_insert( &table, &new_node->state.field, &new_node->state.field );
                        }else{
                            free(new_node);
                        }
                    }

                }
            }
        }

        // Free dead tree branches, until reach the upper parent node
        if(isDead){
            free_node(n, stack_top()->parent);
        }

        // Game over, free and exit
        if(expanded_nodes >= budget){
            free_all_memory(n, &table);
            return;
        }

    }

}

// First see the board of the node in the given hashTable
bool is_seen_first_time(HashTable * hashTable, node_t * node){
    return !ht_contains(hashTable, &(node->state.field));
}

// Summarize all free tasks
void free_all_memory(node_t* main_branch, HashTable * table){
    free_node(main_branch, NULL);
    ht_destroy(table);
    free_stack();

}

// Free a node and its parents, until reach given node
void free_node(node_t* node, node_t* until_reach){
    node_t* tmp;
    while( node != until_reach ){
        tmp = node;
        node = node->parent;
        free(tmp);
    }

}