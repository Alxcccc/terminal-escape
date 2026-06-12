#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

typedef struct {
    char matrix[10][10];
    size_t rows;
    size_t columns;
} Map;

typedef struct {
    const char token;
    int x;
    int y;
} Object;

// Function prototypes
void map_init(Map* m);
void showMap(Map map);
void moveObject(Object* player, int deltaX, int deltaY, Object wall, Object enemy, int spawn[2], Map* m);

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

        // Movement Keys - Look how clean this is now!
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

        Sleep(500); 
    }
    
    return 0;
}

void map_init(Map* m){
    m->rows = 10;
    m->columns = 10;
    memset(m->matrix, '#', sizeof(m->matrix));
}

void showMap(Map m) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.columns; j++) {
            printf("%c ", m.matrix[i][j]);
        }
        printf("\n");
    }
}

// The new universal movement function
void moveObject(Object* player, int deltaX, int deltaY, Object wall, Object enemy, int spawn[2], Map* m) {
    // 1. Calculate where the player WANTS to go
    int nextX = player->x + deltaX;
    int nextY = player->y + deltaY;

    // 2. Map Boundary Check (Fixed the rows vs columns bug here)
    if (nextX < 0 || nextX >= m->rows || nextY < 0 || nextY >= m->columns) {
        return; // Out of bounds, stop function immediately
    }

    // 3. Wall Collision Check (Affirmative logic is much easier to read)
    if (nextX == wall.x && nextY == wall.y) {
        return; // Hit a wall, stop function immediately
    }

    // 4. Enemy Collision (Respawn Logic)
    if (nextX == enemy.x && nextY == enemy.y) {
        m->matrix[player->x][player->y] = '#'; // Clear old spot
        player->x = spawn[0];                  // Reset coordinates
        player->y = spawn[1];
        m->matrix[player->x][player->y] = player->token; // Draw at spawn
        return; 
    }

    // 5. Normal Movement (If we made it past all the checks above)
    m->matrix[nextX][nextY] = player->token;   // Draw player at new spot
    m->matrix[player->x][player->y] = '#';     // Leave a trail behind
    player->x = nextX;                         // Update player's actual X
    player->y = nextY;                         // Update player's actual Y
}