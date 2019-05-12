#include "Hooks.h"
#include "../Directx/Directx.h"

Hooks    g_Hooks;
bool firstTime = true;
void Hooks::Init()
{
	logF("Setting up Hooks...");
	// GameMode::tick Signature
	// CC 8B 41 ?? 89 41 ?? C3
	//    ^^ Function starts here
	void* func = reinterpret_cast<void*>(Utils::FindSignature("CC 8B 41 ?? 89 41 ?? C3") + 1);
	g_Hooks.gameMode_tickHook = std::make_unique<FuncHook>(func, Hooks::GameMode_tick);
	g_Hooks.gameMode_tickHook->init();

	// SurvivalMode::tick Sig
	// 48 8B C4 55 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 10 48 89 70 18 48 89 78 20 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B F9 8B 41 ??
	void* surv_tick = reinterpret_cast<void*>(Utils::FindSignature("48 8B C4 55 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 10 48 89 70 18 48 89 78 20 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B F9 8B 41 ??"));
	g_Hooks.survivalMode_tickHook = std::make_unique<FuncHook>(surv_tick, Hooks::SurvivalMode_tick);
	g_Hooks.survivalMode_tickHook->init();

	// ChatScreenController::_sendChatMessage
	// 40 57 48 83 EC 20 48 83 B9 ?? ?? ?? ?? 00 48 8B F9 0F 85
	// ^^ 
	void* _sendChatMessage = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 20 48 83 B9 ?? ?? ?? ?? 00 48 8B F9 0F 85"));
	g_Hooks.chatScreen_sendMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::ChatScreenController_sendChatMessage);
	g_Hooks.chatScreen_sendMessageHook->init();

	//IDXGISwapChain::present;
	// using vtable found with dummy thing
	void** swapChainVtable = static_cast<void**>(getSwapChain());
	if (swapChainVtable != nullptr) {
		void* presentFunc = swapChainVtable[8];
		g_Hooks.d3d11_presentHook = std::make_unique<FuncHook>(presentFunc, Hooks::d3d11_present);
		g_Hooks.d3d11_presentHook->init();
	}
	

	// 
	void* _shit = reinterpret_cast<void*>(Utils::FindSignature("30 5F C3 CC 48 8B C4 55 56 57 41 54") + 4);
	g_Hooks.renderTextHook = std::make_unique<FuncHook>(_shit, Hooks::renderText);
	g_Hooks.renderTextHook->init();

	// I8n::get doesnt want to work
	//void *_shitshikt = reinterpret_cast<void*>(g_Data.getModule()->ptrBase + 0xB577C0);
	//g_Hooks.I8n_getHook = std::make_unique<FuncHook>(_shitshikt, Hooks::I8n_get);
	//g_Hooks.I8n_getHook->init();

	//void *shat = reinterpret_cast<void*>(g_Data.getModule()->ptrBase + 0x6908A0);
	//g_Hooks.Options_getVersionStringHook = std::make_unique<FuncHook>(shat, Hooks::Options_getVersionString);
	//g_Hooks.Options_getVersionStringHook->init();

	void* boii = reinterpret_cast<void*>(Utils::FindSignature("0F 28 C2 C7 42 0C 00 00 80 3F F3"));
	g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(boii, Hooks::Dimension_getFogColor);
	g_Hooks.Dimension_getFogColorHook->init();

	void* destroyBlok = reinterpret_cast<void*>(Utils::FindSignature("55 57 41 56 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 45 0F B6 F0") - 3);
	g_Hooks.GameMode_destroyBlockHook = std::make_unique <FuncHook>(destroyBlok, Hooks::GameMode_destroyBlock);
	g_Hooks.GameMode_destroyBlockHook->init();

	void* ChestTick = reinterpret_cast<void*>(Utils::FindSignature("40 53 57 48 83 EC ?? 48 8B 41 ?? 48 8B FA 48 89 6C 24 ?? 48 8B D9 4C 89 74 24 ?? 48 85 C0 75 10 48 8D 51 ?? 48 8B CF E8 ?? ?? ?? ?? 48 89 43 ?? FF 43 ?? 48 85 C0"));
	g_Hooks.ChestBlockActor_tickHook = std::make_unique <FuncHook>(ChestTick, Hooks::ChestBlockActor_tick);
	g_Hooks.ChestBlockActor_tickHook->init();

	void* lerpFunc = reinterpret_cast<void*>(Utils::FindSignature("8B 02 89 81 ?? 0F ?? ?? 8B 42 04 89 81 ?? ?? ?? ?? 8B 42 08 89 81 ?? ?? ?? ?? C3"));
	g_Hooks.Actor_lerpMotionHook = std::make_unique <FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);
	g_Hooks.Actor_lerpMotionHook->init();

	void* getGameEdition = reinterpret_cast<void*>(Utils::FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
	g_Hooks.AppPlatform_getGameEditionHook = std::make_unique <FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);
	g_Hooks.AppPlatform_getGameEditionHook->init();

	void* autoComplete = reinterpret_cast<void*>(Utils::FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 44 89 4C 24 ?? 4D 8B F8"));
	g_Hooks.autoComplete_Hook = std::make_unique <FuncHook>(autoComplete, Hooks::pleaseAutoComplete);
	g_Hooks.autoComplete_Hook->init();
	//logF("Hooks hooked");
}

