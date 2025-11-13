#include "MatchService.h"
#include "UndoService.h"

bool MatchService::MatchMoveInData(CardModel* deckCard, CardModel* handsCard, GameModel* gameModel) {
	if (!deckCard || !handsCard) {
		return false;
	}

	deckCard->setPosition( handsCard->getPosition());
	gameModel->removeDeckFieldCard(deckCard->getCardId());
	gameModel->addHandsFieldCard(deckCard);

	return true;
}

bool MatchService::isMatch(CardModel* card1, CardModel* card2)
{
	if (!card1->isValid() || !card2->isValid()) {
		return false;
	}

	int faceDiff = std::abs((int)card1->getFace() - (int)card2->getFace());
	return faceDiff == 1;
}

bool MatchService::canMoveToHand(int deckfieldCardId, GameModel* _gameModel)
{
	if (!_gameModel) {
		return false;
	}

	// 获取桌面卡牌
	CardModel* playfieldCard = _gameModel->findDeckFieldCard(deckfieldCardId);
	if (!playfieldCard) {
		return false;
	}

	// 获取手牌区顶部卡牌
	CardModel* handTopCard = _gameModel->getCurrentHands();
	if (!handTopCard) {
		return false;
	}

	// 检查面值是否相邻（差1）
	return MatchService::isMatch(playfieldCard, handTopCard);
}

bool MatchService::executeMoveToHand(int playfieldCardId, GameModel* _gameModel, UndoManager* _undoManager)
{
	if (!_gameModel || !_undoManager) {
		return false;
	}

	// 获取桌面卡牌和手牌区顶部卡牌
	CardModel* playfieldCard = _gameModel->findDeckFieldCard(playfieldCardId);
	CardModel* handTopCard = _gameModel->getCurrentHands();

	if (!playfieldCard || !handTopCard) {
		return false;
	}

	// 创建撤销记录
	UndoModel::UndoRecord undoRecord = UndoService::createDeckFieldToHandRecord(
		_gameModel, playfieldCardId);

	// 添加撤销记录
	_undoManager->addUndoRecord(undoRecord);

	// 执行移动
	return MatchService::MatchMoveInData(playfieldCard, handTopCard, _gameModel);
}

bool MatchService::updateCardFaceUp(GameModel* _gameModel) {
	if (!_gameModel) {
		return false;
	}

	//获取现在的桌面牌区情况
	std::vector<CardModel*> deckFieldCards = _gameModel->getDeckFieldCards();

	//进行faceUp判断


	return true;
}