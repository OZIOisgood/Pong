#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
 
using namespace sf;

int height  = 1000;
int width   = 1500;

struct Position
{
    int x;
    int y;
} a[3], b[3];


bool checkOut(int i) // Y borders for pong
{
        if (a[i].y == -1 || a[i].y + 49 == height) return 1;
 
    return 0;
}

bool checkEdge(int i) // Y borders for player
{
    if (i == 1)
        if (a[0].y <= 10 - 1 || a[0].y + 250 >= height - 10 + 1)
            return 1;
    
    if (i == 2)
        if (a[1].y <= 10 - 1 || a[1].y + 250 >= height - 10 + 1)
            return 1;

    return 0;
}

bool checkPing(int i) // Check pong ricochet from player
{
    if (i == 1)
    {
        if (a[2].x <= a[0].x + 25 && ((a[2].y + 50 >= a[0].y && a[2].y + 50 <= a[0].y + 250) || (a[2].y >= a[0].y && a[2].y <= a[0].y + 250)))
            return 1;
    }
    
    
    if (i == 2)
    {
        if (a[2].x >= a[1].x - 25 && ((a[2].y + 50 >= a[1].y && a[2].y + 50 <= a[1].y + 250) || (a[2].y >= a[1].y && a[2].y <= a[1].y + 250)))
            return 1;
    }

    return 0;
}

bool checkGoal() // Check for goal
{
    if (a[2].x < 0 || a[2].x > width) return 1;

    return 0;
}