void Hooks::Restore()
{
	g_Hooks.gameMode_tickHook->Restore();
	g_Hooks.chatScreen_sendMessageHook->Restore();
	g_Hooks.d3d11_presentHook->Restore();
	g_Hooks.renderTextHook->Restore();
	g_Hooks.Options_getVersionStringHook->Restore();
	//g_Hooks.I8n_getHook->Restore();
	g_Hooks.GameMode_destroyBlockHook->Restore();
	g_Hooks.AppPlatform_getGameEditionHook->Restore();
	g_Hooks.autoComplete_Hook->Restore();
}

void __fastcall Hooks::GameMode_tick(C_GameMode * _this)
{
	static auto oTick = g_Hooks.gameMode_tickHook->GetOriginal<GameMode_tick_t>();
	oTick(_this); // Call Original Func

	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer()) {
		moduleMgr->onTick(_this);
	}
}

void __fastcall Hooks::SurvivalMode_tick(C_GameMode * _this)
{
	static auto oTick = g_Hooks.survivalMode_tickHook->GetOriginal<SurvivalMode_tick_t>();
	oTick(_this); // Call Original Func
	GameData::updateGameData(_this);
	if (_this->player == g_Data.getLocalPlayer()) {
		moduleMgr->onTick(_this);

		_this->player->getEntityTypeId();
	}
}
void __fastcall Hooks::ChestBlockActor_tick(C_ChestBlockActor* _this, void* a)
{
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetOriginal<ChestBlockActor_tick_t>();
	oTick(_this,a); // Call Original Func
	GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(C_Entity * _this, vec3_t motVec)
{
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetOriginal<Actor_lerpMotion_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static IModule* mod = moduleMgr->getModule<NoKnockBack>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<NoKnockBack>();
	else if (mod->isEnabled()) {
		// Do nothing i guess
		// Do some stuff with modifiers here maybe
		static void* networkSender = reinterpret_cast<void*>(Utils::FindSignature("EB ?? 4C 8B 64 24 ?? 49 8B 44 24 ?? 48 8B"));
		if (networkSender == 0x0)
			logF("Network Sender not Found!!!");
		if (networkSender == _ReturnAddress()) // Check if we are being called from the network receiver
			return;
	}

	oLerp(_this, motVec);
}

__int64 Hooks::AppPlatform_getGameEdition(__int64 _this)
{
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetOriginal<AppPlatform_getGameEdition_t>();

	static EditionFaker* mod = static_cast<EditionFaker*>(moduleMgr->getModule<EditionFaker>());
	if (mod == nullptr)
		mod = static_cast<EditionFaker*>(moduleMgr->getModule<EditionFaker>());
	else if (mod->isEnabled()) {
		// Do nothing i guess
		// Do some stuff with modifiers here maybe
		return mod->getFakedEditon(); // Dont call lerpMotion
	}

	return oGetEditon(_this);
}

void Hooks::pleaseAutoComplete(__int64 a1, __int64 a2, TextHolder * text, int a4)
{
	static auto oAutoComplete = g_Hooks.autoComplete_Hook->GetOriginal<autoComplete_t>();
	char* tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1; // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower); // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			ICommand* command;
			bool shouldReplace = true;

			bool operator<(const LilPlump &o) const {
				return cmdAlias < o.cmdAlias;
			}
		}; // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<ICommand*>* commandList = cmdMgr->getCommandList();
		for (std::vector<ICommand*>::iterator it = commandList->begin(); it != commandList->end(); ++it) { // Loop through commands
			ICommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (std::vector<std::string>::iterator it = aliasList->begin(); it != aliasList->end(); ++it) { // Loop through aliases
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) { // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					}
					else if (i >= cmd.size())
						goto nope;
						
					if (car != cmd.at(i)) // and compare
						goto nope;
				}
				// Not at nope? Then we got a good result!
				{
					cmd.insert(0, 1, '.'); // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			nope:
				continue;
			}
		}

		if (searchResults.size() > 0) {
			LilPlump firstResult = (*searchResults.begin());

			if (searchResults.size() > 1) {
				g_Data.getGuiData()->displayClientMessageF("==========");
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			else {
				g_Data.getGuiData()->displayClientMessageF("==========");
				if(firstResult.command->getUsage()[0] == 0x0)
					g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
				else
					g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(), GRAY, firstResult.command->getDescription());
			}
			
			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1)
					firstResult.cmdAlias.append(" ");
				text->setText(firstResult.cmdAlias); // Set text
				// now sync with the UI thread that shows the cursor n stuff
				// If we loose this sig we are kinda fucked
				using syncShit = void(__fastcall*)(TextHolder*, TextHolder*);
				static syncShit sync = reinterpret_cast<syncShit>(Utils::FindSignature("40 53 48 83 EC ?? 8B 0D ?? ?? ?? ?? 48 8B DA FF 15 ?? ?? ?? ?? 48 85 C0 74 ?? 48 83 38 00 74 ?? E8 ?? ?? ?? ?? 48 8B D3 48 8B 08 48 8B 01"));
				sync(text, text);
			}
			
		}

		return;
	}
	oAutoComplete(a1, a2, text, a4);
	
}

