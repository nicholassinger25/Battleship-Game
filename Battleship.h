/*************************************************************************
* Programmer: Nick Singer                                               *
* Class: CptS 121; Lab Section 5                                        *
* Programming Assignment: Programming Assignment 6                      *
* Date: 11/1/18                                                         *
*                                                                       *
* Description: This program allows the user to play a game of battleship*
*			   between them and a computer. They will take turns trying *
*			   to sink the opponent's ships until one of them win.      *
*                                                                       *
*			   															*
************************************************************************/

#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_ROWS 10 
#define NUM_COLS 10

typedef struct boats //struct for all the hits the ships take
{
	int carrier;
	int battleship;
	int cruiser;
	int submarine;
	int destroyer;

	int ships_sunk; //struct for the amount of players ships have sunk
} Boats;

typedef struct stats //struct for file stats
{
	int total_hits;
	int total_misses;
	int total_shots;
	double hits_misses;
} Stats;


/*************************************************************
* Function: game();                                         *
* Description: runs the full game of battleship             *
* Input parameters: None                                    *
* Returns: Nothing                                          *
* Preconditions: None                                       *
* Postconditions: runs the full game of battleship          *
*************************************************************/
void game(void);

/*************************************************************
* Function: welcome_screen();                               *
* Description: prints the welcome screen and rules          *
* Input parameters: None                                    *
* Returns: Nothing                                          *
* Preconditions: None                                       *
* Postconditions: prints the welcome screen and rules       *
*************************************************************/
void welcome_screen(void);

/*************************************************************
* Function: initialize_board();                             *
* Description: initializes a game board                     *
* Input parameters: board, num of rows and num of columns   *
* Returns: Nothing                                          *
* Preconditions: board, num of rows and num of columns      *
* Postconditions: initializes a game board                  *
*************************************************************/
void initialize_board(char board[10][10], int num_rows, int num_cols);

/*************************************************************
* Function: print_board();                                  *
* Description: prints a game board                          *
* Input parameters: board, num of rows and num of columns   *
* Returns: Nothing                                          *
* Preconditions: board, num of rows, and num of columns     *
* Postconditions: prints a game board                       *
*************************************************************/
void print_board(char board[10][10], int num_rows, int num_cols);

/*************************************************************
* Function: initialize_stats();                             *
* Description: initializes the stats                        *
* Input parameters: boats and stats                         *
* Returns: Nothing                                          *
* Preconditions: boats and stats                            *
* Postconditions: initializes the stats                     *
*************************************************************/
void initialize_stats(Boats* boats, Stats* stats);

/*************************************************************
* Function: get_char();                                     *
* Description: gets an input character                      *
* Input parameters: None                                    *
* Returns: input                                            *
* Preconditions: None                                       *
* Postconditions: get an input character                    *
*************************************************************/
char get_char(void);

/*************************************************************
* Function: random_or_manual_ships();                       *
* Description: decides to randomly or manually place ships  *
* Input parameters: board                                   *
* Returns: Nothing                                          *
* Preconditions: board                                      *
* Postconditions: places ships on board                     *
*************************************************************/
void random_or_manual_ships(char board[10][10]);

/*************************************************************
* Function: manually_place_ships();                         *
* Description: manually places ships                        *
* Input parameters: board                                   *
* Returns: Nothing                                          *
* Preconditions: board                                      *
* Postconditions: manually places ships in board            *
*************************************************************/
void manually_place_ships(char board[10][10]);

/*************************************************************
* Function: randomly_place_ships();                         *
* Description: randomly places ships                        *
* Input parameters: board and player                        *
* Returns: Nothing                                          *
* Preconditions: board and player                           *
* Postconditions: randomly places ships in board            *
*************************************************************/
void randomly_place_ships(char board[10][10], char player);

/*************************************************************
* Function: random_position();                              *
* Description: randomly generates a row, column, and        *
*			   direction                                    *
* Input parameters: row, col, direction, size               *
* Returns: Nothing                                          *
* Preconditions: row, col, direction, size                  *
* Postconditions: randomly generates a row, column, and     *
*				  direction                                 *
*************************************************************/
void random_position(int* row, int* col, int* direction, int size);

/*************************************************************
* Function: place_ship();                                   *
* Description: places ships on board                        *
* Input parameters: row, column, board, boat, direction,	*
*					and player								*
* Returns: Nothing                                          *
* Preconditions: row, column, board, boat, direction,	    *
*					and player                              *
* Postconditions: places ships on board                     *
*************************************************************/
void place_ship(int row, int column, char board[10][10], char boat, char direction, char player);

