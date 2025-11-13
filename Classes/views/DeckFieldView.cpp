#include "DeckFieldView.h"
#include "../utils/CardUtils.h"

USING_NS_CC;

DeckFieldView::DeckFieldView()
	: _gameModel(nullptr)
	, _cardClickCallback(nullptr)
	, _cardSize(CardUtils::getCardSize())
	, _cardSpacing(10.0f)
{
}

DeckFieldView::~DeckFieldView()
{
}

DeckFieldView* DeckFieldView::create(const GameModel* gameModel)
{
	return create(gameModel, nullptr);
}

DeckFieldView* DeckFieldView::create(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
	DeckFieldView* view = new (std::nothrow) DeckFieldView();
	if (view && view->init(gameModel, cardClickCallback)) {
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return nullptr;
}

bool DeckFieldView::init(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
	if (!Layer::init()) {
		return false;
	}

	_gameModel = gameModel;
	_cardClickCallback = cardClickCallback;

	//关键：设置内容尺寸（例如宽 1200，高 300）
	const float width = 1080.0f;
	const float height = 1500.0f;
	setContentSize(cocos2d::Size(width, height));

	// 推荐：设锚点为中点（方便居中定位），或底部中点（牌区常用）
	setAnchorPoint(cocos2d::Vec2(540,300));        // (0.5, 0.5)

	//颜色设置
	_bgDrawNode = cocos2d::DrawNode::create();
	_bgDrawNode->setPosition(cocos2d::Vec2(0,580));
	_bgDrawNode->setTag(999); // 方便后续查找
	this->addChild(_bgDrawNode, 0); // 底层

	// 绘制不透明背景
	Size size = getContentSize();
	Vec2 vertices[4] = {
		Vec2(0, 0),
		Vec2(size.width, 0),
		Vec2(size.width, size.height),
		Vec2(0, size.height)
	};

	// 不透明的颜色（alpha = 1.0f 或 255）
	Color4F bgColor(0.439f, 0.259f, 0.078f, 1.0f);  // 木质褐色，完全不透明
	_bgDrawNode->drawSolidRect(vertices[0], vertices[2], bgColor);
	
	// 不需要在这里设置触摸事件，CardView会处理自己的触摸事件

	// 初始化显示
	updateDisplay();

	return true;
}

void DeckFieldView::updateDisplay()
{
	// 清除现有卡牌视图
	for (auto* cardView : _cardViews) {
		if (cardView) {
			cardView->removeFromParent();
		}
	}
	_cardViews.clear();

	if (!_gameModel) {
		return;
	}

	// 创建新的卡牌视图
	const auto& cards = _gameModel->getDeckFieldCards();
	for (auto* card : cards) {
		if (card) {
			CardView* cardView = createCardView(card);
			if (cardView) {
				addChild(cardView);
				_cardViews.push_back(cardView);
			}
		}
	}

	// 更新卡牌位置
	updateCardPositions();
}

void DeckFieldView::playCardMoveAnimation(int cardId, const Vec2& targetPosition, float duration, std::function<void()> callback)
{

	for (auto* cardView : _cardViews) {
		if (cardView && cardView->getCardId() == cardId) {
			cardView->runAction(Sequence::create(
				DelayTime::create(1.0f),  // 延迟 1 秒
				CallFunc::create([cardView]() {
				cocos2d::log("开始移动！从 (%.1f, %.1f) 到 (400, 300)",
					cardView->getPosition().x, cardView->getPosition().y);

				// 关键：MoveTo 动画
				auto move = MoveTo::create(2.0f, Vec2(400, 300));
				auto logFinish = CallFunc::create([]() {
					cocos2d::log("移动完成！");
				});
				cardView->runAction(Sequence::create(move, logFinish, nullptr));
			}),
				nullptr
				));
			break;
		}
	}
}

void DeckFieldView::playCardFlipAnimation(int cardId, std::function<void()> callback)
{
	for (auto* cardView : _cardViews) {
		if (cardView && cardView->getCardId() == cardId) {
			cardView->playFlipAnimation(callback);
			break;
		}
	}
}

void DeckFieldView::removeCard(int cardId)
{
	for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it) {
		if (*it && (*it)->getCardId() == cardId) {
			(*it)->removeFromParent();
			_cardViews.erase(it);
			break;
		}
	}
}

void DeckFieldView::addCard(CardModel* card)
{
	if (card) {
		CardView* cardView = createCardView(card);
		if (cardView) {
			addChild(cardView);
			_cardViews.push_back(cardView);
			updateCardPositions();
		}
	}
}

CardView* DeckFieldView::createCardView(CardModel* card)
{
	if (!card) {
		return nullptr;
	}

	CardView* cardView = CardView::create(card, [this](int cardId) {
		onCardClicked(cardId);
	});

	return cardView;
}

void DeckFieldView::onCardClicked(int cardId)
{
	if (_cardClickCallback) {
		_cardClickCallback(cardId);
	}
}

void DeckFieldView::updateCardPositions()
{
	const auto& cards = _gameModel->getDeckFieldCards();
	for (size_t i = 0; i < cards.size() && i < _cardViews.size(); ++i) {
		if (cards[i] && _cardViews[i]) {
			_cardViews[i]->setPosition(cards[i]->getPosition()-cocos2d::Vec2(80,-300));
		}
	}
}