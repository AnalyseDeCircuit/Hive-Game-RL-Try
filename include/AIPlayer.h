#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "Player.h"
#include <vector>
#include <random>
#include <memory>

// 前向声明
class GameState;
class Action;
class NeuralNetwork;
class Game;

// 游戏状态表示
class GameState {
public:
    // 棋盘状态 (10x10x8) - 每个位置最多8种棋子类型
    std::vector<std::vector<std::vector<int>>> board;
    
    // 玩家手牌信息
    std::map<std::string, int> player1Pieces;
    std::map<std::string, int> player2Pieces;
    
    // 当前玩家 (0 = player1, 1 = player2)
    int currentPlayer;
    
    // 游戏阶段信息
    int turnCount;
    bool player1QueenPlaced;
    bool player2QueenPlaced;
    
    GameState();
    GameState(const ChessBoard& board, const Player& p1, const Player& p2, int current, int turn);
    
    // 状态编码为神经网络输入
    std::vector<float> encode() const;
    
    // 获取所有合法动作
    std::vector<Action> getLegalActions() const;
    
    // 检查游戏是否结束
    bool isGameOver() const;
    
    // 获取游戏结果 (1=player1胜利, -1=player2胜利, 0=平局或未结束)
    int getGameResult() const;
};

// 动作表示
class Action {
public:
    enum ActionType {
        PLACE_PIECE,
        MOVE_PIECE
    };
    
    ActionType type;
    int x, y;           // 目标位置
    int fromX, fromY;   // 移动起始位置 (仅用于MOVE_PIECE)
    int pieceType;      // 棋子类型
    
    Action(ActionType t, int x, int y, int pieceType);
    Action(ActionType t, int fromX, int fromY, int toX, int toY, int pieceType);
    
    // 动作编码
    int encode() const;
    static Action decode(int encoded);
    
    // 验证动作是否合法
    bool isValid(const GameState& state) const;
};

// 简化的神经网络接口
class NeuralNetwork {
private:
    std::vector<std::vector<float>> weights1;
    std::vector<std::vector<float>> weights2;
    std::vector<float> bias1;
    std::vector<float> bias2;
    
    int inputSize;
    int hiddenSize;
    int outputSize;
    
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize);
    
    // 前向传播
    std::vector<float> forward(const std::vector<float>& input);
    
    // 简单的权重更新
    void updateWeights(const std::vector<float>& input, 
                      const std::vector<float>& target, 
                      float learningRate);
    
    // 保存和加载模型
    void saveModel(const std::string& filename);
    void loadModel(const std::string& filename);
    
    // 随机初始化权重
    void randomizeWeights();
};

// AI玩家类
class AIPlayer : public Player {
private:
    std::unique_ptr<NeuralNetwork> network;
    std::mt19937 rng;
    
    // 训练参数
    float epsilon;          // 探索率
    float learningRate;     // 学习率
    float discountFactor;   // 折扣因子
    
    // 经验回放缓冲区
    struct Experience {
        GameState state;
        Action action;
        float reward;
        GameState nextState;
        bool done;
    };
    std::vector<Experience> replayBuffer;
    size_t maxBufferSize;
    
public:
    AIPlayer(const std::string& name, float epsilon = 0.1, float learningRate = 0.001);
    
    // AI决策接口
    Action selectAction(const GameState& state);
    
    // 训练接口
    void addExperience(const GameState& state, const Action& action, 
                      float reward, const GameState& nextState, bool done);
    void trainOnBatch(size_t batchSize = 32);
    
    // 参数调整
    void setEpsilon(float eps) { epsilon = eps; }
    void setLearningRate(float lr) { learningRate = lr; }
    
    // 模型管理
    void saveModel(const std::string& filename);
    void loadModel(const std::string& filename);
    
    // 重写父类方法以支持AI决策
    void makeMove(ChessBoard& board, const GameState& currentState);
    
private:
    // 辅助方法
    float evaluateState(const GameState& state);
    Action selectBestAction(const GameState& state);
    Action selectRandomAction(const GameState& state);
    std::vector<float> computeQValues(const GameState& state);
};

// 训练管理器
class AITrainer {
private:
    std::shared_ptr<AIPlayer> aiPlayer1;
    std::shared_ptr<AIPlayer> aiPlayer2;
    std::shared_ptr<Game> game;
    
    // 训练统计
    int totalGames;
    int ai1Wins;
    int ai2Wins;
    int draws;
    
public:
    AITrainer();
    
    // 训练接口
    void trainSelfPlay(int numGames);
    void trainAgainstRandom(int numGames);
    
    // 评估接口
    void evaluateAI(int numGames);
    
    // 统计信息
    void printTrainingStats();
    void resetStats();
    
    // 获取训练好的AI
    std::shared_ptr<AIPlayer> getTrainedAI() { return aiPlayer1; }
};

#endif // AI_PLAYER_H

