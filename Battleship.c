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
*																		*
************************************************************************/

#include "battleship.h"

void game(void)
{
	srand((unsigned int)time(NULL));
	char p1_board[NUM_ROWS][NUM_COLS] = { { '\0' } }, comp_board[NUM_ROWS][NUM_COLS] = { { '\0' } }, hidden_board[NUM_ROWS][NUM_COLS] = { { '\0' } };
	int player = 0;

	FILE* outfile = NULL;
	//sets outfile to write in the output file
	outfile = fopen("battleship.log", "w");

	//welcome screen
	welcome_screen(); //welcome screen

	//sets the stats and boats to the structs
	Stats p1stats;
	Stats compstats;
	Boats p1boats;
	Boats compboats;

	//initializes the stats
	initialize_stats(&p1boats, &p1stats);
	initialize_stats(&compboats, &compstats);

	//intializes the boards
	initialize_board(p1_board, NUM_ROWS, NUM_COLS);
	initialize_board(comp_board, NUM_ROWS, NUM_COLS);
	initialize_board(hidden_board, NUM_ROWS, NUM_COLS);

	//determines if player1 wants to manually place ships or randomly
	random_or_manual_ships(p1_board);
	//randomly places ships for player2
	randomly_place_ships(comp_board, 'c');
	//prints player1 board
	print_board(p1_board, NUM_ROWS, NUM_COLS);
	printf("This is your final game board!\n");
	//randomly decides who goes first
	select_who_starts_first(&player);

	printf("The computer's board has been generated!\n");
	system("pause");
	system("cls");
	//runs through the rounds until a winner is found
	game_turns(p1_board, comp_board, hidden_board, player, &p1boats, &p1stats, &compboats, &compstats, outfile);


	fclose(outfile);
}

void welcome_screen(void) //welcome screen and rules for battleship
{
	printf("***** Welcome to Battleship! *****\n");
	printf("Here are the rules:\n");
	printf("1. This is a two-player game.\n");
	printf("2. You are Player 1 and the computer is Player 2.\n");
	printf("3. You will be asked to choose the location of 5 ships on a 10x10 board.\n");
	printf("4. Your ships consist of a 5 cell Carrier (c), a 4 cell Battleship (b), a 3 cell Cruiser (r),\n");
	printf("   a 3 cell Submarine (s), and a 2 cell Destroyer (d).\n");
	printf("5. It is randomly decided whether you or the computer goes first.\n");
	printf("6. For each turn, you will be asked pick a spot on your opponent's board to try and sink their ships.\n");
	printf("7. (m) means that you missed their ship, while (*) means that you hit their ship.\n");
	printf("8. If you sink all their ships, you WIN!\n");
	printf("9. If all your ships are sunk, you LOSE!\n\n");
	printf("**********************************\n");
	system("pause");
	system("cls");
}

void initialize_board(char board[10][10], int num_rows, int num_cols) //initializes a board
{
	int row_index = 0, col_index = 0;

	for (; row_index < num_rows; ++row_index)
	{
		for (col_index = 0; col_index < num_cols; ++col_index)
		{
			board[row_index][col_index] = '~';
		}
	}
}

void print_board(char board[10][10], int num_rows, int num_cols) //prints a board
{
	int row_index = 0, col_index = 0;

	printf("   0 1 2 3 4 5 6 7 8 9\n");
	for (; row_index < num_rows; ++row_index)
	{
		printf("%d  ", row_index);
		for (col_index = 0; col_index < num_cols; ++col_index)
		{
			printf("%c ", board[row_index][col_index]);
		}
		putchar('\n');
	}
}

void initialize_stats(Boats* boats, Stats* stats) //initializes stats
{
	boats->carrier = 0;
	boats->battleship = 0;
	boats->cruiser = 0;
	boats->submarine = 0;
	boats->destroyer = 0;
	boats->ships_sunk = 0;
	stats->total_hits = 0;
	stats->total_misses = 0;
	stats->total_shots = 0;
	stats->hits_misses = 0;
}

char get_char(void) //gets character input
{
	char input = '\0';

	scanf(" %c", &input);

	return input;
}

