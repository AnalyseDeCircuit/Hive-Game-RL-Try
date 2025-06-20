#include "Game.h"
#include "AIPlayer.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

// 静态成员变量定义
std::shared_ptr<Game> Game::instance = nullptr;

Game::Game() : isGameOver(false), turnCount(0) {
    try {
        // 初始化棋盘
        board = ChessBoard::getInstance();
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;
        
    } catch (const std::exception& e) {
        std::cerr << "Game构造函数错误: " << e.what() << std::endl;
        throw;
    }
}

std::shared_ptr<Game> Game::getInstance() {
    try {
        if (instance == nullptr) {
            instance = std::shared_ptr<Game>(new Game());
        }
        return instance;
    } catch (const std::exception& e) {
        std::cerr << "getInstance错误: " << e.what() << std::endl;
        throw;
    }
}

void Game::initializeGame(const std::string& player1Name, const std::string& player2Name) {
    try {
        // 创建两个普通玩家
        player1 = std::make_shared<Player>(player1Name);
        player2 = std::make_shared<Player>(player2Name);
        
        // 设置当前玩家为玩家1
        currentPlayer = player1;
        
        // 重置游戏状态
        isGameOver = false;
        turnCount = 0;
        
        // 清空棋盘
        if (board) {
            board->clearBoard();
        }
        
        std::cout << "游戏初始化完成: " << player1Name << " vs " << player2Name << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "initializeGame错误: " << e.what() << std::endl;
        throw;
    }
}

void Game::initializeGameWithAI(std::shared_ptr<Player> human, std::shared_ptr<Player> ai) {
    try {
        if (!human || !ai) {
            throw std::invalid_argument("玩家指针不能为空");
        }
        
        player1 = human;
        player2 = ai;
        
        // 设置当前玩家为玩家1
        currentPlayer = player1;
        
        // 重置游戏状态
        isGameOver = false;
        turnCount = 0;
        
        // 清空棋盘
        if (board) {
            board->clearBoard();
        }
        
        std::cout << "AI游戏初始化完成" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "initializeGameWithAI错误: " << e.what() << std::endl;
        throw;
    }
}

void Game::restartGame() {
    try {
        // 重置游戏状态
        isGameOver = false;
        turnCount = 0;
        
        // 清空棋盘
        if (board) {
            board->clearBoard();
        }
        
        // 重置当前玩家为玩家1
        if (player1) {
            currentPlayer = player1;
        }
        
        std::cout << "游戏重新开始" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "restartGame错误: " << e.what() << std::endl;
    }
}

void Game::playTurn() {
    try {
        // 安全检查
        if (isGameOver) {
            return;
        }
        
        if (!currentPlayer || !board) {
            throw std::runtime_error("游戏状态无效");
        }
        
        // 让当前玩家做出移动
        try {
            std::cout << "玩家 " << currentPlayer->getName() << " 的回合" << std::endl;
            
            // 检查是否是AI玩家
            auto aiPlayer = std::dynamic_pointer_cast<AIPlayer>(currentPlayer);
            if (aiPlayer) {
                // AI玩家使用特殊的makeMove方法
                GameState currentState(*board, *player1, *player2, (currentPlayer == player1) ? 0 : 1, turnCount);
                aiPlayer->makeMove(*board, currentState);
            } else {
                // 普通玩家的简化移动
                std::cout << "普通玩家移动（简化）" << std::endl;
            }
            
            turnCount++;
            
            // 检查游戏是否结束
            if (checkGameOver()) {
                isGameOver = true;
                return;
            }
            
            // 切换玩家
            switchPlayer();
            
        } catch (const std::exception& e) {
            std::cerr << "playTurn移动错误: " << e.what() << std::endl;
            // 发生错误时切换玩家
            switchPlayer();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "playTurn错误: " << e.what() << std::endl;
        // 发生严重错误时结束游戏
        isGameOver = true;
    }
}

void Game::playAITurn() {
    try {
        // AI回合不需要用户输入，直接调用playTurn
        playTurn();
        
    } catch (const std::exception& e) {
        std::cerr << "playAITurn错误: " << e.what() << std::endl;
        isGameOver = true;
    }
}

void Game::switchPlayer() {
    try {
        if (!player1 || !player2) {
            return;
        }
        
        if (currentPlayer == player1) {
            currentPlayer = player2;
        } else {
            currentPlayer = player1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "switchPlayer错误: " << e.what() << std::endl;
    }
}

bool Game::checkGameOver() {
    try {
        if (!board) {
            return true;
        }
        
        // 简单的游戏结束检查
        if (turnCount > 20) { // 减少回合数，加快测试
            return true;
        }
        
        return false;
        
    } catch (const std::exception& e) {
        std::cerr << "checkGameOver错误: " << e.what() << std::endl;
        return true;
    }
}

std::shared_ptr<Player> Game::getWinner() const {
    try {
        if (!isGameOver) {
            return nullptr;
        }
        
        // 简单的获胜者判断
        return (turnCount % 2 == 0) ? player1 : player2;
        
    } catch (const std::exception& e) {
        std::cerr << "getWinner错误: " << e.what() << std::endl;
        return nullptr;
    }
}

void Game::displayBoard() const {
    try {
        if (board) {
            board->displayBoard();
        } else {
            std::cout << "棋盘未初始化" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "displayBoard错误: " << e.what() << std::endl;
    }
}

void Game::displayTurnCount() const {
    try {
        std::cout << "当前回合数: " << turnCount << std::endl;
        if (currentPlayer) {
            std::cout << "当前玩家: " << currentPlayer->getName() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "displayTurnCount错误: " << e.what() << std::endl;
    }
}

void Game::endGame() {
    try {
        isGameOver = true;
        
        std::cout << "\n=== 游戏结束 ===" << std::endl;
        displayBoard();
        displayTurnCount();
        
        std::shared_ptr<Player> winner = getWinner();
        if (winner) {
            std::cout << "获胜者: " << winner->getName() << std::endl;
        } else {
            std::cout << "游戏平局" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "endGame错误: " << e.what() << std::endl;
    }
}

std::shared_ptr<Player> Game::getCurrentPlayer() const {
    return currentPlayer;
}

