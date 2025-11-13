#include "GameController.h"
USING_NS_CC;

GameController::GameController()
	: _parent(nullptr)
	, _gameModel(nullptr)
	, _gameView(nullptr)
	, _undoManager(nullptr)
	, _deckFieldController(nullptr)
	, _bottomFieldController(nullptr)
{
}

GameController::~GameController()
{
	if (_gameModel) {
		delete _gameModel;
		_gameModel = nullptr;
	}

	if (_undoManager) {
		delete _undoManager;
		_undoManager = nullptr;
	}

	if (_deckFieldController) {
		delete _deckFieldController;
		_deckFieldController = nullptr;
	}

	if (_bottomFieldController) {
		delete _bottomFieldController;
		_bottomFieldController = nullptr;
	}
}

bool GameController::startGame(const std::string& levelId)
{
	// 加载关卡配置
	LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
	if (!levelConfig) {
		cocos2d::log("GameController: Failed to load level config for level %s", levelId.c_str());
		return false;
	}

	// 生成游戏模型
	_gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
	if (!_gameModel) {
		cocos2d::log("GameController: Failed to generate game model");
		delete levelConfig;
		return false;
	}

	// 初始化撤销管理器
	_undoManager->init(_gameModel, [this](bool success) {
		onUndoComplete(success);
	});

	// 初始化子控制器
	_deckFieldController->init(_gameModel, _undoManager, [this]() {
		onDeckFieldCardClicked();
	});
	_bottomFieldController->init(_gameModel, _undoManager
	);

	//将子视图交给主视图
	auto _deckFieldView = _deckFieldController->getDeckFieldView();
	auto _bottomFieldView = _bottomFieldController->getStackView();

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	cocos2d::Vec2 mainFieldPos = origin + cocos2d::Vec2(540, 1330);

	_deckFieldView->setContentSize(visibleSize);        // 关键！赋予尺寸
	_deckFieldView->setPosition(Vec2::ZERO);         // 改为左下角
	_gameView->addChild(_deckFieldView, 1);

	_bottomFieldView->setContentSize(visibleSize);        // 关键！赋予尺寸
	_bottomFieldView->setPosition(Vec2::ZERO);
	_gameView->addChild(_bottomFieldView, 2);

	// 更新游戏视图
	updateGameView();

	delete levelConfig;
	return true;
}

bool GameController::handleUndo()
{
	if (!_undoManager) {
		return false;
	}

	return _undoManager->executeUndo();
}

bool GameController::init(cocos2d::Node* parent) {
	if (!parent) {
		return false;
	}

	_parent = parent;
	
	// 初始化撤销管理器
	_undoManager = new UndoManager();

	// 初始化游戏视图
	if (!initGameView()) {
		return false;
	}

	// 初始化子控制器
	if (!createSubControllers()) {
		return false;
	}

	return true;
}

bool GameController::createSubControllers()
{
	_deckFieldController = new DeckFieldController();
	_bottomFieldController = new BottomFieldController();

	return _deckFieldController != nullptr && _bottomFieldController != nullptr;
}

bool GameController::initGameView()
{
	_gameView = GameView::create(nullptr, [this]() {
		handleUndo();
	});

	if (!_gameView) {
		return false;
	}


	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	_gameView->setContentSize(visibleSize);        // 关键！赋予尺寸
	_gameView->setAnchorPoint(Vec2::ZERO);         // 改为左下角
	_gameView->setPosition(origin);                // 对齐屏幕左下
	_parent->addChild(_gameView);
	
	return true;
}

void GameController::onUndoComplete(bool success)
{
	if (success) {
		updateGameView();
	}
}

void GameController::updateGameView()
{	
	_deckFieldController->getDeckFieldView()->updateDisplay();
	_bottomFieldController->getStackView()->updateDisplay();
}

void GameController::onDeckFieldCardClicked() {
	_bottomFieldController->getStackView()->updateDisplay();
	cocos2d::log("bottomField Updated");
}