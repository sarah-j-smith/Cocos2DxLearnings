//
//  Player.hpp
//  airhockey
//
//  Created by Sarah Smith on 26/Dec/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>

#include "GameSprite.hpp"

struct Player {
    
    int score;
    
    GameSprite *sprite;
}

#endif /* Player_hpp */
