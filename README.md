# `DiEventLibCPP (C++ Static Library)`

**_C++ static library for HE2 (.dvscene)_**

## Description

A library with simple reading and writing function for the .dvscene file from HE2
</br>
</br>
<b>Huge credit goes to <a href="https://github.com/ik-01">ik-01</a>, who did the research of the .dvscene files, without his research and help this library wouldn't exist.</b>

## Projects

- DiEventLib - The actual C++ static library itself.
- dvscene-lib-test - Testing grounds for the C++ static library.

## Building

### Requirements

- CMake 3.27 or higher

## Documentation

### Reading

```cpp
dv::DvScene dvscene;
dvscene.read(fileData, fileSize);
```

### Writing

```cpp
dv::internal::Buffer newData = dvscene.write();
```
