//
//  GProtocol.h
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#ifndef GProtocol_h
#define GProtocol_h

#include "cocos2d.h"

namespace gprotocol {
    static const std::string MODE_USER_LOGIN = "GModeUser_login";
    static const std::string MODE_USER_HERTBEAT = "GModeUser_heartBeat";
    static const std::string MODE_USER_ENTERROOM = "GModeUser_enterRoom";
    static const std::string MODE_USER_RECCONN = "GModeUser_recConn";
    static const std::string MODE_USER_UPDATENAME = "GModeUser_updateName";
    static const std::string MODE_USER_UPDATHEAD = "GModeUser_updateHead";
    static const std::string MODE_USER_INFOS = "GModeUser_infos";
    static const std::string MODE_USER_BUYSKIN = "GModeUser_buySkin";
    static const std::string MODE_USER_USESKIN = "GModeUser_useSkin";
    static const std::string MODE_USER_SHARE = "GModeUser_share";

    
    static const std::string MODE_USER_LOGIN_RESULT = "GModeUser_loginResult";
    static const std::string MODE_USER_HERTBEAT_RESULT = "GModeUser_heartBeatResult";
    static const std::string MODE_USER_ENTERROOM_RESULT = "GModeUser_enterRoomResult";
    static const std::string MODE_USER_RECCONN_RESULT = "GModeUser_recConnResult";
    static const std::string MODE_USER_UPDATENAME_RESULT = "GModeUser_updateNameResult";
    static const std::string MODE_USER_UPDATHEAD_RESULT = "GModeUser_updateHeadResult";
    static const std::string MODE_USER_INFOS_RESULT = "GModeUser_infosResult";
    static const std::string MODE_USER_BUYSKIN_RESULT = "GModeUser_buySkinResult";
    static const std::string MODE_USER_USESKIN_RESULT = "GModeUser_useSkinResult";
    static const std::string MODE_USER_SHARE_RESULT = "GModeUser_shareResult";

    //游戏模块
    static const std::string MODE_GAME_STARTGAME = "GModeGame_startGame";
    static const std::string MODE_GAME_MOVE = "GModeGame_move";
    static const std::string MODE_GAME_STOPMOVE = "GModeGame_stopMove";
    static const std::string MODE_GAME_ROOMCOUNTDOWN = "GModeGame_roomCountDown";
    static const std::string MODE_GAME_ROOMTIMEOUT = "GModeGame_roomTimeOut";
    static const std::string MODE_GAME_LEAVEROOM = "GModeGame_leaveRoom";
    static const std::string MODE_GAME_QUESTLEAVEROOM = "GModeGame_questLeaveRoom";
    static const std::string MODE_GAME_ADDBLOCK = "GModeGame_addBlock";
    static const std::string MODE_GAME_EATBLOCK = "GModeGame_eatBlock";
    static const std::string MODE_GAME_ATTACK = "GModeGame_attack";
    static const std::string MODE_GAME_BULLETCOLLISION = "GModeGame_bulletCollision";
    static const std::string MODE_GAME_BULLETCHANGETARGET = "GModeGame_bulletChangeTarget";
    static const std::string MODE_GAME_SPEEDUP = "GModeGame_speedUp";
    static const std::string MODE_GAME_STOPSPEEDUP = "GModeGame_stopSpeedUp";
    static const std::string MODE_GAME_ALLOTROBOT = "GModeGame_allotRobot";
    static const std::string MODE_GAME_ADDROBOT = "GModeGame_addRobot";
    static const std::string MODE_GAME_RANKING = "GModeGame_ranking";
    static const std::string MODE_GAME_RELIVED = "GModeGame_relived";
    static const std::string MODE_GAME_DROP = "GModeGame_drop";
    static const std::string MODE_GAME_COLL = "GModeGame_coll";
    static const std::string MODE_GAME_UPDATEPOS = "GModeGame_updatePos";
    static const std::string MODE_GAME_UPDATEHP = "GModeGame_updateHp";

    
    static const std::string MODE_GAME_STARTGAME_RESULT = "GModeGame_startGameResult";
    static const std::string MODE_GAME_JOINROOM_RESULT = "GModeGame_joinRoomResult";
    static const std::string MODE_GAME_MOVE_RESULT = "GModeGame_moveResult";
    static const std::string MODE_GAME_STOPMOVE_RESULT = "GModeGame_stopMoveResult";
    static const std::string MODE_GAME_ROOMCOUNTDOWN_RESULT = "GModeGame_roomCountDownResult";
    static const std::string MODE_GAME_ROOMTIMEOUT_RESULT = "GModeGame_roomTimeOutResult";
    static const std::string MODE_GAME_LEAVEROOM_RESULT = "GModeGame_leaveRoomResult";
    static const std::string MODE_GAME_QUESTLEAVEROOM_RESULT = "GModeGame_questLeaveRoomResult";
    static const std::string MODE_GAME_ADDBLOCK_RESULT = "GModeGame_addBlockResult";
    static const std::string MODE_GAME_EATBLOCK_RESULT = "GModeGame_eatBlockResult";
    static const std::string MODE_GAME_ATTACKL_RESULT = "GModeGame_attackResult";
    static const std::string MODE_GAME_BULLETCOLLISION_RESULT = "GModeGame_bulletCollisionResult";
    static const std::string MODE_GAME_BULLETCHANGETARGET_RESULT = "GModeGame_bulletChangeTargetResult";
    static const std::string MODE_GAME_SPEEDUP_RESULT = "GModeGame_speedUpResult";
    static const std::string MODE_GAME_STOPSPEEDUP_RESULT = "GModeGame_stopSpeedUpResult";
    static const std::string MODE_GAME_ALLOTROBOT_RESULT = "GModeGame_allotRobotResult";
    static const std::string MODE_GAME_ADDROBOT_RESULT = "GModeGame_addRobotResult";
    static const std::string MODE_GAME_RANKING_RESULT = "GModeGame_rankingResult";
    static const std::string MODE_GAME_RELIVED_RESULT = "GModeGame_relivedResult";
    static const std::string MODE_GAME_DROP_RESULT = "GModeGame_dropResult";
    static const std::string MODE_GAME_COLL_RESULT = "GModeGame_collResult";
    static const std::string MODE_GAME_UPDATEPOS_RESULT = "GModeGame_updatePosResult";
    static const std::string MODE_GAME_UPDATEHP_RESULT = "GModeGame_updateHpResult";


}


#endif /* GProtocol_h */
