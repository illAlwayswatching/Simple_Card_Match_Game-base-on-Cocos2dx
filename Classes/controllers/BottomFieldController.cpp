#include "BottomFieldController.h"
#include "../services/BackupService.h"

BottomFieldController::BottomFieldController()
	: _gameModel(nullptr)
	, _stackView(nullptr)
	, _undoManager(nullptr)
{
}

BottomFieldController::~BottomFieldController()
{
}

bool BottomFieldController::init(GameModel* gameModel, UndoManager* undoManager)
{
	if (!gameModel || !undoManager) {
		return false;
	}

	_gameModel = gameModel;
	_stackView = BottomFieldView::create(_gameModel, [this](int cardId)
	{handleCardClick(cardId); }
	);
	_undoManager = undoManager;
	return true;
}

bool BottomFieldController::handleCardClick(int cardId) {
	if (!_gameModel || !_undoManager) {
		return false;
	}

	// 检查是否还有备用牌
	if (!BackupService::isBackup(_gameModel)) {
		return false;
	}

	// 执行移动操作
	if (!BackupService::executeMoveToHand(cardId, _gameModel, _undoManager)) {
		return false;
	}

	//// 重新更新卡牌的正面朝上情况
	//if (!MatchService::updateCardFaceUp(_gameModel)) {
	//	return false;
	//}
	return updateBottomFieldView();
	
}

bool BottomFieldController::updateBottomFieldView() {
	_stackView->updateDisplay();
	return true;
}