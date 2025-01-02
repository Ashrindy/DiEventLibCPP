# `DiEventLibCPP (C++ Static Library)`

**_C++ static library for HE2 (.dvscene)_**

## 📜 Description 📜

A library with simple reading and writing function for the .dvscene file from HE2
</br>
</br>
<b>Huge credit goes to <a href="https://github.com/ik-01">ik-01</a>, who did the research of the .dvscene files, without his research and help this library wouldn't exist.</b>

## 🗂️ Projects 🗂️

- DiEventLib - The actual C++ static library itself.

## Setting up the development environment

### Requirements

- CMake 3.27 or higher

```sh
cmake -B build
```

## 📝 Documentation 📝

### Reading

```c++
dv::DvScene dvscene;
dvscene.read(fileData, fileSize);
```

### Template Reading (still W.I.P)

For template reading a dievtdb file is required (or its .json version).

```c++
dv::db::DiEventDataBase dievtdb;
dievtdb.read(fileData, fileSize);
std::map<std::string, dv::DvNode::Field> data = dvscene.dvCommon->node->getTemplateData(dievtdb);
```

### Writing

```csharp
dv::internal::Buffer newData = dvscene.write();
```
