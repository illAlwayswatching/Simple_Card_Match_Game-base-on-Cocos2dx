
#include "LevelConfig.h"

LevelConfig::LevelConfig()
{
}

LevelConfig::~LevelConfig()
{
}

void LevelConfig::clear()
{
	_mainFieldCards.clear();
	_bottomFieldCards.clear();
	_backFieldCards.clear();
}

bool LevelConfig::isValid() const
{
	// 检查是否有卡牌配置
	if (_mainFieldCards.empty() && _bottomFieldCards.empty() && _backFieldCards.empty()) {
		return false;
	}

	// 检查主牌堆卡牌配置的有效性
	for (const auto& card : _mainFieldCards) {
		if (card.cardFace < 0 || card.cardFace > 12 ||
			card.cardSuit < 0 || card.cardSuit > 3) {
			return false;
		}
	}

	// 检查底牌堆卡牌配置的有效性
	for (const auto& card : _bottomFieldCards) {
		if (card.cardFace < 0 || card.cardFace > 12 ||
			card.cardSuit < 0 || card.cardSuit > 3) {
			return false;
		}
	}

	// 检查备用牌堆卡牌配置的有效性
	for (const auto& card : _backFieldCards) {
		if (card.cardFace < 0 || card.cardFace > 12 ||
			card.cardSuit < 0 || card.cardSuit > 3) {
			return false;
		}
	}

	return true;
}
