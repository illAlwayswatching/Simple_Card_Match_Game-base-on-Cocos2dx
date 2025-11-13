#pragma once

#include "cocos2d.h"
#include <vector>

/**
 * @class LevelConfig
 * @brief 关卡配置数据类 —— 存储一关中所有卡牌的静态布局信息（含位置、点数、花色）。
 * 
 * 使用场景：游戏初始化或关卡切换时，由 LevelConfigLoader 加载 JSON 后填充此对象，
 * 供关卡场景（如 LevelScene）读取并生成对应的卡牌精灵，实现“数据驱动布局”。
 */
class LevelConfig
{
public:
    /**
     * @struct CardConfig
     * @brief 单张卡牌的配置项。
     * 
     * - cardFace: 点数（0=A, 1=2, ..., 9=10, 10=J, 11=Q, 12=K）  
     * - cardSuit: 花色（0=♣梅花, 1=♦方块, 2=♥红桃, 3=♠黑桃）  
     * - position: 卡牌在屏幕上的初始坐标（单位：点，基于 Cocos2d 坐标系）
     */
    struct CardConfig
    {
        int cardFace;           ///< 点数值（0~12）
        int cardSuit;           ///< 花色值（0~3）
        cocos2d::Vec2 position; ///< 初始位置

        CardConfig() : cardFace(0), cardSuit(0) {}
        CardConfig(int face, int suit, const cocos2d::Vec2& pos)
            : cardFace(face), cardSuit(suit), position(pos) {}
    };

    LevelConfig();
    ~LevelConfig();

    // ———————— Getter ————————
    const std::vector<CardConfig>& getMainFieldCards() const { return _mainFieldCards; }   ///< 主区卡牌（如桌面中央布局）
    const std::vector<CardConfig>& getBottomFieldCards() const { return _bottomFieldCards; } ///< 底部区域卡牌（如玩家手牌起始位）
    const std::vector<CardConfig>& getBackFieldCards() const { return _backFieldCards; }   ///< 背景/隐藏区卡牌（如牌堆背面）

    // ———————— Setter ————————
    void setMainFieldCards(const std::vector<CardConfig>& cards) { _mainFieldCards = cards; }
    void setBottomFieldCards(const std::vector<CardConfig>& cards) { _bottomFieldCards = cards; }
    void setBackFieldCards(const std::vector<CardConfig>& cards) { _backFieldCards = cards; }

    // ———————— Adder ————————
    void addMainFieldCard(const CardConfig& card) { _mainFieldCards.push_back(card); }
    void addBottomFieldCard(const CardConfig& card) { _bottomFieldCards.push_back(card); }
    void addBackFieldCard(const CardConfig& card) { _backFieldCards.push_back(card); }

    /**
     * 清空所有卡牌配置。
     */
    void clear();

    /**
     * 检查配置是否有效（例如：各区域卡牌数是否符合关卡规则）。
     * @return true 表示数据完整可用；false 表示缺失关键配置（如主区为空）。
     */
    bool isValid() const;

private:
    std::vector<CardConfig> _mainFieldCards;    ///< 主操作区卡牌列表
    std::vector<CardConfig> _bottomFieldCards;  ///< 底部显示区卡牌列表（常用于玩家视角）
    std::vector<CardConfig> _backFieldCards;    ///< 后台/初始牌堆区（如未翻开的牌）
};