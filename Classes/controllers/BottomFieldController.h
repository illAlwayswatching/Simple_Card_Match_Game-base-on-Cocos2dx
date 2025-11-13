#pragma once

#include "../models/GameModel.h"
#include "../views/BottomFieldView.h"
#include "../managers/UndoManager.h"
#include "../models/UndoModel.h"
#include "../services/UndoService.h"

/**
 * @class BottomFieldController
 * @brief 底部区域控制器 —— 负责协调游戏逻辑（GameModel）、界面（BottomFieldView）与撤销操作（UndoManager）。
 * 
 * 使用场景：当玩家点击底部区域的卡牌时，触发业务逻辑（如移动、翻牌），并记录可撤销操作；
 * 是 MVC/MVVM 架构中典型的“Controller”角色，实现逻辑与视图解耦。
 */
class BottomFieldController
{
public:
    /**
     * @brief 卡牌点击事件回调类型。
     * @param cardId 被点击卡牌的唯一ID（由 GameModel 分配）。
     */
    using CardClickCallback = std::function<void(int cardId)>;

    BottomFieldController();
    ~BottomFieldController();

    /**
     * 初始化控制器，绑定模型与服务。
     * @param gameModel 游戏核心数据（非空），提供卡牌状态与规则校验。
     * @param undoManager 撤销管理器（非空），用于注册本次操作。
     * @return true 初始化成功；false 失败（如参数为空）。
     */
    bool init(GameModel* gameModel, UndoManager* undoManager);

    /**
     * 处理卡牌点击事件（业务入口）。
     * @param cardId 被点击的卡牌 ID。
     * @return true 操作成功（如移动合法）；false 无效点击或操作被拒绝。
     * 
     * @note 会触发 _cardClickCallback（若已设置），并调用 updateBottomFieldView() 刷新视图。
     */
    bool handleCardClick(int cardId);

    /**
     * 获取底层视图对象，供上层场景添加到节点树。
     * @return 指向 BottomFieldView 的指针（生命周期由本 Controller 管理）。
     */
    BottomFieldView* getStackView() { return _stackView; }

    // 可选扩展：设置外部回调（例如通知主场景播放音效、更新计分）
    void setCardClickCallback(const CardClickCallback& callback) { _cardClickCallback = callback; }

private:
    /**
     * 根据 GameModel 当前状态，刷新底部视图的卡牌外观（位置、朝向、高亮等）。
     * @return true 刷新成功；false 模型异常。
     */
    bool updateBottomFieldView();

    // ——— 成员变量说明 ———
    GameModel*    _gameModel;     ///< 游戏数据核心，提供卡牌状态与规则
    BottomFieldView* _stackView;  ///< 底部区域的 UI 视图
    CardClickCallback _cardClickCallback; ///< 外部注册的点击回调（如主控制器监听）
    UndoManager*  _undoManager;   ///< 撤销系统入口，用于记录本次操作
};