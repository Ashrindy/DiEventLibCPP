#include <dieventlib/dvscene.h>

using namespace dv;

void DvScene::readCommon(RawDvCommon* rawDvCommon) {
	dvCommon = new DvCommon{};
	dvCommon->frameStart = rawDvCommon->frameStart;
	dvCommon->frameEnd = rawDvCommon->frameEnd;
	dvCommon->drawNodeNumber = rawDvCommon->drawNodeNumber;
	dvCommon->chainCameraIn = rawDvCommon->chainCameraIn;
	dvCommon->chainCameraOut = rawDvCommon->chainCameraOut;
	dvCommon->type = rawDvCommon->type;
	dvCommon->skipPointTick = rawDvCommon->skipPointTick;
}

void DvScene::readCuts(float* cuts, size_t count) {
	for (auto x = 0; x < count; x++)
		dvCommon->cuts.push_back(cuts[x]);
}

void DvScene::readPages(const char* pages, size_t count) {
	size_t pagePos = 0;
	for (auto x = 0; x < count; x++) {
		DvPage* page = new DvPage{};
		auto* y = (RawDvPage*)&pages[pagePos];
		page->frameStart = (float)(y->frameStart / 100);
		page->frameEnd = (float)(y->frameEnd / 100);
		page->index = y->index;
		page->skipFrame = (float)(y->skipFrame / 100);
		memcpy(page->name, y->name, 32);
		pagePos += sizeof(RawDvPage);
		int* unkData = (int*)&pages[pagePos];
		for (unsigned int z = 0; z < y->pageSize; z++)
			page->unkData.push_back(unkData[z]);
		pagePos += y->pageSize * 4;
		auto* transitions = (char*)&pages[pagePos];
		size_t transitionPos = 0;
		for (unsigned int z = 0; z < y->transitionCount; z++) {
			DvTransition* transition = new DvTransition{};
			auto* rawTransition = (RawDvTransition*)&transitions[transitionPos];
			transitionPos += sizeof(RawDvTransition);
			transition->destinationPageID = rawTransition->destinationPageID;
			auto* conditions = (char*)&transitions[transitionPos];
			size_t conditionPos = 0;
			for (auto i = 0; i < rawTransition->conditionCount; i++) {
				DvCondition* condition = new DvCondition{};
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

void DvScene::readDisableFrames(DvDisableFrame* disableFrames, size_t count) {
	for (auto x = 0; x < count; x++)
		dvCommon->disableFrameInfo.push_back(disableFrames[x]);
}

void DvScene::readResourceCuts(float* resourceCuts, size_t count) {
	for (auto x = 0; x < count; x++)
		dvCommon->resourceCutInfo.push_back(resourceCuts[x]);
}

void DvScene::readSoundCuts(float* soundCuts, size_t count) {
	for (auto x = 0; x < count; x++)
		dvCommon->soundCutInfo.push_back(soundCuts[x]);
}

DvNode* DvScene::readNode(RawDvNode* rawNode) {
	DvNode* node = new DvNode{};
	memcpy(node->name, rawNode->name, 64);
	node->category = rawNode->category;
	node->guid = rawNode->guid;
	node->nodeFlags = rawNode->nodeFlags;
	node->priority = rawNode->priority;
	node->dataSize = rawNode->nodeSize * 4;
	node->data = &((char*)rawNode)[sizeof(RawDvNode)];
	for (auto& x : rawNode->getChildNodes())
		node->childNodes.push_back(*readNode(x));
	return node;
}

void DvScene::readResources(Resource* buffer, size_t count) {
	for (auto x = 0; x < count; x++)
		dvResource.push_back(buffer[x]);
}

size_t DvScene::getDvCommonSize() {
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
			for (auto& z : y.conditions) 
				commonSize += z.dataSize;
		}
	}
	commonSize += 0x10;
	commonSize += dvCommon->disableFrameInfo.size() * sizeof(DvDisableFrame);
	commonSize += 0x10;
	commonSize += dvCommon->resourceCutInfo.size() * sizeof(float);
	commonSize += 0x10;
	commonSize += dvCommon->soundCutInfo.size() * sizeof(float);
	getNodeSize(commonSize, dvCommon->node);
	return commonSize;
}

size_t DvScene::getDvResourceSize()  {
	size_t resourceSize = 0;
	resourceSize += 0x10;
	resourceSize += dvResource.size() * sizeof(Resource);
	return resourceSize;
}

size_t DvScene::getDvSceneSize() {
	size_t filesize = sizeof(DvHeader);
	size_t commonSize = getDvCommonSize();
	size_t resourceSize = getDvResourceSize();
	filesize += commonSize + resourceSize;
	return filesize;
}

void DvScene::getNodeSize(size_t& size, DvNode* node) {
	size += sizeof(RawDvNode);
	size += node->dataSize;
	for (auto& x : node->childNodes)
		getNodeSize(size, &x);
}

void DvScene::writeCommon(char* buffer, size_t& pos) {
	RawDvCommon* rawDvCommon = (RawDvCommon*)&buffer[pos];
	rawDvCommon->frameStart = dvCommon->frameStart;
	rawDvCommon->frameEnd = dvCommon->frameEnd;
	rawDvCommon->drawNodeNumber = dvCommon->drawNodeNumber;
	rawDvCommon->chainCameraIn = dvCommon->chainCameraIn;
	rawDvCommon->chainCameraOut = dvCommon->chainCameraOut;
	pos += sizeof(RawDvCommon);
}

void DvScene::writeCuts(char* buffer, size_t& pos) {
	DvObject<float>* cuts = (DvObject<float>*)&buffer[pos];
	cuts->count = (int)dvCommon->cuts.size();
	pos += sizeof(DvObject<float>);
	memcpy(&buffer[pos], (char*)dvCommon->cuts.data(), dvCommon->cuts.size() * sizeof(float));
	pos += cuts->count * sizeof(float);
}

void DvScene::writePages(char* buffer, size_t& pos) {
	DvObject<char>* pages = (DvObject<char>*)&buffer[pos];
	pages->count = (int)dvCommon->pages.size();
	pages->size += (int)dvCommon->pages.size() * sizeof(RawDvPage);
	for (auto& x : dvCommon->pages) {
		pages->size += (int)x.unkData.size() * sizeof(int);
		pages->size += (int)x.transition.size() * sizeof(RawDvTransition);
		for (auto& y : x.transition) {
			pages->size += (int)y.conditions.size() * sizeof(RawDvCondition);
			for (auto& z : y.conditions)
				pages->size += (int)z.dataSize;
		}
	}
	pos += sizeof(DvObject<char>);
	for (auto& x : dvCommon->pages)
	{
		RawDvPage* rawPage = (RawDvPage*)&buffer[pos];
		rawPage->frameStart = (unsigned int)(x.frameStart * 100);
		rawPage->frameEnd = (unsigned int)(x.frameEnd * 100);
		rawPage->transitionCount = (unsigned int)x.transition.size();
		rawPage->transitionSize += (unsigned int)x.transition.size() * sizeof(RawDvTransition);
		for (auto& transition : x.transition)
		{
			rawPage->transitionSize +=(unsigned int)transition.conditions.size() * sizeof(RawDvTransition);
			for (auto& cond : transition.conditions)
				rawPage->transitionSize += (unsigned int)cond.dataSize;
		}
		rawPage->skipFrame = (unsigned int)(x.skipFrame * 100);
		rawPage->index = x.index;
		memcpy(rawPage->name, x.name, 32);
		rawPage->pageSize = (unsigned int)x.unkData.size();
		pos += sizeof(RawDvPage);
		int* unkData = (int*)&buffer[pos];
		for (int unk = 0; unk < x.unkData.size(); unk++)
			unkData[unk] = x.unkData[unk];
		pos += x.unkData.size() * sizeof(int);
		for (auto& transition : x.transition) {
			RawDvTransition* rawTransition = (RawDvTransition*)&buffer[pos];
			rawTransition->destinationPageID = transition.destinationPageID;
			rawTransition->conditionCount = (int)transition.conditions.size();
			rawTransition->conditionSize += transition.conditions.size() * sizeof(RawDvCondition);
			for (auto& cond : transition.conditions)
				rawTransition->conditionSize += cond.dataSize;
			pos += sizeof(RawDvTransition);
			for (auto& cond : transition.conditions) {
				RawDvCondition* rawCond = (RawDvCondition*)&buffer[pos];
				rawCond->type = (int)cond.type;
				rawCond->parametersSize = (int)cond.dataSize;
				pos += sizeof(RawDvCondition);
				memcpy(&buffer[pos], cond.data, cond.dataSize);
				pos += cond.dataSize;
			}
		}
	}
}

void DvScene::writeDisableFrames(char* buffer, size_t& pos) {
	DvObject<DvDisableFrame>* disableFrames = (DvObject<DvDisableFrame>*)&buffer[pos];
	disableFrames->count = (unsigned int)dvCommon->disableFrameInfo.size();
	pos += sizeof(DvObject<DvDisableFrame>);
	memcpy(&buffer[pos], (char*)dvCommon->disableFrameInfo.data(), dvCommon->disableFrameInfo.size() * sizeof(DvDisableFrame));
	pos += disableFrames->count * sizeof(float);
}

void DvScene::writeResourceCuts(char* buffer, size_t& pos) {
	DvObject<float>* cuts = (DvObject<float>*) & buffer[pos];
	cuts->count = (unsigned int)dvCommon->resourceCutInfo.size();
	pos += sizeof(DvObject<float>);
	memcpy(&buffer[pos], (char*)dvCommon->resourceCutInfo.data(), dvCommon->resourceCutInfo.size() * sizeof(float));
	pos += cuts->count * sizeof(float);
}

void DvScene::writeSoundCuts(char* buffer, size_t& pos) {
	DvObject<float>* cuts = (DvObject<float>*)&buffer[pos];
	cuts->count = (unsigned int)dvCommon->soundCutInfo.size();
	pos += sizeof(DvObject<float>);
	memcpy(&buffer[pos], (char*)dvCommon->soundCutInfo.data(), dvCommon->soundCutInfo.size() * sizeof(float));
	pos += cuts->count * sizeof(float);
}

void DvScene::writeNode(char* buffer, size_t& pos, DvNode* node) {
	RawDvNode* rawNode = (RawDvNode*)&buffer[pos];
	rawNode->guid = node->guid;
	rawNode->category = node->category;
	rawNode->nodeSize = (unsigned int)node->dataSize / 4;
	rawNode->childCount = (unsigned int)node->childNodes.size();
	rawNode->nodeFlags = node->nodeFlags;
	rawNode->priority = node->priority;
	memcpy(rawNode->name, node->name, 64);
	pos += sizeof(RawDvNode);
	memcpy(&buffer[pos], node->data, node->dataSize);
	pos += node->dataSize;
	for (auto x : node->childNodes)
		writeNode(buffer, pos, &x);
}

void DvScene::writeResources(char* buffer, size_t& pos) {
	DvObject<Resource>* resources = (DvObject<Resource>*)&buffer[pos];
	resources->count = (unsigned int)dvResource.size();
	pos += sizeof(DvObject<Resource>);
	memcpy(&buffer[pos], (char*)dvResource.data(), dvResource.size() * sizeof(Resource));
	pos += resources->count * sizeof(Resource);
}

void DvScene::read(const char* data, size_t /*size*/) {
	DvHeader* header = (DvHeader*)data;
			
	auto* rawDvCommon = header->dvCommon.get();
	readCommon(rawDvCommon);

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

	auto* rawDvResource = header->dvResource.get();
	readResources(rawDvResource->getItems(), rawDvResource->count);
}

internal::Buffer DvScene::write() {
    internal::Buffer buffer{};

	buffer.size = getDvSceneSize();
	buffer.data = new char[buffer.size];
	memset(buffer.data, 0, buffer.size);

	DvHeader* header = (DvHeader*)buffer.data;

	char* dataBuffer = &buffer.data[sizeof(DvHeader)];
	size_t pos = 0;
			
	RawDvCommon* rawDvCommon = (RawDvCommon*)&dataBuffer[pos];
	writeCommon(dataBuffer, pos);

	rawDvCommon->cuts.ptr = (int)pos;
	writeCuts(dataBuffer, pos);

	rawDvCommon->pages.ptr = (int)pos;
	writePages(dataBuffer, pos);

	rawDvCommon->disableFrameInfo.ptr = (int)pos;
	writeDisableFrames(dataBuffer, pos);

	rawDvCommon->resourceCutInfo.ptr = (int)pos;
	writeResourceCuts(dataBuffer, pos);

	rawDvCommon->soundCutInfo.ptr = (int)pos;
	writeSoundCuts(dataBuffer, pos);

	rawDvCommon->node.ptr = (int)pos;
	writeNode(dataBuffer, pos, dvCommon->node);

	header->dvResource.ptr = (int)pos;
	writeResources(dataBuffer, pos);

	return buffer;
}

void DvScene::free() {
	delete dvCommon;
}
