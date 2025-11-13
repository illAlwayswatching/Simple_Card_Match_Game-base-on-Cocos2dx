#include "UndoService.h"
#include <algorithm>

UndoModel::UndoRecord UndoService::createHandSwapRecord(const GameModel* gameModel)
{
	UndoModel::UndoRecord record;
	record.actionType = UAT_HAND_SWAP;

	if (gameModel) {
		CardModel* fromCard = gameModel->getCurrentBackup();
		CardModel* toCard = gameModel->getCurrentHands();

		record.sourceCardId = fromCard->getCardId();
		record.targetCardId = toCard->getCardId();

		if (fromCard) {
			record.sourcePosition = fromCard->getPosition();
		}
		if (toCard) {
			record.targetPosition = toCard->getPosition();
		}
	}

	return record;
}

UndoModel::UndoRecord UndoService::createDeckFieldToHandRecord(const GameModel* gameModel,
	int deckFieldCardId)
{
	UndoModel::UndoRecord record;
	record.actionType = UAT_PLAYFIELD_TO_HAND;
	record.sourceCardId = deckFieldCardId;

	if (gameModel) {
		CardModel* deckFieldCard = gameModel->findDeckFieldCard(deckFieldCardId);
		CardModel* stackCard = gameModel->getCurrentHands();
		record.targetCardId = stackCard->getCardId();

		if (deckFieldCard) {
			record.sourcePosition = deckFieldCard->getPosition();
		}
		if (stackCard) {
			record.targetPosition = stackCard->getPosition();
		}

		record.deckFieldIndex = UndoService::findPlayfieldCardIndex(gameModel, deckFieldCardId);
	}

	return record;
}

bool UndoService::executeUndo(GameModel* gameModel, const UndoModel::UndoRecord& record)
{
	if (!gameModel) {
		return false;
	}

	switch (record.actionType) {
	case UAT_HAND_SWAP: {
		CardModel* cardForRecover = gameModel->getCurrentHands();
		if (cardForRecover) {
			cardForRecover->setPosition(record.sourcePosition);
			gameModel->removeHandsFieldCard();
			gameModel->addBackupFieldCard(cardForRecover);
			return true;
		}
		break;
	}

	case UAT_PLAYFIELD_TO_HAND: {
		CardModel* cardForRecover = gameModel->getCurrentHands();

		if (cardForRecover) {
			// »Ö¸´Î»ÖÃ
			cardForRecover->setPosition(record.sourcePosition);
			gameModel->removeHandsFieldCard();
			gameModel->addDeckFieldCard(cardForRecover);
			return true;
		}
		break;
	}

	default:
		return false;
	}

	return false;
}

int UndoService::findPlayfieldCardIndex(const GameModel* gameModel, int deckFieldCardId) {
	const auto deckFieldCards = gameModel->getDeckFieldCards();
	int index = 0;
	for (auto card : deckFieldCards) {
		if (card->getCardId() == deckFieldCardId) {
			return index;
		}
		index++;
	}
	return -1;
}