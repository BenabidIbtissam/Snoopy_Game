#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <string.h>

const int TIME_LIMIT =  100 ;
int elapsedTime = 0;

void display_rules()
{
    printf("Règles du jeu :\n");
    printf("1. Le but est de récupérer les 4 oiseaux aux 4 coins du niveau en un temps imparti.\n");
    printf("2. psnoopy se déplace avec les touches directionnelles du clavier.\n");
    printf("3. La balle rebondit constamment dans le niveau pour freiner psnoopy.\n");
    printf("4. Il y a des pièges comme des téléporteurs, des cases piégées et des blocs.\n");
    printf("5. Il y a 3 niveaux au total, avec un mot de passe pour chaque niveau.\n");
    printf("6. Chaque niveau a un timer de 120 secondes. Si le temps atteint 0, le joueur perd une vie.\n");
    printf("7. Le score est calculé en fonction du temps restant à la fin de chaque niveau.\n");
    printf("8. Le joueur peut sauvegarder sa partie en appuyant sur la touche 's'.\n");
    printf("9. Pour charger une partie, sélectionnez 'Charger une partie' dans le menu principal.\n");
    printf("10. Les touches spéciales : 'p' pour mettre le jeu en pause.\n");
    printf("Bonne chance et amusez-vous bien !\n");
}

// Function to initialize the matrix with empty spaces (' ')
void initialize_matrix(GameData *game)
{

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            game->matrix[i][j] = ' ';
        }
    }
}

// Function to place psnoopy in the center of the matrix
void place_psnoopy_center(GameData *game)
{
    game->psnoopy = (Snoopy *)malloc(sizeof(Snoopy));
    if (game->psnoopy == NULL)
    {
        printf("Erreur : Échec de l'allocation mémoire pour la balle.\n");
        return; // Exit the function if allocation fails
    }
    game->psnoopy->position.x = COLS / 2;
    game->psnoopy->position.y = ROWS / 2;
    game->matrix[game->psnoopy->position.y][game->psnoopy->position.x] = 'S';
}

// Function to place birds at the corners of the matrix
void place_birds_corners(GameData *game)
{
    game->pbird = (Bird *)malloc(sizeof(Bird));
    if (game->pbird == NULL)
    {
        printf("Erreur : Échec de l'allocation mémoire pour la balle.\n");
        return; // Exit the function if allocation fails
    }
    game->matrix[0][COLS - 1] = '#';        // Top-right corner
    game->matrix[ROWS - 1][0] = '#';        // Bottom-left corner
    game->matrix[ROWS - 1][COLS - 1] = '#'; // Bottom-right corner
    game->matrix[0][0] = '#';
}

// Function to place the ball before the last bird
void place_ball(GameData *game)
{
    game->pball = (Ball *)malloc(sizeof(Ball));
    if (game->pball == NULL)
    {
        printf("Erreur : Échec de l'allocation mémoire pour la balle.\n");
        return; // Exit the function if allocation fails
    }
    game->pball->position.x = ROWS-1;
    game->pball->position.y = COLS-2;
    game->matrix[game->pball->position.x][game->pball->position.y ] = 'O';
}

void initialize_score_lives(GameData *game)
{
    game->score = 0;
    game->lives = 3;
}

