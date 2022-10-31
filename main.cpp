#include "raylib.h"
#define MAX_Obs   400


class declaration
{
public:
    Vector2  pos={200,600};
    int gravity = 7;
    Texture2D player = LoadTexture("C:\\Users\\dhair\\Desktop\\raylib\\resources\\cube.png");
    Texture2D Spikeobs = LoadTexture("C:\\Users\\dhair\\Desktop\\raylib\\resources\\spike.png");
    bool move = true;
    bool fall = true;
    bool isjump = false;
    int frames =0;
    int speed= 0;
    Sound jump = LoadSound("C:\\Users\\dhair\\Desktop\\raylib\\resources\\jump.wav");
    Sound coll = LoadSound("C:\\Users\\dhair\\Desktop\\raylib\\resources\\hit.wav");
    Sound yes = LoadSound("C:\\Users\\dhair\\Desktop\\raylib\\resources\\restart.wav");
    Sound lvlup = LoadSound("C:\\Users\\dhair\\Desktop\\raylib\\resources\\speedup.wav");
    Sound music = LoadSound("C:\\Users\\dhair\\Desktop\\raylib\\resources\\rain.wav");



};

class cube : declaration
{

public:
    Rectangle hit[MAX_Obs] = { 0 };
    Rectangle bruh = {pos.x ,pos.y ,64,64};
    Rectangle ground = {0, 700 , 600000 ,100};
    Rectangle ground2 ={1000000, 700 , ground.width,ground.height};
    Texture2D bgrain = LoadTexture("C:\\Users\\dhair\\Desktop\\raylib\\resources\\rain.png");
    Vector2 positionbg = { 00, -40 };
    Rectangle frameRec = { 0.0f, 0.0f, (float)ground.width, (float)800};
    int score =0;
    int currentFrame = 0;
    int framesCounter = 0;
    float rotation = 0;


    void draw()
    {
        if (IsKeyPressed(KEY_UP))
        {
            rotation +=90;
        }
        DrawRectangleRec(bruh,BLUE);
        DrawTexture(player,bruh.x,bruh.y,RAYWHITE);


    }

    void calc()
    {
        int spacing = 100;


        for(int i = 0; i < MAX_Obs; i++)
        {
            hit[i].height = 10;
            hit[i].width = 10;
            hit[i].y = 656;
            hit[i].x = 800 + spacing;

            spacing += GetRandomValue(429,1050);

        }

    }
    void check()
    {

        if(bruh.x < ground2.x)
        {
            frames++;
        }
        else
        {
            frames = 0;
        }
        if(((frames/600)%2) == 1 && bruh.x < ground2.x )
        {
            speed += 1;
            PlaySound(lvlup);
            frames = 0;
        }


        if(move == true)
        {
            bruh.x+=speed ;
        }



       if(fall == true)
        {
            if(!CheckCollisionRecs(bruh, ground)) {
                bruh.y += gravity ;

            }
            else if(CheckCollisionRecs(ground,bruh))
            {
                isjump = false;
            }
        }
        if (fall == false)
        {
            bruh.y +=0;
            isjump = true;
        }

        if (bruh.y < ground.y -200)
        {
            isjump = true;
           // PlaySound(jump);

        }

        if (IsKeyDown(KEY_UP) && !isjump)
        {
            bruh.y -= 39;
            PlaySound(music);

        }



    }

    void restart()
    {
        PlaySound(yes);
        score =0;
        speed = 10;
        bruh.x =pos.x;
        bruh.y = pos.y;
        fall = true;
        move = true;
        PlaySound(music);


    }

    void hitdet()
    {
        for (int i = 0; i < MAX_Obs; ++i)
        {
            if (CheckCollisionRecs(bruh,hit[i]))
            {
                bruh.x = ground2.x +1000;
                bruh.y = ground2.y - 64;
                move = false;
                fall = false;
                PlaySound(coll);
                StopSound(music);

            }

        }

        if (IsKeyPressed(KEY_ENTER))
        {
            restart();
        }
    }




    void drawgrnd()
    {
        DrawRectangleRec(ground,DARKGRAY);
        DrawRectangleRec(ground2,DARKGRAY);

    }



   void drawbg()
   {
           DrawTextureRec(bgrain, frameRec, positionbg, RAYWHITE);

   }

    void drawspike()
    {
        for(int i = 0; i < MAX_Obs; i++)
        {
            DrawRectangleRec(hit[i],RED);
            DrawTexture(Spikeobs,hit[i].x -24,636,RAYWHITE);

        }
    }

    void bgcalc()
    {
        framesCounter++;

        if (framesCounter >= (60/8))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 5) currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)1200;
        }


    };



};

int main()
{
   

    const int screenWidth = 1200;
    const int screenHeight = 800;


    InitWindow(screenWidth, screenHeight, "Geometry Dash");
    InitAudioDevice();



    cube player;
    Camera2D camera;
    camera.target = (Vector2){player.bruh.x - 100.0f, 600};
    camera.offset = (Vector2){player.bruh.x , player.bruh.y};
    camera.zoom = 1.0f;
    camera.rotation={0};


    SetTargetFPS(60);

     player.calc();



    while (!WindowShouldClose())
    {


        player.bgcalc();


        if(player.bruh.x > player.ground.x && player.bruh.x < player.ground2.x)
        {
            player.score++;

        }


        camera.target = (Vector2){player.bruh.x  , 600};



        BeginDrawing();
        BeginMode2D(camera);



        ClearBackground(BLACK);

        player.hitdet();
        player.check();
        player.drawbg();
        player.drawgrnd();
        player.drawspike();
        player.draw();


        DrawText(TextFormat("Your Score Was : " ), player.ground2.x+ 1100,player.ground2.y -350 ,50,WHITE);
        DrawText(TextFormat("%i", (player.score / 60)), player.ground2.x + 1600, player.ground2.y - 350, 50, WHITE);
        DrawText(TextFormat("Press Enter to Restart"), player.ground2.x +1400, player.ground2.y -100, 30, WHITE);
        DrawText("Press Enter To Start", 200 ,400 ,30,RAYWHITE);


        DrawFPS(player.bruh.x -190,10);



        EndMode2D();
        EndDrawing();

    }


    CloseWindow();

    return 0;
}




