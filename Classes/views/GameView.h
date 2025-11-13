#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../models/GameModel.h"
#include "DeckFieldView.h"
#include "StackView.h"         // 注：当前未使用，可后续扩展（如主操作栈）
#include "UndoView.h"          // 注：当前未使用，可后续替换 _undoButton 为更丰富的撤销面板
#include "BottomFieldView.h"
#include "BackFieldView.h"
#include <functional>

/**
 * @class GameView
 * @brief 游戏主视图容器 —— 聚合并协调各子区域视图（主牌堆、手牌区、按钮等），管理全局 UI 布局与交互动画。
 * 
 * 使用场景：一局游戏的核心界面，由 GameController 创建并持有；
 * 负责整体入场/退场动画、撤销按钮响应，并作为事件分发中心（将卡牌点击路由给各子 View）。
 * 
 * @note 是典型的“View Group”，自身不处理业务逻辑，仅组合与调度子视图。
 */
class GameView : public cocos2d::Layer
{
public:
    /// 撤销按钮点击回调
    using UndoButtonCallback = std::function<void()>;

    /// 卡牌点击回调（统一入口，可路由到不同区域）
    using CardClickCallback = std::function<void(int cardId)>;

    /// 抽牌操作回调（如点击后备区触发）
    using DrawCardCallback = std::function<void()>;

    GameView();
    ~GameView();

    /**
     * 创建仅含基础视图的 GameView（无撤销按钮回调）。
     * @param gameModel 游戏模型（只读）
     * @return 新建实例
     */
    static GameView* create(const GameModel* gameModel);

    /**
     * 创建带撤销功能的 GameView。
     * @param gameModel    游戏模型
     * @param undoCallback 撤销按钮点击时触发
     * @return 新建实例
     */
    static GameView* create(const GameModel* gameModel, UndoButtonCallback undoCallback);

    /**
     * 初始化实例（供 create 内部调用）。
     * @return true 初始化成功
     */
    bool init(const GameModel* gameModel, UndoButtonCallback undoCallback = nullptr);

    /**
     * 全局刷新：调用所有子视图的 updateDisplay()，确保 UI 与 GameModel 一致。
     * @note 推荐在 GameController 的 updateGameView() 中调用。
     */
    void updateGame();

    /**
     * 设置抽牌回调（由 BackFieldView 等子视图使用）。
     * @param callback 外部逻辑处理器（如触发 BackupService::executeMoveToHand）
     */
    void setDrawCardCallback(DrawCardCallback callback);

    /**
     * 【冗余接口，建议移除或合并】
     * 当前与 updateGame() 功能重复，保留仅为兼容旧代码。
     */
    void updateDisplay();

    /**
     * 播放单张卡牌的撤销回滚动画（如从手牌飞回主牌堆）。
     * @param cardId         卡牌 ID
     * @param targetPosition 回滚目标位置
     * @param callback       动画结束回调（常用于同步模型后刷新 UI）
     */
    void playUndoAnimation(int cardId,
                           const cocos2d::Vec2& targetPosition,
                           std::function<void()> callback = nullptr);

    /**
     * 播放入场动画（如淡入 + 缩放，提升用户体验）。
     * @param callback 动画结束回调（常用于开始游戏逻辑）
     */
    void playEnterAnimation(std::function<void()> callback = nullptr);

    /**
     * 播放退场动画（如淡出，用于关卡切换或返回主菜单）。
     * @param callback 动画结束回调（常用于清理资源或跳转场景）
     */
    void playExitAnimation(std::function<void()> callback = nullptr);

private:
    // ——— 成员变量说明 ———
    const GameModel*    _gameModel;          ///< 只读模型引用
    DeckFieldView*      _deckFieldView;      ///< 主牌堆区域（桌面可操作牌）
    BottomFieldView*    _bottomFieldView;    ///< 手牌/目标区（底部固定位）
    // BackFieldView*   _backFieldView;      // ← 当前未声明，建议补充（若使用）

    cocos2d::ui::Button* _undoButton;        ///< 撤销按钮（简单实现，可替换为 UndoView）
    UndoButtonCallback   _undoCallback;      ///< 按钮事件处理器
    CardClickCallback    _cardClickCallback; ///< 预留：全局卡牌点击分发（当前由子 View 直连）
    DrawCardCallback     _drawCardCallback;  ///< 供子 View（如 BackFieldView）调用

    /**
     * 创建并配置撤销按钮（位置、样式、事件绑定）。
     */
    void createUndoButton();

    /**
     * 设置整体 UI 布局（如子视图锚点、相对位置、适配屏幕）。
     * @note 建议使用相对坐标（如 _deckFieldView 置中，_bottomFieldView 底部对齐）。
     */
    void setupLayout();

    /**
     * 撤销按钮点击事件处理：校验有效性后触发 _undoCallback。
     */
    void onUndoButtonClicked(cocos2d::Ref* sender,
                             cocos2d::ui::Widget::TouchEventType eventType);
};