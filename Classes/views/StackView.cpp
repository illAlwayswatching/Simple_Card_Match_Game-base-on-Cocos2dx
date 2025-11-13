#include "StackView.h"

StackView* StackView::create()
{
	return create(nullptr);
}

StackView* StackView::create(StackCardClickCallback clickCallback)
{
	StackView* ret = new StackView();
	if (ret && ret->init(clickCallback)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool StackView::init(StackCardClickCallback clickCallback)
{
	if (!Node::init()) {
		return false;
	}

	_cardClickCallback = clickCallback;
	setupLayout();
	return true;
}

void StackView::updateStack(const GameModel* gameModel)
{
	if (!gameModel) return;

	// 清理现有卡牌视图
	clearAllCardViews();

	// 创建新的卡牌视图
	const auto& stackCards = gameModel->getBottomFieldCards();
	int handTopIndex = gameModel->getBottomFieldTopIndex();

	for (size_t i = 0; i < stackCards.size(); ++i) {
		const CardModel* cardModel = stackCards[i];
		if (cardModel) {
			cocos2d::Vec2 position;
			if (i == handTopIndex) {
				// 顶部卡牌放在顶部位置
				position = _handTopPosition;
			}
			else {
				// 其他卡牌放在扇形位置
				int fanIndex = std::min((int)_fanPositions.size() - 1, (int)i);
				position = _fanPositions[fanIndex];
			}

			CardView* cardView = createCardView(cardModel, position);
			if (cardView) {
				_cardViews.push_back(cardView);
				addChild(cardView);
			}
		}
	}
}

void StackView::playSwapAnimation(int fromCardId, int toCardId, std::function<void()> callback)
{
	CardView* fromCardView = getCardView(fromCardId);
	CardView* toCardView = getCardView(toCardId);

	if (fromCardView && toCardView) {
		cocos2d::Vec2 fromPos = fromCardView->getPosition();
		cocos2d::Vec2 toPos = toCardView->getPosition();

		// 交换位置
		fromCardView->playMoveAnimation(toPos, 0.25f);
		toCardView->playMoveAnimation(fromPos, 0.25f);

		if (callback) {
			auto delay = cocos2d::DelayTime::create(0.26f);
			auto callFunc = cocos2d::CallFunc::create(callback);
			auto sequence = cocos2d::Sequence::create(delay, callFunc, nullptr);
			runAction(sequence);
		}
	}
}

void StackView::playUndoAnimation(int cardId, const cocos2d::Vec2& targetPosition,
	std::function<void()> callback)
{
	CardView* cardView = getCardView(cardId);
	if (cardView) {
		cardView->playUndoAnimation(targetPosition, 0.25f, callback);
	}
}

CardView* StackView::getCardView(int cardId) const
{
	for (CardView* cardView : _cardViews) {
		if (cardView && cardView->getCardId() == cardId) {
			return cardView;
		}
	}
	return nullptr;
}

CardView* StackView::createCardView(const CardModel* cardModel, const cocos2d::Vec2& position)
{
	if (!cardModel) return nullptr;

	auto clickCallback = [this](int cardId) {
		onCardClicked(cardId);
	};

	CardView* cardView = CardView::create(cardModel, clickCallback);
	if (cardView) {
		cardView->setPosition(position);
	}
	return cardView;
}

void StackView::clearAllCardViews()
{
	for (CardView* cardView : _cardViews) {
		if (cardView) {
			cardView->removeFromParent();
		}
	}
	_cardViews.clear();
}

void StackView::setupLayout()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	// 手牌区位置（底部580像素高度）
	_handTopPosition = cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + 580 - 120);

	// 扇形位置
	const float fanOffset = 60.0f;
	for (int i = -2; i <= 2; ++i) {
		cocos2d::Vec2 fanPos = _handTopPosition + cocos2d::Vec2(i * fanOffset, -i * 2);
		_fanPositions.push_back(fanPos);
	}
}

void StackView::onCardClicked(int cardId)
{
	if (_cardClickCallback) {
		_cardClickCallback(cardId);
	}
}
