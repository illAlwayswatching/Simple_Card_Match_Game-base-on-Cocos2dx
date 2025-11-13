#include "DeckFieldController.h"
#include "../utils/CardUtils.h"
#include "../services/UndoService.h"
#include "../services/MatchService.h"

DeckFieldController::DeckFieldController()
	: _gameModel(nullptr)
	, _deckFieldView(nullptr)
	, _cardClickCallback(nullptr)
	, _undoManager(nullptr)
{
}

DeckFieldController::~DeckFieldController()
{
}


bool DeckFieldController::init(GameModel* gameModel, UndoManager* undoManager, CardClickCallback cardClickCallback) {
	if (!gameModel || !undoManager) {
		return false;
	}

	_gameModel = gameModel;
	_deckFieldView = DeckFieldView::create(_gameModel, [this](int cardId)
		{handleCardClick(cardId);}
		);
	_undoManager = undoManager;
	_cardClickCallback = std::move(cardClickCallback);
	return true;
}


bool DeckFieldController::handleCardClick(int cardId) {
	if (!_gameModel || !_undoManager) {
		return false;
	}

	// 检查是否可以移动到手牌区
	if (!MatchService::canMoveToHand(cardId, _gameModel)) {
		return false;
	}

	// 执行移动操作
	if (!MatchService::executeMoveToHand(cardId, _gameModel, _undoManager)) {
		return false;
	}

	// 重新更新卡牌的正面朝上情况
	if (!MatchService::updateCardFaceUp(_gameModel)) {
		return false;
	}

	if (_cardClickCallback) {
		_cardClickCallback();  // ← 向上冒泡事件
	}
	_deckFieldView->playCardMoveAnimation(cardId, cocos2d::Vec2(30, 30),2.0,nullptr);
	return updateDeckFieldView();
}

bool DeckFieldController::updateDeckFieldView() {
    
	_deckFieldView->updateDisplay();
	return true;
}
