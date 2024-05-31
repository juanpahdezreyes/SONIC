#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Coin.hpp"
#include "Grid.hpp"
using namespace sf;
using namespace std;

float gravity = 0;
int jump=0;
bool canJump = 0;
bool Aa=0;
bool Dd=0;
bool Ss=0;
bool Rr=0;
bool stopG = 0;
bool ball = 0;
bool cambiado = 1;
float movex = 0;
float movey = 0;
float desliz = 0;
bool dont = 0;
int lastmove = 1;
int camaraMove = 1;
int EnNivel = 0;
float moveForce = 1;
int JumpLimit = 25;

int rotateFrame = 1;
int rotateVel=20;   // Variable para la velocidad de giro de la bola

int walkframe=1;    // Frame en el que va la animación caminar
int walkcounter=0;  // Contador para ir cambiando entre sprites
int walkwait=10;    // Cuantos frames pasan entre cada diferente sprite/textura de animación de caminar
// Variables necesarias para la aniamción de caminar

float deltaY;
float deltaX;
float intersectX;
float intersectY;

void SonicJump(){   // Siempre hay que escribir Sonic.move(0,-10); antes de llamar a esta función
    jump++;
    gravity = 0;
    if (jump>JumpLimit){
        jump=0;
        gravity=1;
    }
}   // Hay 25 frames de salto, durante el salto la gravedad se desactiva, cuando el salto se acaba vuelve la gravedad

class Mapas{
public:
int grid[40][7];
};

