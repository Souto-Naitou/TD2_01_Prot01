#include <ImGuiDebugManager/DebugManager.h>

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx12.h>
#endif // _DEBUG

DebugManager::DebugManager()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.1f, 0.1f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.0f, 0.90f));
}

DebugManager::~DebugManager()
{
	
}

void DebugManager::DeleteComponent(const char* _strID)
{
	componentList_.remove_if([_strID](std::pair<std::string, std::function<void(void)>> _component)
		{
			return _component.first.compare(_strID) == 0;
		}
	);
}

void DebugManager::DrawUI()
{
#ifdef _DEBUG
	// 登録されていないなら早期リターン
	if (componentList_.size() == 0) return;

	ImGui::PushID("DEBUG_MANAGER");

	// デバッグウィンドウ描画
	ImGui::Begin("デバッグ");

	ImGui::BeginTabBar("## TABBAR");
	for (auto& component : componentList_)
	{
		if (ImGui::BeginTabItem(component.first.c_str()))
		{
			std::string id_str = component.first + "TABITEM";
			ImGui::PushID(id_str.c_str());
			component.second();
			ImGui::PopID();
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();


	ImGui::End();
	

	ImGui::PopID();
#endif // _DEBUG
}

void DebugManager::ChangeFont()
{
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig fontcfg;
	fontcfg.MergeMode = 0;
	fontcfg.OversampleH = 1;
	fontcfg.PixelSnapH = 1;
	fontcfg.GlyphOffset = ImVec2(0.0f, 0.0f);

	ImFont* resultFont = io.Fonts->AddFontFromFileTTF(
		"Resources/Font/NotoSansCJKjp-Light.ttf",
		13,
		&fontcfg,
		io.Fonts->GetGlyphRangesJapanese()
	);

	io.FontDefault = resultFont;

	io.Fonts->Build();
	ImGui_ImplDX12_CreateDeviceObjects();
}
