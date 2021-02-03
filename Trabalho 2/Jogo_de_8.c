#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>

#define TILE_LEN	16
#define ROW_LEN		4
#define COL_LEN		4
#define MOVES		4
#define TILE		1
#define BLANK_TILE	0

typedef struct node{
	int state[16];
	int g;
	int f;
	int prev_move;
} node;

/**
 * Variaveis Globais 
 */

// usado para rastrear a posição do espaço em branco em um estado,
// portanto, não precisa ser pesquisado toda vez que verificamos se um operador é aplicável
// Quando aplicamos um operador, blank_pos é atualizado

int blank_pos;

// Nó inicial do problema
node initial_node;

// Estatísticas sobre o número de nós gerados e gastos
unsigned long generated;
unsigned long expanded;



 // O id das quatro ações disponíveis para mover o espaço em branco (slot vazio). ex.
  //Esquerda: move o espaço em branco para a esquerda, etc.
  

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3


 //Matrizes auxiliares para a função aplicável
 //aplicabilidade dos operadores: 0 = esquerda, 1 = direita, 2 = cima, 3 = baixo
 
int ap_opLeft[]  = { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 };
int ap_opRight[]  = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
int ap_opUp[]  = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int ap_opDown[]  = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
int *ap_ops[] = { ap_opLeft, ap_opRight, ap_opUp, ap_opDown };


/* estado de impressão */

void print_state( int* s )
{
	int i;
	
	for( i = 0; i < 16; i++ )
		printf( "%2d%c", s[i], ((i+1) % 4 == 0 ? '\n' : ' ') );
}
      
void printf_comma (long unsigned int n) {
    if (n < 0) {
        printf ("-");
        printf_comma (-n);
        return;
    }
    if (n < 1000) {
        printf ("%lu", n);
        return;
    }
    printf_comma (n/1000);
    printf (",%03lu", n%1000);
}


void state_copy(int* new_state, int* prev_state){
	int i;
	for (i = 0; i < TILE_LEN; i++) {
		new_state[i] = prev_state[i];
	}
}

/*retornar a soma das distâncias de manhattan do estado até a meta */

int manhattan( int* state )
{
	int sum = 0;
	int row,col,x,y;
	int index;
	for (col = 0; col < COL_LEN; col++) {
		for (row = 0; row < ROW_LEN; row++) {
			index = (col * COL_LEN) + row;
			// Manhattan Distance 1D -> 2D excluding Blank Tile
			if (state[index] != BLANK_TILE) {
				y = state[index] / COL_LEN;
				x = state[index] - (y * ROW_LEN);
				sum += abs(row - x) + abs(col - y);
			}
		}
	}

	return( sum );
}


/* retorna 1 se op é aplicável no estado, caso contrário, retorna 0 */

int applicable( int op )
{
    return( ap_ops[op][blank_pos] );
}



void apply( node* n, int op )
{
	int t;

	
	t = blank_pos + (op == LEFT ? -1 : (op == RIGHT ? 1 : (op == UP ? -4 : 4)));

	//apply op
	n->state[blank_pos] = n->state[t];
	n->state[t] = BLANK_TILE;
	

	blank_pos = t;
}


void find_blank_pos(int* state) {
	int i;
	for (i = 0; i < TILE_LEN; i++) {
		if (state[i] == BLANK_TILE) {
			blank_pos = i;
			break;
		}
	}
}	

/*IDA recursiva */

