#pragma once

#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include <functional>

/**
 * @class UndoManager
 * @brief 撤销管理器 —— 统一收集、存储和执行撤销操作，解耦业务逻辑与撤销流程。
 * 
 * 使用场景：各控制器（如 BottomFieldController）在执行操作后调用 `addUndoRecord()` 注册撤销步骤；
 * 玩家点击“撤销”按钮时，本类负责回滚 GameModel 状态并通知上层。
 */
class UndoManager
{
public:
    /// 撤销完成回调类型：参数表示撤销是否成功（如栈空时失败）
    using UndoCompleteCallback = std::function<void(bool success)>;

    UndoManager();
    ~UndoManager();

    /**
     * 初始化撤销管理器。
     * @param gameModel 游戏数据模型（非空），用于执行状态回滚。
     * @param callback  可选回调，撤销结束后触发（如刷新UI/播放动画）。
     */
    void init(GameModel* gameModel, UndoCompleteCallback callback = nullptr);

    /**
     * 添加一条撤销记录（通常在执行操作后立即调用）。
     * @param record 包含：操作类型、原始卡牌状态、目标状态等，用于反向恢复。
     */
    void addUndoRecord(const UndoModel::UndoRecord& record);

    /**
     * 执行一次撤销：弹出最近记录，调用其反向操作，更新 GameModel。
     * @return true 成功撤销；false 无可撤销操作（记录栈为空）。
     */
    bool executeUndo();

private:
    // ——— 成员变量说明 ———
    UndoModel* _undoModel;            ///< 撤销数据模型（含操作栈、记录结构）
    GameModel* _gameModel;            ///< 关联的游戏状态，供撤销时修改
    UndoCompleteCallback _undoCompleteCallback; ///< 外部注册的完成回调

    /**
     * 内部统一回调触发函数，避免重复判空。
     * @param success 撤销是否成功
     */
    void _notifyUndoComplete(bool success);
};