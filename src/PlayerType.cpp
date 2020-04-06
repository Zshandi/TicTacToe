#include "PlayerType.h"

#include "HumanPlayer.h"
#include "RandomComputerPlayer.h"
#include "RowComputerPlayer.h"

PlayerType::PlayerType(Type type){
    this->type = type;
}

Player* PlayerType::createPlayer(){
    switch(type){
    case Type::HUMAN:
        return new HumanPlayer();
    case Type::COMP_EASY:
        return new RandomComputerPlayer();
    case Type::COMP_MED:
        return new RowComputerPlayer();
    default:
        return PlayerType().createPlayer();
    }
}

PlayerType PlayerType::getNextType(){
    switch(type){
    case Type::HUMAN:
        return PlayerType(Type::COMP_EASY);
    case Type::COMP_EASY:
        return PlayerType(Type::COMP_MED);
    default:
        return PlayerType(DEFAULT_TYPE);
    }
};
