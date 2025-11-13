#include "BackupService.h"
#include "UndoService.h"

bool BackupService::isBackup(GameModel* gameModel) {
	if (!gameModel) {
		return false;
	}

	int size = gameModel->getBackupFieldCardsSize();

	if (size == 0) {
		return false;
	}

	return true;
}

bool  BackupService::executeMoveToHand(int cardId, GameModel* gameModel, UndoManager* undoManager) {
	if (!gameModel || !undoManager) {
		return false;
	}

	UndoModel::UndoRecord undoRecord = UndoService::createHandSwapRecord(gameModel);

	undoManager->addUndoRecord(undoRecord);

	return MoveInDataModels(gameModel);
}

bool BackupService::MoveInDataModels(GameModel* gameModel) {
	auto backupCard = gameModel->getCurrentBackup();
	if (!backupCard) {
		return false;
	}

	gameModel->removeBackupFieldCard();
	gameModel->addHandsFieldCard(backupCard);

	return true;
}
