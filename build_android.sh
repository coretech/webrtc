export PATH=/home/nutshell/projects/tryout/depot_tools:$PATH

gn gen out/Release-arm --args='is_component_build=false rtc_libvpx_build_vp9=true is_debug=false target_cpu="arm" target_os="android"'

gn gen out/Release-arm64 --args='is_component_build=false rtc_libvpx_build_vp9=true is_debug=false target_cpu="arm64" target_os="android"'

gn gen out/Release-x86 --args='is_component_build=false rtc_libvpx_build_vp9=true is_debug=false target_cpu="x86" target_os="android"'

gn gen out/Release-x64 --args='is_component_build=false rtc_libvpx_build_vp9=true is_debug=false target_cpu="x64" target_os="android"'

ninja -C out/Release-arm libwebrtc libjingle_peerconnection_so

ninja -C out/Release-arm64 libwebrtc libjingle_peerconnection_so

ninja -C out/Release-x86 libwebrtc libjingle_peerconnection_so

ninja -C out/Release-x64 libwebrtc libjingle_peerconnection_so

cp out/Release-arm/lib.java/sdk/android/libwebrtc.jar ../../build

cp out/Release-arm/libjingle_peerconnection_so.so ../../build/lib/armeabi-v7a
cp out/Release-arm64/libjingle_peerconnection_so.so ../../build/lib/arm64-v8a
cp out/Release-x86/libjingle_peerconnection_so.so ../../build/lib/x86
cp out/Release-x64/libjingle_peerconnection_so.so ../../build/lib/x86_64

cd ../../build

jar cvfM libjingle_peerconnection.so.jar lib
