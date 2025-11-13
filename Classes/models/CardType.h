#pragma once

/**
 * @enum CardFaceType
 * @brief 卡牌点数类型 —— 表示扑克牌的面值（A~K），用于 CardModel 等模块。
 * 
 * 数值连续：CFT_ACE = 0, CFT_TWO = 1, ..., CFT_KING = 12  
 * CFT_NONE 表示无效/未初始化；  
 * CFT_NUM_CARD_FACE_TYPES 为总数（13），可用于数组大小或范围校验。
 */
enum CardFaceType
{
    CFT_NONE = -1,          ///< 无效点数
    CFT_ACE,                ///< A (可作 1 或 14，依游戏规则而定)
    CFT_TWO,                ///< 2
    CFT_THREE,              ///< 3
    CFT_FOUR,               ///< 4
    CFT_FIVE,               ///< 5
    CFT_SIX,                ///< 6
    CFT_SEVEN,              ///< 7
    CFT_EIGHT,              ///< 8
    CFT_NINE,               ///< 9
    CFT_TEN,                ///< 10
    CFT_JACK,               ///< J
    CFT_QUEEN,              ///< Q
    CFT_KING,               ///< K
    CFT_NUM_CARD_FACE_TYPES ///< 点数类型总数（13），非实际点数
};

/**
 * @enum CardSuitType
 * @brief 卡牌花色类型 —— 表示四种标准花色，用于 CardModel、资源匹配等。
 * 
 * 顺序固定（常用于资源文件命名或排序）：梅花→方块→红桃→黑桃  
 * CST_NONE 表示无效花色；  
 * CST_NUM_CARD_SUIT_TYPES 为总数（4），可用于循环或数组维度。
 */
enum CardSuitType
{
    CST_NONE = -1,          ///< 无效花色
    CST_CLUBS,              ///< ♣ 梅花（Clubs）
    CST_DIAMONDS,           ///< ♦ 方块（Diamonds）
    CST_HEARTS,             ///< ♥ 红桃（Hearts）
    CST_SPADES,             ///< ♠ 黑桃（Spades）
    CST_NUM_CARD_SUIT_TYPES ///< 花色类型总数（4），非实际花色
};