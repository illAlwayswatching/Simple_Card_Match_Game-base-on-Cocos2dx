
#include "GameModelFromLevelGenerator.h"
#include <random>
#include <algorithm>

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig)
{
	return generateGameModel(levelConfig, false);
}

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig, bool randomize)
{
	if (!levelConfig || !levelConfig->isValid()) {
		return nullptr;
	}

	GameModel* gameModel = new GameModel();

	// 生成主牌堆卡牌
	const auto& mainFieldCards = levelConfig->getMainFieldCards();
	for (const auto& cardConfig : mainFieldCards) {
		CardModel* cardModel = createCardModel(cardConfig, "MainField");
		if (cardModel) {
			gameModel->addDeckFieldCard(cardModel);
		}
	}

	// 生成底牌堆卡牌
	const auto& bottomFieldCards = levelConfig->getBottomFieldCards();
	for (const auto& cardConfig : bottomFieldCards) {
		CardModel* cardModel = createCardModel(cardConfig, "BottomField");
		if (cardModel) {
			gameModel->addHandsFieldCard(cardModel);
		}
	}

	// 生成备用牌堆卡牌
	const auto& backFieldCards = levelConfig->getBackFieldCards();
	for (const auto& cardConfig : backFieldCards) {
		CardModel* cardModel = createCardModel(cardConfig, "BackField");
		if (cardModel) {
			gameModel->addBackupFieldCard(cardModel);
		}
	}

	return gameModel;
}

CardModel* GameModelFromLevelGenerator::createCardModel(const LevelConfig::CardConfig& cardConfig, const std::string& FieldType)
{
	CardFaceType face = (CardFaceType)cardConfig.cardFace;
	CardSuitType suit = (CardSuitType)cardConfig.cardSuit;
	cocos2d::Vec2 position = cardConfig.position;

	CardModel* card = new CardModel(face, suit, position);

	// 根据牌区类型设置卡牌状态
	if (FieldType == "MainField") {
		// 主牌区：所有牌都翻开且可点击
		card->setFaceUp(true);
	}
	else if (FieldType == "BottomField") {
		// 底牌区：翻开但不可点击（作为匹配目标）
		card->setFaceUp(true);
	}
	else if (FieldType == "BackField") {
		// 备用牌区：所有牌都翻开且可点击
		card->setFaceUp(false);
	}

	return card;
}

