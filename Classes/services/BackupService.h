#pragma once

#include "../models/CardModel.h"
#include "../models/GameModel.h"
#include "../managers/UndoManager.h"

/**
 * @class BackupService
 * @brief 备用牌操作服务类 —— 封装备用牌堆（_backupFieldCards）相关的业务规则与原子操作。
 * 
 * 使用场景：当玩家从备用区取牌、自动补牌等逻辑触发时，由控制器（如 BottomFieldController）
 * 调用本类静态方法，确保操作符合规则，并自动注册撤销记录。
 * 
 * @note 纯静态工具类，无状态，线程不安全（游戏主线程调用即可）。
 */
class BackupService
{
public:
    /**
     * 检查当前游戏状态是否允许启用备用牌逻辑（如：主牌堆为空时自动启用备用区补牌）。
     * @param gameModel 游戏模型（非空）。
     * @return true：满足启用条件（如 _deckFieldCards.empty() && !_backupFieldCards.empty()）。
     */
    static bool isBackup(GameModel* gameModel);

    /**
     * 将指定卡牌从备用区移入手牌区（如“抽一张备用牌”）。
     * @param cardId   目标卡牌 ID（需存在于 _backupFieldCards 中）。
     * @param gameModel 游戏模型（非空），用于修改状态。
     * @param undoManager 撤销管理器（可为空），若提供则自动记录本次移动。
     * @return true：操作成功；false：卡牌不存在、区域状态非法等。
     * 
     * @note 成功时会：1) 从备用区移除 card；2) 加入手牌栈；3) 注册撤销记录（若 undoManager 非空）。
     */
    static bool executeMoveToHand(int cardId, GameModel* gameModel, UndoManager* undoManager);

    /**
     * 执行备用区内部的数据整理（如将备用区末尾牌移至主牌堆底部，实现“洗回重抽”）。
     * @param gameModel 游戏模型（非空）。
     * @return true：整理成功；false：无数据可整理或规则不允许。
     * 
     * @usage 典型场景：主牌堆耗尽时，自动将备用区牌“洗回”形成新主堆。
     */
    static bool MoveInDataModels(GameModel* gameModel);
};