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

// background color, 
// Raylib uses this struct to hold the data
const Color bgColor = {
    .r = 54,
    .g = 55,
    .b = 55,
    .a = 255
};

// the overly color for the messege box, 
// Raylib uses this struct to hold the data
const Color viniette = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 192
};

//text lines to be drawn. each is a line.
const char line1[] = "PRESS SPACEBAR";
const char line2[] = "TO SERVE!";
const char p1[] = "PLAYER 1";
const char p2[] = "PLAYER 2";
const char pwin[] = "WINS!";

//paddle struct
typedef struct {
    float x;
    float y;
    int width;
    int height;
} Paddle;

//ball struct
typedef struct {
    float x;
    float y;
    int width;
    int height;
    bool isNegX;
    bool isNegY;
    bool isVisible;
} Ball;

//scores
typedef struct {
    unsigned char player1;
    unsigned char player2;
} Score;

//game board. both holds some logic values and the "bounds" the paddles and ball can move within
typedef struct {
    int x;
    int y;
    int width;
    int height;

    //couldnt think of a "better" place for this. its the number of rec that make up the "net"
    int net_count;
    bool isServed;
    bool isOver; 
} GameBoard;

//game objects
Paddle left_paddle;
Paddle right_paddle;
Ball ball;
Score score;
GameBoard board;

//sound effects
Sound pong_wall;
Sound pong_paddle;
Sound pong_score;

//initalizes game objects with default values
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
    board.isOver = false;
    
    //set seed for raylib RNG using system time
    SetRandomSeed((unsigned int) time(NULL));
}

//a hacky way of converting the nemeric score the game tracks
//to the ascii value rendered
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

//checks if either player has won
void checkWinner(){
    if(score.player1 >= 10 || score.player2 >= 10){
        board.isOver = true;
    }
}

//checks if either player has scored
void checkScore(){
    if(ball.x > (right_paddle.x + right_paddle.width)){
        ball.isVisible = false;
        ball.x = BALL_START_X;
        ball.y = BALL_START_Y;
        score.player1++;
        board.isServed = false;
        PlaySound(pong_score);
    }
    
    if((ball.x) < left_paddle.x - left_paddle.width){
        ball.isVisible = false;
        ball.x = BALL_START_X;
        ball.y = BALL_START_Y;
        score.player2++;
        board.isServed = false;
        PlaySound(pong_score);
    }
}

//checks if the ball is colliding with a paddle
//note there are some bugs but it works "ok"
void checkPaddleCollision(){
    if((ball.x + ball.width > right_paddle.x)){
        if(ball.y > right_paddle.y && ball.y < right_paddle.y + right_paddle.height){
            ball.isNegX = !ball.isNegX;
            PlaySound(pong_paddle);
        }
    }
    if(ball.x < left_paddle.x+left_paddle.width){
        if(ball.y > left_paddle.y && ball.y < left_paddle.y + left_paddle.height){
            ball.isNegX = !ball.isNegX;
            //ball.isNegY = !ball.isNegY;
            PlaySound(pong_paddle);
        }
    }
}

//a "debug" function to make one side win.
//used for setting the winning messeges up correctly
//commented out by default
void debug(){
    if(IsKeyPressed(KEY_F1)){
        score.player1 = 10;
    }
    if(IsKeyPressed(KEY_F2)){
        score.player2 = 10;
    }
}

//updates the game each frame;
void update(){

    //debug();


    //if the game is not over
    if(!board.isOver){
        //if the ball has been served
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
                if(ball.y > board.height - ball.height){
                    ball.y = board.height - ball.height;
                    ball.isNegY = false;
                    PlaySound(pong_wall);
                }
            } else {
                ball.y -= 0.01f;
                 if(ball.y < board.y){
                    ball.y = board.y;
                    ball.isNegY = true;
                    PlaySound(pong_wall);
                }
            }
        }
        checkPaddleCollision();
        checkScore();
        checkWinner();
    } 
}

