#include "UndoManager.h"
#include "../services/UndoService.h"

UndoManager::UndoManager()
	: _undoModel(nullptr)
	, _gameModel(nullptr)
	, _undoCompleteCallback(nullptr)
{
}

UndoManager::~UndoManager()
{
	if (_undoModel) {
		delete _undoModel;
		_undoModel = nullptr;
	}
}

void UndoManager::init(GameModel* gameModel, UndoCompleteCallback callback)
{
	_gameModel = gameModel;
	_undoCompleteCallback = callback;

	if (!_undoModel) {
		_undoModel = new UndoModel();
	}
}

void UndoManager::addUndoRecord(const UndoModel::UndoRecord& record)
{
	if (_undoModel) {
		_undoModel->addUndoRecord(record);
	}
}

bool UndoManager::executeUndo()
{
	if (!_undoModel || !_gameModel) {
		_notifyUndoComplete(false);
		return false;
	}

	if (!_undoModel->hasUndoableAction()) {
		_notifyUndoComplete(false);
		return false;
	}

	UndoModel::UndoRecord record = _undoModel->getLastUndoRecord();
	_undoModel->removeLastUndoRecord();

	bool success = UndoService::executeUndo(_gameModel, record);
	_notifyUndoComplete(success);
	return success;
}

void UndoManager::_notifyUndoComplete(bool success)
{
	if (_undoCompleteCallback) {
		_undoCompleteCallback(success);
	}
}
