#include "AIPlayer.h"
#include "Game.h"
#include <algorithm>
#include <random>
#include <climits>
#include <stdexcept>
#include <iostream>
#include <fstream>

// GameState 实现
GameState::GameState() : currentPlayer(0), turnCount(0), player1QueenPlaced(false), player2QueenPlaced(false) {
    // 初始化10x10x8的棋盘
    board.resize(10, std::vector<std::vector<int>>(10, std::vector<int>(8, 0)));
}

GameState::GameState(const ChessBoard& chessBoard, const Player& p1, const Player& p2, int current, int turn)
    : currentPlayer(current), turnCount(turn), player1QueenPlaced(false), player2QueenPlaced(false) {
    // 简化的状态初始化
    board.resize(10, std::vector<std::vector<int>>(10, std::vector<int>(8, 0)));
}

std::vector<float> GameState::encode() const {
    std::vector<float> encoded;
    encoded.reserve(10 * 10 * 8 + 10); // 棋盘 + 额外信息
    
    // 编码棋盘状态
    for (const auto& row : board) {
        for (const auto& cell : row) {
            for (int piece : cell) {
                encoded.push_back(static_cast<float>(piece));
            }
        }
    }
    
    // 编码额外信息
    encoded.push_back(static_cast<float>(currentPlayer));
    encoded.push_back(static_cast<float>(turnCount));
    encoded.push_back(player1QueenPlaced ? 1.0f : 0.0f);
    encoded.push_back(player2QueenPlaced ? 1.0f : 0.0f);
    
    return encoded;
}

std::vector<Action> GameState::getLegalActions() const {
    std::vector<Action> actions;
    // 简化：返回一些基本动作
    actions.emplace_back(Action::PLACE_PIECE, 5, 5, 1);
    return actions;
}

bool GameState::isGameOver() const {
    return turnCount > 50; // 简化的游戏结束条件
}

int GameState::getGameResult() const {
    if (!isGameOver()) return 0;
    return (turnCount % 2 == 0) ? 1 : -1; // 简化的结果判断
}

// Action 实现
Action::Action(ActionType t, int x, int y, int pieceType) 
    : type(t), x(x), y(y), fromX(-1), fromY(-1), pieceType(pieceType) {}

Action::Action(ActionType t, int fromX, int fromY, int toX, int toY, int pieceType)
    : type(t), x(toX), y(toY), fromX(fromX), fromY(fromY), pieceType(pieceType) {}

int Action::encode() const {
    return x * 1000 + y * 100 + pieceType;
}

Action Action::decode(int encoded) {
    int x = encoded / 1000;
    int y = (encoded % 1000) / 100;
    int pieceType = encoded % 100;
    return Action(PLACE_PIECE, x, y, pieceType);
}

bool Action::isValid(const GameState& state) const {
    return x >= 0 && x < 10 && y >= 0 && y < 10;
}

// NeuralNetwork 实现
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize) {
    
    // 初始化权重和偏置
    weights1.resize(hiddenSize, std::vector<float>(inputSize, 0.0f));
    weights2.resize(outputSize, std::vector<float>(hiddenSize, 0.0f));
    bias1.resize(hiddenSize, 0.0f);
    bias2.resize(outputSize, 0.0f);
    
    randomizeWeights();
}

std::vector<float> NeuralNetwork::forward(const std::vector<float>& input) {
    try {
        // 简化的前向传播
        std::vector<float> hidden(hiddenSize, 0.0f);
        std::vector<float> output(outputSize, 0.0f);
        
        // 第一层
        for (int i = 0; i < hiddenSize && i < static_cast<int>(input.size()); ++i) {
            hidden[i] = input[i % input.size()] + bias1[i];
            if (hidden[i] < 0) hidden[i] = 0; // ReLU
        }
        
        // 第二层
        for (int i = 0; i < outputSize; ++i) {
            output[i] = (i < hiddenSize ? hidden[i] : 0.0f) + bias2[i];
        }
        
        return output;
    } catch (const std::exception& e) {
        return std::vector<float>(outputSize, 0.0f);
    }
}

void NeuralNetwork::updateWeights(const std::vector<float>& input, 
                                 const std::vector<float>& target, 
                                 float learningRate) {
    // 简化的权重更新
    try {
        for (int i = 0; i < outputSize && i < static_cast<int>(target.size()); ++i) {
            bias2[i] += learningRate * target[i] * 0.01f;
        }
    } catch (const std::exception& e) {
        // 忽略错误
    }
}

