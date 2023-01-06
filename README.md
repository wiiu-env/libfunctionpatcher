[![Publish Docker Image](https://github.com/wiiu-env/libfunctionpatcher/actions/workflows/push_image.yml/badge.svg)](https://github.com/wiiu-env/libfunctionpatcher/actions/workflows/push_image.yml)

# libfunctionpatcher
Requires the [FunctionPatcherModule](https://github.com/wiiu-env/FunctionPatcherModule) to be running via [WUMSLoader](https://github.com/wiiu-env/WUMSLoader).
Requires [wut](https://github.com/devkitPro/wut) for building.
Install via `make install`.

## Usage
Make also sure to define 
```
WUMS_ROOT := $(DEVKITPRO)/wums
```
and add `-lfunctionpatcher` to `LIBS` and `$(WUMS_ROOT)` to `LIBDIRS`.

After that you can simply include `<function_patcher/function_patching.h>` to get access to the function patcher functions after calling `FunctionPatcher_InitLibrary()`.

## Use this lib in Dockerfiles.
A prebuilt version of this lib can found on dockerhub. To use it for your projects, add this to your Dockerfile.
```
[...]
COPY --from=wiiuenv/libfunctionpatcher:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://hub.docker.com/r/wiiuenv/libfunctionpatcher/tags). 
It's highly recommended to pin the version to the **latest date** instead of using `latest`.

## Format the code via docker
`docker run --rm -v ${PWD}:/src wiiuenv/clang-format:13.0.0-2 -r ./source ./include -i`
