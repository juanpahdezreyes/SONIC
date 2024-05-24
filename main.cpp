#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

float gravity = 0;
int jump=0;
bool canJump = 0;
bool Aa=0;
bool Dd=0;
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
    RenderWindow window(VideoMode(1400, 900), "Sonic works!");
    window.setFramerateLimit(60);
    CircleShape Sonic(32.f);
    Sonic.setFillColor(Color::Blue);
    Sonic.setOrigin(16,16);
    Sonic.setPosition(700,0);
    // Creación del Jugador 64x64 la Textura
    vector <RectangleShape> nivel; // Vector de bloques que conforman el nivel
    RectangleShape col(Vector2f(500,100)); // Ejemplo base que se va a hace .push_back en el vector de nivel
    col.setFillColor(Color::Green);
    col.setOrigin(250,50);
    col.setPosition(250,800);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel.push_back(col);
    nivel[1].setPosition(650,500);
    nivel[2].setPosition(500,700);
    nivel[3].setPosition(-200,700);
    nivel[3].setFillColor(Color::Magenta);// Se establece cada bloque del nivel en su posición

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
                
                deltaX = Sonic.getPosition().x - nivel[i].getPosition().x;
                deltaY = Sonic.getPosition().y - nivel[i].getPosition().y;

                intersectX = abs(deltaX) - ((Sonic.getRadius())+(nivel[i].getSize().x/2));
                intersectY = abs(deltaY) - ((Sonic.getRadius())+(nivel[i].getSize().y/2));
                /*
                 Hilze Vonck. (2017). SFML 2.4 para principiantes - 12: Detección de colisiones (AABB)..
                 Youtube. https://www.youtube.com/watch?v=l2iCYCLi6MU
                */
                
                if (Sonic.getPosition().y < nivel[i].getPosition().y && intersectX < intersectY){ 
                    gravity=0;
                    while(Sonic.getGlobalBounds().intersects(nivel[i].getGlobalBounds())){
                        Sonic.setPosition(Sonic.getPosition().x,Sonic.getPosition().y-1);
                    }
                    canJump=1;
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
        if (Keyboard::isKeyPressed(Keyboard::R))    // Reinicia la posición del Sonic (Por si se cae al vacio)
            Sonic.setPosition(700,0);
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

        Sonic.move(0,gravity);
        if (gravity<=25){
        gravity+=1;
        }   //Gravedad del Jugador

        Sonic.move(movex,movey);// Mueve al jugador finalmente cuanto se le indique
        window.clear();
        for (int i=0;i<nivel.size();i++){
        window.draw(nivel[i]);
        }   // Dibuja el vector de cuadros
        window.draw(Sonic);
        window.display();
    }

    return 0;
}