node* ida( node* node, int threshold, int* newThreshold )
{
	int a;
	int estimate;
	int actions[MOVES] = {};
	struct node* r = NULL;
	struct node new_node;

	// Descubra todos os movimentos aplicáveis ??no estado atual
	
	for (a = 0; a < MOVES; a++) {
		actions[a] = applicable(a);
	}


	for (a = 0; a < MOVES; a++) {
		if (actions[a] != 0 && a != node->prev_move) {
						
			/**************************************************************************************/
			new_node.prev_move = (a == LEFT ? RIGHT : (a == RIGHT ? LEFT : (a == UP ? DOWN : UP)));
			/**************************************************************************************/

			generated++;

			
			find_blank_pos(node->state);

			state_copy(new_node.state, node->state);
			apply(&new_node, a);

			
			new_node.g = node->g + 1;

		
			estimate = manhattan(new_node.state);
			new_node.f = new_node.g + estimate;

			// Se o custo estimado for maior que o limite
			
			if (new_node.f > threshold) {
				expanded++;
				if (new_node.f < *newThreshold) {
					
					*newThreshold = new_node.f;
				}
			}
			else {
				if (estimate == 0) {
					
					r = &new_node;
					return r;
				}
				r = ida(&new_node, threshold, newThreshold);
				if (r != NULL) {
					return r;
				}
			}

		}
	}
	return( NULL );
}



int IDA_control_loop(  ){
	struct node* r = NULL;
	
	int threshold;
	

	generated = 0;
	expanded = 0;


	initial_node.f = threshold = manhattan( initial_node.state );

	printf( "Initial Estimate = %d\nThreshold = ", threshold );
	printf("%d ", threshold);


	while (r == NULL) {
	
		int newThreshold = INT_MAX;
		struct node curr_node;
		
		
		state_copy(curr_node.state, initial_node.state);


		curr_node.g = 0;
		curr_node.prev_move = 0;
		
		r = ida(&curr_node, threshold, &newThreshold);
		if (r == NULL) {
			
			threshold = newThreshold;
			printf("%d ", newThreshold);
		}
	}

	if(r)
		return r->g;
	else
		return -1;
}


static inline float compute_current_time()
{
	struct rusage r_usage;
	
	getrusage( RUSAGE_SELF, &r_usage );	
	float diff_time = (float) r_usage.ru_utime.tv_sec;
	diff_time += (float) r_usage.ru_stime.tv_sec;
	diff_time += (float) r_usage.ru_utime.tv_usec / (float)1000000;
	diff_time += (float) r_usage.ru_stime.tv_usec / (float)1000000;
	return diff_time;
}

int main( int argc, char **argv )
{
	int i, solution_length;

	/*verifique se temos um estado inicial como parâmetro */
	
	if( argc != 2 )
	{
		fprintf( stderr, "usage: %s \"<initial-state-file>\"\n", argv[0] );
		return( -1 );
	}



	FILE* initFile = fopen( argv[1], "r" );
	char buffer[256];

	if( fgets(buffer, sizeof(buffer), initFile) != NULL ){
		char* tile = strtok( buffer, " " );
		for( i = 0; tile != NULL; ++i )
			{
				initial_node.state[i] = atoi( tile );
				blank_pos = (initial_node.state[i] == 0 ? i : blank_pos);
				tile = strtok( NULL, " " );
			}		
	}
	else{
		fprintf( stderr, "Filename empty\"\n" );
		return( -2 );

	}
       
	if( i != 16 )
	{
		fprintf( stderr, "invalid initial state\n" );
		return( -1 );
	}


	initial_node.g=0;
	initial_node.f=0;

	print_state( initial_node.state );



	float t0 = compute_current_time();
	
	solution_length = IDA_control_loop();				

	float tf = compute_current_time();


	printf( "\nSolution = %d\n", solution_length);
	printf( "Generated = ");
	printf_comma(generated);		
	printf("\nExpanded = ");
	printf_comma(expanded);		
	printf( "\nTime (seconds) = %.2f\nExpanded/Second = ", tf-t0 );
	printf_comma((unsigned long int) expanded/(tf+0.00000001-t0));
	printf("\n\n");

	/* agregar todas as execuções em um arquivo chamado report.dat para obter os resultados */
	FILE* report = fopen( "report.dat", "a" );

	fprintf( report, "%s", argv[1] );
	fprintf( report, "\n\tSoulution = %d, Generated = %lu, Expanded = %lu", solution_length, generated, expanded);
	fprintf( report, ", Time = %f, Expanded/Second = %f\n\n", tf-t0, (float)expanded/(tf-t0));
	fclose(report);
	fclose(initFile);
	
	return( 0 );
}


