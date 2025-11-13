#pragma once

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "CardView.h"
#include <vector>
#include <functional>

/**
 * @class StackView
 * @brief 手牌栈视图 —— 显示玩家当前持有的卡牌（如“手牌区”），支持拖拽交换、点击选中等交互。
 * 
 * 使用场景：游戏底部或侧边固定区域，以扇形/层叠方式展示多张手牌；
 * 是玩家主动操作的核心区域（如选牌、换位），点击后由 Controller 触发匹配或移动。
 * 
 * @note 继承自 Node（非 Layer），便于嵌入其他容器（如 BottomFieldView 或 GameView）。
 */
class StackView : public cocos2d::Node
{
public:
    /// 手牌点击回调：传入被点击卡牌的 ID
    using StackCardClickCallback = std::function<void(int cardId)>;

    /**
     * 创建无交互的手牌视图（仅展示）。
     * @return 新建实例
     */
    static StackView* create();

    /**
     * 创建带点击交互的手牌视图。
     * @param clickCallback 卡牌点击事件处理器
     * @return 新建实例
     */
    static StackView* create(StackCardClickCallback clickCallback);

    /**
     * 初始化实例（供 create 内部调用）。
     * @return true 初始化成功
     */
    bool init(StackCardClickCallback clickCallback = nullptr);

    /**
     * 动态设置点击回调（支持运行时更换逻辑）。
     */
    void setCardClickCallback(StackCardClickCallback callback) { _cardClickCallback = callback; }

    /**
     * 根据 GameModel 更新手牌显示（数量、顺序、朝向）。
     * @param gameModel 游戏模型（非空），从中读取 _handsFieldCards 等数据
     * @note 会重建/复用 CardView，保持 UI 与模型同步。
     */
    void updateStack(const GameModel* gameModel);

    /**
     * 播放两张手牌交换位置的动画（如拖拽换位）。
     * @param fromCardId 源卡牌 ID
     * @param toCardId   目标卡牌 ID
     * @param callback   动画结束回调（常用于通知 Controller 更新模型）
     */
    void playSwapAnimation(int fromCardId, int toCardId, std::function<void()> callback = nullptr);

    /**
     * 播放单张卡牌撤销回滚动画（如从手牌飞回桌面）。
     * @param cardId         卡牌 ID
     * @param targetPosition 回滚目标坐标（父节点坐标系）
     * @param callback       动画结束回调
     */
    void playUndoAnimation(int cardId,
                           const cocos2d::Vec2& targetPosition,
                           std::function<void()> callback = nullptr);

    /**
     * 根据 ID 查找对应卡牌视图。
     * @param cardId 卡牌唯一 ID
     * @return 对应 CardView；未找到返回 nullptr
     */
    CardView* getCardView(int cardId) const;

    /**
     * 获取“手牌顶部”参考位置（如用于新卡牌入栈定位）。
     * @return 预设锚点坐标（通常为栈中心或左端）
     */
    cocos2d::Vec2 getHandTopPosition() const { return _handTopPosition; }

private:
    // ——— 成员变量说明 ———
    std::vector<CardView*> _cardViews;        ///< 当前所有卡牌 UI 实例
    StackCardClickCallback _cardClickCallback;///< 外部事件处理器
    cocos2d::Vec2 _handTopPosition;           ///< 手牌区域锚点（如 {0, 0}）
    std::vector<cocos2d::Vec2> _fanPositions; ///< 扇形布局预计算位置（角度展开）

    /**
     * 创建单张卡牌视图并定位。
     * @param cardModel 卡牌数据
     * @param position  初始位置（局部坐标）
     * @return 新建 CardView
     */
    CardView* createCardView(const CardModel* cardModel, const cocos2d::Vec2& position);

    /**
     * 清空并释放所有 CardView（用于 updateStack 前重置）。
     */
    void clearAllCardViews();

    /**
     * 计算手牌布局位置（如均匀扇形展开），结果存入 _fanPositions。
     * @note 在卡牌数量变化时调用。
     */
    void setupLayout();

    /**
     * 卡牌点击事件统一入口：校验后触发 _cardClickCallback。
     */
    void onCardClicked(int cardId);
};