void random_or_manual_ships(char board[10][10]) //decides if player1 wants randomly or manually placed ships
{
	char selection = '\0';
	//prompts user for manual or random
	printf("Would you like to place your ships on the board or randomly spawn them?(m for manual or r for random): ");
	selection = get_char();

	//reprompts user if it is an invalid selection
	while (selection != 'm' && selection != 'M' && selection != 'r' && selection != 'R')
	{
		printf("That is an invalid selection, please choose again!: ");
		selection = get_char();
	}
	//calls manually_place_ships function if user chooses m or M
	if (selection == 'm' || selection == 'M')
	{
		system("cls");
		manually_place_ships(board);
	}
	//calls randomly_place_ships if not m or M
	else
	{
		system("cls");
		randomly_place_ships(board, 'p');
	}
}

void manually_place_ships(char board[10][10]) //manually places ships on board
{
	int row = 0, col = 0, check = 0, overlap = 0;
	char direction = '\0';

	//prints board and prompts user for first coordinate and direction of the ship to be placed
	print_board(board, NUM_ROWS, NUM_COLS);
	printf("Choose the first coordinate for your carrier ship (5 spaces) and then the direction it will point towards, \n");
	printf("down or right(row column (d for down or r for right)), ex. 4 5 r): \n");
	scanf("%d %d %c", &row, &col, &direction);

	//checks if ship is in a valid location on board and if it overlaps another ship
	check = check_if_on_board(row, col, direction, 5, board);
	overlap = check_overlap(row, col, direction, 5, board);

	//loops while check or overlap is invalid
	while (check == 0 || overlap == 0)
	{
		printf("You cannot place your boat there, please choose another position: ");
		scanf("%d %d %c", &row, &col, &direction);
		check = check_if_on_board(row, col, direction, 5, board);
		overlap = check_overlap(row, col, direction, 5, board);
	}
	//places carrier ship
	place_ship(row, col, board, 'c', direction, 'p');
	system("cls");

	//repeats for battleship
	print_board(board, NUM_ROWS, NUM_COLS);
	printf("Choose the first coordinate for your battleship (4 spaces) and then the direction it will point towards, \n");
	printf("down or right(row column (d for down or r for right)), ex. 4 5 r): \n");
	scanf("%d %d %c", &row, &col, &direction);

	check = check_if_on_board(row, col, direction, 4, board);
	overlap = check_overlap(row, col, direction, 4, board);

	while (check == 0 || overlap == 0)
	{
		printf("You cannot place your boat there, please choose another position: ");
		scanf("%d %d %c", &row, &col, &direction);
		check = check_if_on_board(row, col, direction, 4, board);
		overlap = check_overlap(row, col, direction, 4, board);
	}

	place_ship(row, col, board, 'b', direction, 'p');
	system("cls");

	//repeats for cruiser
	print_board(board, NUM_ROWS, NUM_COLS);
	printf("Choose the first coordinate for your cruiser (3 spaces) and then the direction it will point towards, \n");
	printf("down or right(row column (d for down or r for right)), ex. 4 5 r): \n");
	scanf("%d %d %c", &row, &col, &direction);

	check = check_if_on_board(row, col, direction, 3, board);
	overlap = check_overlap(row, col, direction, 3, board);

	while (check == 0 || overlap == 0)
	{
		printf("You cannot place your boat there, please choose another position: ");
		scanf("%d %d %c", &row, &col, &direction);
		check = check_if_on_board(row, col, direction, 3, board);
		overlap = check_overlap(row, col, direction, 3, board);
	}

	place_ship(row, col, board, 'r', direction, 'p');
	system("cls");

	//repeats for submarine
	print_board(board, NUM_ROWS, NUM_COLS);
	printf("Choose the first coordinate for your submarine (3 spaces) and then the direction it will point towards, \n");
	printf("down or right(row column (d for down or r for right)), ex. 4 5 r): \n");
	scanf("%d %d %c", &row, &col, &direction);

	check = check_if_on_board(row, col, direction, 3, board);
	overlap = check_overlap(row, col, direction, 3, board);

	while (check == 0 || overlap == 0)
	{
		printf("You cannot place your boat there, please choose another position: ");
		scanf("%d %d %c", &row, &col, &direction);
		check = check_if_on_board(row, col, direction, 3, board);
		overlap = check_overlap(row, col, direction, 3, board);
	}

	place_ship(row, col, board, 's', direction, 'p');
	system("cls");

	//repeats for destroyer
	print_board(board, NUM_ROWS, NUM_COLS);
	printf("Choose the first coordinate for your destroyer (2 spaces) and then the direction it will point towards, \n");
	printf("down or right(row column (d for down or r for right)), ex. 4 5 r): \n");
	scanf("%d %d %c", &row, &col, &direction);

	check = check_if_on_board(row, col, direction, 2, board);
	overlap = check_overlap(row, col, direction, 2, board);

	while (check == 0 || overlap == 0)
	{
		printf("You cannot place your boat there, please choose another position: ");
		scanf("%d %d %c", &row, &col, &direction);
		check = check_if_on_board(row, col, direction, 2, board);
		overlap = check_overlap(row, col, direction, 2, board);
	}

	place_ship(row, col, board, 'd', direction, 'p');
	system("cls");
}

