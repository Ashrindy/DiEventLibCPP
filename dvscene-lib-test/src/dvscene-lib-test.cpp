#include <dieventlib/dvscene.h>

int main() {
	FILE* file = fopen("E:\\Steam\\steamapps\\common\\SonicFrontiers\\image\\x64\\raw\\event\\scene\\bo1xxx\\bo1120.dvscene", "rb");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char* buffer = new char[size];
    fread(buffer, 1, size, file);
    fclose(file);

    dv::DvScene* dvscene = new dv::DvScene{};
    dvscene->read(buffer, size);
    printf("count: %zu\n", dvscene->dvCommon->node->childNodes.size());

    auto* pathData = dvscene->dvCommon->node->getData<dv::nodes::rangers::Path>();
    printf("mat:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
        pathData->world.m[0][0], pathData->world.m[0][1], pathData->world.m[0][2], pathData->world.m[0][3],
        pathData->world.m[1][0], pathData->world.m[1][1], pathData->world.m[1][2], pathData->world.m[1][3],
        pathData->world.m[2][0], pathData->world.m[2][1], pathData->world.m[2][2], pathData->world.m[2][3],
        pathData->world.m[3][0], pathData->world.m[3][1], pathData->world.m[3][2], pathData->world.m[3][3]
    );

    delete dvscene;

	return 0;
}
