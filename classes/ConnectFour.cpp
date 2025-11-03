#include "ConnectFour.h"

ConnectFour::ConnectFour()
{
    _grid = new Grid(7, 6);
}

ConnectFour::~ConnectFour()
{
    delete _grid;
}

Bit* ConnectFour::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class?
    bit->LoadTextureFromFile(playerNumber == AI_PLAYER ? "red.png" : "yellow.png");
    bit->setOwner(getPlayerAt(playerNumber == AI_PLAYER ? 1 : 0));
    return bit;
}

void ConnectFour::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 6;
    _gameOptions.rowY = 7;
    _grid->initializeSquares(80, "square.png");

    if (gameHasAI()) {
        setAIPlayer(AI_PLAYER);
    }

    startGame();
}

bool ConnectFour::actionForEmptyHolder(BitHolder &holder)
{
    ChessSquare* square = static_cast<ChessSquare*>(&holder);
    int col = square->getColumn();
    int empty = lowestEmptyRowInColumn(col);
    if (empty == -1) {
        return false;
    }
    Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);
    if (bit) {
        bit->setPosition(_grid->getSquare(col, empty)->getPosition());
        _grid->getSquare(col, empty)->setBit(bit);
    // save the last move (column, row) into the object so other methods can access it
    _lastMove = { col, empty };
        endTurn();
        return true;
    }   
    return false;
}

bool ConnectFour::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // bits cannot be moved once placed
    return false;
}

bool ConnectFour::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    // bits cannot be moved once placed
    return false;
}

void ConnectFour::stopGame()
{
    // clean up the board
    _grid->forEachSquare([&](BitHolder* square, int x, int y) {
        square->destroyBit();
    });
}

