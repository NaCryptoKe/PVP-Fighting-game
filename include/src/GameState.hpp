#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

enum class GameState
{
    MAIN_MENU,

    CHARACTER_SELECT,
    
    FIGHTING,
    PAUSED,

    ROUND_OVER,
    MATCH_OVER,

    SETTINGS
};

#endif // GAME_STATE_HPP