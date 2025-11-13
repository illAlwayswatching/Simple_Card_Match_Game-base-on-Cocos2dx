#pragma once

#include "UndoModel.h"
#include <sstream>

UndoModel::UndoModel()
{
}

UndoModel::~UndoModel()
{
}

void UndoModel::addUndoRecord(const UndoRecord& record)
{
	_undoRecords.push_back(record);
}

UndoModel::UndoRecord UndoModel::getLastUndoRecord()
{
	if (_undoRecords.empty()) {
		return UndoRecord();
	}
	return _undoRecords.back();
}

void UndoModel::removeLastUndoRecord()
{
	if (!_undoRecords.empty()) {
		_undoRecords.pop_back();
	}
}

bool UndoModel::hasUndoableAction() const
{
	return !_undoRecords.empty();
}

void UndoModel::clearAllRecords()
{
	_undoRecords.clear();
}

