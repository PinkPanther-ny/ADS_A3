#include <time.h>
#include <stdlib.h>
#include <limits.h>
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

    node_t* new_node = NULL;

	//FILL IN MISSING CODE
	new_node = create_init_node(&(n->state));
	copy_state( &(new_node->state),  &(n->state));
    execute_move_t( &(new_node->state), &(new_node->state.cursor), action );

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
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );

	//FILL IN THE GRAPH ALGORITHM

    /* stackPush(n) */
    stack_push(n);

    /* remainingP egs ← numPegs(n) */
    int remainPeg = num_pegs(init_state);

    /* while stack != empty do */
    while (!is_stack_empty()){
        n = stack_top();
        stack_pop();
        expanded_nodes++;

        if(num_pegs(&(n->state)) < remainPeg){
            save_solution(n);
            remainPeg = num_pegs(&(n->state));
        }

        position_s curPos;
        for(int x=0;x<SIZE;x++){
            curPos.x = x;
            for(int y=0;y<SIZE;y++){
                curPos.y = y;

                for(int jump=left;jump<=down;jump++){

                    if(can_apply(&(n->state), &curPos, jump)){
                        n = applyAction(n, &curPos, jump);
                        generated_nodes++;

                        if(won(&(n->state))){
                            save_solution(n);
                            remainPeg = num_pegs(&(n->state));
                            return;
                        }

                        if(!ht_contains(&table, &(n->state.field) )){
                            stack_push(n);
                        }
                    }

                }

            }
        }

        if(expanded_nodes >= budget){
            return;
        }


    }


}