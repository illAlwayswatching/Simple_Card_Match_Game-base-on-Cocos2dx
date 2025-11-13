#pragma once

#include "../models/GameModel.h"
#include "../views/DeckFieldView.h"
#include "../managers/UndoManager.h"

/**
 * @class DeckFieldController
 * @brief 牌堆区域控制器 —— 管理“抽牌堆”（Deck）的交互逻辑，如点击翻牌、触发发牌动作。
 * 
 * 使用场景：当玩家点击牌堆区域（通常显示为背面卡牌堆）时，执行发牌/翻牌操作，
 * 并同步更新视图与撤销记录，是游戏核心流程（如“抽一张牌”）的关键入口。
 */
class DeckFieldController
{
public:
    /**
     * @brief 牌堆点击回调类型（无参数）。
     * 触发时机：牌堆被点击且操作有效时（如还有牌可抽）。
     */
    using CardClickCallback = std::function<void()>;

    DeckFieldController();
    ~DeckFieldController();

    /**
     * 初始化控制器。
     * @param gameModel 游戏数据模型（非空），用于读取/修改牌堆状态。
     * @param undoManager 撤销管理器（非空），用于记录本次抽牌操作。
     * @param callback 点击成功后的回调（如播放翻牌音效、触发自动出牌）。
     * @return true 初始化成功；false 失败（如参数为空）。
     */
    bool init(GameModel* gameModel, UndoManager* undoManager, CardClickCallback callback);

    /**
     * 处理牌堆点击事件。
     * @param cardId 实际未使用（牌堆通常视为整体，ID 可忽略或固定为 -1），保留接口一致性。
     * @return true 成功触发抽牌/翻牌；false 无效操作（如牌堆已空）。
     */
    bool handleCardClick(int cardId);

    /**
     * 根据 GameModel 状态刷新牌堆视图（如剩余张数、是否可点击、背面/正面状态）。
     * @return true 刷新成功；false 模型异常。
     */
    bool updateDeckFieldView();

    /**
     * 获取牌堆视图对象，供场景添加到 UI 层。
     * @return 指向 DeckFieldView 的指针。
     */
    DeckFieldView* getDeckFieldView() { return _deckFieldView; }

private:
    // ——— 成员变量说明 ———
    GameModel*      _gameModel;       ///< 游戏核心模型，含牌堆数据（剩余卡牌、发牌逻辑等）
    DeckFieldView*  _deckFieldView;   ///< 牌堆区域的 UI 视图
    CardClickCallback _cardClickCallback; ///< 外部回调，用于通知上层“有效点击已发生”
    UndoManager*    _undoManager;     ///< 撤销服务，记录本次抽牌为一个可撤销步骤
};