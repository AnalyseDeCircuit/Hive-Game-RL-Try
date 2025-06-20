#ifndef GAME_H
#define GAME_H

#include <memory>
#include <iostream>
#include "Player.h"
#include "Board.h"

class Game {
private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    std::shared_ptr<ChessBoard> board;
    std::shared_ptr<Player> currentPlayer;

    // 游戏状态
    bool isGameOver;

    // 记录当前回合数
    int turnCount;

    // 私有化构造函数，防止外部创建实例
    Game();

    // 私有化拷贝构造函数和赋值操作符，防止拷贝和赋值
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // 静态成员变量，存储唯一实例
    static std::shared_ptr<Game> instance;

    // 切换当前玩家
    void switchPlayer();

    // 检查游戏是否结束
    bool checkGameOver();

    // 获取胜利者
    std::shared_ptr<Player> getWinner() const;

public:
    // 获取唯一实例
    static std::shared_ptr<Game> getInstance();

    // 初始化游戏
    void initializeGame(const std::string& player1Name, const std::string& player2Name);
    
    // 初始化游戏（支持AI玩家）
    void initializeGameWithAI(std::shared_ptr<Player> human, std::shared_ptr<Player> ai);

    // 重新开始游戏
    void restartGame();

    // 进行游戏回合
    void playTurn();
    
    // AI游戏回合（不需要用户输入）
    void playAITurn();

    // 显示当前棋盘
    void displayBoard() const;

    // 显示当前回合数
    void displayTurnCount() const;

    // 结束游戏
    void endGame();

    // 返回当前玩家
    std::shared_ptr<Player> getCurrentPlayer() const;
    
    // 获取棋盘实例
    std::shared_ptr<ChessBoard> getBoard() const { return board; }
    
    // 公共接口供AI训练器使用
    bool isGameFinished() const { return isGameOver; }
    std::shared_ptr<Player> getGameWinner() const { return getWinner(); }
    void nextPlayer() { switchPlayer(); }

};

#endif // GAME_H
