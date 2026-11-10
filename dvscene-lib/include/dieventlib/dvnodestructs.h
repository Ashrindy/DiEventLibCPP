#pragma once
#include "internal.h"

namespace dv::nodes::rangers {
	enum class NodeCategory : unsigned int { // for anyone curious enough, the skips are intentional, since this is all of the DvNodes the game has
		Path = 1,
		Camera = 3,
		CameraMotion = 4,
		Character = 5,
		CharacterMotion = 6,
		Model = 8,
		ModelMotion = 10,
		ModelNode = 11,
		Element = 12,
		Stage = 13,
		FolderCondition = 17
	};

	struct Path {
		internal::Matrix4x4 world;
		int flags;
		char unknown[12];
	};

	struct Camera {
		int flags;
		unsigned int frameProgressionCount;
		unsigned int captionCount;
		int field0c;
		// there's two float array here, right after each other, both of their count is based on "frameProgressionCount"

		inline float* getFrameProgression() const {
			return (float*)&this[1];
		}

		inline float* getFrameProgressionSpeed() const {
			return &getFrameProgression()[frameProgressionCount];
		}
	};

	struct CameraMotion {
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int field0c;
	};

	struct Character {
		bool useInternalName;
	private:
		char padding[3];
	public:
		char modelName[64];
		char skeletonName[64];
		char internalName[64];
		char unkData[76];
	};

	struct CharacterMotion {
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int unknown0;
		char asmStateName[8];
		float speed;
		int field1c;
		char unknown1[16];
	};

	struct Model {
		bool useInternalName;
	private:
		char padding[3];
	public:
		char modelName[64];
		char skeletonName[64];
		char internalName[64];
		char unkData[76];
	};

	struct ModelMotion {
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int unknown0;
		char asmStateName[8];
		float speed;
		int field1c;
		char unknown1[16];
	};

	struct ModelNode {
		int flags;
		char nodeName[64];
		char unknown0[12];
	};

	struct Element {
		enum class Flags : unsigned int {
			UNK,
			LOOSE_LENGTH
		};

		enum class PlayType : unsigned int {
			Normal = 0,
			OneShot,
			Always
		};

		enum class UpdateTiming : unsigned int {
			OnExecPath = 0,
			OnPreUpdate,
			CharacterFixPosture,
			OnPostUpdateCharacter,
			OnUpdatePos,
			OnFixBonePosture,
			OnEvaluateDetailMotion,
			CharacterJobUpdate,
			ModifyPoseAfter,
			JobRegister,
			MotionUpdate
		};

		unsigned int elementID;
		float frameStart;
		float frameEnd;
		int version;
		Flags flags;
		PlayType playType;
		UpdateTiming updateTiming;
		char unknown[4];
	};

	struct Stage;

	struct FolderCondition;
}