int main()
{
    srand(time(0));
    
//    ContextSettings settings;
//    settings.antialiasingLevel = 8;
    
    RenderWindow window
    (
        VideoMode(1500, 1000),
        "Pong",
        Style::Titlebar | Style::Close
    );
    
    window.setFramerateLimit(1000);
    
    
    //~~~~~~~~ Values ~~~~~~~~//
    
    int     p1DirY      = 0;        // Player 1 direction Up or Down
    int     p2DirY      = 0;        // Player 2 direction Up or Down
    int     score1      = 0;
    int     score2      = 0;
    
    // For pong
    int     dirX        = 0;   // Direction X
    int     dirY        = 0;   // Direction Y
    
    // menu
    bool    isMenu      = true;
    int     menuNum     = 0;
    float   timerMenu   = 0;
    
    // countdown
    bool    countdown   = true;
    int     numNum      = 3;
    float   timerNum    = 0;
    float   numDelay    = 1;
    
    
    // game
    bool    game        = false;
    float   timer       = 0;
    float   timerPlayer = 0;
    double  delay       = 0.002;
    
    //~~~~~~~~        ~~~~~~~~//
    
    Font font;
    font.loadFromFile("/System/Library/Fonts/Supplemental/Futura.ttc");
    
    Clock clock;
    
    //~~~~~~~~ Sprites ~~~~~~~~//
    
    RectangleShape line(Vector2f(4.f, 1000.f));
    line.move(750, 0);
    line.setFillColor(Color(200, 200, 255));
    
    
    //
    RectangleShape lineCentre(Vector2f(1500.f, 1.f));
    lineCentre.move(0, 500);
    lineCentre.setFillColor(Color(255, 255, 255));
    //

    
    RectangleShape player1(Vector2f(25.f, 250.f));
    player1.setFillColor(Color(255, 165, 0));
    
    RectangleShape player2(Vector2f(25.f, 250.f));
    player2.setFillColor(Color(255, 165, 0));
    
    RectangleShape pong(Vector2f(50.f, 50.f));
    pong.setFillColor(Color(255, 255, 0));
    
    CircleShape ball(25.f);
    
    
    
    Text playMenuText("Play", font, 150);
    playMenuText.setFillColor(Color(100, 100, 100, 255));
    playMenuText.setPosition(600, 300);
    
    Text exitMenuText("Exit", font, 150);
    exitMenuText.setFillColor(Color(100, 100, 100, 255));
    exitMenuText.setPosition(600, 500);
    
    
    
    
    SoundBuffer pingSound;
    
    pingSound.loadFromFile("/System/Library/PrivateFrameworks/ScreenReader.framework/Versions/A/Resources/Sounds/Collaboration.wav");
    Sound ping;
    ping.setBuffer(pingSound);
    
    SoundBuffer goalSound;
    goalSound.loadFromFile("/System/Library/PrivateFrameworks/SiriVOX.framework/Versions/A/Resources/TwoShot-b238.wav");
    Sound goal;
    goal.setBuffer(goalSound);
    
    SoundBuffer winSound;
    winSound.loadFromFile("/System/Library/PrivateFrameworks/AssistantServices.framework/Versions/A/Resources/announce-messages-tone.wav");
    Sound win;
    win.setBuffer(winSound);
    
    SoundBuffer menuSound;
    menuSound.loadFromFile("/System/Library/PrivateFrameworks/AudioPasscode.framework/Versions/A/Resources/WOCAudioPasscodeTone.wav");
    Sound menu;
    menu.setBuffer(menuSound);
    
    //~~~~~~~~         ~~~~~~~~//
    
    
    
    
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer       += time;
        timerPlayer += time;
        timerNum    += time;
        
        Event event;
        
        while (window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            
            if(event.key.code == Keyboard::Q)
                p1DirY++;

            if(event.key.code ==  Keyboard::A)
                p1DirY--;

            if(event.key.code == Keyboard::P)
                p2DirY--;

            if(event.key.code == Keyboard::L)
                p2DirY++;
        }
        
        
        
        
        
        //~~~~~~~~ Pong ~~~~~~~~//
        
        

        if(isMenu)
        {
            if(IntRect(600, 300, 300, 200).contains(Mouse::getPosition(window)))
            {
                playMenuText.setFillColor(Color(255, 255, 255, 255));
                menuNum = 1;
                
            }
            if(IntRect(600, 500, 300, 200).contains(Mouse::getPosition(window)))
            {
                exitMenuText.setFillColor(Color(255, 255, 255, 255));
                menuNum = 2;
            }
     
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                if(menuNum == 1)
                {
                    menu.play();
                    isMenu = false;
                }
                
                if(menuNum == 2)
                {
                    window.close();
                }
            }
            
            
            window.clear(Color(15, 48, 82,0));
        
            window.draw(playMenuText);
            window.draw(exitMenuText);
            
            player1 .setPosition(0, 250);
            player2 .setPosition(1475, 500);
            ball    .setPosition(300, 125);
            
            window.draw(player1);
            window.draw(player2);
            window.draw(ball);
        
            window.display();
            
            playMenuText.setFillColor(Color(100, 100, 100, 255));
            exitMenuText.setFillColor(Color(100, 100, 100, 255));
        }
        
        
        if(!isMenu)
        {
            if(countdown)
            {
            
                window.clear(Color(15, 48, 82,0));
                
                
                Text numText(std::to_string(numNum), font, 150);
                numText.setFillColor(Color::White);
                numText.setPosition(700, 400);
            
                if(numNum == 4)
                {
                    numText.setString("Goal!");
                    numText.setPosition(550, 400);
                }
            
                window.draw(numText);
            
                window.display();
            
                if(timerNum > numDelay)
                {
                    goal.play();
                    numNum--;
                    timerNum = 0;
            
                    if(numNum == 0)
                        countdown = false;
                }
            }
    
    
            if(!countdown)
            {
                if(!game)
                {
                    int randColor = rand() % 2 * 255;
                    ball.setFillColor(Color(255, 255, randColor));
                    pong.setFillColor(Color(255, 255, randColor));
            
            
                    dirX = rand() % 2;
                    dirY = rand() % 2;
            
                    a[0].x = 0;
                    a[0].y = 375;
                    player1.setPosition(a[0].x, a[0].y);
            
                    a[1].x = 1475;
                    a[1].y = 375;
                    player2.setPosition(a[1].x, a[1].y);
            
                    a[2].x = 725;
                    a[2].y = 475;
                    pong.setPosition(a[2].x, a[2].y);
            
                    game = true;
                }
            
                if (timer > delay)
                {
                    if(dirX == 1)
                    {
                        b[2].x =    a[2].x;
                        a[2].x +=   1;
                    }
                    else
                    {
                        b[2].x =    a[2].x;
                        a[2].x -=   1;
                    }
            
            
                    if(dirY == 1)
                    {
                        b[2].y =    a[2].y;
                        a[2].y +=   1;
                    }
                    else
                    {
                        b[2].y =    a[2].y;
                        a[2].y -=   1;
                    }
            
                    if(checkOut(2))
                    {
                        if(dirY == 1)
                            dirY = 0;
                        else
                            dirY = 1;
                        
                        delay *= 1.2;
                        std::cout << "New delay\t" << delay << std::endl <<     std::endl;
            
                        ping.play();
            
                        pong.setPosition(b[2].x, b[2].y);
                    }
                    else if(checkPing(1) || checkPing(2))
                    {
                        if(dirX == 1)
                            dirX = 0;
                        else
                            dirX = 1;
            
                        a[2].y = b[2].y;
                        
            
                        delay /= 1.7;
            
            
                        std::cout << "New delay\t" << delay << std::endl <<     std::endl;
            
                        pong.setPosition(b[2].x, b[2].y);
                    }
                    else if(checkGoal())
                    {
                        if (a[2].x > width)
                            score1++;
                        if (a[2].x < 0)
                            score2++;
            
                        goal.play();
            
                        game        = false;
                        countdown = true;
                        numNum      = 4;
                        timerNum    = 0;
                        delay       = 0.002;
            
                        std::cout << "numNum " << numNum << std::endl;
                    }
                    else
                        pong.setPosition(a[2].x, a[2].y);
            
            
            
                    timer = 0;
                }
                
            
            
            
            
                if(p1DirY == 1)
                {
                    b[0].y =    a[0].y;
                    a[0].y -=   40;
                }
            
                if(p1DirY == -1)
                {
                    b[0].y =    a[0].y;
                    a[0].y +=   40;
                }
            
            
            
                if(p2DirY == -1)
                {
                    b[1].y =    a[1].y;
                    a[1].y -=   40;
                }
            
                if(p2DirY == 1)
                {
                    b[1].y =    a[1].y;
                    a[1].y +=   40;
                }
            
            
            
            
                if(checkEdge(1))
                {
                    a[0].y = b[0].y;
                    player1.setPosition(0, b[0].y);
                }
                else
                    player1.setPosition(0, a[0].y);
            
                if(checkEdge(2))
                {
                    a[1].y = b[1].y;
                    player2.setPosition(1475, b[1].y);
                }
                else
                    player2.setPosition(1475, a[1].y);
            
                p1DirY = 0;
                p2DirY = 0;
            
                timerPlayer = 0;
            
            
            
            
            
            
            
                //~~~~~~~~      ~~~~~~~~//
            
            
            
                Text tScore1(std::to_string(score1), font, 64);
                tScore1.setFillColor(Color::White);
                tScore1.setPosition(700, 0);
            
                Text tScore2(std::to_string(score2), font, 64);
                tScore2.setFillColor(Color::White);
                tScore2.setPosition(765, 0);
            
            
            
            
                window.clear(Color(15, 48, 82,0));
            
            
            
            
            
            
                window.draw(line);
            
                ball.setPosition(a[2].x, a[2].y);
                window.draw(ball);
            
                window.draw(player1);
                window.draw(player2);
            
                window.draw(tScore1);
                window.draw(tScore2);
            
            
                window.display();
            
            
                if(score1 == 9 || score2 == 9)
                {
                    win.play();
                    score1 = 0;
                    score2 = 0;
                    isMenu = true;
                }
            
            }
        }
    }
    
    
    return 0;
}
