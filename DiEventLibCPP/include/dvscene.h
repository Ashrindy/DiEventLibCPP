#pragma once
#include "dvheader.h"
#include "dvobject.h"
#include <vector>

class DvScene {
private:
	void readCuts(float* cuts, size_t count) {
		for (auto x = 0; x < count; x++) {
			dvCommon->cuts.push_back(cuts[x]);
		}
	}
	void readPages(const char* pages, size_t count) {
		size_t pagePos = 0;
		for (auto x = 0; x < count; x++) {
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
					condition->dataSize = rawCondition->parametersSize;
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
	}
	void readDisableFrames(DvDisableFrame* disableFrames, size_t count) {
		for (auto x = 0; x < count; x++) {
			dvCommon->disableFrameInfo.push_back(disableFrames[x]);
		}
	}
	void readResourceCuts(float* resourceCuts, size_t count) {
		for (auto x = 0; x < count; x++) {
			dvCommon->resourceCutInfo.push_back(resourceCuts[x]);
		}
	}
	void readSoundCuts(float* soundCuts, size_t count) {
		for (auto x = 0; x < count; x++) {
			dvCommon->soundCutInfo.push_back(soundCuts[x]);
		}
	}
	DvNode* readNode(RawDvNode* rawNode) {
		DvNode* node = new DvNode();
		memcpy(node->name, rawNode->name, 64);
		node->category = (DvNode::Category)rawNode->category;
		node->guid = rawNode->guid;
		node->nodeFlags = rawNode->nodeFlags;
		node->priority = rawNode->priority;
		node->dataSize = rawNode->nodeSize*4;
		node->data = &((char*)rawNode)[sizeof(RawDvNode)];
		for (auto &x : rawNode->getChildNodes()) {
			node->childNodes.push_back(*readNode(x));
		}
		return node;
	}


	void getNodeSize(size_t* size, DvNode* node) {
		(*size) += sizeof(RawDvNode);
		(*size) += node->dataSize;
		for (auto& x : node->childNodes)
			getNodeSize(size, &x);
	}

	void writeCuts(char* buffer, size_t* pos) {
		DvObject<float>* cuts = (DvObject<float>*)&buffer[*pos];
		cuts->count = dvCommon->cuts.size();
		*pos += sizeof(DvObject<float>);
		memcpy(&buffer[*pos], (char*)dvCommon->cuts.data(), dvCommon->cuts.size() * sizeof(float));
		*pos += cuts->count * sizeof(float);
	}
	void writePages(char* buffer, size_t* pos) {
		DvObject<char>* pages = (DvObject<char>*)&buffer[*pos];
		pages->count = dvCommon->pages.size();
		pages->size += dvCommon->pages.size() * sizeof(RawDvPage);
		for (auto& x : dvCommon->pages) {
			pages->size += x.unkData.size() * sizeof(int);
			pages->size += x.transition.size() * sizeof(RawDvTransition);
			for (auto& y : x.transition) {
				pages->size += y.conditions.size() * sizeof(RawDvCondition);
				for (auto& z : y.conditions) {
					pages->size += z.dataSize;
				}
			}
		}
		*pos += sizeof(DvObject<char>);
		for (auto &x : dvCommon->pages)
		{
			RawDvPage* rawPage = (RawDvPage*)&buffer[*pos];
			rawPage->frameStart = (unsigned int)(x.frameStart * 100);
			rawPage->frameEnd = (unsigned int)(x.frameEnd * 100);
			rawPage->transitionCount = x.transition.size();
			rawPage->transitionSize += x.transition.size() * sizeof(RawDvTransition);
			for (auto& transition : x.transition)
			{
				rawPage->transitionSize += transition.conditions.size() * sizeof(RawDvTransition);
				for (auto& cond : transition.conditions)
					rawPage->transitionSize += cond.dataSize;
			}
			rawPage->skipFrame = (unsigned int)(x.skipFrame * 100);
			rawPage->index = x.index;
			memcpy(rawPage->name, x.name, 32);
			rawPage->pageSize = x.unkData.size();
			*pos += sizeof(RawDvPage);
			int* unkData = (int*)&buffer[*pos];
			for (int unk = 0; unk < x.unkData.size(); unk++)
				unkData[unk] = x.unkData[unk];
			*pos += x.unkData.size() * sizeof(int);
			for (auto& transition : x.transition) {
				RawDvTransition* rawTransition = (RawDvTransition*)&buffer[*pos];
				rawTransition->destinationPageID = transition.destinationPageID;
				rawTransition->conditionCount = transition.conditions.size();
				rawTransition->conditionSize += transition.conditions.size() * sizeof(RawDvCondition);
				for (auto& cond : transition.conditions)
					rawTransition->conditionSize += cond.dataSize;
				*pos += sizeof(RawDvTransition);
				for (auto& cond : transition.conditions) {
					RawDvCondition* rawCond = (RawDvCondition*)&buffer[*pos];
					rawCond->type = (int)cond.type;
					rawCond->parametersSize = cond.dataSize;
					*pos += sizeof(RawDvCondition);
					memcpy(&buffer[*pos], cond.data, cond.dataSize);
					*pos += cond.dataSize;
				}
			}
		}
	}
	void writeDisableFrames(char* buffer, size_t* pos) {
		DvObject<DvDisableFrame>* disableFrames = (DvObject<DvDisableFrame>*) & buffer[*pos];
		disableFrames->count = dvCommon->disableFrameInfo.size();
		*pos += sizeof(DvObject<DvDisableFrame>);
		memcpy(&buffer[*pos], (char*)dvCommon->disableFrameInfo.data(), dvCommon->disableFrameInfo.size() * sizeof(DvDisableFrame));
		*pos += disableFrames->count * sizeof(float);
	}
	void writeResourceCuts(char* buffer, size_t* pos) {
		DvObject<float>* cuts = (DvObject<float>*) & buffer[*pos];
		cuts->count = dvCommon->resourceCutInfo.size();
		*pos += sizeof(DvObject<float>);
		memcpy(&buffer[*pos], (char*)dvCommon->resourceCutInfo.data(), dvCommon->resourceCutInfo.size() * sizeof(float));
		*pos += cuts->count * sizeof(float);
	}
	void writeSoundCuts(char* buffer, size_t* pos) {
		DvObject<float>* cuts = (DvObject<float>*) & buffer[*pos];
		cuts->count = dvCommon->soundCutInfo.size();
		*pos += sizeof(DvObject<float>);
		memcpy(&buffer[*pos], (char*)dvCommon->soundCutInfo.data(), dvCommon->soundCutInfo.size() * sizeof(float));
		*pos += cuts->count * sizeof(float);
	}
	void writeNode(char* buffer, size_t* pos, DvNode* node) {
		RawDvNode* rawNode = (RawDvNode*)&buffer[*pos];
		rawNode->guid = node->guid;
		rawNode->category = (unsigned int)node->category;
		rawNode->nodeSize = node->dataSize / 4;
		rawNode->childCount = node->childNodes.size();
		rawNode->nodeFlags = node->nodeFlags;
		rawNode->priority = node->priority;
		memcpy(rawNode->name, node->name, 64);
		*pos += sizeof(RawDvNode);
		memcpy(&buffer[*pos], node->data, node->dataSize);
		*pos += node->dataSize;
		for (auto x : node->childNodes)
			writeNode(buffer, pos, &x);
	}
	void writeResources(char* buffer, size_t* pos) {
		DvObject<Resource>* resources = (DvObject<Resource>*)&buffer[*pos];
		resources->count = dvResource.size();
		*pos += sizeof(DvObject<Resource>);
		memcpy(&buffer[*pos], (char*)dvResource.data(), dvResource.size() * sizeof(Resource));
		*pos += resources->count * sizeof(Resource);
	}

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
		readCuts(cuts->getItems(), cuts->count);

		auto* pages = rawDvCommon->pages.get();
		readPages(pages->getItems(), pages->count);

		auto* disableFrames = rawDvCommon->disableFrameInfo.get();
		readDisableFrames(disableFrames->getItems(), disableFrames->count);

		auto* resourceCuts = rawDvCommon->resourceCutInfo.get();
		readResourceCuts(resourceCuts->getItems(), resourceCuts->count);

		auto* soundCuts = rawDvCommon->soundCutInfo.get();
		readSoundCuts(soundCuts->getItems(), soundCuts->count);

		auto* node = rawDvCommon->node.get();
		dvCommon->node = readNode(node);
	}

