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
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );
    node_t* new_node;
	//FILL IN THE GRAPH ALGORITHM

    /* stackPush(n) */
    stack_push(n);
    ht_insert(&table, &n->state.field, n);

    /* remainingPegs ← numPegs(n) */
    int remainPeg = num_pegs(&n->state);

    clock_t start = clock();
    /* while stack != empty do */
    while (!is_stack_empty()){
        n = stack_top();
        stack_pop();
        expanded_nodes++;

        if(num_pegs(&n->state) < remainPeg){
            save_solution(n);
            remainPeg = num_pegs(&n->state);
            if(DEBUG){ printf(DEBUG_LOG); }
        }

        position_s curPos;
        for(curPos.x=0;curPos.x<SIZE;curPos.x++){
            for(curPos.y=0;curPos.y<SIZE;curPos.y++){

                // Optimized checking condition, gain 17% more performance
                if(n->state.field[ curPos.x ][ curPos.y ] !='o'){
                    continue;
                }

                for(int jump=left;jump<=down;jump++){

                    if(can_apply(&n->state, &curPos, jump)){
                        new_node = applyAction(n, &curPos, jump);
                        generated_nodes++;

                        if(won( &new_node->state )){
                            remainPeg = num_pegs(&new_node->state);
                            save_solution(new_node);

                            if(DEBUG){ printf(DEBUG_LOG); }
                            return;
                        }
                        /*
                        bool isDuplicate = false;
                        for(int i=0;i<4;i++){
                            rotateBoard(&new_node->state);
                            if(ht_contains( &table, new_node->state.field )){
                                isDuplicate = true;
                                break;
                            }
                        }*/

                        if(!ht_contains( &table, new_node->state.field )){
                            //if(!isDuplicate){
                            stack_push(new_node);
                            ht_insert( &table, new_node->state.field, new_node );
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