void randomly_place_ships(char board[10][10], char player) //randomly places ships on board
{
	int row = 0, col = 0, direct = 0, overlap = 0;
	char direction = '\0';

	//while the ships don't overlap, randomly place carrier
	do
	{
		random_position(&row, &col, &direct, 5);
		if (direct == 1)
		{
			direction = 'd';
		}
		else
		{
			direction = 'r';
		}

		overlap = check_overlap(row, col, direction, 5, board);
	} while (overlap == 0);

	place_ship(row, col, board, 'c', direction, player);

	//while ships don't overlap, randomly place battleship
	do
	{
		random_position(&row, &col, &direct, 4);
		if (direct == 1)
		{
			direction = 'd';
		}
		else
		{
			direction = 'r';
		}

		overlap = check_overlap(row, col, direction, 4, board);
	} while (overlap == 0);

	place_ship(row, col, board, 'b', direction, player);

	//while ships don't overlap, randomly place cruiser
	do
	{
		random_position(&row, &col, &direct, 3);
		if (direct == 1)
		{
			direction = 'd';
		}
		else
		{
			direction = 'r';
		}

		overlap = check_overlap(row, col, direction, 3, board);
	} while (overlap == 0);

	place_ship(row, col, board, 'r', direction, player);

	//while ships don't overlap, randomly place submarine
	do
	{
		random_position(&row, &col, &direct, 3);
		if (direct == 1)
		{
			direction = 'd';
		}
		else
		{
			direction = 'r';
		}

		overlap = check_overlap(row, col, direction, 3, board);
	} while (overlap == 0);

	place_ship(row, col, board, 's', direction, player);

	//whiel ships don't overlap, randomly place destroyer
	do
	{
		random_position(&row, &col, &direct, 2);
		if (direct == 1)
		{
			direction = 'd';
		}
		else
		{
			direction = 'r';
		}

		overlap = check_overlap(row, col, direction, 2, board);
	} while (overlap == 0);

	place_ship(row, col, board, 'd', direction, player);
}

void random_position(int* row, int* col, int* direction, int size) //chooses a random row, column, and direction
{
	*row = rand() % (10 - size);
	*col = rand() % (10 - size);
	*direction = rand() % 2;
}

void place_ship(int row, int col, char board[10][10], char boat, char direction, char player) //places ships
{
	int i = 0;
	for (i = 0; i < size(boat); i++)
	{
		board[row][col] = ship_type(boat, player);
		//if direction is down, then increment the row
		if (direction == 'D' || direction == 'd')
		{
			row++;
		}
		//if direction is right, then increment the column
		else if (direction == 'R' || direction == 'r')
		{
			col++;
		}
	}
}

