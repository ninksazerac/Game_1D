#include "Bullet.h"

Bullet::Bullet(sf::Texture* texture, int* effect)
{
    this->effect = effect;
    for (int i = 0; i < fire_num; i++) {
        Sword sword(texture, sf::Vector2u(4, 6), 0.3f, sf::Vector2f(100, 100));
        sword.push_back(sword);
    }
}

Bullet::~Bullet()
{
}

void Bullet::Draw(sf::RenderWindow& window)
{
    deltaTime = clock.restart().asSeconds();
    this->action();
    for (int i = 0; i < fire_num; i++)
    {
        if (sword[i].isActive() == true)
        {
            //std::cout << *this->effect << std::endl;

            sword[i].Update(deltaTime);
            sword[i].Draw(window);

            if (sword[i].getDeltaDistance() > 200 && sword[i].getEffect() == 1) {
                sword[i].setEffect(0);
                sword[i + 1].setEffect(0);
                sword[i + 2].setEffect(0);
                sword[i].setY(-50);
                sword[i + 1].setY(+50);
                i += 3;
            }
        }
    }
}

sf::Vector2f Bullet::getPosition()
{
    for (int i = 0; i < sword_num; i++) {
        return this->sword[i].getPosition();
    }

}


sf::Vector2f Bullet::getHalfSize()
{
    for (int i = 0; i < fire_num; i++) {
        return this->sword[i].getHalfSize();
    }
}

bool Bullet::canAttack()
{
    for (int i = 0; i < fire_num; i++)
    {
        if (this->sword[i].isActive() == false) {
            return true;
        }
    }
    return false;
}

void Bullet::action()
{
    if ((*this->evnt).type == sf::Event::EventType::TextEntered
        and (*this->evnt).text.unicode == ' '
        and this->isEPrees == false)
    {
        //std::cout << "set isSpacebarPrees: true" << std::endl;
        this->isEPrees = true;
        for (int i = 0; i < fire_num; i++)
        {
            if (this->sword[i].isActive() == false) {
                //this->fires[i].setPosition(sf::Vector2f(300.0f,300.0f)); => fixed : 31/10/20 12.20AM.

                //std::cout << "fire [" << i << "]" << std::endl;
                //std::cout << fires[i].getPosition().x << std::endl;
                switch (*this->effect)
                {
                case 0:
                    sword[i].setPosition(this->positionPlayer);
                    this->sword[i].setActive(true);
                    break;
                case 2:
                    sword[i].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y - 50));
                    sword[i + 1].setPosition(sf::Vector2f(this->positionPlayer.x + 30, this->positionPlayer.y));
                    sword[i + 2].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y + 50));
                    this->sword[i].setActive(true);
                    this->sword[i + 1].setActive(true);
                    this->sword[i + 2].setActive(true);
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
    if ((*this->evnt).type == sf::Event::EventType::KeyReleased && this->isEPrees == true)
    {
        this->isEPrees = false;
        // std::cout << "set isSpacebarPrees: false" << std::endl;
    }

}