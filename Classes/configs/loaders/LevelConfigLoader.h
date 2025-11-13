#pragma once

#include "cocos2d.h"
#include "../models/LevelConfig.h"
#include <string>

/**
 * @class LevelConfigLoader
 * @brief 关卡配置加载器 —— 负责从 JSON 数据源（资源文件或路径）解析并构建 LevelConfig 实例。
 * 
 * @details
 * 本类采用静态工厂模式，封装了关卡配置的反序列化逻辑，解耦数据源与业务层。
 * 支持从预定义资源 ID 或任意本地文件路径加载配置，便于：
 *   - 游戏启动/关卡切换时动态加载；
 *   - 编辑器工具链中实时预览配置；
 *   - 自动化测试时注入 mock 配置文件。
 * 
 * @note 为提升鲁棒性，所有失败路径均返回 nullptr，调用方需显式判空。
 * @warning 非线程安全，建议在主线程或加载阶段单线程调用。
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 通过关卡 ID 从内置资源加载关卡配置。
     * 
     * @param levelId 关卡唯一标识符（如 "level_01", "tutorial"），对应 resources/config/ 下的 level_{id}.json。
     * @return LevelConfig* 成功时返回堆上分配的新建配置对象；失败（如文件缺失、JSON 格式错误、字段缺失）时返回 nullptr。
     * 
     * @note 内部调用 cocos2d::FileUtils::getInstance()->getStringFromFile() 读取资源。
     * @usage 适用于游戏运行时标准关卡加载流程。
     */
    static LevelConfig* loadLevelConfig(const std::string& levelId);

    /**
     * @brief 通过显式文件路径加载关卡配置（支持绝对/相对路径）。
     * 
     * @param filePath JSON 配置文件的完整路径（如 "custom_levels/test.json"）。
     * @return LevelConfig* 成功时返回新建配置对象；失败时返回 nullptr。
     * 
     * @note 不依赖资源 ID 命名约定，灵活性高。
     * @usage 适用于：
     *   - 关卡编辑器导出/导入；
     *   - 用户自定义关卡加载；
     *   - 单元测试中指定测试用例文件。
     */
    static LevelConfig* loadLevelConfigFromFile(const std::string& filePath);

private:
    /**
     * @brief 将原始 JSON 字符串解析为 LevelConfig 对象。
     * 
     * @param jsonData 合法的 JSON 格式字符串（需符合 LevelConfig 的 schema）。
     * @return LevelConfig* 解析成功返回对象；若 JSON 无效、必填字段缺失或类型不匹配，返回 nullptr。
     * 
     * @details 本函数为核心解析逻辑，独立于 I/O 层，便于：
     *   - 单元测试（直接传入 JSON 字符串）；
     *   - 扩展支持网络/加密配置源（只需将数据解密后传入）；
     *   - 与序列化模块对称设计（未来可添加 toJson()）。
     * 
     * @note 内部使用 rapidjson 或 cocos2d 自带 JSON 解析器（视项目配置而定）。
     * @invariant 调用者需确保 jsonData 非空且为 UTF-8 编码。
     */
    static LevelConfig* parseJsonToLevelConfig(const std::string& jsonData);
};
