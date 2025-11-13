#include "CardResConfig.h"

cocos2d::Sprite* CardResConfig::createCardSprite(const CardModel* cardModel)
{
	if (!cardModel) return  nullptr;

	// 使用卡牌背景
	auto _cardSprite = cocos2d::Sprite::create("card_general.png");
	if (_cardSprite) {
		_cardSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		_cardSprite->setPosition(cocos2d::Vec2(91, 141)); // 卡牌中心位置

		// 添加数字图片
		_cardSprite->addChild(createSmallNumberSprite(cardModel));
		_cardSprite->addChild(createBigNumberSprite(cardModel));
		// 添加花色图片
		_cardSprite->addChild(createSuitSprite(cardModel));
	}

	return _cardSprite;
}

cocos2d::Sprite* CardResConfig::createSmallNumberSprite(const CardModel* cardModel)
{
	if (!cardModel) return nullptr;

	// 根据卡牌面值选择数字图片
	std::string numberFile = getNumberImageFile(cardModel);

	if (!numberFile.empty()) {
		// 小的数字在左上角
		auto smallNumberSprite = cocos2d::Sprite::create(numberFile);
		if (smallNumberSprite) {
			smallNumberSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
			smallNumberSprite->setPosition(cocos2d::Vec2(45, 300)); // 左上角，稍微右移
			smallNumberSprite->setScale(0.4f);
			return smallNumberSprite;
		}
	}
}

cocos2d::Sprite* CardResConfig::createBigNumberSprite(const CardModel* cardModel)
{
	if (!cardModel) return nullptr;

	// 根据卡牌面值选择数字图片
	std::string numberFile = getNumberImageFile(cardModel);

	if (!numberFile.empty()) {

		// 大的数字在中间偏下
		auto bigNumberSprite = cocos2d::Sprite::create(numberFile);
		if (bigNumberSprite) {
			bigNumberSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
			bigNumberSprite->setPosition(cocos2d::Vec2(120, 130)); // 中间偏下，稍微上移
			bigNumberSprite->setScale(1.0f);
			return bigNumberSprite;
		}
	}
}

cocos2d::Sprite* CardResConfig::createSuitSprite(const CardModel* cardModel)
{
	if (!cardModel) return  nullptr;

	// 根据卡牌花色选择花色图片
	std::string suitFile = getSuitImageFile(cardModel);
	if (!suitFile.empty()) {
		// 花色在右上角
		auto suitSprite = cocos2d::Sprite::create(suitFile);
		if (suitSprite) {
			suitSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
			suitSprite->setPosition(cocos2d::Vec2(185, 300)); // 右上角，稍微左移
			suitSprite->setScale(0.6f);
			return suitSprite;
		}
	}
}

std::string CardResConfig::getNumberImageFile(const CardModel* cardModel)
{
	if (!cardModel) return "";

	CardFaceType face = cardModel->getFace();
	CardSuitType suit = cardModel->getSuit();

	// 判断是红色还是黑色
	bool isRed = (suit == CST_HEARTS || suit == CST_DIAMONDS);
	std::string colorPrefix = isRed ? "big_red_" : "big_black_";

	// 根据面值选择图片
	switch (face) {
	case CFT_ACE: return "number/" + colorPrefix + "A.png";
	case CFT_TWO: return "number/" + colorPrefix + "2.png";
	case CFT_THREE: return "number/" + colorPrefix + "3.png";
	case CFT_FOUR: return "number/" + colorPrefix + "4.png";
	case CFT_FIVE: return "number/" + colorPrefix + "5.png";
	case CFT_SIX: return "number/" + colorPrefix + "6.png";
	case CFT_SEVEN: return "number/" + colorPrefix + "7.png";
	case CFT_EIGHT: return "number/" + colorPrefix + "8.png";
	case CFT_NINE: return "number/" + colorPrefix + "9.png";
	case CFT_TEN: return "number/" + colorPrefix + "10.png";
	case CFT_JACK: return "number/" + colorPrefix + "J.png";
	case CFT_QUEEN: return "number/" + colorPrefix + "Q.png";
	case CFT_KING: return "number/" + colorPrefix + "K.png";
	default: return "";
	}
}

std::string CardResConfig::getSuitImageFile(const CardModel* cardModel)
{
	if (!cardModel) return "";

	CardSuitType suit = cardModel->getSuit();

	switch (suit) {
	case CST_CLUBS: return "suits/club.png";
	case CST_DIAMONDS: return "suits/diamond.png";
	case CST_HEARTS: return "suits/heart.png";
	case CST_SPADES: return "suits/spade.png";
	default: return "";
	}
}