int check_if_on_board(int row, int col, char direction, int size, char board[10][10]) //checks if the user input is on the board
{
	int check = 0;
	//if on the board and direction is down, true
	if ((direction == 'D' || direction == 'd') && row <= 10 - size && row >= 0)
	{
		check = 1;
	}
	//if direction is right and on the board, true
	else if ((direction == 'R' || direction == 'r') && col <= 10 - size && row >= 0)
	{
		check = 1;
	}
	//else false
	else
	{
		check = 0;
	}
	return check;
}

int check_overlap(int row, int col, char direction, int size, char board[10][10])
{
	int i = 0, check = 0;

	//checks if each ship coordinate down is in valid water
	if (direction == 'D' || direction == 'd')
	{
		for (i = row; i < row + size; i++)
		{
			if (board[i][col] == '~')
			{
				check = 1;
			}
			else
			{
				check = 0;
				break;
			}
		}
	}
	//checks if each ship coordinate right is in valid water
	else if (direction == 'R' || direction == 'r')
	{
		for (i = col; i < col + size; i++)
		{
			if (board[row][i] == '~')
			{
				check = 1;
			}
			else
			{
				check = 0;
				break;
			}
		}
	}
	else
	{
		check = 0;
	}
	return check;
}

char ship_type(char boat, char player) //sets ship type based on player
{
	//if player is player1, sets all boats to correct character
	if (player == 'p')
	{
		if (boat == 'c')
		{
			return 'c';
		}
		else if (boat == 'b')
		{
			return 'b';
		}
		else if (boat == 'r')
		{
			return 'r';
		}
		else if (boat == 's')
		{
			return 's';
		}
		else
		{
			return 'd';
		}
	}
	//if player is player2, sets all boats to correct character
	else
	{
		if (boat == 'c')
		{
			return 'c';
		}
		else if (boat == 'b')
		{
			return 'b';
		}
		else if (boat == 'r')
		{
			return 'r';
		}
		else if (boat == 's')
		{
			return 's';
		}
		else
		{
			return 'd';
		}
	}
}

int size(char boat) //determines size of boat based on character
{
	if (boat == 'c')
	{
		return 5;
	}
	else if (boat == 'b')
	{
		return 4;
	}
	else if (boat == 'r')
	{
		return 3;
	}
	else if (boat == 's')
	{
		return 3;
	}
	else
	{
		return 2;
	}
}

void select_who_starts_first(int* player) //randomly selects who starts first
{
	//randomly chooses to go first
	*player = rand() % 2 + 1;
	if (*player == 1)
	{
		printf("You have been randomly chosen to go first!\n");
	}
	else
	{
		printf("The computer has been randomly chosen to go first!\n");
	}
}

void one_turn(char board[10][10], char hidden_board[10][10], char player, Boats* playerboat, Stats* playerstat, FILE* outfile) //executes one turn of battleship
{
	int row = 0, col = 0, shot = 0;

	//if player1
	if (player == 'p')
	{
		//prompts user for a coordinate
		printf("Please choose a target location (row column): ");
		scanf("%d %d", &row, &col);
		shot = check_shot(board, hidden_board, row, col, playerboat, playerstat, outfile, player);

		//while shot is invalid
		while (shot == -1)
		{
			printf("That location is invalid, choose another: ");
			scanf("%d %d", &row, &col);
			shot = check_shot(board, hidden_board, row, col, playerboat, playerstat, outfile, player);
		}
	}
	//if player2
	else
	{
		do
		{
			random_shot(&row, &col);
			shot = check_shot(board, hidden_board, row, col, playerboat, playerstat, outfile, player);
		} while (shot == -1);
	}
}

