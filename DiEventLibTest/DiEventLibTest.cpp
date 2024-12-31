// DiEventLibTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "dvscene.h"
#include "template\dievtdbbin.h"
#include <fstream>

int main()
{
    std::cout << "Hello World!\n";
    std::string filepath;
    std::getline(std::cin, filepath);

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    char* fileData = (char*)malloc(fileSize);
    file.read(fileData, fileSize);
    file.close();
    dv::DvScene dvscene;
    dvscene.read(fileData, fileSize);

    std::cout << "Template\n";
    std::getline(std::cin, filepath);

    file = std::ifstream(filepath, std::ios::binary | std::ios::ate);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    fileData = (char*)malloc(fileSize);
    file.read(fileData, fileSize);
    file.close();

    dv::db::DiEventDataBase dievtdb;
    dievtdb.read((const char*)fileData, fileSize);

    auto templateData = dvscene.dvCommon->node->childNodes[1].getTemplateData(dievtdb);
    auto data = dvscene.dvCommon->node->childNodes[1].getData<dv::nodes::rangers::Element>();

    auto newData = dvscene.write();
    std::ofstream ofile(std::string(filepath + ".dvscene"), std::ios::binary);
    ofile.write(newData.data, newData.size);
    ofile.close();

    /*DvScene* scene = (DvScene*)fileData;
    DvCommon* common = scene->dvCommon.get();
    auto x = scene->dvResource.get()->getItems()[0];
    auto cut = common->cuts.get()->getItems()[0];
    auto node = common->node.get();
    auto nodeData = common->node.get()->getData<Path>();
    auto childnodes = common->node.get()->getChildNodes();*/
    return 0;
}