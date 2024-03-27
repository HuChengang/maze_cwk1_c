#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} Position;

typedef struct {
    char **grid;
    int width, height;
    Position start, end;
} Maze;

typedef struct {
    Position position;
} Player;

Maze loadMaze(const char *filename, int width, int height);
void printMaze(const Maze *maze, const Player *player);
void movePlayer(Maze *maze, Player *player, char command);
int checkWin(const Player *player, const Maze *maze);
void freeMaze(Maze *maze);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <maze_file> <width> <height>\n", argv[0]);
        return 1;
    }

    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    Maze maze = loadMaze(argv[1], width, height);

    Player player;
    player.position = maze.start;

    printMaze(&maze, &player);

    while (1) {
        char command;
        printf("Enter move (WASD): ");
        scanf(" %c", &command); // Space before %c to consume any newline characters

        movePlayer(&maze, &player, command);
        printMaze(&maze, &player);

        if (checkWin(&player, &maze)) {
            printf("Congratulations, you reached the end!\n");
            break;
        }
    }

    freeMaze(maze);

    return 0;
}

Maze loadMaze(const char *filename, int width, int height) {
    Maze maze;
    maze.width = width;
    maze.height = height;
    maze.grid = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        maze.grid[i] = (char *)malloc((width + 1) * sizeof(char)); // +1 for '\0'
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[width + 2]; // +2 for '\n' and '\0'
    for (int i = 0; i < height; i++) {
        fgets(line, sizeof(line), file);
        for (int j = 0; j < width; j++) {
            maze.grid[i][j] = line[j];
            if (line[j] == 'S') {
                maze.start.x = i;
                maze.start.y = j;
            } else if (line[j] == 'E') {
                maze.end.x = i;
                maze.end.y = j;
            }
        }
    }

    fclose(file);

    return maze;
}

void printMaze(const Maze *maze, const Player *player) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (i == player->position.x && j == player->position.y)
                printf("X");
            else
                printf("%c", maze->grid[i][j]);
        }
        printf("\n");
    }
}

void movePlayer(Maze *maze, Player *player, char command) {
    int newX = player->position.x, newY = player->position.y;

    switch (command) {
        case 'W': newX--; break;
        case 'A': newY--; break;
        case 'S': newX++; break;
        case 'D': newY++; break;
        default: printf("Invalid move.\n"); return;
    }

    if (newX >= 0 && newX < maze->height && newY >= 0 && newY < maze->width && maze->grid[newX][newY] != '#') {
        player->position.x = newX;
        player->position.y = newY;
    } else {
        printf("Move blocked by a wall.\n");
    }
}

int checkWin(const Player *player, const Maze *maze) {
    return player->position.x == maze->end.x && player->position.y == maze->end.y;
}

void freeMaze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
}