void place_walls(GameData *game)
{
    srand((unsigned int)time(NULL));
    const int MAX_OBSTACLES = 6;
    int obstaclePositions[MAX_OBSTACLES][2]; // Array to store obstacle positions

    for (int i = 0; i < MAX_OBSTACLES; ++i)
    {
        int randX, randY;
        int collision;
        do
        {
            randX = rand() % ROWS; // Generate a random X coordinate
            randY = rand() % COLS; // Generate a random Y coordinate
            collision = 0;         // Reset collision flag

            // Check collision with Snoopy
            if (randX == game->psnoopy->position.x && randY == game->psnoopy->position.y)
            {
                collision = 1;
            }
            // Check collision with the ball
            if (randX == game->pball->position.x && randY == game->pball->position.y)
            {
                collision = 1;
            }
            // Check collision with birds
            if ((randX == 0 && randY == 0) ||
                (randX == 0 && randY == COLS - 1) ||
                (randX == ROWS - 1 && randY == 0) ||
                (randX == ROWS - 1 && randY == COLS - 1))
            {
                collision = 1;
            }
            // Check collision with other obstacles
            for (int j = 0; j < i; ++j)
            {
                if (randX == obstaclePositions[j][0] && randY == obstaclePositions[j][1])
                {
                    collision = 1;
                    break;
                }
            }
        } while (collision); // Repeat until a valid position is found

        // Store the obstacle position in the array
        obstaclePositions[i][0] = randX;
        obstaclePositions[i][1] = randY;

        // Place the obstacles
        if (i < 3)
        {
            game->matrix[randX][randY] = '&'; // Place the first type of obstacle
        }
        else
        {
            game->matrix[randX][randY] = '*'; // Place the second type of obstacle
        }
    }
}

void initialize_password(GameData *game){
 game->plevel->idLevel = 1  ;
     strcpy(game->plevel->password, "ibtissam123"); 
}


// Function to initialize the level
void initialize_level(GameData *game)
{
    initialize_password(game) ;
    initialize_matrix(game);

    place_psnoopy_center(game);

    // Place birds at the corners of the matrix
    place_birds_corners(game);

    // Place the ball before the last bird
    place_ball(game);

    // Randomly place blocks and traps
    place_walls(game);
    initialize_score_lives(game);
}



// Function to display the game matrix with separators and additional information
void display_game(GameData *game)
{
    system("clear");
    printf("Score: %d\tLives: %d\n", game->score, game->lives);

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            printf(" %c ", game->matrix[i][j]); // Display each element of the game matrix with spaces
            if (j < COLS - 1)
            {
                printf("|"); // Add vertical separator between columns
            }
        }
        printf("\n");
        if (i < ROWS - 1)
        {
            for (int k = 0; k < COLS; ++k)
            {
                printf("---"); // Add horizontal separator between ROW
                if (k < COLS - 1)
                {
                    printf("+"); // Add crossing point between horizontal and vertical separators
                }
            }
            printf("\n");
        }
    }
}

// Function to get a character from the terminal without waiting for enter
char getch()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

