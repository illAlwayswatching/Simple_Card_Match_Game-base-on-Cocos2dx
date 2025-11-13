
#include "LevelConfigLoader.h"
#include "external/json/document.h"
#include "external/json/filereadstream.h"
#include <cstdio>

LevelConfig* LevelConfigLoader::loadLevelConfig(const std::string& levelId)
{
	std::string filePath = "levels/level" + levelId + ".json";
	return loadLevelConfigFromFile(filePath);
}

LevelConfig* LevelConfigLoader::loadLevelConfigFromFile(const std::string& filePath)
{
	// 获取文件完整路径
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);

	// 读取文件内容
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
	if (content.empty()) {
		cocos2d::log("LevelConfigLoader: Failed to read file %s", filePath.c_str());
		return nullptr;
	}

	return parseJsonToLevelConfig(content);
}

LevelConfig* LevelConfigLoader::parseJsonToLevelConfig(const std::string& jsonData)
{
	using namespace rapidjson;

	Document document;
	document.Parse(jsonData.c_str());

	if (document.HasParseError() || !document.IsObject()) {
		cocos2d::log("LevelConfigLoader: Failed to parse JSON data");
		return nullptr;
	}

	LevelConfig* config = new LevelConfig();

	// 解析主牌堆配置
	if (document.HasMember("Playfield") && document["Playfield"].IsArray()) {
		cocos2d::log("Parsing Succ1");
		const Value& mainFieldArray = document["Playfield"];
		for (auto it = mainFieldArray.Begin(); it != mainFieldArray.End(); ++it) {
			if (it->IsObject() &&
				it->HasMember("CardFace") && it->HasMember("CardSuit") && it->HasMember("Position")) {

				int cardFace = (*it)["CardFace"].GetInt();
				int cardSuit = (*it)["CardSuit"].GetInt();

				const Value& pos = (*it)["Position"];
				if (pos.IsObject() && pos.HasMember("x") && pos.HasMember("y")) {
					float x = pos["x"].GetFloat();
					float y = pos["y"].GetFloat();

					LevelConfig::CardConfig cardConfig(cardFace, cardSuit, cocos2d::Vec2(x, y));
					config->addMainFieldCard(cardConfig);
				}
			}
		}
	}

	// 解析底牌堆配置
	if (document.HasMember("Stack") && document["Stack"].IsArray()) {
		const Value& bottomFieldArray = document["Stack"];
		SizeType count = bottomFieldArray.Size();  // 获取数组元素个数
		SizeType index = 0;
		for (auto it = bottomFieldArray.Begin(); it != bottomFieldArray.End(); ++it,++index) {
			if (it->IsObject() &&
				it->HasMember("CardFace") && it->HasMember("CardSuit") && it->HasMember("Position")) {

				int cardFace = (*it)["CardFace"].GetInt();
				int cardSuit = (*it)["CardSuit"].GetInt();

				const Value& pos = (*it)["Position"];
				if (pos.IsObject() && pos.HasMember("x") && pos.HasMember("y")) {
					float x = pos["x"].GetFloat();
					float y = pos["y"].GetFloat();

					LevelConfig::CardConfig cardConfig(cardFace, cardSuit, cocos2d::Vec2(x, y));
					
					//关键：判断是否是最后一个元素
					if (index == count - 1) {
						config->addBottomFieldCard(cardConfig);   // 最后一个：Bottom
					}
					else {
						config->addBackFieldCard(cardConfig);     // 其余：Back
					}
				}
			}
		}
	}

	// 验证配置有效性
	if (!config->isValid()) {
		delete config;
		cocos2d::log("LevelConfigLoader: Invalid level configuration");
		return nullptr;
	}

	return config;
}
