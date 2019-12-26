//
//  Player.h
//  airhockey
//
//  Created by Sarah Smith on 26/Dec/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#pragma once

#include <stdio.h>

class GameSprite;

struct Player {
    
    enum Side { North, South };
    
    int score;
    
    Side side;
    
    GameSprite *sprite;
};