void move_snoopy(GameData *game)
{
    int dx = -1 , dy = -1;
    char input;
      time_t startTime = time(NULL);
      printf("Use arrow keys to move Snoopy. Press 'q' to quit.\n");

    while (game->lives >0)
    {
        time_t currentTime = time(NULL);
        elapsedTime = difftime(currentTime, startTime);
        display_game(game);
        printf("Time Remaining: %d seconds\n", TIME_LIMIT - elapsedTime);
        
        int snoopyRow, snoopyCol;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (game->matrix[i][j] == 'S')
                {
                    snoopyRow = i;
                    snoopyCol = j;
                }
            }
        }


        int newX = game->pball->position.x + dx;
        int newY = game->pball->position.y + dy;

     
        input = getch();

        // Check arrow key input
        if (input == '\033')
        {            // Arrow keys are represented by escape sequences
            getch(); // Skip '['
            switch (getch())
            {
            case 'A':
                if (snoopyRow > 0)
                {
                    if (game->matrix[snoopyRow - 1][snoopyCol] == '#')
                    {
                        game->score++;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow - 1][snoopyCol] = 'S';
                    }
                    else if (game->matrix[snoopyRow - 1][snoopyCol] == '&')
                    {
                    }
                    else if (game->matrix[snoopyRow - 1][snoopyCol] == '*')
                    {
                        game->lives--;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow - 1][snoopyCol] = 'S';
                    }
                    else if(snoopyRow - 1 == game->pball->position.x && snoopyCol == game->pball->position.y) {
                        
                        game->lives--;
                    }
                    else
                    {
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow - 1][snoopyCol] = 'S';
                    }
                }
                break;
            case 'B':
                if (snoopyRow < ROWS - 1)
                {
                    if (game->matrix[snoopyRow + 1][snoopyCol] == '#')
                    {
                        game->score++;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow + 1][snoopyCol] = 'S';
                    }
                    else if (game->matrix[snoopyRow + 1][snoopyCol] == '&')
                    {
                        // Obstacle blocking the way
                        // No movement
                    }  else if (snoopyRow + 1 == game->pball->position.x && snoopyCol == game->pball->position.y)
                    {
                        game->lives--;
                    }
                    else if (game->matrix[snoopyRow + 1][snoopyCol] == '*')
                    {
                        game->lives--;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow + 1][snoopyCol] = 'S';
                    }
                    else
                    {
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow + 1][snoopyCol] = 'S';
                    }
                }
                break;
            case 'C':
                if (snoopyCol < COLS - 1)
                {
                    if (game->matrix[snoopyRow][snoopyCol + 1] == '#')
                    {
                        game->score++;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow][snoopyCol + 1] = 'S';
                    }
                    else if (game->matrix[snoopyRow][snoopyCol + 1] == '&')
                    {
                        // Obstacle blocking the way
                        // No movement
                    }else if (snoopyRow  == game->pball->position.x && snoopyCol + 1 == game->pball->position.y)
                    {
                        game->lives--;
                    }
                    else if (game->matrix[snoopyRow][snoopyCol + 1] == '*')
                    {
                        game->lives--;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow][snoopyCol + 1] = 'S';
                    }
                    else
                    {
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow][snoopyCol + 1] = 'S';
                    }
                }
                break;

            case 'D':
                if (snoopyCol > 0)
                {
                    if (game->matrix[snoopyRow][snoopyCol - 1] == '#')
                    {
                        game->score++;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow][snoopyCol - 1] = 'S';
                    }
                    else if (game->matrix[snoopyRow][snoopyCol - 1] == '&')
                    {
                        // Obstacle blocking the way
                        // No movement
                    } else if (snoopyRow  == game->pball->position.x && snoopyCol -1 == game->pball->position.y)
                    {
                        game->lives--;
                    }
                    else if (game->matrix[snoopyRow][snoopyCol - 1] == '*')
                    {
                        game->lives--;
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow][snoopyCol - 1] = 'S';
                    }
                    else
                    {
                        game->matrix[snoopyRow][snoopyCol] = ' ';
                        game->matrix[snoopyRow][snoopyCol - 1] = 'S';
                    }
                }
                break;
            }
        }
        else if (input == 'q' || input == 'Q')
        {
            printf("Exiting...\n");
            break;
        }

        
       if (game->score == 4) {
        printf("Congrats !");

            break;
        }

           // Check if the new position is within bounds
        if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS) {
            game->matrix[game->pball->position.x][game->pball->position.y] = ' ';
            game->pball->position.x = newX;
            game->pball->position.y = newY;
            game->matrix[newX][newY] = '0';
        } else {
            // If ball reaches the matrix boundaries, change direction
            dx = -dx; // Invert direction on x-axis
            dy = -dy; // Invert direction on y-axis
        }

    }

    if (game->lives == 0 ) {
        printf("Game Over! You ran out of lives.\n");
        printf("Final Score: %d\n", game->score);
    } else if (elapsedTime >= TIME_LIMIT) {
        printf("Game Over! You ran out of time.\n");
        printf("Final Score: %d\n", game->score);
    }  else {
            printf("Final Score: %d\n", game->score);
    }
}

void launch_new_game(GameData *game, int num) {
    if (num == 1) {
        initialize_level(game);
        move_snoopy(game);
    } else if (num == 2) {
        char password[20]; // Assuming a maximum length for the password

            printf("Enter password: ");
            scanf("%s", password);

            if (strcmp(password, "ibtissam123") == 0) {
                initialize_level(game);
                move_snoopy(game);
            } else {
                printf("Wrong password\n");
            }
        }
    
}
