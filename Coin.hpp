#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class Coin{
    public:
CircleShape mon;
bool picked;

void UpdateCoin(){
    if (this->picked==1){
        this->mon.setPosition(0,this->mon.getPosition().y);
        this->mon.setFillColor(Color::Cyan);
    }
}   // Checa si ya ha sido tomada si es asi la coloca al borde (donde no se podra agarrar más) si no 

Coin(CircleShape coin){
    this->picked = 0;
    this->mon = coin;
    this->mon.setPosition(rand()%1601,rand()%961);
}

};