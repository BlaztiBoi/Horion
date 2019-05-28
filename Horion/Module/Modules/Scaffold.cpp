#include "Scaffold.h"





Scaffold::Scaffold() : IModule(VK_NUMPAD1)
{
}


Scaffold::~Scaffold()
{
}

std::string Scaffold::getModuleName()
{
	return std::string("Scaffold");
}

bool Scaffold::tryScaffold(vec3_t blockBelow)
{
	blockBelow = blockBelow.floor();

	using yeetBoi_t = __int64(__fastcall*)(uintptr_t, const vec3_ti&);
	static yeetBoi_t yeetBoi = reinterpret_cast<yeetBoi_t>(Utils::FindSignature("40 53 48 83 EC ?? 48 8B DA 8B 52 ?? 85 D2"));
	

	
	DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(1), 0.4f);
	// BlockSource::getBlock()::getMaterial()::isReplaceable()
	if ((*(uint8_t *)(*(uintptr_t *)(**(uintptr_t **)(yeetBoi(g_Data.getLocalPlayer()->region, vec3_ti(blockBelow)) + 16) + 120i64) + 7i64))) {
		
		vec3_ti* blok = new vec3_ti(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.size() == 0) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}
		bool foundCandidate = false;
		int i = 0;
		for (auto it = checklist.begin(); it != checklist.end(); ++it) {
			vec3_ti* current = *it;

			vec3_ti* calc = blok->subAndReturn(*current);
			if (!(*(uint8_t *)(*(uintptr_t *)(**(uintptr_t **)(yeetBoi(g_Data.getLocalPlayer()->region, *calc) + 16) + 120i64) + 7i64))) {
				// Found a solid block to click
				foundCandidate = true;
				blok->set(calc);
				delete calc;
				break;
			}
			delete calc;
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(blok, i);
			delete blok;
			return true;
		}
		delete blok;
	}
	return false;
}

void Scaffold::onPostRender()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->itemId == 0 || g_Data.getLocalPlayer()->itemId > 255) // Block in hand?
		return;

	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0; // Block below the player
	blockBelow.y -= g_Data.getLocalPlayer()->height;     
	blockBelow.y -= 0.5f;


	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel.normalize(); // Only use values from 0 - 1
	
	DrawUtils::setColor(0.3f, 0.2f, 0.8f, 1);
	if (!tryScaffold(blockBelow)) {
		if (speed > 0.05f) { // Are we actually walking?
			DrawUtils::setColor(0.8f, 0.8f, 0.2f, 1);
			blockBelow.z -= vel.z * 0.4f;
			if (!tryScaffold(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryScaffold(blockBelow);
				}
			}
		}
		
	}
}