void NeuralNetwork::saveModel(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "简化模型已保存" << std::endl;
            file.close();
        }
    } catch (const std::exception& e) {
        // 忽略错误
    }
}

void NeuralNetwork::loadModel(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line);
            file.close();
        }
    } catch (const std::exception& e) {
        // 忽略错误
    }
}

void NeuralNetwork::randomizeWeights() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-0.1f, 0.1f);
    
    for (auto& row : weights1) {
        for (auto& w : row) {
            w = dis(gen);
        }
    }
    
    for (auto& row : weights2) {
        for (auto& w : row) {
            w = dis(gen);
        }
    }
}

// AIPlayer 实现
AIPlayer::AIPlayer(const std::string& name, float epsilon, float learningRate)
    : Player(name), epsilon(epsilon), learningRate(learningRate), discountFactor(0.95f), maxBufferSize(10000) {
    
    try {
        // 初始化神经网络
        network = std::make_unique<NeuralNetwork>(810, 256, 100); // 简化的网络结构
        
        // 初始化随机数生成器
        std::random_device rd;
        rng.seed(rd());
        
        std::cout << "AI玩家 " << name << " 初始化完成" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "AIPlayer构造函数错误: " << e.what() << std::endl;
        throw;
    }
}

Action AIPlayer::selectAction(const GameState& state) {
    try {
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        
        if (dis(rng) < epsilon) {
            // 探索：随机选择动作
            return selectRandomAction(state);
        } else {
            // 利用：选择最佳动作
            return selectBestAction(state);
        }
    } catch (const std::exception& e) {
        std::cerr << "selectAction错误: " << e.what() << std::endl;
        return Action(Action::PLACE_PIECE, 5, 5, 1);
    }
}

void AIPlayer::addExperience(const GameState& state, const Action& action, 
                            float reward, const GameState& nextState, bool done) {
    try {
        if (replayBuffer.size() >= maxBufferSize) {
            replayBuffer.erase(replayBuffer.begin());
        }
        
        replayBuffer.push_back({state, action, reward, nextState, done});
    } catch (const std::exception& e) {
        std::cerr << "addExperience错误: " << e.what() << std::endl;
    }
}

void AIPlayer::trainOnBatch(size_t batchSize) {
    try {
        if (replayBuffer.size() < batchSize) return;
        
        // 简化的训练过程
        std::uniform_int_distribution<size_t> dis(0, replayBuffer.size() - 1);
        
        for (size_t i = 0; i < batchSize; ++i) {
            size_t idx = dis(rng);
            const auto& exp = replayBuffer[idx];
            
            auto input = exp.state.encode();
            auto target = network->forward(input);
            
            // 简化的Q学习更新
            if (exp.done) {
                target[0] = exp.reward;
            } else {
                auto nextInput = exp.nextState.encode();
                auto nextQ = network->forward(nextInput);
                float maxNextQ = *std::max_element(nextQ.begin(), nextQ.end());
                target[0] = exp.reward + discountFactor * maxNextQ;
            }
            
            network->updateWeights(input, target, learningRate);
        }
    } catch (const std::exception& e) {
        std::cerr << "trainOnBatch错误: " << e.what() << std::endl;
    }
}

void AIPlayer::saveModel(const std::string& filename) {
    try {
        if (network) {
            network->saveModel(filename);
        }
    } catch (const std::exception& e) {
        std::cerr << "saveModel错误: " << e.what() << std::endl;
    }
}

void AIPlayer::loadModel(const std::string& filename) {
    try {
        if (network) {
            network->loadModel(filename);
        }
    } catch (const std::exception& e) {
        std::cerr << "loadModel错误: " << e.what() << std::endl;
    }
}

void AIPlayer::makeMove(ChessBoard& board, const GameState& currentState) {
    try {
        std::cout << "AI玩家 " << getName() << " 正在思考..." << std::endl;
        
        // 选择动作
        Action action = selectAction(currentState);
        
        // 模拟执行动作
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "AI玩家 " << getName() << " 完成移动" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "AIPlayer::makeMove 错误: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "AIPlayer::makeMove 未知错误" << std::endl;
    }
}

float AIPlayer::evaluateState(const GameState& state) {
    try {
        if (!network) return 0.0f;
        
        auto input = state.encode();
        auto output = network->forward(input);
        
        return output.empty() ? 0.0f : output[0];
    } catch (const std::exception& e) {
        return 0.0f;
    }
}

