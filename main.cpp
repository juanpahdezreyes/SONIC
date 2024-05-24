#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Coin.hpp"
using namespace sf;
using namespace std;

float gravity = 0;
int jump=0;
bool canJump = 0;
bool Aa=0;
bool Dd=0;
bool Ee=0;
bool stopG = 0;
bool ball = 0;
float movex = 0;
float movey = 0;
float desliz = 0;

float deltaY;
float deltaX;
float intersectX;
float intersectY;

void SonicJump(){   // Siempre hay que escribir Sonic.move(0,-10); antes de llamar a esta función
    jump++;
    gravity = 0;
    if (jump>25){
        jump=0;
        gravity=1;
    }
}   // Hay 25 frames de salto, durante el salto la gravedad se desactiva, cuando el salto se acaba vuelve la gravedad

int main()
{
    RenderWindow window(VideoMode(1600, 960), "¡Sonic!");
    window.setFramerateLimit(60);
    RectangleShape Sonic(Vector2f(64.f,128.f));
    Sonic.setFillColor(Color::Blue);
    Sonic.setOrigin(16,16);
    Sonic.setPosition(700,0);
    // Creación del Jugador 32x64 la Textura
    vector <RectangleShape> nivel; // Vector de bloques que conforman el nivel
    RectangleShape col(Vector2f(512,128)); // Ejemplo base que se va a hace .push_back en el vector de nivel
    col.setFillColor(Color::Green);
    col.setOrigin(256,64);
    col.setPosition(256,896);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel[0].setPosition(800,nivel[0].getPosition().y);
    nivel[0].setSize(Vector2f(1600,128));
    nivel[0].setOrigin(800,64);
    nivel[1].setPosition(640,768);
    nivel[1].setFillColor(Color::Magenta);
    nivel[2].setPosition(500,640);
    nivel[3].setPosition(-128,768);
    nivel[3].setFillColor(Color::Magenta);// Se establece cada bloque del nivel en su posición
    

    vector <Coin> monedas;  // Vector de las monedas del nivel
    CircleShape coin(16);
    coin.setOrigin(8,8);
    coin.setFillColor(Color::Yellow);   // Moneda base
    Coin blah(coin);    // Clase Coin base
    monedas.push_back(blah);
    monedas.push_back(blah);
    monedas.push_back(blah);
    monedas.push_back(blah);

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
                    Sonic.setFillColor(Color::Green);
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

        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)){
            if (canJump==1){
                Sonic.move(0,-10);
                SonicJump();
                canJump=0;
            }   // Si la ´W´ es presionada y puede saltar (Esta colisionando con el suelo) llama a la secuencia de Salto a empezar e indica que ya no se puede saltar más (No esta tocando suelo)
        }else{
            jump=0;
        }   // Si la W no es presionada detiene el salto (Saltas el tiempo que la ´W´ sea presionada)
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){   
            movex=-10;
            Aa=1; 
            // Si la ´A´ es presionada indica la cantidad de movimiento en x con la variable movex e indica que ha sido presionada con su variable de presión Aa
        }else{
            Aa=0;
        }   // Si la ´A´ no es presionada vuelve su variable de presionada falsa
        if (Keyboard::isKeyPressed(Keyboard::R)){
            Sonic.setPosition(700,0);
            for (int i=0;i<monedas.size();i++){
                if (monedas[i].picked==0){
                    monedas[i].mon.setPosition(Sonic.getPosition().x+(rand()%800),Sonic.getPosition().y+rand()%961);
                }
            }
        }    // Reinicia la posición del Sonic (Por si se cae al vacio)
        if (Keyboard::isKeyPressed(Keyboard::E)){
                ball = 1;
                Sonic.setSize(Vector2f(64.f,64.f));
                Ee = 1;
        }else{
            Ee=0;
        }
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
            movex=10;
            Dd=1;
            // Si la ´D´ es presionada indica la cantidad de movimiento en x con la variable movex e indica que ha sido presionada con su variable de presión Aa
        }else{
            Dd=0;
        }   // Si la ´D´ no es presionada vuelve su variable de presionada falsa
        if (jump>0){
        Sonic.move(0,-10);
        SonicJump();
        Sonic.setFillColor(Color::White);
        }   // Continuación de la secuencia de salto
    
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
        }//Sirve para el deslice del jugador (Menos ddeslice = Hacer el deliz más grande)

        if (movey==0 && movex==0 && canJump==1){
            Sonic.setFillColor(Color::Magenta);
        }else{
            if (canJump==1){
            Sonic.setFillColor(Color::Green);
            }
        }
        //Para saber en qeu movimiento esta Sonic (Si completamente detenido o moviendose en una plataforma)

        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y+1);
        for(int i=0;i<nivel.size();i++){
            if (Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                stopG=1;
            }
        } 
        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-1);
        // Sirve para que no ande cayendo todo el tiempo y que el sprite no ande parpadeando entre cayendo y caminando

        if (Ee==0 && Sonic.getSize() == Vector2f(64.f,64.f)){
            ball = 0;
            Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-64);
            Sonic.setSize(Vector2f(64.f,128.f));
        }

        Sonic.move(0,gravity);
        if (gravity<=25 && stopG==0 && jump==0){
        gravity+=1;
        canJump=0;
        Sonic.setFillColor(Color::Red);
        }   //Gravedad del Jugador
        stopG=0;
        Sonic.move(movex,movey);// Mueve al jugador finalmente cuanto se le indique
        nivel[0].setPosition(Sonic.getPosition().x,nivel[0].getPosition().y);
        window.clear();
        for (int i=0;i<nivel.size();i++){
        window.draw(nivel[i]);
        }   // Dibuja el vector de cuadros
        for (int i=0;i<monedas.size();i++){
        window.draw(monedas[i].mon);
        }   // Dibuja el vector de monedas
        window.draw(Sonic);
        // Blanco = Saltando
        // Morado = Completamente detenido
        // Verde = Moviendose en una plataforma
        // Rojo = Cayendo
        window.display();
    }

    return 0;
}