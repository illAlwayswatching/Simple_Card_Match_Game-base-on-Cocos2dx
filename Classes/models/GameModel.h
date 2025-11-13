#pragma once

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <stack>  // 补充缺失头文件（因使用了 std::stack）

class GameModel
{
public:
    GameModel();
    ~GameModel();

    // ——— Getter（只读）———
    const std::vector<CardModel*>& getDeckFieldCards() const { return _deckFieldCards; }    ///< 主牌堆：待抽牌区（通常背面朝上）
    const std::vector<CardModel*>& getBackupFieldCards() const { return _backupFieldCards; }///< 备用牌堆：已抽/暂存区（如弃牌堆或缓冲区）
    const std::string& getGameState() const { return _gameState; }                          ///< 当前游戏状态（如 "playing", "win", "lose"）

    // ——— Getter（可写）———
    std::vector<CardModel*>& getDeckFieldCards() { return _deckFieldCards; }
    std::vector<CardModel*>& getBackupFieldCards() { return _backupFieldCards; }  // ✅ 修正拼写：原 gettBackupFieldCards → getBackupFieldCards

    // ——— 特殊访问 ———
    /**
     * 获取当前手牌顶部卡牌（即将出/可操作的牌）。
     * @return 栈顶卡牌；若手牌为空，行为未定义（建议先判空）。
     */
    CardModel* getCurrentHands() const { 
        return _handsFieldCards.empty() ? nullptr : _handsFieldCards.top(); 
    }

    /**
     * 获取备用牌堆最后一张卡牌（常用于“回退”或“重抽”逻辑）。
     * @return 末尾卡牌；若为空，返回 nullptr。
     */
    CardModel* getCurrentBackup() const { 
        return _backupFieldCards.empty() ? nullptr : _backupFieldCards.back(); 
    }

    /**
     * 获取备用牌堆卡牌数量。
     * @return 卡牌张数（非负整数）；安全转换，无需 try-catch（vector::size() 不抛异常）。
     */
    int getBackupFieldCardsSize() const { 
        return static_cast<int>(_backupFieldCards.size()); 
    }

    // ——— Setter ———
    void setDeckFieldCards(const std::vector<CardModel*>& cards);
    void setHandsFieldCards(const std::vector<CardModel*>& cards);
    void setBackupFieldCards(const std::vector<CardModel*>& cards);
    void setGameState(const std::string& state) { _gameState = state; }

    // ——— 增删改查 ———
    void addDeckFieldCard(CardModel* card);
    void addHandsFieldCard(CardModel* card);      // 入栈（push）
    void addBackupFieldCard(CardModel* card);     // 尾插（push_back）

    bool removeDeckFieldCard(int cardId);         // 按 ID 删除（需遍历）
    bool removeHandsFieldCard();                  // 弹出栈顶（pop），无参（无需 cardId）
    bool removeBackupFieldCard();                 // 弹出末尾（pop_back），无参

    CardModel* findDeckFieldCard(int cardId) const;  // 按 ID 在主牌堆查找

    // ——— 工具方法 ———
    void clearAllCards();         ///< 清空所有区域卡牌（注意：不 delete 内存，仅清理指针）
    bool isGameOver() const;      ///< 根据规则判断是否结束（如主牌堆空 + 手牌无合法操作）

private:
    std::vector<CardModel*> _deckFieldCards;     ///< 主牌堆（抽牌区）
    std::stack<CardModel*>  _handsFieldCards;    ///< 手牌堆（LIFO，如“当前可操作牌”栈）
    std::vector<CardModel*> _backupFieldCards;   ///< 备用堆（FIFO/LIFO 混合，如弃牌区）
    std::string             _gameState;          ///< 状态标识，可用于 UI 提示或存档
};