
#include "Player.h"

Player::~Player(){}

bool Player::isComputer(){
    return !isHuman();
}
