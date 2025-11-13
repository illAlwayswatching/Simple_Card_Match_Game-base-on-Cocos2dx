#pragma once

#include "CardView.h"
#include "../models/GameModel.h"

#include <functional>

/**
* 主牌堆视图
* 职责：显示主牌堆中的卡牌，处理卡牌点击事件
* 使用场景：游戏主界面，显示待消除的卡牌
*/
class DeckFieldView : public cocos2d::Layer
{
public:
	DeckFieldView();
	~DeckFieldView();

	/**
	* 卡牌点击回调函数类型     
	* @param cardId 被点击的卡牌ID
	*/
	using CardClickCallback = std::function<void(int cardId)>;

	/**
	* 创建主牌堆视图
	* @param gameModel 游戏模型
	* @param cardClickCallback 卡牌点击回调
	* @return 主牌堆视图实例
	*/
	static DeckFieldView* create(const GameModel* gameModel, CardClickCallback cardClickCallback);

	static  DeckFieldView* create(const GameModel* gameModel);

	/**
	* 初始化
	* @param gameModel 游戏模型
	* @param cardClickCallback 卡牌点击回调
	* @return 是否初始化成功
	*/
	bool init(const GameModel* gameModel, CardClickCallback cardClickCallback);

	/**
	* 更新显示
	*/
	void updateDisplay();

	/**
	* 播放卡牌移动动画
	* @param cardId 卡牌ID
	* @param targetPosition 目标位置
	* @param duration 动画持续时间
	* @param callback 动画完成回调
	*/
	void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, float duration, std::function<void()> callback = nullptr);

	/**
	* 播放卡牌翻转动画
	* @param cardId 卡牌ID
	* @param callback 动画完成回调
	*/
	void playCardFlipAnimation(int cardId, std::function<void()> callback = nullptr);

	/**
	* 移除卡牌
	* @param cardId 卡牌ID
	*/
	void removeCard(int cardId);

	/**
	* 添加卡牌
	* @param card 卡牌模型
	*/
	void addCard(CardModel* card);

private:
	cocos2d::DrawNode* _bgDrawNode = nullptr;
	const GameModel* _gameModel;                    ///< 游戏模型（只读）
	CardClickCallback _cardClickCallback;           ///< 卡牌点击回调
	std::vector<CardView*> _cardViews;              ///< 卡牌视图列表
	cocos2d::Size _cardSize;                        ///< 卡牌尺寸
	float _cardSpacing;                             ///< 卡牌间距

	/**
    * 创建卡牌视图
    * @param card 卡牌模型
    * @return 卡牌视图
    */
    CardView* createCardView(CardModel* card);
    
    /**
    * 处理卡牌点击事件
    * @param cardId 卡牌ID
    */
    void onCardClicked(int cardId);
    
    /**
    * 更新卡牌位置
    */
    void updateCardPositions();

};

