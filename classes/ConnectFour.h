#pragma once
#include "Game.h"
#include <utility>

class ConnectFour : public Game
{
public:
    ConnectFour();
    ~ConnectFour();

    void       setUpBoard() override;
    Player*    checkForWinner() override;
    bool       checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void       setStateString(const std::string &s) override;
    bool       actionForEmptyHolder(BitHolder &holder) override;
    bool       canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool       canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void       stopGame() override;

    void       updateAI() override;
    bool       gameHasAI() override { return true; }
    Grid* getGrid() override { return _grid; }
private:
    Bit *      PieceForPlayer(const int playerNumber);
    Player*    ownerAt(int index ) const;
    int        lowestEmptyRowInColumn(int col) const;
    int        negamax(std::string& state, int depth, int playerColor);
    Grid*      _grid;
    // stores the last move made: {column, row}
    std::pair<int,int> _lastMove{ -1, -1 };


};