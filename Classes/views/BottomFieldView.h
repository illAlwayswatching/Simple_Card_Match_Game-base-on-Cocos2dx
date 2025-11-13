#pragma once

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "CardView.h"
#include <functional>

/**
 * @class BottomFieldView
 * @brief 底部区域视图 —— 显示当前手牌或可操作牌区（如“当前匹配目标牌”），支持点击交互与动画反馈。
 * 
 * 使用场景：游戏界面底部固定区域，通常展示 1~2 张关键牌（如当前手牌顶部牌 + 备用牌顶部牌），
 * 是玩家主要操作入口之一，点击后触发匹配或移动逻辑。
 * 
 * @note 继承自 Layer，由 BottomFieldController 管理生命周期与事件绑定。
 */
class BottomFieldView : public cocos2d::Layer
{
public:
    /// 卡牌点击回调：传入被点击卡牌的 ID
    using CardClickCallback = std::function<void(int cardId)>;

    /**
     * 静态创建方法（推荐使用）。
     * @param gameModel         游戏模型（只读），用于初始渲染
     * @param cardClickCallback 点击事件回调（可为空）
     * @return 新建的 BottomFieldView 实例；失败返回 nullptr
     */
    static BottomFieldView* create(const GameModel* gameModel,
                                   CardClickCallback cardClickCallback = nullptr);

    /**
     * 初始化实例（供 create 内部调用）。
     * @return true 初始化成功
     */
    bool init(const GameModel* gameModel,
              CardClickCallback cardClickCallback = nullptr);

    /**
     * 刷新视图：根据 GameModel 当前手牌/备用牌状态更新卡牌显示（数量、朝向、位置）。
     */
    void updateDisplay();

    /**
     * 播放卡牌移动入场动画（如从桌面飞入底部区）。
     * @param cardId         目标卡牌 ID
     * @param targetPosition 动画终点坐标（本视图局部坐标）
     * @param duration       动画时长（秒）
     * @param callback       动画结束回调（可为空）
     * 
     * @note 动画结束后，应由 Controller 调用 updateDisplay() 确保状态同步。
     */
    void playCardMoveAnimation(int cardId,
                               const cocos2d::Vec2& targetPosition,
                               float duration,
                               std::function<void()> callback = nullptr);

private:
    // ——— 成员变量说明 ———
    const GameModel*   _gameModel;           ///< 只读模型引用
    CardView*          _topCardView;         ///< 当前手牌顶部卡牌视图（通常唯一显式显示的牌）
    CardView*          _BackUpTopCardView;   ///< 备用牌顶部卡牌视图（如支持双目标匹配）
    std::vector<CardView*> _cardViews;       ///< 所有卡牌视图（便于统一管理内存与事件）
    CardClickCallback  _cardClickCallback;   ///< 外部回调，通知 Controller 处理点击
    cocos2d::Size      _cardSize;            ///< 卡牌统一尺寸（由配置或设计稿决定）

    BottomFieldView();
    ~BottomFieldView();

    /**
     * 创建单张卡牌视图，并自动绑定点击事件。
     * @param card 卡牌数据（非空）
     * @return 新建的 CardView 实例
     */
    CardView* createCardView(CardModel* card);

    /**
     * 重新计算并设置各卡牌视图的位置（如居中、错位排列）。
     * @note 在 updateDisplay() 或窗口缩放时调用。
     */
    void updateCardPositions();
};