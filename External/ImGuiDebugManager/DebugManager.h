#pragma once

#include <functional>
#include <list>
#include <string>
#include <tuple>

#include "ImGuiTemplates.h"

class DebugManager
{
public:
    static DebugManager* GetInstance() { static DebugManager instance; return &instance; }

    DebugManager(const DebugManager&)               = delete;
    DebugManager& operator=(const DebugManager&)    = delete;
    DebugManager(DebugManager&&)                    = delete;
    DebugManager& operator=(const DebugManager&&)   = delete;

    /// <summary>
    /// デバッグ用コンポーネントの登録
    /// </summary>
    /// <param name="_strID">タブに表示される名前</param>
    /// <param name="_component">関数ポインタ。std::bindを使用することがほとんど</param>
    void SetComponent(std::string _strID, const std::function<void(void)>& _component)
    {
        componentList_.push_back(std::make_tuple(std::string("null-name"), _strID, _component, false));
    }

    /// <summary>
    /// デバッグ用コンポーネントの登録 (リスト用)
    /// </summary>
    /// <param name="_parentID">オブジェクトの種類</param>
    /// <param name="_childID">オブジェクトの名前</param>
    /// <param name="_component">関数ポインタ。std::bindを使用することがほとんど</param>
    void SetComponent(std::string _parentID, std::string _childID, const std::function<void(void)>& _component)
    {
        componentList_.emplace(
            GetInsertIterator(_parentID),
            _parentID,
            _childID,
            _component,
            false
        );
    }

    void DeleteComponent(const char* _strID);
    void DeleteComponent(const char* _parentID, const char* _childID);

    void DrawUI();
    void ChangeFont();

private:
    DebugManager();
    ~DebugManager();

    std::list<std::tuple<std::string, std::string, const std::function<void(void)>, bool>> componentList_;

private:
    void Window_ObjectList();
    std::list<std::tuple<std::string,std::string,const std::function<void(void)>,bool>>::iterator
        GetInsertIterator(std::string _parentName);
};