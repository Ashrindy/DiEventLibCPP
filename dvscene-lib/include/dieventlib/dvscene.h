#pragma once
#include "dvheader.h"
#include "dvobject.h"
#include "dvnodestructs.h"
#include <vector>

namespace dv {
	class DvScene {
	private:
		void readCommon(RawDvCommon* rawDvCommon);
		void readCuts(float* cuts, size_t count);
		void readPages(const char* pages, size_t count);
		void readDisableFrames(DvDisableFrame* disableFrames, size_t count);
		void readResourceCuts(float* resourceCuts, size_t count);
		void readSoundCuts(float* soundCuts, size_t count);
		DvNode* readNode(RawDvNode* rawNode);
		void readResources(Resource* buffer, size_t count);

		size_t getDvCommonSize();
		size_t getDvResourceSize();
		size_t getDvSceneSize();
		void getNodeSize(size_t& size, DvNode* node);

		void writeCommon(char* buffer, size_t& pos);
		void writeCuts(char* buffer, size_t& pos);
		void writePages(char* buffer, size_t& pos);
		void writeDisableFrames(char* buffer, size_t& pos);
		void writeResourceCuts(char* buffer, size_t& pos);
		void writeSoundCuts(char* buffer, size_t& pos);
		void writeNode(char* buffer, size_t& pos, DvNode* node);
		void writeResources(char* buffer, size_t& pos);

	public:
		DvCommon* dvCommon;
		std::vector<Resource> dvResource;

		void read(const char* data, size_t size);
		internal::Buffer write();
		void free();
		inline ~DvScene() { free(); }
	};
}