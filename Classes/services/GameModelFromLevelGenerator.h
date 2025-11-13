#pragma once

#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

/**
 * @class GameModelFromLevelGenerator
 * @brief 关卡→游戏模型生成器 —— 将静态关卡配置（LevelConfig）转换为可运行的游戏数据（GameModel）。
 * 
 * 使用场景：游戏启动时（如 GameController::startGame()），加载 LevelConfig 后调用本类，
 * 生成初始 GameModel，支持是否打乱卡牌位置（用于重玩/随机化关卡）。
 * 
 * @note 纯静态工厂类，不持有状态；生成的 GameModel 由调用方负责管理生命周期。
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * 从关卡配置生成游戏模型（默认按配置固定布局）。
     * @param levelConfig 关卡配置（非空）。
     * @return 新建的 GameModel 实例；失败（如配置非法）返回 nullptr。
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);

    /**
     * 从关卡配置生成游戏模型，并可选随机化卡牌初始位置。
     * @param levelConfig 关卡配置（非空）。
     * @param randomize   true：在原位置附近加随机偏移（如 ±20 点），避免完全重叠；false：严格按配置坐标。
     * @return 新建的 GameModel；失败返回 nullptr。
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig, bool randomize);

private:
    /**
     * 创建单张卡牌模型，并根据所属区域（pileType）设置初始状态（如是否翻面）。
     * @param cardConfig 来自 LevelConfig 的卡牌描述。
     * @param pileType   区域类型字符串（如 "main"/"bottom"/"back"），用于决定初始朝向：
     *                   - "back" 区通常为背面（_isFaceUp = false）；
     *                   - "main"/"bottom" 通常为正面（_isFaceUp = true）。
     * @return 新建的 CardModel；失败返回 nullptr。
     */
    static CardModel* createCardModel(const LevelConfig::CardConfig& cardConfig, const std::string& pileType);
};