	Buffer write() {
		Buffer buffer{};
		size_t filesize = sizeof(DvHeader);

		size_t commonSize = 0;
		commonSize += sizeof(RawDvCommon);
		commonSize += 0x10;
		commonSize += dvCommon->cuts.size() * sizeof(float);
		commonSize += 0x10;
		commonSize += dvCommon->pages.size() * sizeof(RawDvPage);
		for (auto& x : dvCommon->pages) {
			commonSize += x.unkData.size() * sizeof(int);
			commonSize += x.transition.size() * sizeof(RawDvTransition);
			for (auto& y : x.transition) {
				commonSize += y.conditions.size() * sizeof(RawDvCondition);
				for (auto& z : y.conditions) {
					commonSize += z.dataSize;
				}
			}
		}
		commonSize += 0x10;
		commonSize += dvCommon->disableFrameInfo.size() * sizeof(DvDisableFrame);
		commonSize += 0x10;
		commonSize += dvCommon->resourceCutInfo.size() * sizeof(float);
		commonSize += 0x10;
		commonSize += dvCommon->soundCutInfo.size() * sizeof(float);
		getNodeSize(&commonSize, dvCommon->node);

		size_t resourceSize = 0;
		resourceSize += 0x10;
		resourceSize += dvResource.size() * sizeof(Resource);

		filesize += commonSize + resourceSize;

		buffer.size = filesize;
		buffer.data = new char[buffer.size];
		std::memset(buffer.data, 0, buffer.size);

		DvHeader* header = (DvHeader*)buffer.data;

		char* dataBuffer = &buffer.data[0x20];
		size_t pos = 0;
		RawDvCommon* rawDvCommon = (RawDvCommon*)&dataBuffer[pos];
		rawDvCommon->frameStart = dvCommon->frameStart;
		rawDvCommon->frameEnd = dvCommon->frameEnd;
		rawDvCommon->drawNodeNumber = dvCommon->drawNodeNumber;
		rawDvCommon->chainCameraIn = dvCommon->chainCameraIn;
		rawDvCommon->chainCameraOut = dvCommon->chainCameraOut;
		pos += sizeof(RawDvCommon);

		rawDvCommon->cuts.ptr = pos;
		writeCuts(dataBuffer, &pos);

		rawDvCommon->pages.ptr = pos;
		writePages(dataBuffer, &pos);

		rawDvCommon->disableFrameInfo.ptr = pos;
		writeDisableFrames(dataBuffer, &pos);

		rawDvCommon->resourceCutInfo.ptr = pos;
		writeResourceCuts(dataBuffer, &pos);

		rawDvCommon->soundCutInfo.ptr = pos;
		writeSoundCuts(dataBuffer, &pos);

		rawDvCommon->node.ptr = pos;
		writeNode(dataBuffer, &pos, dvCommon->node);

		header->dvResource.ptr = pos;
		writeResources(dataBuffer, &pos);

		return buffer;
	}
};