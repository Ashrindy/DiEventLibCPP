// DiEventLibTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "dvscene.h"
#include "template\dievtdbbin.h"
#include <fstream>

int main()
{
    std::cout << "Hello World!\n";

    std::ifstream file("E:\\Steam\\steamapps\\common\\SonicFrontiers\\image\\x64\\raw\\event\\scene\\zev_blow_rifleboss\\zev_blow_rifleboss.dvscene", std::ios::binary | std::ios::ate);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    void* fileData = malloc(fileSize);
    file.read((char*)fileData, fileSize);
    file.close();
    DvScene dvscene;
    dvscene.read((const char*)fileData, fileSize);
    auto data = dvscene.write();
    std::ofstream ofile("E:\\Steam\\steamapps\\common\\SonicFrontiers\\image\\x64\\raw\\event\\scene\\zev_blow_rifleboss\\zev_blow_rifleboss.dvscene.dvscene", std::ios::binary);
    ofile.write(data.data, data.size);
    ofile.close();
    /*DiEventDataBase dievtdb;
    dievtdb.read((const char*)fileData, fileSize);*/
    /*DvScene* scene = (DvScene*)fileData;
    DvCommon* common = scene->dvCommon.get();
    auto x = scene->dvResource.get()->getItems()[0];
    auto cut = common->cuts.get()->getItems()[0];
    auto node = common->node.get();
    auto nodeData = common->node.get()->getData<Path>();
    auto childnodes = common->node.get()->getChildNodes();*/
    return 0;
}