/*************************************************************
* Function: check_if_on_board();                            *
* Description: checks if input is on board                  *
* Input parameters: row, col, direction, size, board        *
* Returns: check                                            *
* Preconditions: row, col, direction, size, board           *
* Postconditions: check                        		        *
*************************************************************/
int check_if_on_board(int row, int col, char direction, int size, char board[10][10]);

/*************************************************************
* Function: check_overlap();                                *
* Description: checks if input overlaps itself on board     *
* Input parameters: row, col, direction, size, board        *
* Returns: chech for overlap                                *
* Preconditions: row, col, direction, size, board           *
* Postconditions: check for overlap                         *
*************************************************************/
int check_overlap(int row, int col, char direction, int size, char board[10][10]);

/*************************************************************
* Function: select_who_starts_first();                      *
* Description: selects who goes first                       *
* Input parameters: player                                  *
* Returns: Nothing                                          *
* Preconditions: player                                     *
* Postconditions: decides who goes first                    *
*************************************************************/
void select_who_starts_first(int* player);

/*************************************************************
* Function: ship_type();                                    *
* Description: decides the character for the ship           *
* Input parameters: boat and player                         *
* Returns: ship character                                   *
* Preconditions: boat and player                            *
* Postconditions: character for the ship                    *
*************************************************************/
char ship_type(char boat, char player);

/*************************************************************
* Function: size();                                         *
* Description: determines size dependant on the ship        *
* Input parameters: boat                                    *
* Returns: size of ship                                     *
* Preconditions: boat                                       *
* Postconditions: size of ship                              *
*************************************************************/
int size(char boat);
/*************************************************************
* Function: one_turn();                                     *
* Description: executes one turn of battleship              *
* Input parameters: board, hidden_board, player, playerboat,*
*					playerstat, outfile						*
* Returns: Nothing                                          *
* Preconditions: board, hidden_board, player, playerboat,   *
*				 playerstat, outfile		                *
* Postconditions: executes one turn of battleship           *
*************************************************************/
void one_turn(char board[10][10], char hidden_board[10][10], char player, Boats* playerboat, Stats* playerstat, FILE* outfile);

/*************************************************************
* Function: check_shot();                                   *
* Description: checks if shot is valid and adjusts board    *
* Input parameters: board, hidden_board, player, playerboat,*
*					playerstat, outfile						*
* Returns: check                                            *
* Preconditions: board, hidden_board, row, col, playerboat, *
*				 playerstat, outfile, player                *
* Postconditions: check                                     *
*************************************************************/
int check_shot(char board[10][10], char hidden_board[10][10], int row, int col, Boats* playerboat, Stats* playerstat, FILE* outfile, char player);

/*************************************************************
* Function: random_shot();                                  *
* Description: randomly shoots at a location                *
* Input parameters: row and column                          *
* Returns: Nothing                                          *
* Preconditions: row and column                             *
* Postconditions: randomly shoots at a location             *
*************************************************************/
void random_shot(int* row, int* column);

/*************************************************************
* Function: game_turns();                                   *
* Description: executes the rounds and stops if there is a	*
*			   winner										*
* Input parameters: player1 board, computer board, empty	*
*					board, player, playerboat, playerstat,	*
*					computerboat, computerstat, outfile     *
* Returns: Nothing                                          *
* Preconditions: player1 board, computer board, empty		*
*					board, player, playerboat, playerstat,	*
*					computerboat, computerstat, outfile     *
* Postconditions: executes rounds and stops if winner found *
*************************************************************/
void game_turns(char player1[10][10], char computer[10][10], char empty[10][10], int player, Boats* playerboat, Stats* playerstat, Boats* computerboat, Stats* computerstat, FILE* outfile);

/*************************************************************
* Function: is_winner();                                    *
* Description: determines if winner is found                *
* Input parameters: player                                  *
* Returns: winner                                           *
* Preconditions: player                                     *
* Postconditions: returns 1 if winner found                 *
*************************************************************/
int is_winner(Boats* player);

/*************************************************************
* Function: final_stats();                                  *
* Description: prints final stats of round to outfile       *
* Input parameters: player, computer, outfile               *
* Returns: Nothing                                          *
* Preconditions: player, computer, outfile                  *
* Postconditions: prints final stats to outfile             *
*************************************************************/
void final_stats(Stats* player, Stats* computer, FILE* outfile);

/*************************************************************
* Function: hit_miss_ratio();                               *
* Description: determines the hit to miss ratio             *
* Input parameters: player, computer                        *
* Returns: Nothing                                          *
* Preconditions: player, computer                           *
* Postconditions: determines the hit to miss ratio          *
*************************************************************/
void hit_miss_ratio(Stats* player, Stats* computer);

#endif