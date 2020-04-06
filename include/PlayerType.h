#ifndef PLAYERTYPES_H_INCLUDED
#define PLAYERTYPES_H_INCLUDED

#include "Player.h"

class PlayerType{
public:

    static enum class Type : unsigned char{
        HUMAN,
        COMP_EASY,
        COMP_MED
    }const DEFAULT_TYPE = Type::HUMAN;

    PlayerType(Type type = DEFAULT_TYPE);

    Player* createPlayer();

    PlayerType getNextType();

private:
    Type type;
};

#endif // PLAYERTYPES_H_INCLUDED
