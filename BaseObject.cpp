#include "BaseObject.h"

#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"

void BaseObject::DebugWindow()
{
    /// DebugWindowがオーバーライドされていないときの
    /// デフォルトの動作を記述

    DebugManager::GetInstance()->SetComponent(objectID_.c_str(), std::bind(&BaseObject::DebugWindow, this));

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("position_", position_);
        ImGuiTemplate::VariableTableRow("hp_", hp_);
    };

    ImGuiTemplate::VariableTable(objectID_, pFunc);

}
