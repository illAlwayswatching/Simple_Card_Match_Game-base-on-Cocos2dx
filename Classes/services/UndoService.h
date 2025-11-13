#pragma once

#include "../models/UndoModel.h"
#include "../models/GameModel.h"

/**
 * @class UndoService
 * @brief 撤销业务逻辑服务 —— 负责生成撤销记录（快照）与执行回滚操作，解耦 UndoManager 与具体游戏规则。
 * 
 * 使用场景：各 Controller 在执行操作后调用 `createXxxRecord()` 生成记录；
 * UndoManager 在撤销时调用 `executeUndo()` 执行逆向操作，确保回滚逻辑集中、可测、可维护。
 */
class UndoService
{
public:
    /**
     * 生成“手牌交换”操作的撤销记录（例如：拖拽换位）。
     * @param gameModel 当前游戏模型（快照源，非空）
     * @return 包含交换前后卡牌 ID 与位置的 UndoRecord
     * 
     * @note 记录应足够还原状态，如：卡A原位→卡B位，卡B原位→卡A位。
     */
    static UndoModel::UndoRecord createHandSwapRecord(const GameModel* gameModel);

    /**
     * 生成“桌面牌 → 手牌”移动操作的撤销记录。
     * @param gameModel        当前游戏模型（非空）
     * @param playfieldCardId  被移动的桌面卡牌 ID（如主牌堆中某张）
     * @return 包含源/目标位置、卡牌 ID、手牌栈状态的 UndoRecord
     * 
     * @note 原函数声明中 `stackCardId` 未使用，已移除（依实现可后续补充）。
     */
    static UndoModel::UndoRecord createDeckFieldToHandRecord(
        const GameModel* gameModel,
        int playfieldCardId);

    /**
     * 执行单条撤销记录的回滚操作（逆向恢复状态）。
     * @param gameModel 游戏模型（非空），将被修改
     * @param record    待执行的撤销记录
     * @return true：回滚成功；false：记录无效或模型异常
     * 
     * @note 根据 record.actionType 分发执行不同逻辑（如 swap ↔ unswap, move ↔ move back）。
     */
    static bool executeUndo(GameModel* gameModel, const UndoModel::UndoRecord& record);

    /**
     * 在主牌堆中查找指定卡牌的索引位置。
     * @param gameModel        游戏模型（非空）
     * @param deckFieldCardId  目标卡牌 ID
     * @return 索引（≥0）；未找到返回 -1
     * 
     * @usage 典型用于回滚时定位卡牌在 _deckFieldCards 中的位置。
     */
    static int findPlayfieldCardIndex(const GameModel* gameModel, int deckFieldCardId);
};