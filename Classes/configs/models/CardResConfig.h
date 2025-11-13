#pragma once

#include "../../models/CardModel.h"
#include "cocos2d.h"

/**
 * @class CardResConfig
 * @brief 扑克牌资源配置类 —— 根据牌面数据（CardModel）生成对应的游戏精灵（Sprite）或资源路径。
 * 
 * 使用场景：在游戏运行时动态创建扑克牌的各个视觉组件（如数字、花色），支持不同尺寸显示（如手牌小号数字 vs 桌面大号数字），
 * 实现表现层与数据层（CardModel）解耦，便于换肤或资源更新。
 */
class CardResConfig
{
public:
    /**
     * 创建完整的卡牌精灵（含背景、数字、花色等组合）。
     * @param cardModel 指向卡牌数据的指针（非空），含花色和点数信息。
     * @return cocos2d::Sprite* 新建的精灵对象；失败时返回 nullptr。
     */
    static cocos2d::Sprite* createCardSprite(const CardModel* cardModel);

    /**
     * 创建小号数字精灵
     * @param cardModel 卡牌数据（如：A, 5, J）。
     * @return cocos2d::Sprite* 小数字精灵；若资源缺失则返回 nullptr。
     */
    static cocos2d::Sprite* createSmallNumberSprite(const CardModel* cardModel);

    /**
     * 创建大号数字精灵
     * @param cardModel 卡牌数据。
     * @return cocos2d::Sprite* 大数字精灵；失败返回 nullptr。
     */
    static cocos2d::Sprite* createBigNumberSprite(const CardModel* cardModel);

    /**
     * 创建花色精灵（♣♦♥♠ 对应图标）。
     * @param cardModel 卡牌数据。
     * @return cocos2d::Sprite* 花色精灵；无效花色返回 nullptr。
     */
    static cocos2d::Sprite* createSuitSprite(const CardModel* cardModel);

    /**
     * 获取数字部分对应的图片文件名（不含路径）。
     * @param cardModel 卡牌数据。
     * @return std::string 图片文件名，如 "num_ace.png"、"num_10.png"。
     */
    static std::string getNumberImageFile(const CardModel* cardModel);

    /**
     * 获取花色部分对应的图片文件名（不含路径）。
     * @param cardModel 卡牌数据。
     * @return std::string 花色图片文件名，如 "suit_heart.png"。
     */
    static std::string getSuitImageFile(const CardModel* cardModel);
};

