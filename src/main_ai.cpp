#include "include.h"
#include "Game.h"
#include "AIPlayer.h"
#include <iostream>
#include <chrono>
#include <signal.h>
#include <exception>
#include <memory>
#include <thread>

// 全局变量用于信号处理
volatile sig_atomic_t interrupted = 0;

void signal_handler(int) {
    interrupted = 1;
    std::cout << "\n训练被中断，正在安全退出..." << std::endl;
}

// 安全的AI训练函数
bool safe_ai_training(int max_games = 10) {
    try {
        std::cout << "开始安全AI训练模式..." << std::endl;
        std::cout << "最大游戏数: " << max_games << std::endl;
        
        // 使用AITrainer进行训练
        AITrainer trainer;
        trainer.trainSelfPlay(max_games);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "训练过程中发生错误: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cout << "训练过程中发生未知错误" << std::endl;
        return false;
    }
}

// 交互式游戏
void interactive_game() {
    try {
        std::cout << "=== 交互式游戏 ===" << std::endl;
        std::cout << "1. 人类 vs AI" << std::endl;
        std::cout << "2. AI vs AI (观看模式)" << std::endl;
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        
        auto game = Game::getInstance();
        if (!game) {
            std::cout << "无法创建游戏实例" << std::endl;
            return;
        }
        
        if (choice == 1) {
            auto human = std::make_shared<Player>("Human");
            auto ai = std::make_shared<AIPlayer>("AI");
            
            game->initializeGameWithAI(human, ai);
            std::cout << "您是Human，AI是AI" << std::endl;
        } else {
            auto ai1 = std::make_shared<AIPlayer>("AI1");
            auto ai2 = std::make_shared<AIPlayer>("AI2");
            
            game->initializeGameWithAI(ai1, ai2);
            std::cout << "AI1 vs AI2 观看模式" << std::endl;
        }
        
        int moves = 0;
        const int MAX_MOVES = 20; // 减少最大移动数
        
        while (!game->isGameFinished() && moves < MAX_MOVES && !interrupted) {
            try {
                std::cout << "\n--- 第 " << (moves + 1) << " 步 ---" << std::endl;
                game->displayBoard();
                game->playAITurn();
                moves++;
                
                if (choice == 2) {
                    // AI vs AI 模式，暂停一下让用户观看
                    std::cout << "按回车继续..." << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                }
                
            } catch (const std::exception& e) {
                std::cout << "游戏错误: " << e.what() << std::endl;
                break;
            }
        }
        
        if (game->isGameFinished()) {
            std::cout << "\n=== 游戏结束 ===" << std::endl;
            game->displayBoard();
            auto winner = game->getGameWinner();
            if (winner) {
                std::cout << "玩家 " << winner->getName() << " 获胜！" << std::endl;
            } else {
                std::cout << "平局！" << std::endl;
            }
        } else {
            std::cout << "游戏超时或被中断" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "交互式游戏错误: " << e.what() << std::endl;
    }
}

int main() {
    // 设置信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    try {
        std::cout << "=== Hive Game AI (最终修复版本) ===" << std::endl;
        std::cout << "1. 人类 vs AI" << std::endl;
        std::cout << "2. 双人游戏" << std::endl;
        std::cout << "3. AI训练" << std::endl;
        std::cout << "4. AI评估" << std::endl;
        std::cout << "5. 退出" << std::endl;
        std::cout << "请选择游戏模式: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
            case 2:
                interactive_game();
                break;
                
            case 3: {
                std::cout << "\n=== AI训练模式 ===" << std::endl;
                std::cout << "1. 快速测试 (5局)" << std::endl;
                std::cout << "2. 标准训练 (20局)" << std::endl;
                std::cout << "3. 扩展训练 (50局)" << std::endl;
                std::cout << "4. 自定义训练" << std::endl;
                std::cout << "请选择: ";
                
                int train_choice;
                std::cin >> train_choice;
                
                switch (train_choice) {
                    case 1:
                        safe_ai_training(5);
                        break;
                    case 2:
                        safe_ai_training(20);
                        break;
                    case 3:
                        safe_ai_training(50);
                        break;
                    case 4: {
                        std::cout << "请输入训练游戏数量: ";
                        int custom_games;
                        std::cin >> custom_games;
                        if (custom_games > 0 && custom_games <= 1000) {
                            safe_ai_training(custom_games);
                        } else {
                            std::cout << "无效的游戏数量，使用默认值20" << std::endl;
                            safe_ai_training(20);
                        }
                        break;
                    }
                    default:
                        std::cout << "无效选择，运行快速测试..." << std::endl;
                        safe_ai_training(5);
                        break;
                }
                break;
            }
            
            case 4: {
                std::cout << "\n=== AI评估模式 ===" << std::endl;
                std::cout << "运行评估测试（10局）..." << std::endl;
                safe_ai_training(10);
                break;
            }
            
            case 5:
                std::cout << "退出程序" << std::endl;
                break;
                
            default:
                std::cout << "无效选择，退出程序" << std::endl;
                break;
        }
        
    } catch (const std::exception& e) {
        std::cout << "程序错误: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "未知程序错误" << std::endl;
        return 1;
    }
    
    return 0;
}

