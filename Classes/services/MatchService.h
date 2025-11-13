#pragma once

#include "../models/CardModel.h"
#include "../models/GameModel.h"
#include "../managers/UndoManager.h"

/**
 * @class MatchService
 * @brief 匹配规则服务类 —— 封装卡牌之间的匹配逻辑与移动判定，解耦业务规则与控制器。
 * 
 * 使用场景：玩家尝试移动卡牌时（如点击桌面牌→手牌），由控制器调用本类判断是否合法、执行移动，
 * 并自动注册撤销记录，确保规则集中管理、易于调整（如改玩“接龙”或“记忆翻牌”规则）。
 */
class MatchService
{
public:
    /**
     * 尝试将一张桌面牌（主牌堆）与当前手牌进行匹配并移动。
     * @param deckCard  桌面卡牌（待匹配源，非空）
     * @param handsCard 当前手牌（目标，非空）
     * @param gameModel 游戏模型（非空），用于执行状态变更
     * @return true：匹配成功且移动完成；false：不匹配或操作失败
     * 
     * @note 成功时通常会：移除桌面牌 → 入手牌栈（或合并/消除，依规则而定）
     */
    static bool MatchMoveInData(CardModel* deckCard, CardModel* handsCard, GameModel* gameModel);

    /**
     * 判断两张卡牌是否满足匹配条件（核心规则）。
     * @param card1 卡牌1（非空）
     * @param card2 卡牌2（非空）
     * @return true：匹配（如点数相同、花色互补等，具体由实现定义）
     * 
     * @usage 示例规则：CFT_ACE + CFT_KING = 匹配；或同点数即匹配。
     */
    static bool isMatch(CardModel* card1, CardModel* card2);

    /**
     * 检查指定桌面牌是否允许移入手牌（前置校验）。
     * @param deckFieldCardId 桌面卡牌 ID（如主牌堆中某张）
     * @param gameModel        游戏模型（非空）
     * @return true：可移动（如牌已翻开、位置可达、手牌可接收等）
     */
    static bool canMoveToHand(int deckFieldCardId, GameModel* gameModel);

    /**
     * 执行桌面牌 → 手牌的移动操作（含状态变更与撤销注册）。
     * @param deckFieldCardId 桌面卡牌 ID
     * @param gameModel        游戏模型（非空）
     * @param undoManager      撤销管理器（可为空）；若提供，则记录本次移动
     * @return true：移动成功；false：卡牌不存在或操作非法
     */
    static bool executeMoveToHand(int deckFieldCardId, GameModel* gameModel, UndoManager* undoManager);

    /**
     * 更新相关卡牌的朝向状态（如：移动后翻开下一张牌）。
     * @param gameModel 游戏模型（非空）
     * @return true：更新成功；false：无变化或异常
     * 
     * @usage 典型场景：移走某张牌后，将其下方/后继牌设为 _isFaceUp = true。
     */
    static bool updateCardFaceUp(GameModel* gameModel);
};