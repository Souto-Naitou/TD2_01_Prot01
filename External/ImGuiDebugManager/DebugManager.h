#pragma once

#include <functional>
#include <utility>
#include <list>
#include <string>

#include "ImGuiTemplates.h"

class DebugManager
{
public:
    static DebugManager* GetInstance() { static DebugManager instance; return &instance; }

    DebugManager(const DebugManager&)               = delete;
    DebugManager& operator=(const DebugManager&)    = delete;
    DebugManager(DebugManager&&)                    = delete;
    DebugManager& operator=(const DebugManager&&)   = delete;

    void SetComponent(const char* _strID, const std::function<void(void)>& _component) { componentList_.push_back({ _strID, _component }); }
    void DeleteComponent(const char* _strID);

    void DrawUI();
    void ChangeFont();

private:
    DebugManager();
    ~DebugManager();

    std::list<std::pair<std::string, std::function<void(void)>>> componentList_;
};