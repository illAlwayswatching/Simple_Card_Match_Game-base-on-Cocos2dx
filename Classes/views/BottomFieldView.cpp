#include "BottomFieldView.h"
#include "../utils/CardUtils.h"

USING_NS_CC;

BottomFieldView::BottomFieldView()
	: _gameModel(nullptr)
	, _topCardView(nullptr)
	, _BackUpTopCardView(nullptr)
	, _cardClickCallback(nullptr)
	, _cardSize(CardUtils::getCardSize())
{
}

BottomFieldView::~BottomFieldView()
{
	for (auto* cardView : _cardViews) {
		if (cardView) {
			cardView->removeFromParent();
		}
	}
	_cardViews.clear();
}

BottomFieldView* BottomFieldView::create(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
	BottomFieldView* view = new (std::nothrow) BottomFieldView();
	if (view && view->init(gameModel, cardClickCallback)) {
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return nullptr;
}

bool BottomFieldView::init(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
	if (!Layer::init()) {
		return false;
	}

	_gameModel = gameModel;
	_cardClickCallback = cardClickCallback;

	// 初始化显示
	updateDisplay();

	return true;
}

void BottomFieldView::updateDisplay()
{	
	cocos2d::Vec2 normalPos = cocos2d::Vec2(700, 100);

	// 清除现有卡牌视图
	if (_topCardView) {
		_topCardView->removeFromParent();
		_topCardView = nullptr;
	}
	
	for (auto* cardView : _cardViews) {
		if (cardView) {
			cardView->removeFromParent();
			cardView = nullptr;
		}
	}
	_cardViews.clear();

	if (!_gameModel) {
		return;
	}

	// 只显示顶部卡牌（底牌区只有一张牌）
	CardModel* topCard = _gameModel->getCurrentHands();
	if (topCard) {
		cocos2d::log("BottomFieldView::updateDisplay - Creating card view for card %d, face: %d, suit: %d, pos: (%.1f, %.1f)",
			topCard->getCardId(), topCard->getFace(), topCard->getSuit(),
			topCard->getPosition().x, topCard->getPosition().y);

		_topCardView = createCardView(topCard);
		if (_topCardView) {
			addChild(_topCardView);
			_topCardView->setPosition(normalPos);
			cocos2d::log("BottomFieldView::updateDisplay - Card view created and positioned");
		}
		else {
			cocos2d::log("BottomFieldView::updateDisplay - Failed to create card view");
		}
	}
	else {
		cocos2d::log("BottomFieldView::updateDisplay - No top card found");
	}

 	cocos2d::Vec2 normalPosForBackup = cocos2d::Vec2(100, 100);
	cocos2d::Vec2 interval = cocos2d::Vec2(80, 0);

	auto backupCards = _gameModel->getBackupFieldCards();

	for (auto backupCard : backupCards) {
		auto backupCardView = createCardView(backupCard);
		backupCardView->setPosition(normalPosForBackup);
		addChild(backupCardView);
		_cardViews.push_back(backupCardView);
		normalPosForBackup = normalPosForBackup + interval;
	}
}

CardView* BottomFieldView::createCardView(CardModel* card)
{
	if (!card) {
		return nullptr;
	}

	CardView* cardView = CardView::create(card, [this](int cardId) {
		// 底牌堆卡牌点击回调
		if (_cardClickCallback) {
			_cardClickCallback(cardId);
		}
	});

	return cardView;
}

void BottomFieldView::playCardMoveAnimation(int cardId, const Vec2& targetPosition, float duration, std::function<void()> callback)
{
	if (_topCardView && _topCardView->getCardId() == cardId) {
		auto moveAction = MoveTo::create(duration, targetPosition);
		if (callback) {
			auto sequence = Sequence::create(moveAction, CallFunc::create(callback), nullptr);
			_topCardView->runAction(sequence);
		}
		else {
			_topCardView->runAction(moveAction);
		}
	}
}


void BottomFieldView::updateCardPositions()
{
	int  backupSize = _gameModel->getBackupFieldCardsSize();
	cocos2d::Vec2 normalPos = cocos2d::Vec2(100, 100);
	cocos2d::Vec2 interval = cocos2d::Vec2(30, 0);

	auto backupCards = _gameModel->getBackupFieldCards();

	for (auto backupCard : backupCards) {
		auto backupCardView = createCardView(backupCard);
		backupCardView->setPosition(normalPos);
		addChild(backupCardView);
		_cardViews.push_back(backupCardView);
		_BackUpTopCardView = backupCardView;
		normalPos = normalPos + interval;
	}

	cocos2d::Vec2 normalPosForHands = cocos2d::Vec2(700, 100);
	_topCardView->setPosition(normalPosForHands);

}