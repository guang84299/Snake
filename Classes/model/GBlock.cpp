//
//  GBlock.cpp
//  OnlineGame
//
//  Created by guang on 16/10/9.
//
//

#include "GBlock.h"

GBlock* GBlock::create()
{
    GBlock* block = new GBlock();
    block->autorelease();
    return block;
}

GBlock* GBlock::create(GJsonObject* obj)
{
    GBlock* block = new GBlock();
    
    block->_id = obj->getInt("id");
    block->type = obj->getInt("type");
    block->blockType = obj->getInt("blockType");
    block->exp = obj->getInt("exp");
    block->x = obj->getDouble("x");
    block->y = obj->getDouble("y");
    std::string state = obj->getString("state");
    
    block->state = block->toGBlockState(state);
    
    block->autorelease();
    return block;
}

GBlock::State GBlock::toGBlockState(std::string &state)
{
    GBlock::State s = GBlock::State::IDLE;
    if(state == "DIE")
        s = GBlock::State::DIE;
    return s;
}
