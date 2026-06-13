#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

/* 10x10 grid representing the game world, initially filled with '#' walls */
typedef struct {
    char matrix[10][10];
    size_t rows;
    size_t columns;
} Map;

/* Entity on the map with a display token and grid coordinates */
typedef struct {
    const char token;
    int x;
    int y;
} Object;

// Function prototypes
void map_init(Map* m);
void showMap(Map map);
void moveObject(Object* player, int deltaX, int deltaY, Object wall, Object enemy, int spawn[2], Map* m);

/* Game loop: read input, update state, render map until win or ESC */
int main(){
    Map map;
    map_init(&map);

    Object player = {'P', 9, 5};
    Object wall = {'W', 3, 4};
    Object enemy = {'O', 3, 9};
    Object exit = {'E', 0, 0};

    int playerSpawn[2] = {player.x, player.y};

    map.matrix[player.x][player.y] = player.token;
    map.matrix[wall.x][wall.y] = wall.token;
    map.matrix[enemy.x][enemy.y] = enemy.token;
    map.matrix[exit.x][exit.y] = exit.token;

    while (1) {
        // Win Condition
        if (player.x == exit.x && player.y == exit.y) {
            system("cls");
            printf("Nice you got to the exit, you won!!!!");
            break;
        }

        // Exit Key
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Exiting program...\n");
            break;
        }

        // Movement Keys
        if (GetAsyncKeyState('W') & 0x8000) {
            moveObject(&player, -1, 0, wall, enemy, playerSpawn, &map);
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            moveObject(&player, 1, 0, wall, enemy, playerSpawn, &map);
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            moveObject(&player, 0, -1, wall, enemy, playerSpawn, &map);
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            moveObject(&player, 0, 1, wall, enemy, playerSpawn, &map);
        }

        system("cls");
        showMap(map);

        Sleep(400); 
    }
    
    return 0;
}

/* Fill entire matrix with '#' wall tiles */
void map_init(Map* m){
    m->rows = 10;
    m->columns = 10;
    memset(m->matrix, '#', sizeof(m->matrix));
}

/* Print the game grid to the console */
void showMap(Map m) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.columns; j++) {
            printf("%c ", m.matrix[i][j]);
        }
        printf("\n");
    }
}

/* Move the player by (deltaX, deltaY) handling collisions with walls and enemies */
/*
 * Collision order: bounds -> wall (blocked) -> enemy (respawn) -> move
 */
void moveObject(Object* player, int deltaX, int deltaY, Object wall, Object enemy, int spawn[2], Map* m) {

    int nextX = player->x + deltaX;
    int nextY = player->y + deltaY;

    // Out of bounds
    if (nextX < 0 || nextX >= m->rows || nextY < 0 || nextY >= m->columns) {
        return;
    }

    // Wall collision — blocked
    if (nextX == wall.x && nextY == wall.y) {
        return;
    }
    
    // Enemy collision — respawn to spawn point
    if (nextX == enemy.x && nextY == enemy.y) {
        m->matrix[player->x][player->y] = '#';
        player->x = spawn[0];
        player->y = spawn[1];
        m->matrix[player->x][player->y] = player->token;
        return; 
    }

    // Normal move
    m->matrix[nextX][nextY] = player->token;
    m->matrix[player->x][player->y] = '#';   
    player->x = nextX;  
    player->y = nextY;
}
