
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fucntions.h"
#include <unistd.h>
#include <termios.h>
#include <string.h>

int main() {

    GameData game;

int choice;

    do {
        // Displaying the menu options
        printf("\nMain Menu\n");
        printf("1. Game Rules\n");
        printf("2. Start a New Game from Level 1\n");
        printf("3. Load a Saved Game\n");
        printf("4. Start a Level Directly via its Password\n");
        printf("5. Scores\n");
        printf("6. Quit\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_rules();
                break;
            case 2:
                launch_new_game(&game,1);
                break;
            case 3:
               // loadSavedGame();
                break;
            case 4:
                launch_new_game(&game,2);
                break;
            case 5:
               // displayScores();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }

        // Pause the output (You can replace this based on your system's pause method)
        printf("\nPress Enter to continue...");
        getchar(); // Clear newline character from buffer
        getchar(); // Wait for user to press Enter
        system("clear"); // For clearing the console screen (use "cls" on Windows)
    } while (choice != 6);


    return 0;
}






