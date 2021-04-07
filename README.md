# libfunctionpatcher
Requires the [FunctionPatcherModule](https://github.com/wiiu-env/FunctionPatcherModule) to be running via [SetupPayload](https://github.com/wiiu-env/SetupPayload).
Requires [wut](https://github.com/decaf-emu/wut) for building.
Install via `make install`.

## Usage
When linking, make sure to add the `libfunctionpatcher.ld` file to the LDFLAGS.

Example:
```
LDFLAGS	=	-g $(ARCH) $(RPXSPECS) -Wl,-Map,$(notdir $*.map) -T$(WUMS_ROOT)/share/libfunctionpatcher.ld
```

Make also sure to define 
```
WUMS_ROOT := $(DEVKITPRO)/wums
```
and add `-lfunctionpatcher` to `LIBS` and `$(WUMS_ROOT)` to `LIBDIRS`.

After that you can simply include `<function_patcher/function_patching.h>` to get access to the function patcher functions.

## Use this lib in Dockerfiles.
A prebuilt version of this lib can found on dockerhub. To use it for your projects, add this to your Dockerfile.
```
[...]
COPY --from=wiiuenv/libfunctionpatcher:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://hub.docker.com/r/wiiuenv/libfunctionpatcher/tags). 
It's highly recommended to pin the version to the **latest date** instead of using `latest`.