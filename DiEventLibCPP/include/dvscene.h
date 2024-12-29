#pragma once
#include "dvheader.h"
#include <vector>

class DvScene {
public:
	DvCommon* dvCommon;
	std::vector<Resource> dvResource;

	void read(const char* data, size_t size) {
		DvHeader* header = (DvHeader*)data;
		auto* rawDvResource = header->dvResource.get();
		for (auto x = 0; x < rawDvResource->count; x++)
			dvResource.push_back(rawDvResource->getItems()[x]);
		auto* rawDvCommon = header->dvCommon.get();
		dvCommon = new DvCommon();
		dvCommon->frameStart = rawDvCommon->frameStart;
		dvCommon->frameEnd = rawDvCommon->frameEnd;
		dvCommon->drawNodeNumber = rawDvCommon->drawNodeNumber;
		dvCommon->chainCameraIn = rawDvCommon->chainCameraIn;
		dvCommon->chainCameraOut = rawDvCommon->chainCameraOut;
		dvCommon->type = rawDvCommon->type;
		dvCommon->skipPointTick = rawDvCommon->skipPointTick;
		auto* cuts = rawDvCommon->cuts.get();
		for (auto x = 0; x < cuts->count; x++) {
			dvCommon->cuts.push_back(cuts->getItems()[x]);
		}
		auto* pages = rawDvCommon->pages.get()->getItems();
		size_t pagePos = 0;
		for (auto x = 0; x < rawDvCommon->pages.get()->count; x++) {
			DvPage* page = new DvPage();
			auto* y = (RawDvPage*)&pages[pagePos];
			page->frameStart = (float)(y->frameStart / 100);
			page->frameEnd = (float)(y->frameEnd / 100);
			page->index = y->index;
			page->skipFrame = (float)(y->skipFrame / 100);
			memcpy(page->name, y->name, 32);
			pagePos += sizeof(RawDvPage);
			int* unkData = (int*)&pages[pagePos];
			for (auto z = 0; z < y->pageSize; z++) {
				page->unkData.push_back(unkData[z]);
			}
			pagePos += y->pageSize * 4;
			auto* transitions = (char*)&pages[pagePos];
			size_t transitionPos = 0;
			for (auto z = 0; z < y->transitionCount; z++) {
				DvTransition* transition = new DvTransition();
				auto* rawTransition = (RawDvTransition*)&transitions[transitionPos];
				transitionPos += sizeof(RawDvTransition);
				transition->destinationPageID = rawTransition->destinationPageID;
				auto* conditions = (char*)&transitions[transitionPos];
				size_t conditionPos = 0;
				for (auto i = 0; i < rawTransition->conditionCount; i++) {
					DvCondition* condition = new DvCondition();
					auto* rawCondition = (RawDvCondition*)&conditions[conditionPos];
					conditionPos += sizeof(RawDvCondition);
					condition->type = (DvCondition::ConditionType)rawCondition->type;
					condition->data = new char[rawCondition->parametersSize];
					memcpy(condition->data, &conditions[conditionPos], rawCondition->parametersSize);
					conditionPos += rawCondition->parametersSize;
					transition->conditions.push_back(*condition);
				}
				transitionPos += conditionPos;
				page->transition.push_back(*transition);
			}
			pagePos += transitionPos;
			dvCommon->pages.push_back(*page);
		}
		auto* disableFrames = rawDvCommon->disableFrameInfo.get();
		for (auto x = 0; x < disableFrames->count; x++) {
			dvCommon->disableFrameInfo.push_back(disableFrames->getItems()[x]);
		}
		auto* resourceCuts = rawDvCommon->resourceCutInfo.get();
		for (auto x = 0; x < resourceCuts->count; x++) {
			dvCommon->resourceCutInfo.push_back(resourceCuts->getItems()[x]);
		}
		auto* soundCuts = rawDvCommon->soundCutInfo.get();
		for (auto x = 0; x < soundCuts->count; x++) {
			dvCommon->soundCutInfo.push_back(soundCuts->getItems()[x]);
		}
	}
};