int check_shot(char board[10][10], char hidden_board[10][10], int row, int col, Boats* playerboat, Stats* playerstat, FILE* outfile, char player)
{
	//sets target to the coordinate
	char target = board[row][col];

	//if row and col is an invalid number
	if (row < 0 || row >= 10 || col < 0 || col >= 10)
	{
		return -1;
	}
	//if player1
	if (player == 'p')
	{
		//if target is water
		if (target == '~')
		{
			//sets coordinate to miss
			board[row][col] = 'm';
			hidden_board[row][col] = 'm';

			//increments total_misses and total_shots
			playerstat->total_misses += 1;
			playerstat->total_shots += 1;

			printf("Player1 selects: %d, %d\n", row, col);
			printf("%d, %d is a miss\n", row, col);

			//prints to outfile
			fprintf(outfile, "Player1");
			fprintf(outfile, ": %d,%d MISS\n", row, col);
			return 0;
		}
		//if target is already miss or hit, return invalid
		else if (target == 'm' || target == '*')
		{
			return -1;
		}
		else
		{
			//if target is carrier, increment carrier hit and print to outfile
			if (target == 'c')
			{
				board[row][col] = '*';
				hidden_board[row][col] = '*';

				playerboat->carrier += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("Player1 selects: %d, %d\n", row, col);
				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 1");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->carrier == 5)
				{
					printf("The Carrier has been sunk!\n\n");
					fprintf(outfile, "Sunk Carrier!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}

			//if target is battleship, increment battleship hit and print to outfile
			else if (target == 'b')
			{
				board[row][col] = '*';
				hidden_board[row][col] = '*';

				playerboat->battleship += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("Player1 selects: %d, %d\n", row, col);
				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 1");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->battleship == 4)
				{
					printf("The Battleship has been sunk!\n");
					fprintf(outfile, "Sunk Battleship!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}

			//if target is cruiser, increment cruiser hit and print to outfile
			else if (target == 'r')
			{
				board[row][col] = '*';
				hidden_board[row][col] = '*';

				playerboat->cruiser += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("Player1 selects: %d, %d\n", row, col);
				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->cruiser == 3)
				{
					printf("The Cruiser has been sunk!\n");
					fprintf(outfile, " Sunk Cruiser!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}

			//if target is submarine, increment submarine hit and print to outfile
			else if (target == 's')
			{
				board[row][col] = '*';
				hidden_board[row][col] = '*';

				playerboat->submarine += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("The computer selects: %d, %d\n", row, col);
				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->submarine == 3)
				{
					printf("The Submarine has been sunk!\n");
					fprintf(outfile, " Sunk Submarine!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}

			//if target is destroyer, increment destroyer hit and print to outfile
			else if (target == 'd')
			{
				board[row][col] = '*';
				hidden_board[row][col] = '*';

				playerboat->destroyer += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("The computer selects: %d, %d\n", row, col);
				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->destroyer == 2)
				{
					printf("The Destroyer has been sunk!\n");
					fprintf(outfile, " Sunk Destroyer!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}

	else //else repeat for player2
	{
		if (target == '~')
		{
			board[row][col] = 'm';

			playerstat->total_misses += 1;
			playerstat->total_shots += 1;

			printf("Player2 chose: %d, %d\n", row, col);
			printf("%d, %d is a miss\n", row, col);
			fprintf(outfile, "Player 2");
			fprintf(outfile, ": %d,%d MISS\n", row, col);
			return 0;
		}
		else if (target == 'm' || target == '*')
		{
			return -1;
		}
		else
		{
			if (target == 'c')
			{
				board[row][col] = '*';

				playerboat->carrier += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->carrier == 5)
				{
					printf("The Carrier has been sunk!\n\n");
					fprintf(outfile, "Sunk Carrier!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}


			else if (target == 'b')
			{
				board[row][col] = '*';

				playerboat->battleship += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->battleship == 4)
				{
					printf("The Battleship has been sunk!\n");
					fprintf(outfile, "Sunk Battleship!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}


			else if (target == 'r')
			{
				board[row][col] = '*';

				playerboat->cruiser += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->cruiser == 3)
				{
					printf("The Cruiser has been sunk!\n");
					fprintf(outfile, " Sunk Cruiser!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}


			else if (target == 's')
			{
				board[row][col] = '*';

				playerboat->submarine += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->submarine == 3)
				{
					printf("The Submarine has been sunk!\n");
					fprintf(outfile, " Sunk Submarine!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}


			else if (target == 'd')
			{
				board[row][col] = '*';

				playerboat->destroyer += 1;
				playerstat->total_hits += 1;
				playerstat->total_shots += 1;

				printf("%d,%d is a hit!\n", row, col);
				fprintf(outfile, "Player 2");
				fprintf(outfile, ": %d,%d HIT\n", row, col);

				if (playerboat->destroyer == 2)
				{
					printf("The Destroyer has been sunk!\n");
					fprintf(outfile, " Sunk Destroyer!\n");
					playerboat->ships_sunk += 1;
				}
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
}

void random_shot(int* row, int* column) //randomly chooses a shot
{
	*row = rand() % 10;
	*column = rand() % 10;
}

void game_turns(char player1[10][10], char computer[10][10], char hidden_board[10][10], int player, Boats* playerboat, Stats* playerstat, Boats* computerboat, Stats* computerstat, FILE* outfile) //executes rounds
{
	int i = player;
	int winner = 0;

	//loops while winner is not found
	while (winner == 0)
	{
		//determines if it is player1 or player2's turn
		if ((i % 2) == 1)
		{
			//prints boards
			printf("Player1 Board\n");
			print_board(player1, NUM_ROWS, NUM_COLS);
			printf("Player2 Board\n");
			print_board(computer, NUM_ROWS, NUM_COLS);

			//executes one turn for player1
			one_turn(computer, hidden_board, 'p', computerboat, playerstat, outfile);
			winner = is_winner(computerboat);
		}

		else
		{
			//prints boards
			printf("Player1 Board\n");
			print_board(player1, NUM_ROWS, NUM_COLS);
			printf("Player2 Board\n");
			print_board(computer, NUM_ROWS, NUM_COLS);

			//executes one turn for player2
			one_turn(player1, hidden_board, 'c', playerboat, computerstat, outfile);
			winner = is_winner(playerboat);
		}
		i++;

		//determines hit to miss ratio
		hit_miss_ratio(playerstat, computerstat);

		//if winner is the player1
		if (is_winner(computerboat) == 1)
		{
			printf("Player1 Wins!\n");
			fprintf(outfile, "Player1 wins, Player2 loses\n");
		}
		//if winner is player2
		if (is_winner(playerboat) == 1)
		{
			printf("Player2 Wins!\n");
			fprintf(outfile, "Player1 loses, Player2 wins\n");
		}

		//prints final stats
		final_stats(playerstat, computerstat, outfile);

		printf("Statistics outputted to logfile successfully!\n");
		system("pause");
		system("cls");
	}
}

int is_winner(Boats* player) //finds if there is a winner
{
	if (player->ships_sunk == 5)
	{
		return 1;
	}
	return 0;
}

void final_stats(Stats* player, Stats* computer, FILE* outfile) //final stats
{
	double h = 0, m = 0;
	fprintf(outfile, "\n*** Player1 Stats ***\n");
	fprintf(outfile, "Number Hits : %d\n", player->total_hits);
	fprintf(outfile, "Number Misses : %d\n", player->total_misses);
	fprintf(outfile, "Total Shots : %d\n", player->total_shots);
	fprintf(outfile, "Hit / Miss Ratio : %.2f%%\n", player->hits_misses * 100);
	fprintf(outfile, "\n*** Player2 Stats ***\n");
	fprintf(outfile, "Number Hits : %d\n", computer->total_hits);
	fprintf(outfile, "Number Misses : %d\n", computer->total_misses);
	fprintf(outfile, "Total Shots : %d\n", computer->total_shots);
	fprintf(outfile, "Hit / Miss Ratio : %.2f%%\n", computer->hits_misses * 100);
}

void hit_miss_ratio(Stats* player, Stats* computer) //determines hit to miss ratio
{
	int i = 0;
	double hit = 0, miss = 0;
	hit = player->total_hits;
	miss = player->total_misses;

	//if hits are greater than misses and misses is 0 for player1
	if (hit > miss && miss == 0)
	{
		player->hits_misses = 1;
	}
	else
	{
		player->hits_misses = hit / miss;
	}

	hit = computer->total_hits;
	miss = computer->total_misses;

	//if hits are greater than misses and misses is 0 for computer
	if (hit > miss && miss == 0)
	{
		computer->hits_misses = 1;
	}
	else
	{
		computer->hits_misses = hit / miss;
	}
}