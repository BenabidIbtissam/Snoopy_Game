#define MAX_LEVELS 3
#define MAX_FILENAME_LENGTH 50

#define ROWS 11
#define COLS 21

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int idLevel;
    char password[10];
} Level;

typedef struct {
    Position position;
} Bird;

typedef struct {
    Position position;
} Block;

typedef struct {
    Position position;
} Trap;

typedef struct {
    Position position;
} Snoopy;

typedef struct {
    Position position;
} Ball;

typedef struct {
    Level *plevel; // Niveau actuel du jeu
    int lives; // Nombre de vies restantes
    int score; // Score du joueur
    int time_remaining; // Temps restant
    int matrix [ROWS][COLS];
    Snoopy *psnoopy;
    Ball *pball;
    Bird *pbird;
    Block *pblocks;
    Trap *ptraps;
} GameData;