//draws all "UI" elements. so the boarder, net, text, and messege boxes
void drawUI(){
    //draw boarders
    DrawRectangle(0, 0, SCREEN_WIDTH, BORDER_SIZE, WHITE);
    DrawRectangle(0, SCREEN_HEIGHT - BORDER_SIZE, SCREEN_WIDTH, BORDER_SIZE, WHITE);
    
    //Draw the "bounds" of the game
    //DrawRectangle(board.x, board.y, board.width, board.height, RED);
    
    //draw the dotted net
    for(int i = 0; i <= board.net_count; i++){
        if(i % 2 == 0){
            DrawRectangle(SCREEN_WIDTH/2 - NET_WIDTH, BORDER_SIZE + (i * NET_LENGTH), NET_WIDTH, NET_LENGTH, WHITE);
        }
    }

    //draw player scores
    DrawText(getScoreAsString(score.player1), (SCREEN_WIDTH/2) - (NET_WIDTH *2) - TEXT_SIZE, BORDER_SIZE + (NET_LENGTH/2), TEXT_SIZE, WHITE);
    DrawText(getScoreAsString(score.player2), (SCREEN_WIDTH/2) - (NET_WIDTH *2) + TEXT_SIZE, BORDER_SIZE + (NET_LENGTH/2), TEXT_SIZE, WHITE);

    //if the game is still over
    if(!board.isOver){
        //if the ball hasnt been served, draw prompt
        if(!board.isServed){
            DrawRectangle(SCREEN_WIDTH/8, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8,
                SCREEN_WIDTH/2 + SCREEN_WIDTH/4, SCREEN_HEIGHT/2 - SCREEN_HEIGHT/4, viniette);
            DrawText(line1, SCREEN_WIDTH/2 - (MeasureText(line1, TEXT_SIZE/2))/2, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE/2 + 4, TEXT_SIZE/2, WHITE);
            DrawText(line2, SCREEN_WIDTH/2 - (MeasureText(line2, TEXT_SIZE/2))/2, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE + 8, TEXT_SIZE/2, WHITE);
        }
    }  else { //draw winner message if game is over
        DrawRectangle(SCREEN_WIDTH/8, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8,
                SCREEN_WIDTH/2 + SCREEN_WIDTH/4, SCREEN_HEIGHT/2 - SCREEN_HEIGHT/4, viniette);
        if(score.player1 > score.player2){
            DrawText(p1, SCREEN_WIDTH/2 - (MeasureText(line1, TEXT_SIZE/2)/4), SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE/2 + 4, TEXT_SIZE/2, WHITE);
            DrawText(pwin, SCREEN_WIDTH/2 - (MeasureText(line2, TEXT_SIZE/2)/4), SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE + 8, TEXT_SIZE/2, WHITE);
        } else {
            DrawText(p2, SCREEN_WIDTH/2 - (MeasureText(line1, TEXT_SIZE/2)/4), SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE/2 + 4, TEXT_SIZE/2, WHITE);
            DrawText(pwin, SCREEN_WIDTH/2 - (MeasureText(line2, TEXT_SIZE/2)/4), SCREEN_HEIGHT/4 + SCREEN_HEIGHT/8 + TEXT_SIZE + 8, TEXT_SIZE/2, WHITE);
        }
    }
}

void drawObjs(){
    //draw paddles
    DrawRectangle((int)left_paddle.x, (int)left_paddle.y, left_paddle.width, left_paddle.height, WHITE);
    DrawRectangle((int)right_paddle.x, (int)right_paddle.y, right_paddle.width, right_paddle.height, WHITE);

    //draw the ball if its visible
    if(ball.isVisible){
        DrawRectangle((int)ball.x, (int)ball.y, ball.width, ball.height, WHITE);
    }
}

int main(){
    
    init();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PONG!");
    InitAudioDevice();

    pong_wall = LoadSound("res/pong_wall.ogg");
    pong_paddle = LoadSound("res/pong_paddle.ogg");
    pong_score = LoadSound("res/pong_score.ogg");

    while (!WindowShouldClose()) {
        
        update();

        BeginDrawing();

        ClearBackground(bgColor);
        drawUI();
        drawObjs();

        EndDrawing();
    }

    UnloadSound(pong_wall);
    UnloadSound(pong_paddle);
    UnloadSound(pong_score);

    CloseAudioDevice();
    CloseWindow();
    

    return 0;
}