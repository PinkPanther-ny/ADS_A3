#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

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

    //all_allocated_nodes[count_allocated_nodes++] = new_n;

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
	//node_t *ini_node = n;
    node_t* new_node = NULL;

	//FILL IN THE GRAPH ALGORITHM
    stack_push(n);
    ht_insert(&table, &n->state.field, n);

    int remainPeg = num_pegs(&n->state);

    // debug use.
    clock_t start = clock();
    bool isDead;
    while (!is_stack_empty()){
        n = stack_top();
        stack_pop();
        expanded_nodes++;

        if(num_pegs(&n->state) < remainPeg){
            save_solution(n);
            remainPeg = num_pegs(&n->state);
            if(DEBUG){ printf(DEBUG_LOG); }
        }

        isDead = true;
        position_s curPos;
        for(curPos.x=0;curPos.x<SIZE;curPos.x++){
            for(curPos.y=0;curPos.y<SIZE;curPos.y++){
                // Optimized checking condition, gain 17% more performance
                if(n->state.field[ curPos.x ][ curPos.y ] !='o'){
                    continue;
                }
                for(int jump=left;jump<=down;jump++){

                    if(can_apply(&n->state, &curPos, jump)){
                        isDead = false;
                        new_node = applyAction(n, &curPos, jump);
                        generated_nodes++;

                        if(won( &new_node->state )){
                            remainPeg = num_pegs(&new_node->state);
                            save_solution(new_node);

                            if(DEBUG){ printf(DEBUG_LOG); }

                            free_node(new_node);
                            //free(new_node);

                            //free(ini_node);
                            //free_all_nodes();
                            ht_destroy(&table);
                            free_stack();
                            return;
                        }

                        stack_push(new_node);
                        if(!ht_contains( &table, new_node->state.field )){

                            ht_insert( &table, new_node->state.field, new_node->state.field );
                        }

                    }

                }

            }
        }

        if(isDead){
            node_t * tmp;
            while(n!=stack_top()->parent){
                tmp = n;
                n = n->parent;
                free(tmp);
            }
        }


        if(expanded_nodes >= budget){

            free_node(n);
            ht_destroy(&table);
            free_stack();
            break;
        }

    }


}


void free_node(node_t* node){
    node_t* tmp;
    int a=0;
    assert(node!=NULL);
    while( node != NULL ){
        printf("%d\n",a++);
        tmp = node;
        node = node->parent;
        free(tmp);
    }

}