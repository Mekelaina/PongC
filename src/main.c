#include <stdio.h>
#include <time.h>
#include <raylib.h>

//Screen Constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Paddle Constants
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define LPADDLE_START_X 20
#define LPADDLE_START_Y (SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2)
#define RPADDLE_START_X 600
#define RPADDLE_START_Y (SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2)

// Ball Constants
#define BALL_WIDTH 20
#define BALL_HEIGHT 20
#define BALL_START_X ((SCREEN_WIDTH/2) - (BALL_WIDTH - BALL_WIDTH/4))
#define BALL_START_Y ((SCREEN_HEIGHT/2) - (BALL_HEIGHT - BALL_HEIGHT/4))
#define BALL_SPEED 5

// UI Constants
#define BORDER_SIZE 10
#define NET_WIDTH 8
#define NET_LENGTH 16
#define TEXT_SIZE 50
#define WINNING_SCORE 10

const Color bgColor = {
    .r = 54,
    .g = 55,
    .b = 55,
    .a = 255
};

const Color viniette = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 192
};

const char line1[] = "PRESS SPACEBAR";
const char line2[] = "TO SERVE!";

typedef struct {
    float x;
    float y;
    int width;
    int height;
} Paddle;

typedef struct {
    float x;
    float y;
    int width;
    int height;
    bool isNegX;
    bool isNegY;
    bool isVisible;
} Ball;

typedef struct {
    unsigned char player1;
    unsigned char player2;
} Score;

typedef struct {
    int x;
    int y;
    int width;
    int height;

    //couldnt think of a "better" place for this. its the number of rec that make up the "net"
    int net_count;
    bool isServed; 
} GameBoard;

Paddle left_paddle;
Paddle right_paddle;
Ball ball;
Score score;
GameBoard board;


void init() {
    left_paddle.x = LPADDLE_START_X;
    left_paddle.y = LPADDLE_START_Y;
    left_paddle.width = PADDLE_WIDTH;
    left_paddle.height = PADDLE_HEIGHT;
    
    right_paddle.x = RPADDLE_START_X;
    right_paddle.y = RPADDLE_START_Y;
    right_paddle.width = PADDLE_WIDTH;
    right_paddle.height = PADDLE_HEIGHT;

    ball.x = BALL_START_X;
    ball.y = BALL_START_Y;
    ball.width = BALL_WIDTH;
    ball.height = BALL_HEIGHT;
    ball.isNegX = false;
    ball.isNegY = false;
    ball.isVisible = false;
    score.player1 = 0;
    score.player2 = 0;

    board.x = 0;
    board.y = BORDER_SIZE;
    board.width = SCREEN_WIDTH;
    board.height = SCREEN_HEIGHT - (2 * BORDER_SIZE);
    board.net_count = (board.height/NET_LENGTH);
    board.isServed = false;
    
    //set seed for raylib RNG using system time
    SetRandomSeed((unsigned int) time(NULL));
}

char* getScoreAsString(unsigned char playerscore){
    switch(playerscore){
        
        case 0:
            return "0";
            break;
        case 1:
            return "1";
            break;
        case 2:
            return "2";
            break;
        case 3:
            return "3";
            break;
        case 4:
            return "4";
            break;
        case 5:
            return "5";
            break;
        case 6:
            return "6";
            break;
        case 7:
            return "7";
            break;
        case 8:
            return "8";
            break;
        case 9:
            return "9";
            break;
        case 10:
            return "10";
            break;
        default:
            return "ER";
            break;
    }
}

