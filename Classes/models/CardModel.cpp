
#include "CardModel.h"
#include <sstream>
#include <algorithm>

int refCardId = 0;

CardModel::CardModel()
	: _cardId(-1)
	, _face(CFT_NONE)
	, _suit(CST_NONE)
	, _position(cocos2d::Vec2::ZERO)
	, _isFaceUp(false)
{
}

CardModel::CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)
	: _cardId(++refCardId)
	, _face(face)
	, _suit(suit)
	, _position(position)
	, _isFaceUp(false)
{
}

CardModel::~CardModel()
{
}

bool CardModel::isValid() const
{
	return _face != CFT_NONE && _suit != CST_NONE;
}