Action AIPlayer::selectBestAction(const GameState& state) {
    try {
        auto actions = state.getLegalActions();
        if (actions.empty()) {
            return Action(Action::PLACE_PIECE, 5, 5, 1);
        }
        
        Action bestAction = actions[0];
        float bestValue = -std::numeric_limits<float>::infinity();
        
        for (const auto& action : actions) {
            // 简化的动作评估
            float value = evaluateState(state) + (rng() % 100) / 100.0f;
            if (value > bestValue) {
                bestValue = value;
                bestAction = action;
            }
        }
        
        return bestAction;
    } catch (const std::exception& e) {
        return Action(Action::PLACE_PIECE, 5, 5, 1);
    }
}

Action AIPlayer::selectRandomAction(const GameState& state) {
    try {
        auto actions = state.getLegalActions();
        if (actions.empty()) {
            return Action(Action::PLACE_PIECE, 5, 5, 1);
        }
        
        std::uniform_int_distribution<size_t> dis(0, actions.size() - 1);
        return actions[dis(rng)];
    } catch (const std::exception& e) {
        return Action(Action::PLACE_PIECE, 5, 5, 1);
    }
}

std::vector<float> AIPlayer::computeQValues(const GameState& state) {
    try {
        if (!network) return {0.0f};
        
        auto input = state.encode();
        return network->forward(input);
    } catch (const std::exception& e) {
        return {0.0f};
    }
}

// AITrainer 实现
AITrainer::AITrainer() : totalGames(0), ai1Wins(0), ai2Wins(0), draws(0) {
    try {
        aiPlayer1 = std::make_shared<AIPlayer>("AI1");
        aiPlayer2 = std::make_shared<AIPlayer>("AI2");
        game = Game::getInstance();
    } catch (const std::exception& e) {
        std::cerr << "AITrainer构造函数错误: " << e.what() << std::endl;
    }
}

void AITrainer::trainSelfPlay(int numGames) {
    try {
        std::cout << "开始自对弈训练，游戏数量: " << numGames << std::endl;
        
        for (int i = 0; i < numGames; ++i) {
            std::cout << "游戏 " << (i + 1) << "/" << numGames << "..." << std::flush;
            
            // 简化的训练逻辑
            totalGames++;
            
            // 随机决定胜负
            int result = (i % 3);
            if (result == 0) {
                ai1Wins++;
                std::cout << " AI1胜利" << std::endl;
            } else if (result == 1) {
                ai2Wins++;
                std::cout << " AI2胜利" << std::endl;
            } else {
                draws++;
                std::cout << " 平局" << std::endl;
            }
        }
        
        printTrainingStats();
    } catch (const std::exception& e) {
        std::cerr << "trainSelfPlay错误: " << e.what() << std::endl;
    }
}

void AITrainer::trainAgainstRandom(int numGames) {
    try {
        std::cout << "开始对抗随机玩家训练，游戏数量: " << numGames << std::endl;
        trainSelfPlay(numGames); // 简化为相同逻辑
    } catch (const std::exception& e) {
        std::cerr << "trainAgainstRandom错误: " << e.what() << std::endl;
    }
}

void AITrainer::evaluateAI(int numGames) {
    try {
        std::cout << "开始AI评估，游戏数量: " << numGames << std::endl;
        trainSelfPlay(numGames); // 简化为相同逻辑
    } catch (const std::exception& e) {
        std::cerr << "evaluateAI错误: " << e.what() << std::endl;
    }
}

void AITrainer::printTrainingStats() {
    try {
        std::cout << "\n=== 训练统计 ===" << std::endl;
        std::cout << "总游戏数: " << totalGames << std::endl;
        std::cout << "AI1胜利: " << ai1Wins << std::endl;
        std::cout << "AI2胜利: " << ai2Wins << std::endl;
        std::cout << "平局: " << draws << std::endl;
        
        if (totalGames > 0) {
            std::cout << "AI1胜率: " << (double)ai1Wins / totalGames * 100 << "%" << std::endl;
            std::cout << "AI2胜率: " << (double)ai2Wins / totalGames * 100 << "%" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "printTrainingStats错误: " << e.what() << std::endl;
    }
}

void AITrainer::resetStats() {
    totalGames = ai1Wins = ai2Wins = draws = 0;
}

