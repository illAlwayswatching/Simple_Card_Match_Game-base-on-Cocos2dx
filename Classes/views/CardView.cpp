#include "CardView.h"

USING_NS_CC;

CardView::CardView()
	: _cardModel(nullptr)
	, _clickCallback(nullptr)
	, _cardSprite(nullptr)
	, _cardLabel(nullptr)
{
}

CardView::~CardView()
{
}

CardView* CardView::create(const CardModel* cardModel)
{
	return create(cardModel, nullptr);
}

CardView* CardView::create(const CardModel* cardModel, CardClickCallback clickCallback)
{
	CardView* ret = new CardView();
	if (ret && ret->init(cardModel, clickCallback)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool CardView::init(const CardModel* cardModel, CardClickCallback clickCallback)
{
	if (!Node::init()) {
		return false;
	}

	_cardModel = cardModel;
	_clickCallback = clickCallback;

	// 设置CardView的锚点为(0,0)，左下角为原点，与碰撞盒原点对齐
	//setAnchorPoint(Vec2(0.5f, 0.5f));  // 改为中心点
	setAnchorPoint(cocos2d::Vec2(0, 0));

	if (_cardModel) {
		createCardSprite(_cardModel);
		setPosition(_cardModel->getPosition());
	}

	setupTouchListener();
	return true;
}

int CardView::getCardId() const
{
	return _cardModel ? _cardModel->getCardId() : -1;
}

bool CardView::getCardFaceUp() const
{
	return _cardModel ? _cardModel->isFaceUp() : false;
}

void CardView::updateCard(const CardModel* cardModel)
{
	_cardModel = cardModel;
	if (_cardModel) {
		setPosition(_cardModel->getPosition());
	}
}

void CardView::playMoveAnimation(const cocos2d::Vec2& targetPosition, float duration,
	std::function<void()> callback)
{
	auto moveAction = cocos2d::MoveTo::create(duration, targetPosition);
	if (callback) {
		auto sequence = cocos2d::Sequence::create(moveAction,
			cocos2d::CallFunc::create(callback),
			nullptr);
		runAction(sequence);
	}
	else {
		runAction(moveAction);
	}
}

void CardView::playMatchAnimation(std::function<void()> callback)
{
	// 播放缩放动画表示匹配
	auto scaleUp = cocos2d::ScaleTo::create(0.1f, 1.2f);
	auto scaleDown = cocos2d::ScaleTo::create(0.1f, 1.0f);
	auto sequence = cocos2d::Sequence::create(scaleUp, scaleDown, nullptr);

	if (callback) {
		auto fullSequence = cocos2d::Sequence::create(sequence,
			cocos2d::CallFunc::create(callback),
			nullptr);
		runAction(fullSequence);
	}
	else {
		runAction(sequence);
	}
}

void CardView::playUndoAnimation(const cocos2d::Vec2& targetPosition, float duration,
	std::function<void()> callback)
{
	playMoveAnimation(targetPosition, duration, callback);
}

void CardView::setCardVisible(bool visible)
{
	setVisible(visible);
}

void CardView::createCardSprite(const CardModel* cardModel)
{
	if (!cardModel) return;

	// 使用卡牌背景
	_cardSprite = CardResConfig::createCardSprite(cardModel);
	if (_cardSprite) {
		addChild(_cardSprite);
	}
}


void CardView::setupTouchListener()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!_cardModel || !_cardSprite || !getCardFaceUp()) {
		return false;
	}

	// 1. 获取触摸坐标
	cocos2d::Vec2 touchWorldPos = touch->getLocation();
	cocos2d::Vec2 localPos = this->convertToNodeSpaceAR(touchWorldPos);

	// 2. 定义 cardSize
	cocos2d::Size cardSize = _cardSprite->getContentSize();

	// 3. 计算边界框
	cocos2d::Vec2 anchor = this->getAnchorPoint();
	float bboxX = -cardSize.width * anchor.x;
	float bboxY = -cardSize.height * anchor.y;
	cocos2d::Rect actualBbox(cocos2d::Vec2(bboxX, bboxY), cardSize);

	// 调试日志：显示锚点和边界框信息
	cocos2d::log("CardView::onTouchBegan - Card %d: anchor(%.1f,%.1f) bbox(%.1f,%.1f,%.1f,%.1f) local(%.1f,%.1f)",
		_cardModel->getCardId(), anchor.x, anchor.y,
		actualBbox.origin.x, actualBbox.origin.y, actualBbox.size.width, actualBbox.size.height,
		localPos.x, localPos.y);

	// 4. 判定触摸是否在边界框内
	if (actualBbox.containsPoint(localPos)) {
		cocos2d::log("CardView::onTouchBegan - Touch hit card %d", _cardModel->getCardId());
		if (_clickCallback) {
			_clickCallback(_cardModel->getCardId());
		}
		return true;
	}
	else {
		cocos2d::log("CardView::onTouchBegan - Touch missed card %d: local(%.1f,%.1f) not in bbox(%.1f,%.1f,%.1f,%.1f)",
			_cardModel->getCardId(), localPos.x, localPos.y,
			actualBbox.origin.x, actualBbox.origin.y,
			actualBbox.size.width, actualBbox.size.height);
	}

	return false;
}

void CardView::playFlipAnimation(std::function<void()> callback)
{
	// 简单的翻转动画 - 使用缩放动画模拟翻转效果
	auto scaleDown = cocos2d::ScaleTo::create(0.15f, 0.0f, 1.0f);
	auto scaleUp = cocos2d::ScaleTo::create(0.15f, 1.0f, 1.0f);
	auto flip = cocos2d::Sequence::create(scaleDown, scaleUp, nullptr);

	if (callback) {
		auto callFunc = cocos2d::CallFunc::create(callback);
		auto sequence = cocos2d::Sequence::create(flip, callFunc, nullptr);
		runAction(sequence);
	}
	else {
		runAction(flip);
	}
}


