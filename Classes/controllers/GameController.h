#pragma once

#include "cocos2d.h"
#include "../configs/models/LevelConfig.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/GameModelFromLevelGenerator.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"
#include "DeckFieldController.h"
#include "BottomFieldController.h"

/**
 * @class GameController
 * @brief 游戏主控制器 —— 协调整个关卡运行：加载配置、初始化模型/视图/子控制器、处理全局操作（如撤销）。
 * 
 * 使用场景：一局游戏的生命周期管理（start → play → undo → end），是 MVC 架构中的顶层 Controller，
 * 持有所有核心模块引用，负责事件串联与状态同步。
 */
class GameController {
private:
    // ——— 成员变量说明 ———
    cocos2d::Node* _parent;               ///< 父节点（如 HelloWorldScene），用于添加子视图
    GameModel*     _gameModel;            ///< 游戏数据核心（卡牌状态、规则校验）
    GameView*      _gameView;             ///< 主视图容器，聚合各区域子视图
    UndoManager*   _undoManager;          ///< 撤销管理器，统一处理所有可撤销操作
    DeckFieldController*    _deckFieldController;      ///< 牌堆区域控制器
    BottomFieldController*  _bottomFieldController;    ///< 底部区域控制器

    /**
     * 创建并初始化子控制器（牌堆、底部区等）。
     * @return true 成功；false 初始化失败（如内存分配失败）。
     */
    bool createSubControllers();

    /**
     * 创建并初始化主视图（GameView），并将子视图添加到场景。
     * @return true 成功；false 视图构建失败。
     */
    bool initGameView();

    // ——— 事件回调 ———
    /**
     * 牌堆点击成功后的回调（如抽一张牌）。
     */
    void onDeckFieldCardClicked();

    /**
     * 底部卡牌点击完成回调。
     * @param success 操作是否成功（由 BottomFieldController 返回）
     */
    void onBottomFieldCardClicked(bool success);

    /**
     * 撤销操作完成回调（用于刷新视图或提示）。
     * @param success 撤销是否成功（如已无操作可撤）
     */
    void onUndoComplete(bool success);

    /**
     * 全局刷新视图：调用各子控制器的 updateXxxView() 并同步 GameView 状态。
     */
    void updateGameView();

public:
    GameController();
    ~GameController();

    /**
     * 初始化控制器基础环境（绑定父节点、创建依赖对象）。
     * @param parent 场景中的宿主节点（非空）。
     * @return true 初始化成功。
     */
    bool init(cocos2d::Node* parent);

    /**
     * 启动一局新游戏：加载关卡配置 → 生成 GameModel → 初始化视图 → 开始渲染。
     * @param levelId 关卡 ID（如 "level_1"）。
     * @return true 启动成功；false 关卡加载/初始化失败。
     */
    bool startGame(const std::string& levelId);

    /**
     * 触发一次撤销操作（交由 UndoManager 执行，并回调更新视图）。
     * @return true 请求已发出（不保证最终成功，结果见 onUndoComplete）。
     */
    bool handleUndo();
};