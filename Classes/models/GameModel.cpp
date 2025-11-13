#include "GameModel.h"
#include <sstream>
#include <algorithm>

GameModel::GameModel()
	:  _gameState("playing")
	, _deckFieldCards()
	, _handsFieldCards()
	, _backupFieldCards()
{
}

GameModel::~GameModel()
{
	clearAllCards();
}

void GameModel::setDeckFieldCards(const std::vector<CardModel*>& cards)
{
	// 清理原有卡牌
	for (auto* card : _deckFieldCards) {
		delete card;
	}
	_deckFieldCards.clear();

	// 设置新卡牌
	_deckFieldCards = cards;
}

void GameModel::setHandsFieldCards(const std::vector<CardModel*>& cards)
{
	// 清理原有卡牌
	while (!_handsFieldCards.empty()) {
		CardModel* card = _handsFieldCards.top();
		_handsFieldCards.pop();
		delete card;  
	}

	for (auto card : cards) {
		_handsFieldCards.push(card);
	}

}

void GameModel::setBackupFieldCards(const std::vector<CardModel*>& cards)
{
	// 清理 _backupFieldCards
	for (auto* card : _backupFieldCards) {
		delete card;
	}
	_backupFieldCards.clear();

	for (auto card : cards) {
		_backupFieldCards.push_back(card);
	}

}

void GameModel::addDeckFieldCard(CardModel* card)
{
	if (card) {
		_deckFieldCards.push_back(card);
	}
}

void GameModel::addHandsFieldCard(CardModel* card)
{
	if (card) {
		_handsFieldCards.push(card);
	}
}

void GameModel::addBackupFieldCard(CardModel* card)
{
	if (card) {
		_backupFieldCards.push_back(card);
	}
}

bool GameModel::removeDeckFieldCard(int cardId)
{
	auto it = std::find_if(_deckFieldCards.begin(), _deckFieldCards.end(),
		[cardId](CardModel* card) { return card && card->getCardId() == cardId; });

	if (it != _deckFieldCards.end()) {
		// 不要删除卡牌对象，只是从列表中移除
		// 卡牌对象会被移动到其他牌堆中
		_deckFieldCards.erase(it);
		return true;
	}
	return false;
}

bool GameModel::removeHandsFieldCard()
{
	if (!_handsFieldCards.empty()) {
		_handsFieldCards.pop();
		return true;
	}
	return false;
}

bool GameModel::removeBackupFieldCard()
{
	if (!_backupFieldCards.empty()) {
		_backupFieldCards.pop_back();
		return true;
	}
	return false;
}

CardModel* GameModel::findDeckFieldCard(int cardId) const
{
	auto it = std::find_if(_deckFieldCards.begin(), _deckFieldCards.end(),
		[cardId](CardModel* card) { return card && card->getCardId() == cardId; });

	return (it != _deckFieldCards.end()) ? *it : nullptr;
}

void GameModel::clearAllCards()
{
	for (auto* card : _deckFieldCards) {
		delete card;
	}
	_deckFieldCards.clear();

	// 清理 _handsFieldCards
	while (!_handsFieldCards.empty()) {
		CardModel* card = _handsFieldCards.top();
		_handsFieldCards.pop();
		if (card) {
			delete card;
		}
	}

	// 清理 _backupFieldCards
	for (auto* card : _backupFieldCards) {
		delete card;
	}
	_backupFieldCards.clear();


}

bool GameModel::isGameOver() const
{
	// 简单判断：主牌堆无卡牌或底牌堆无卡牌
	return _deckFieldCards.empty() || _handsFieldCards.empty();
}
