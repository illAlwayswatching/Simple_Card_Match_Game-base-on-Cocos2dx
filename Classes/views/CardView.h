#pragma once

#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../configs/models/CardResConfig.h"
#include <functional>

/**
 * @class CardView
 * @brief 单张卡牌的可视化组件 —— 负责渲染卡牌外观（正面/背面/数字/花色）并响应点击事件。
 * 
 * 使用场景：作为 UI 基础单元，被 BottomFieldView、BackFieldView 等复用；
 * 支持翻转、移动、匹配等动画，是表现层与数据层（CardModel）的桥梁。
 * 
 * @note 继承自 Node（非 Sprite），便于组合多个子节点（如 sprite + label）。
 */
class CardView : public cocos2d::Node
{
public:
    /// 卡牌点击回调：触发时传入本卡牌 ID
    using CardClickCallback = std::function<void(int cardId)>;

    CardView();
    virtual ~CardView();

    /**
     * 创建卡牌视图（默认无点击回调）。
     * @param cardModel 卡牌数据（非空）
     * @return 新建实例；失败返回 nullptr
     */
    static CardView* create(const CardModel* cardModel);

    /**
     * 创建卡牌视图并绑定点击回调。
     * @param cardModel    卡牌数据
     * @param clickCallback 点击事件处理函数
     * @return 新建实例
     */
    static CardView* create(const CardModel* cardModel, CardClickCallback clickCallback);

    /**
     * 初始化实例（供 create 内部调用）。
     * @return true 初始化成功
     */
    bool init(const CardModel* cardModel, CardClickCallback clickCallback = nullptr);

    // ——— 查询接口 ———
    int getCardId() const;          ///< 获取关联卡牌 ID（来自 _cardModel）
    bool getCardFaceUp() const;     ///< 获取当前朝向（是否正面显示）

    /**
     * 同步更新视图状态（如点数/花色/朝向变化时调用）。
     * @param cardModel 新的卡牌数据（非空）
     * @note 内部会重建 sprite/label 或触发动画（如翻面）。
     */
    void updateCard(const CardModel* cardModel);

    // ——— 动画接口 ———
    /**
     * 播放卡牌移动动画（如拖拽、自动归位）。
     * @param targetPosition 目标位置（本节点父坐标系）
     * @param duration       持续时间（秒）
     * @param callback       动画结束回调
     */
    void playMoveAnimation(const cocos2d::Vec2& targetPosition, float duration,
                           std::function<void()> callback = nullptr);

    /**
     * 播放匹配成功动画（如缩放+闪光+消失）。
     * @param callback 动画结束后回调（常用于移除节点）
     */
    void playMatchAnimation(std::function<void()> callback = nullptr);

    /**
     * 播放撤销回滚动画（如反向移动）。
     * @param targetPosition 回滚目标位置
     * @param duration       动画时长
     * @param callback       结束回调
     */
    void playUndoAnimation(const cocos2d::Vec2& targetPosition, float duration,
                           std::function<void()> callback = nullptr);

    /**
     * 设置卡牌整体可见性（慎用：隐藏后仍占触摸区域）。
     * @param visible true 显示；false 隐藏
     */
    void setCardVisible(bool visible);

    /**
     * 播放翻面动画（正面↔背面切换）。
     * @param callback 翻面完成回调（常用于同步 model 的 _isFaceUp）
     */
    void playFlipAnimation(std::function<void()> callback = nullptr);

private:
    // ——— 辅助方法 ———
    /**
     * 根据 cardModel 创建/更新卡牌精灵（调用 CardResConfig）。
     * @note 若 _isFaceUp=false，显示统一背面图；否则显示点数+花色。
     */
    void createCardSprite(const CardModel* cardModel);

    /**
     * 注册触摸监听事件（支持点击检测）。
     */
    void setupTouchListener();

    /**
     * 触摸开始回调（用于判断是否点中本卡牌）。
     * @return true 消费事件；false 传递给其他节点
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    // ——— 成员变量说明 ———
    const CardModel*   _cardModel;     ///< 关联的卡牌数据（弱引用，不负责生命周期）
    CardClickCallback  _clickCallback; ///< 点击回调
    cocos2d::Sprite*   _cardSprite;    ///< 卡牌主图（背面 or 正面组合图）
    cocos2d::Label*    _cardLabel;     ///< 可选：辅助文本（如调试用 ID 显示）
};