void update(){
    if(!board.isServed){
        if(IsKeyPressed(KEY_SPACE)){
            board.isServed = true;
            ball.isVisible = true;
            unsigned int tmp = GetRandomValue(0, 1);
            ball.isNegX = (bool)tmp;
            tmp = GetRandomValue(0, 1);
            ball.isNegY = (bool)tmp;
        } else {
            return;
        }
    }
    
    if(IsKeyDown(KEY_W)){
        left_paddle.y -= 0.02f;
        if(left_paddle.y < board.y){
            left_paddle.y = board.y;
        }
    }
    if(IsKeyDown(KEY_S)){
        left_paddle.y += 0.02f;
        if(left_paddle.y > (board.height - PADDLE_HEIGHT + BORDER_SIZE)){
            left_paddle.y = (board.height - PADDLE_HEIGHT + BORDER_SIZE);
        }
    }
    if(IsKeyDown(KEY_UP)){
        right_paddle.y -= 0.02f;
        if(right_paddle.y < board.y){
            right_paddle.y = board.y;
        }

    }
    if(IsKeyDown(KEY_DOWN)){
        right_paddle.y += 0.02f;
        if(right_paddle.y > (board.height - PADDLE_HEIGHT + BORDER_SIZE)){
            right_paddle.y = (board.height - PADDLE_HEIGHT + BORDER_SIZE);
        }
    }

    if(ball.isVisible){
        if(ball.isNegX){
            ball.x += 0.01f;
        } else {
            ball.x -= 0.01f;
        }

        if(ball.isNegY){
            ball.y += 0.01f;
        } else {
            ball.y -= 0.01f;
        }
    }
}

void drawUI(){
    DrawRectangle(0, 0, SCREEN_WIDTH, BORDER_SIZE, WHITE);
    DrawRectangle(0, SCREEN_HEIGHT - BORDER_SIZE, SCREEN_WIDTH, BORDER_SIZE, WHITE);
    
    //Draw the "bounds" of the game
    //DrawRectangle(board.x, board.y, board.width, board.height, RED);
    
    //DrawRectangle(SCREEN_WIDTH/2 - NET_WIDTH, BORDER_SIZE, NET_WIDTH, NET_LENGTH, WHITE);
    for(int i = 0; i <= board.net_count; i++){
        if(i % 2 == 0){
            DrawRectangle(SCREEN_WIDTH/2 - NET_WIDTH, BORDER_SIZE + (i * NET_LENGTH), NET_WIDTH, NET_LENGTH, WHITE);
        }
    }

    DrawText(getScoreAsString(score.player1), (SCREEN_WIDTH/2) - (NET_WIDTH *2) - TEXT_SIZE, BORDER_SIZE + (NET_LENGTH/2), TEXT_SIZE, WHITE);
    DrawText(getScoreAsString(score.player2), (SCREEN_WIDTH/2) - (NET_WIDTH *2) + TEXT_SIZE, BORDER_SIZE + (NET_LENGTH/2), TEXT_SIZE, WHITE);

    if(!board.isServed){
        DrawRectangle(SCREEN_WIDTH/8, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8,
            SCREEN_WIDTH/2 + SCREEN_WIDTH/4, SCREEN_HEIGHT/2 - SCREEN_HEIGHT/4, viniette);
        DrawText(line1, SCREEN_WIDTH/2 - (MeasureText(line1, TEXT_SIZE/2))/2, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE/2 + 4, TEXT_SIZE/2, WHITE);
        DrawText(line2, SCREEN_WIDTH/2 - (MeasureText(line2, TEXT_SIZE/2))/2, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE + 8, TEXT_SIZE/2, WHITE);
    }


}

void drawObjs(){
    DrawRectangle((int)left_paddle.x, (int)left_paddle.y, left_paddle.width, left_paddle.height, WHITE);
    DrawRectangle((int)right_paddle.x, (int)right_paddle.y, right_paddle.width, right_paddle.height, WHITE);

    if(ball.isVisible){
        DrawRectangle((int)ball.x, (int)ball.y, ball.width, ball.height, BLUE);
    }
}

int main(){
    
    init();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PONG!");

    while (!WindowShouldClose()) {
        
        update();

        BeginDrawing();

        ClearBackground(bgColor);
        drawUI();
        drawObjs();

        EndDrawing();
    }
    

    return 0;
}