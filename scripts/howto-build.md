# Building WebRTC

This document shows how to prepare a WebRTC build for its inclusion in this
plugin.

The build will be made with the `build-webrtc.94.py` Python script located in the
`scripts/` directory.

## Prerequisites

One should have:
* Python 2 (at least)
* latest Google Ninja build system installed globally on the system
* C/C++ environment via build-essential package for Linux machine
* Xcode command line tools for Mac machine
* Java JDK is needed for the Linux machine

## Preparing the build

Running the script with `--setup` will download all necessary tools for building
WebRTC. The script must be run with a target directory where all WebRTC source
code and resulting build artifacts will be placed. A `build_webrtc` directory
will be created containing it all.

The setup process only needs to be carried out once.

### Create working folder

```
mkdir WEBRTC-94
```

### iOS

```
python build-webrtc.94.py --setup --ios WEBRTC-94
```

### Android

NOTE: Make sure you have the Java JDK installed beforehand. On Debian and
Ubuntu systems this can be accomplished by installing the `default-jdk-headless`
package.

```
python build-webrtc.94.py --setup --android WEBRTC-94
```

## Selecting the branch

Once the setup process has finished, the target branch must be selected, also
adding any required cherry-picks. The following example shows how the coretech MOB-5251 branch
was made:

```
cd WEBRTC-94/build_webrtc/webrtc/ios/src/
git remote set-url origin git@github.com:coretech/webrtc.git
git pull
git checkout -b MOB-5251
cd ../../../../../
```

## Building

### iOS

If you have switched branches, first run:

```
python build-webrtc.94.py --sync --ios WEBRTC-94
```

Now build it:

```
python build-webrtc.94.py --build --ios WEBRTC-94
```

The build artifacts will be located in `WEBRTC-94/build_webrtc/build/ios/`.

### Android

**NOTE**: WebRTC for Android can only be built on Linux at the moment.

If you have switched branches, first run:

```
python build-webrtc.94.py --sync --android WEBRTC-94
```

Now build it:

```
python build-webrtc.94.py --build --android WEBRTC-94
```

The build artifacts will be located in `WEBRTC-94/build_webrtc/build/android/`.
