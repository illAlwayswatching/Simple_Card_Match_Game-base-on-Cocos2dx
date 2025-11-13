#pragma once

#include "cocos2d.h"
#include <vector>

/**
 * @enum UndoActionType
 * @brief 撤销操作类型 —— 标识可回滚的动作种类，用于反向执行。
 */
enum UndoActionType
{
    UAT_NONE = 0,              ///< 无效操作
    UAT_HAND_SWAP,             ///< 手牌内部交换（如拖拽换位）
    UAT_PLAYFIELD_TO_HAND      ///< 从桌面移回手牌（或反向操作的逆动作）
};

/**
 * @class UndoModel
 * @brief 撤销数据模型 —— 存储操作历史记录栈，供 UndoManager 调度执行。
 * 
 * 使用场景：每次玩家有效操作后，由对应 Controller 构造 UndoRecord 并添加；
 * 撤销时由 UndoManager 弹出最新记录，按 actionType 反向恢复状态。
 */
class UndoModel
{
public:
    /**
     * @struct UndoRecord
     * @brief 单次操作的撤销快照，记录足够的信息以逆向还原。
     * 
     * - actionType     操作类型，决定如何回滚  
     * - sourceCardId   操作起点卡牌 ID（如被移动的牌）  
     * - targetCardId   操作终点卡牌 ID（如目标位置已有牌，或 -1 表示空位）  
     * - sourcePosition 原始坐标（回滚时恢复位置）  
     * - targetPosition 目标坐标（用于校验或辅助定位）  
     * - deckFieldIndex 若涉及主牌堆，记录其在 _deckFieldCards 中的索引（如抽牌位置）
     */
    struct UndoRecord
    {
        UndoActionType actionType;
        int sourceCardId;
        int targetCardId;
        cocos2d::Vec2 sourcePosition;
        cocos2d::Vec2 targetPosition;
        int deckFieldIndex;

        UndoRecord()
            : actionType(UAT_NONE)
            , sourceCardId(-1)
            , targetCardId(-1)
            , sourcePosition(cocos2d::Vec2::ZERO)
            , targetPosition(cocos2d::Vec2::ZERO)
            , deckFieldIndex(-1)
        {}
    };

    UndoModel();
    ~UndoModel();

    /**
     * 添加一条撤销记录（压入栈顶）。
     * @param record 待保存的操作快照（建议 move 语义优化，当前为 const&）
     */
    void addUndoRecord(const UndoRecord& record);

    /**
     * 获取最新（栈顶）撤销记录。
     * @return 最新记录；若无记录，返回默认构造的 UndoRecord（actionType == UAT_NONE）。
     * @note 不修改栈（仅 peek），安全调用。
     */
    UndoRecord getLastUndoRecord() const;

    /**
     * 移除最新撤销记录（弹出栈顶）。
     * @note 若栈为空，静默忽略（不 crash）。
     */
    void removeLastUndoRecord();

    /**
     * 检查是否有可撤销的操作。
     * @return true：_undoRecords 非空。
     */
    bool hasUndoableAction() const { return !_undoRecords.empty(); }

    /**
     * 清空所有撤销记录（如新游戏开始时调用）。
     */
    void clearAllRecords();

    /**
     * 获取当前撤销记录总数（用于 UI 显示“可撤 × 次”）。
     * @return 记录数量（≥0）。
     */
    int getRecordCount() const { return static_cast<int>(_undoRecords.size()); }

private:
    std::vector<UndoRecord> _undoRecords;  ///< 撤销记录栈（vector 模拟栈：push_back / pop_back）
};