void __fastcall Hooks::ChatScreenController_sendChatMessage(uint8_t * _this)
{
	static auto oSendMessage = g_Hooks.chatScreen_sendMessageHook->GetOriginal<ChatScreen_sendChatMessage_t>();
	uintptr_t* idk = reinterpret_cast<uintptr_t*>(_this + 0x688);
	if (*idk) {
		char* message = reinterpret_cast<char*>(_this + 0x678);
		if (*reinterpret_cast<__int64*>(_this + 0x690) >= 0x10)
			message = *reinterpret_cast<char**>(message);
		__int64* v6 = reinterpret_cast<__int64*>(_this + 0x690);

		if (*message == '.') {
			cmdMgr->execute(message);
			*message = 0x0; // Remove command in textbox
			*v6 = 0x0; // text length
			*idk = 0x0; // text length
			return;
		}
	}
	oSendMessage(_this);
}

HRESULT __stdcall Hooks::d3d11_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static auto oPresent = g_Hooks.d3d11_presentHook->GetOriginal<d3d11_present_t>();
	return oPresent(pSwapChain, SyncInterval, Flags);
}

__int64 __fastcall Hooks::renderText(__int64 yeet, C_MinecraftUIRenderContext* renderCtx) // I have no idea what this function is, only thing i know is that screencontext is in yote
{
	static auto oText = g_Hooks.renderTextHook->GetOriginal<renderText_t>();

	DrawUtils::setCtx(renderCtx, g_Data.getClientInstance()->getGuiData());
	moduleMgr->onPreRender();
	DrawUtils::flush();
	
	__int64 retval = oText(yeet, renderCtx);

	moduleMgr->onPostRender();

	float y = 0;

	std::string textStr = std::string("Horion");
	float leng = DrawUtils::getTextLength(&textStr);
	DrawUtils::fillRectangle(vec4_t(0, y, leng + 3, y + 12), new MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.5f);
	DrawUtils::drawText(vec2_t(1, y + 1), &textStr, new MC_Color(0.3f, 1, 0.3f, 1));

	y += 12;

	/*textStr = std::string("Close (CTRL + L)");
	leng = DrawUtils::getTextLength(&textStr);
	DrawUtils::fillRectangle(vec4_t(0, y, leng + 3, y + 12), new MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.3f);
	DrawUtils::drawText(vec2_t(1, y + 1), &textStr, new MC_Color(0.5f, 0.5f, 0.5f, 1));

	y += 12;*/

	bool showShit = g_Data.getLocalPlayer() == nullptr ? true : (GameData::canUseMoveKeys() ? true : false);

	std::vector<IModule*>* modules = moduleMgr->getModuleList();
	for (std::vector<IModule*>::iterator it = modules->begin(); it != modules->end(); ++it) {
		if (!(*it)->isEnabled())
			continue;
		std::string textStr = (*it)->getModuleName();

		std::ostringstream strStream;
		strStream << textStr;
		strStream << " (" << (char) (*it)->getKeybind() << ")";
		textStr = strStream.str();

		float leng = DrawUtils::getTextLength(&textStr);

		DrawUtils::fillRectangle(vec4_t(0, y, leng + 3, y + 12), new MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.5f);
		DrawUtils::drawText(vec2_t(1, y + 1), &textStr, new MC_Color(0.3f, 1, 0.3f, 1));
		y += 12;
	}
	if (showShit) {
		for (std::vector<IModule*>::iterator it = modules->begin(); it != modules->end(); ++it) {
			if ((*it)->isEnabled())
				continue;
			std::string textStr = (*it)->getModuleName();

			std::ostringstream strStream;
			strStream << textStr;
			strStream << " (" << (char)(*it)->getKeybind() << ")";
			textStr = strStream.str();

			float leng = DrawUtils::getTextLength(&textStr);

			DrawUtils::fillRectangle(vec4_t(0, y, leng + 3, y + 12), new MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.15f);
			DrawUtils::drawText(vec2_t(1, y + 1), &textStr, new MC_Color(0.5f, 0.2f, 0.2f, 0.1f));
			y += 12;
		}
	}
	

	DrawUtils::flush();

	return retval; 
}