Player* ConnectFour::ownerAt(int index) const
{
    auto square = _grid->getSquare(index / 7, index % 7);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* ConnectFour::checkForWinner() {
    Player* currPlayer = _grid->getSquare(_lastMove.first, _lastMove.second)->bit()->getOwner();
    int count = 1;
    int increment = 1;
    int exit = 0;
    //checking horizontal'
    ChessSquare *positiveSquare = _grid->getSquare(_lastMove.first + increment, _lastMove.second);
    ChessSquare *negativeSquare = _grid->getSquare(_lastMove.first - increment, _lastMove.second);
    while (positiveSquare || negativeSquare) {
        increment++;
        if (positiveSquare && positiveSquare->bit() && positiveSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (negativeSquare && negativeSquare->bit() && negativeSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (exit >=2) break;
        exit = 0;
        if (count >= 4) {
            return currPlayer;
        }
        positiveSquare = _grid->getSquare(_lastMove.first + increment, _lastMove.second);
        negativeSquare = _grid->getSquare(_lastMove.first - increment, _lastMove.second);
    }
    //checking vertical
    count = 1;
    increment = 1;
    exit = 0;
    positiveSquare = _grid->getSquare(_lastMove.first, _lastMove.second + increment);
    negativeSquare = _grid->getSquare(_lastMove.first, _lastMove.second - increment);
    while (positiveSquare || negativeSquare) {
        increment++;
        if (positiveSquare && positiveSquare->bit() && positiveSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (negativeSquare && negativeSquare->bit() && negativeSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (exit >=2) break;
        exit = 0;
        if (count >= 4) {
            return currPlayer;
        }
        positiveSquare = _grid->getSquare(_lastMove.first, _lastMove.second + increment);
        negativeSquare = _grid->getSquare(_lastMove.first, _lastMove.second - increment);
    }
    //checking diagonal /
    count = 1;
    increment = 1;
    exit = 0;
    positiveSquare = _grid->getSquare(_lastMove.first + count, _lastMove.second + count);
    negativeSquare = _grid->getSquare(_lastMove.first - count, _lastMove.second - count);
    while (positiveSquare || negativeSquare) {
        increment++;
        if (positiveSquare && positiveSquare->bit() && positiveSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (negativeSquare && negativeSquare->bit() && negativeSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (exit >=2) break;
        exit = 0;
        if (count >= 4) {
            return currPlayer;
        }
        positiveSquare = _grid->getSquare(_lastMove.first + increment, _lastMove.second + increment);
        negativeSquare = _grid->getSquare(_lastMove.first - increment, _lastMove.second - increment);
    }
    //checking diagonal \ 
    count = 1;
    increment = 1;
    exit = 0;
    positiveSquare = _grid->getSquare(_lastMove.first + count, _lastMove.second - count);
    negativeSquare = _grid->getSquare(_lastMove.first - count, _lastMove.second + count);
    while (positiveSquare || negativeSquare) {
        increment++;
        if (positiveSquare && positiveSquare->bit() && positiveSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (negativeSquare && negativeSquare->bit() && negativeSquare->bit()->getOwner() == currPlayer) count++;
        else exit++;
        if (exit >=2) break;
        exit = 0;
        if (count >= 4) {
            return currPlayer;
        }
        positiveSquare = _grid->getSquare(_lastMove.first + increment, _lastMove.second - increment);
        negativeSquare = _grid->getSquare(_lastMove.first - increment, _lastMove.second + increment);
    }
    printf("no winner yet\n");
    return nullptr;
}

bool ConnectFour::checkForDraw()
{
    bool isDraw = true;
    // check to see if the board is full
    _grid->forEachSquare([&isDraw](BitHolder* square, int x, int y) {
        if (!square->bit()) {
            isDraw = false;
        }
    });
    return isDraw;
}

std::string ConnectFour::initialStateString()
{
    return std::string(42, '0');
}

std::string ConnectFour::stateString()
{
    std::string s = std::string(42, '0');
    // Grid is row-major: index = y * width + x (width == 7)
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        Bit *bit = square->bit();
        if (bit) {
            s[y * 7 + x] = std::to_string(bit->getOwner()->playerNumber()+1)[0];
        }
    });
    return s;
}

void ConnectFour::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y*7 + x;
        int playerNumber = s[index] - '0';  
        if (playerNumber) {
            square->setBit( PieceForPlayer(playerNumber-1) );
        } else {
            square->setBit( nullptr );
        }
    });
}

int ConnectFour::lowestEmptyRowInColumn(int col) const
{
    for (int row = 5; row >= 0; --row) {
        if (!_grid->getSquare(col, row)->bit()) {
            printf("%d %d\n", col, row);
            return row;
        }
    }
    return -1; // Column is full
}

void ConnectFour::updateAI() 
{
    int bestVal = -1000;
    BitHolder* bestMove = nullptr;
    std::string state = stateString();

    for (int col = 0; col < 7; ++col) {
        int row = lowestEmptyRowInColumn(col);
        if (row != -1) {
            // Make the move
            state[row * 7 + col] = '2'; // AI is player 2
            int moveVal = -negamax(state, 0, HUMAN_PLAYER);
            // Undo the move
            state[row * 7 + col] = '0';
            // If the value of the current move is more than the best value, update best
            if (moveVal > bestVal) {
                bestMove = _grid->getSquare(col, row);
                bestVal = moveVal;
            }
        }
    }

    if (bestMove) {
        if (actionForEmptyHolder(*bestMove)) {
        }
    }
}

int ConnectFour::negamax(std::string& state, int depth, int playerColor) {
    int score = evaluateAIBoard(state);
}

bool isAIBoardFull(const std::string& state) {
    return state.find('0') == std::string::npos;
}
//diagonals just check if we go out of bounds
//up down also just checks for out of bounds
//left right needs to check for row changes (store x % 7, if x % 7 - increment < 0 then stop, if x % 7 + increment > 6 then stop)
int evaluateAIBoard(const std::string& state) {
    int score = 0;
    int positionInRow;
    int increment = 1;
    int count = 1;
    int exit = 0;
    for (int x = 0; x < state.size(); x++) {
        if (state[x] == '0') {
            continue;
        } else {
            //check horizontal
            positionInRow = x % 7;
            while (positionInRow - increment >= 0 || positionInRow + increment <= 6) {
                if (positionInRow - increment >= 0 && (state[x - increment] == state[x] || state[x - increment] == '0')) {
                    count++;
                } else exit++;
                if (positionInRow + increment <= 6 && (state[x + increment] == state[x] || state[x + increment] == '0')) {
                    count++;
                } else exit++;
                if (exit >=2) break;
                if (count >= 4) {
                    if (state[x] == '2') score++;
                    else score--;
                    break;
                }
                exit = 0;
                increment++;
            }
            //check vertical
            while (x - increment * 7 >= 0 || x + increment * 7 < state.size()) {
                if (x - increment * 7 >= 0 && (state[x - increment * 7] == state[x] || state[x - increment * 7] == '0')) {
                    count++;
                } else exit++;
                if (x + increment * 7 < state.size() && (state[x + increment * 7] == state[x] || state[x + increment * 7] == '0')) {
                    count++;
                } else exit++;
                if (exit >=2) break;
                if (count >= 4) {
                    if (state[x] == '2') score++;
                    else score--;
                    break;
                }
                exit = 0;
                increment++;
            }
            //check diagonal /
            while (x - increment * 6 >= 0 || x + increment * 6 < state.size()) {
                if (x - increment * 6 >= 0 && (state[x - increment * 6] == state[x] || state[x - increment * 6] == '0')) {
                    count++;
                } else exit++;
                if (x + increment * 6 < state.size() && (state[x + increment * 6] == state[x] || state[x + increment * 6] == '0')) {
                    count++;
                } else exit++;
                if (exit >=2) break;
                if (count >= 4) {
                    if (state[x] == '2') score++;
                    else score--;
                    break;
                }
                exit = 0;
                increment++;
            }
            //check diagonal \ 
            while (x - increment * 8 >= 0 || x + increment * 8 < state.size()) {
                if (x - increment * 8 >= 0 && (state[x - increment * 8] == state[x] || state[x - increment * 8] == '0')) {
                    count++;
                } else exit++;
                if (x + increment * 8 < state.size() && (state[x + increment * 8] == state[x] || state[x + increment * 8] == '0')) {
                    count++;
                } else exit++;
                if (exit >=2) break;
                if (count >= 4) {
                    if (state[x] == '2') score++;
                    else score--;
                    break;
                }
                exit = 0;
                increment++;
            }
        }
    }
    return score;
}