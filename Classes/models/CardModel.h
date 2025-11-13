#pragma once

#include "cocos2d.h"
#include "CardType.h"

extern int refCardId;  ///< 全局卡牌ID计数器（用于自动分配唯一 _cardId），由外部模块初始化

/**
 * @class CardModel
 * @brief 卡牌数据模型 —— 封装单张卡牌的核心状态：点数、花色、位置、朝向、唯一ID。
 * 
 * 使用场景：游戏运行时每张卡牌的“数据本体”，供 GameModel 管理、UndoModel 记录状态、View 层渲染依据；
 * 支持序列化/反序列化（如存档、网络同步、撤销回滚）。
 */
class CardModel
{
public:
    /// 默认构造：ID 自增（基于 refCardId），其余字段为默认值（A♠，位置(0,0)，背面朝上）
    CardModel();

    /**
     * 构造函数：完整初始化卡牌。
     * @param face   点数类型（CardFaceType，如 ACE, TWO, ..., KING）
     * @param suit   花色类型（CardSuitType，如 SPADE, HEART 等）
     * @param position 初始屏幕坐标
     */
    CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);

    ~CardModel();

    // ——— Getter ———
    CardFaceType getFace() const { return _face; }       ///< 获取点数
    CardSuitType getSuit() const { return _suit; }       ///< 获取花色
    const cocos2d::Vec2& getPosition() const { return _position; }  ///< 获取当前位置
    int getCardId() const { return _cardId; }            ///< 获取唯一ID（创建时分配，全局唯一）
    bool isFaceUp() const { return _isFaceUp; }          ///< 是否正面朝上（true=可见，false=背面）

    // ——— Setter ———
    void setCardId(int cardId) { _cardId = cardId; }
    void setFace(CardFaceType face) { _face = face; }
    void setSuit(CardSuitType suit) { _suit = suit; }
    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    void setFaceUp(bool isFaceUp) { _isFaceUp = isFaceUp; }

    /**
     * 检查卡牌数据是否合法。
     * @return true：点数/花色在有效枚举范围内；false：非法值（如 suit = 99）。
     */
    bool isValid() const;

private:
    int _cardId;               ///< 唯一ID（由构造函数基于 refCardId 分配）
    CardFaceType _face;        ///< 点数（A~K）
    CardSuitType _suit;        ///< 花色（♣♦♥♠）
    cocos2d::Vec2 _position;   ///< 当前坐标（单位：点）
    bool _isFaceUp;            ///< 朝向状态：true=翻面可见，false=背面隐藏
};