char* __fastcall Hooks::I8n_get(void* f, char* str)
{
	static auto oGet = g_Hooks.renderTextHook->GetOriginal<I8n_get_t>();

	//static char* shit = "yeet";

	//BigCantWork yote;
	//strcpy_s(yote.boiii, 16, shit);
	//yote.length1 = 4;
	//yote.length2 = 4;
	//yote.length3 = 4;

	//if (strcmp(str, "menu.play") == 0)
		//return &yote;
	return oGet(f, str);
}

TextHolder * __fastcall Hooks::Options_getVersionString(void * ya, __int64 idk)
{
	static auto oGetVer = g_Hooks.Options_getVersionStringHook->GetOriginal<Options_getVersionString_t>();
	TextHolder* version = oGetVer(ya, idk);
	version->inlineText[0] = 'E';
	return version;
}

float * Hooks::Dimension_getFogColor(__int64 a1, float * color, float brightness)
{
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetOriginal<Dimension_getFogColor_t>();

	static float rcolors[4];

	static IModule* mod = moduleMgr->getModule<RainbowSky>();
	if(mod == nullptr)
		mod = moduleMgr->getModule<RainbowSky>();
	else if(mod->isEnabled()){
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.2f;
			rcolors[2] = 0.2f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]); // perfect code, dont question this

		rcolors[0] += 0.001f;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		return rcolors;
	}
	return oGetFogColor(a1, color, brightness);

	
}

void Hooks::GameMode_destroyBlock(void * _this, C_BlockPos * pos, uint8_t face)
{
	static auto oDestroyBlock = g_Hooks.GameMode_destroyBlockHook->GetOriginal<GameMode_destroyBlock_t>();

	oDestroyBlock(_this, pos, face);
}