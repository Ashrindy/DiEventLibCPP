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
	public:
		internal::Matrix4x4 world;
		int flags;
		char unknown[12];
	};

	struct Camera {
	public:
		int flags;
		unsigned int frameProgressionCount;
		unsigned int captionCount;
		int field0c;
		// there's two float array here, right after each other, both of their count is based on "frameProgressionCount"
	};

	struct CameraMotion {
	public:
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int field0c;
	};

	struct Character {
	public:
		bool useInternalName;
		char modelName[64];
		char skeletonName[64];
		char internalName[64];
		char unkData[76];
	};

	struct CharacterMotion {
	public:
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int unknown0;
		char ASMstateName[8];
		float speed;
		int field1c;
		char unknown1[16];
	};

	struct Model {
	public:
		bool useInternalName;
		char modelName[64];
		char skeletonName[64];
		char internalName[64];
		char unkData[76];
	};

	struct ModelMotion {
	public:
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int unknown0;
		char ASMstateName[8];
		float speed;
		int field1c;
		char unknown1[16];
	};

	struct ModelNode {
	public:
		int flags;
		char nodeName[64];
		char unknown0[12];
	};

	struct Element {
	public:
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
		int flags;
		PlayType playType;
		UpdateTiming updateTiming;
		char unknown[4];
	};

	struct Stage;

	struct FolderCondition;
}

namespace dv::nodes::miller {
	enum class NodeCategory : unsigned int { // for anyone curious enough, the skips are intentional, since this is all of the DvNodes the game has. somewhere on this list is CameraInterpolateHelper, it isn't mentioned in the DvNode create function
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
	public:
		internal::Matrix4x4 world;
		int flags;
		char unknown[12];
	};

	struct Camera {
	public:
		int flags;
		unsigned int frameProgressionCount;
		unsigned int captionCount;
		int field0c;
		// there's two float array here, right after each other, both of their count is based on "frameProgressionCount"
	};

	struct CameraMotion {
	public:
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int field0c;
	};

	struct Character {
	public:
		bool useInternalName;
		char modelName[64];
		char skeletonName[64];
		char internalName[64];
		char unkData[76];
	};

	struct CharacterMotion {
	public:
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int unknown0;
		char ASMstateName[8];
		float speed;
		int field1c;
		char unknown1[16];
	};

	struct Model {
	public:
		bool useInternalName;
		char modelName[64];
		char skeletonName[64];
		char internalName[64];
		char unkData[76];
	};

	struct ModelMotion {
	public:
		int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		int unknown0;
		char ASMstateName[8];
		float speed;
		int field1c;
		char unknown1[16];
	};

	struct ModelNode {
	public:
		int flags;
		char nodeName[64];
		char unknown0[12];
	};

	struct Element {
	public:
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
		int flags;
		PlayType playType;
		UpdateTiming updateTiming;
		char unknown[4];
	};

	struct Stage;

	struct FolderCondition;

	struct CameraInterpolateHelper;
}