int main()
{

    sf::Texture bloques;
    bloques.loadFromFile("assets/Sbloque.png");
    sf::Sprite Block(bloques);

    sf::Texture back;
    back.loadFromFile("assets/Sfondo.png");
    sf::Sprite fondo(back);
    fondo.setPosition(-128,0);
    fondo.setScale(4,4);

    sf::Sprite fondo2(back);
    fondo2.setPosition(-1792,0);
    fondo2.setScale(4,4);

    sf::Sprite fondo3(back);
    fondo3.setPosition(1576,0);
    fondo3.setScale(4,4);

    vector<sf::Sprite> texturing;

    sf::Texture texturas;
    texturas.loadFromFile("standing.png");
    sf::Sprite Pose(texturas);
    Pose.setOrigin(32,64);//sprite de Sonic

    RenderWindow window(VideoMode(1664, 1024), "¡Sonic!");
    window.setFramerateLimit(60);
    RectangleShape Sonic(Vector2f(64.f,128.f));
    Sonic.setFillColor(Color::Blue);
    Sonic.setOrigin(32,64);
    Sonic.setPosition(700,0);
    // Creación del Jugador 32x64 la Textura
    vector <RectangleShape> nivel; // Vector de bloques que conforman el nivel
    RectangleShape col(Vector2f(512,128)); // Ejemplo base que se va a hace .push_back en el vector de nivel
    col.setFillColor(Color::Green);
    col.setPosition(256,896);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel[0].setSize(Vector2f(3200,128));
    nivel[0].setPosition(800,960-64);
    nivel[0].setFillColor(Color::Transparent);
    nivel[1].setPosition(640,384);
    nivel[1].setFillColor(Color::Magenta);
    nivel[2].setPosition(500,640);
    nivel[3].setPosition(-128,384);
    nivel[3].setFillColor(Color::Magenta);// Se establece cada bloque del nivel en su posición
    nivel[3].setSize(Vector2f(128,896));

    for (int i=0;i<nivel.size();i++){
        nivel[i].setOrigin(nivel[i].getSize().x/2,nivel[i].getSize().y/2);
    }
    
    Grid ejemplo;
    ejemplo.sprite.setTexture(bloques);
    ejemplo.sprite.setOrigin(64,64);
    vector <Grid> mapa;

    sf::View camara = window.getDefaultView();//Crea la variable camara
    RectangleShape CdontMove(Vector2f(512,512));//Crea el cuadrado en el que la camara se va a detener (para un efecto mejor)
    CdontMove.setOrigin(160,160);
    CdontMove.setPosition(Sonic.getPosition());
    CdontMove.setFillColor(Color::Transparent);

    vector <Coin> monedas;  // Vector de las monedas del nivel
    CircleShape coin(16);
    coin.setOrigin(8,8);
    coin.setFillColor(Color::Yellow);   // Moneda base
    Coin blah(coin);    // Clase Coin base
    monedas.push_back(blah);
    monedas.push_back(blah);
    monedas.push_back(blah);
    monedas.push_back(blah);

    for (int i=0;i<monedas.size();i++){
        if (monedas[i].picked==0){
            monedas[i].mon.setPosition(0+(rand()%1600),0+rand()%576 + 256);
        }
    }//Randomiza las monedas al inicio

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        for (int i=0;i<nivel.size();i++){ // Recorre el vector de nivel
            if (Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){ // Checa si hay alguna colisión entre cualquiera de los bloques existentes en el vector de nivel
                
                //      REFERENCIA EMPIEZA AQUÍ
                deltaX = Sonic.getPosition().x - nivel[i].getPosition().x;
                deltaY = Sonic.getPosition().y - nivel[i].getPosition().y;

                intersectX = abs(deltaX) - ((Sonic.getSize().x/2)+(nivel[i].getSize().x/2));
                intersectY = abs(deltaY) - ((Sonic.getSize().y/2)+(nivel[i].getSize().y/2));
                /*     
                 Hilze Vonck. (2017). SFML 2.4 para principiantes - 12: Detección de colisiones (AABB)..
                 Youtube. https://www.youtube.com/watch?v=l2iCYCLi6MU
                        REFERENCIA ACABA AQUÍ
                */
                
                if (Sonic.getPosition().y < nivel[i].getPosition().y && intersectX < intersectY){ 
                    gravity=0;
                    while(Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-1);
                    }
                    Sonic.setFillColor(Color::Transparent);
                    canJump=1;
                    stopG = 1;
                }   // Checa colisiones por arriba (El jugador cayendo) detiene la gravedad y permite el salto si es el caso

                if (Sonic.getPosition().y > nivel[i].getPosition().y && intersectX < intersectY){
                    while(Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y+1);
                    }
                    jump=0;
                }   // Checa colisiones por abajo (El jugador saltando y chocando con la parte baja de un bloque) detiene al jugador y la secuencia de salto

                if (Sonic.getPosition().x < nivel[i].getPosition().x){
                    while(Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                        Sonic.setPosition(Sonic.getPosition().x-1,Sonic.getPosition().y);
                    }
                }   // Checa colisiones por la izquierda

                if (Sonic.getPosition().x > nivel[i].getPosition().x){
                    while(Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                        Sonic.setPosition(Sonic.getPosition().x+1,Sonic.getPosition().y);
                    }
                }   // Checa colisiones por la derecha
                
            }
        }
        for (int i=0;i<monedas.size();i++){
            if (Sonic.getGlobalBounds().intersects(monedas[i].mon.getGlobalBounds())){
                monedas[i].picked=1;
                monedas[i].UpdateCoin();
            }
        }   // Checa si hay colisiones con el Jugador. Si es el caso pone la moneda como picked y actualiza la moneda

        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space)){
            Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-1);
            for(int i=0;i<nivel.size();i++){
                if (Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                    dont=1;
                }
            } 
            Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y+1);
            // Checa arriba del jugador que no halla un bloque (Para un bug)
            if (canJump==1 && dont==0){
                Sonic.move(0,-10);
                SonicJump();
                canJump=0;
            }   // Si la ´W´ es presionada y puede saltar (Esta colisionando con el suelo) llama a la secuencia de Salto a empezar e indica que ya no se puede saltar más (No esta tocando suelo)
            dont=0;
        }else{
            jump=0;
        }   // Si la W no es presionada detiene el salto (Saltas el tiempo que la ´W´ sea presionada)
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){  
            if (canJump==1) walkcounter++;  // indica que inicie la animación de caminar
            lastmove = -1;
            movex=-10*moveForce;
            Aa=1; 
            // Si la ´A´ es presionada indica la cantidad de movimiento en x con la variable movex e indica que ha sido presionada con su variable de presión Aa
        }else{
            Aa=0;
        }   // Si la ´A´ no es presionada vuelve su variable de presionada falsa
        if (Keyboard::isKeyPressed(Keyboard::R) && Rr==0){
            Sonic.setPosition(700,0);
            CdontMove.setPosition(700,0);
            camara = window.getDefaultView();
            //camara.setCenter(Sonic.getPosition().x,Sonic.getPosition().y);
            Rr=1;
            for (int i=0;i<monedas.size();i++){
                if (monedas[i].picked==0){
                    monedas[i].mon.setPosition(0+(rand()%1600),0+(rand()%576)+256);
                }
            }
        }else{  // Reinicia la posición del Sonic (Por si se cae al vacio) (Ya no es necesario pues el suelo lo sigue)
            Rr=0;
        }    
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)){
                ball = 1;
                Sonic.setSize(Vector2f(64.f,64.f));
                Sonic.setOrigin(32,32);
                texturas.loadFromFile("assets/Sbola2.png");
                moveForce=1.8;
                Ss = 1;
                JumpLimit=15;
                // Indica que se transforme en bola
        }else{
            JumpLimit=25;
            moveForce=1;
            Ss=0;
        }
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
            if (canJump==1) walkcounter++;  // indica que inicie la animación de caminar
            lastmove = -1;
            lastmove = 1;
            movex=10*moveForce;
            Dd=1;
            // Si la ´D´ es presionada indica la cantidad de movimiento en x con la variable movex e indica que ha sido presionada con su variable de presión Aa
        }else{
            Dd=0;
        }   // Si la ´D´ no es presionada vuelve su variable de presionada falsa
        if (jump>0){
        Sonic.move(0,-10);
        SonicJump();
        Sonic.setFillColor(Color::Transparent);
        if (lastmove==1)Pose.setScale(1,1);
        if(lastmove==-1)Pose.setScale(-1,1);
        if(ball==0)texturas.loadFromFile("assets/SjumpUp.png");
        }   // Continuación de la secuencia de salto
        
        if (Dd==0 && Aa==0){
            walkframe=1;
            walkcounter=0;
        }   // Reinicia las variables de la aniamción de caminar

        if (movex<0){
            desliz=0.5;
            if (Aa==0){
                movex+=desliz;
            }
        }else{
            if (movex>0){
            if (Dd==0){
                movex+=-desliz;
            }
            }
        }   // Sirve para el deslice del jugador (Menos deslice = Hacer el deliz más grande)

        if (movey==0 && movex==0 && canJump==1){
            Sonic.setFillColor(Color::Transparent);
            texturas.loadFromFile("standing.png");
            ball=0;
            Pose.setScale(lastmove,1);
        }else{
            if (canJump==1){
            Sonic.setFillColor(Color::Transparent);
            }
        }
        // Para saber en que movimiento esta Sonic (Si completamente detenido o moviendose en una plataforma)

        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y+1);
        for(int i=0;i<nivel.size();i++){
            if (Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                stopG=1;
            }
        } 
        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-1);
        // Sirve para que no ande cayendo todo el tiempo y que el sprite no ande parpadeando entre cayendo y caminando

        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-64);
            for(int i=0;i<nivel.size();i++){
                if (Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                    dont=1;
                }
            } 
            Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y+64);

        if (Ss==0 && Sonic.getSize() == Vector2f(64.f,64.f) && dont==0){
            ball = 0;
            Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-64);
            Sonic.setSize(Vector2f(64.f,128.f));
            Sonic.setOrigin(32,64);
        }   // Transición de bola a forma normal
        dont=0;

        if (walkcounter>0 && canJump==1 && ball==0){ //animación de caminado
            if (walkcounter>walkframe*walkwait){
                walkframe++;
            }   // Calculos para ver cuando cambiar de sprite/textura
            if (walkframe>=9){  // Número mayor por uno al número de frames de la animación
                walkcounter=1;
                walkframe=1;
            }   // Reinicia walkframe (Para que sea un ciclo)
            if (walkframe==1)texturas.loadFromFile("assets/Swalk1.png");
            if (walkframe==2)texturas.loadFromFile("assets/Swalk2.png");
            if (walkframe==3)texturas.loadFromFile("assets/Swalk3.png");
            if (walkframe==4)texturas.loadFromFile("assets/Swalk4.png");
            if (walkframe==5)texturas.loadFromFile("assets/Swalk5.png");
            if (walkframe==6)texturas.loadFromFile("assets/Swalk6.png");
            if (walkframe==7)texturas.loadFromFile("assets/Swalk5.png");
            if (walkframe==8)texturas.loadFromFile("assets/Swalk7.png");    // Modificar para los sprites de Sonic

            if(lastmove==1)Pose.setScale(1,1);
            if(lastmove==-1)Pose.setScale(-1,1);    // Hace que el sprite mire en la dirección en la que se mira
        }else{
            walkcounter = 0;
            walkframe=1;
        }   // Reinicia las variables de la aniamción de caminar

        Sonic.move(0,gravity);
        if (gravity<=25 && stopG==0 && jump==0){
        gravity+=1;
        canJump=0;
        Sonic.setFillColor(Color::Transparent);
        if (lastmove==1)Pose.setScale(1,1);
        if(lastmove==-1)Pose.setScale(-1,1);
        if (ball==0 && movex!=0)texturas.loadFromFile("assets/SfallDown2.png");
        }   //Gravedad del Jugador
        stopG=0;
        Sonic.move(movex,movey);// Mueve al jugador finalmente cuanto se le indique
        nivel[0].setPosition(Sonic.getPosition().x,nivel[0].getPosition().y);
        window.clear();

        if (ball==1){
            Pose.setRotation(rotateFrame * lastmove);
            rotateFrame+=rotateVel;
            if (rotateFrame==360)rotateFrame=1;
        }else{
            Pose.setRotation(0);
            rotateFrame=0;
        }   // Rotación de la bola

        window.draw(fondo);
        window.draw(fondo2);
        window.draw(fondo3);
        window.draw(CdontMove);

        for (int i=0;i<nivel.size();i++){
        window.draw(nivel[i]);
        }   // Dibuja el vector de cuadros
        for (int i=0;i<monedas.size();i++){
        window.draw(monedas[i].mon);
        }   // Dibuja el vector de monedas
        window.draw(Sonic);
        Pose.setPosition(Sonic.getPosition());
        window.draw(Pose);
        // Blanco = Saltando
        // Morado = Completamente detenido
        // Verde = Moviendose en una plataforma
        // Rojo = Cayendo
        // Ya no es necesario
        if (Sonic.getGlobalBounds().intersects(CdontMove.getGlobalBounds())){
            camara.move(0,movey);
        }else{
            CdontMove.setPosition(CdontMove.getPosition().x,Sonic.getPosition().y);
            CdontMove.move(0,gravity);
            CdontMove.move(movex,movey);
            camara.move(movex,0);
        }   // Movimiento de la camara
        window.setView(camara);
        window.display();
    }

    return 0;
}