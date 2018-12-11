#include "heuristic.h"
#include <algorithm>

float heuristic::numPoints(const gameState& state, int player){
    return state.scores()[player];
}

float heuristic::numGoAgainTurns(const std::array<int, holes>& playerHoles){
    int turnsAvailable{0};
    for(int i=0;i<holes;i++){
        if (playerHoles[i] == (holes - i)) turnsAvailable++;
    }
    return turnsAvailable;
}

float heuristic::numStonesOnSide(const std::array<int, holes>& playerHoles){
    int total{0};
    for(const int& h: playerHoles){
        total += h;
    }
}

float heuristic::emptyHoles(const std::array<int, holes>& playerHoles){
    return std::count(playerHoles.begin(),playerHoles.end(),0);
}

float heuristic::vulnerableHoles(const std::array<int, holes>& playerHoles, const std::array<int, holes>& opponentHoles){
    int vulnerable{0};
    for(int i=0;i<holes;i++){
        if (playerHoles[i] > 0 && opponentHoles[holes-i-1] == 0)
            vulnerable++; 
    }
    return vulnerable;
}

float heuristic::operator()(const gameState& state, int player){
    // get state properties
    int opponent = state.otherPlayer(player);
    const gameBoard& board = state.getGameBoard();
    std::array<int,holes> playerHoles;
    std::array<int,holes> opponentHoles;
    for(int i=0;i<holes;i++){
        playerHoles[i] = board[state.holeIndex(i+1,player)];
        opponentHoles[i] = board[state.holeIndex(i+1,opponent)];
    }

    // mix heuristics
    float heuristicValue = 
        + numPoints(state, player) * weights[0]
        - numPoints(state, opponent) * weights[1]
        + numGoAgainTurns(playerHoles) * weights[2]
        - numGoAgainTurns(opponentHoles) * weights[3]
        + (2 * numStonesOnSide(playerHoles) - 1) * weights[4]
        + (2 * numStonesOnSide(opponentHoles) - 1) * weights[5]
        + (2 * emptyHoles(playerHoles) - 1) * weights[6]
        + (2 * emptyHoles(opponentHoles) - 1) * weights[7]
        - vulnerableHoles(playerHoles, opponentHoles) * weights[8]
        + vulnerableHoles(opponentHoles, playerHoles) * weights[9];

    return heuristicValue;
}
