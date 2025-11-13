#include "GameView.h"
USING_NS_CC;

GameView::GameView()
{
}

GameView::~GameView()
{
}

GameView* GameView::create(const GameModel* gameModel)
{
	return create(gameModel, nullptr);
}

GameView* GameView::create(const GameModel* gameModel, UndoButtonCallback undoCallback)
{
	GameView* ret = new GameView();
	if (ret && ret->init(gameModel, undoCallback)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool GameView::init(const GameModel* gameModel, UndoButtonCallback undoCallback)
{
	if (!Node::init()) {
		return false;
	}

	_gameModel = gameModel;
	_undoCallback = undoCallback;
	_deckFieldView = nullptr;
	_bottomFieldView = nullptr;
	_undoButton = nullptr;
	_cardClickCallback = nullptr;
	_drawCardCallback = nullptr;

	createUndoButton();
	setupLayout();
	updateGame();


	return true;
}

void GameView::setDrawCardCallback(DrawCardCallback callback)
{
	_drawCardCallback = callback;
}

void GameView::updateGame()
{
	updateDisplay();
}


void GameView::updateDisplay()
{
	if (_deckFieldView) {
		_deckFieldView->updateDisplay();
	}
	if (_bottomFieldView) {
		_bottomFieldView->updateDisplay();
	}
}


void GameView::playEnterAnimation(std::function<void()> callback)
{
	setScale(0.0f);
	auto scaleAction = cocos2d::ScaleTo::create(0.3f, 1.0f);
	auto easeAction = cocos2d::EaseBackOut::create(scaleAction);

	if (callback) {
		auto sequence = cocos2d::Sequence::create(easeAction,
			cocos2d::CallFunc::create(callback),
			nullptr);
		runAction(sequence);
	}
	else {
		runAction(easeAction);
	}
}

void GameView::playExitAnimation(std::function<void()> callback)
{
	auto scaleAction = cocos2d::ScaleTo::create(0.3f, 0.0f);
	auto easeAction = cocos2d::EaseBackIn::create(scaleAction);

	if (callback) {
		auto sequence = cocos2d::Sequence::create(easeAction,
			cocos2d::CallFunc::create(callback),
			nullptr);
		runAction(sequence);
	}
	else {
		runAction(easeAction);
	}
}

void GameView::createUndoButton()
{
	_undoButton = cocos2d::ui::Button::create("", "", "");
	_undoButton->setTitleFontName("fonts/arial.ttf"); // 路径相对 Resources
	_undoButton->setTitleText("UNDO");
	_undoButton->setTitleFontSize(36);
	_undoButton->setTitleColor(cocos2d::Color3B::WHITE);
	_undoButton->setColor(cocos2d::Color3B(100, 100, 100));
	_undoButton->setPosition(cocos2d::Vec2(950,650));
	_undoButton->addTouchEventListener(CC_CALLBACK_2(GameView::onUndoButtonClicked, this));
	this->addChild(_undoButton, 10);
}

void GameView::setupLayout()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::Vec2 mainFieldPos = origin + cocos2d::Vec2(540, 1330);

	// 添加分隔线
	auto drawNode = cocos2d::DrawNode::create();
	cocos2d::Vec2 start(origin.x, origin.y + visibleSize.height - 1500);
	cocos2d::Vec2 end(origin.x + visibleSize.width, origin.y + visibleSize.height - 1500);
	drawNode->drawLine(start, end, cocos2d::Color4F::GRAY);
	addChild(drawNode, 0);
}

void GameView::onUndoButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
		if (_undoCallback) {
			_undoCallback();
		}
	}
}