:: Set android ndk root
@ECHO OFF
@SETLOCAL

d:
cd D:\Tengine

:: Notice that ANDROID_NDK must be set as below
@SET ANDROID_NDK=D:/study/android/android-ndk-r18b
:: @SET ANDROID_NDK="D:\Program\android\NDK\android-ndk-r18b"

:: Notice that cmake should be found in %PATH%, if not you can set %PATH% as below
@SET PATH=D:\360_download\cmake-3.18.2-win64-x64\bin;%PATH%
:: @SET PATH="D:\Program\CMake\bin";%PATH%

:: android armv7
mkdir build-android-armv7
pushd build-android-armv7
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%/build/cmake/android.toolchain.cmake -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_ABI="armeabi-v7a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-21 ..
cmake --build . --parallel %NUMBER_OF_PROCESSORS%
cmake --build . --target install
popd

:: android aarch64
mkdir build-android-aarch64
pushd build-android-aarch64
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%/build/cmake/android.toolchain.cmake -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-24 ..
cmake --build . --parallel %NUMBER_OF_PROCESSORS%
cmake --build . --target install
popd

:: android x86
mkdir build-android-x86
pushd build-android-x86
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%/build/cmake/android.toolchain.cmake -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_ABI="x86" -DANDROID_PLATFORM=android-19 ..
cmake --build . --parallel %NUMBER_OF_PROCESSORS%
cmake --build . --target install
popd

:: android x86_64
mkdir build-android-x86_64
pushd build-android-x86_64
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%/build/cmake/android.toolchain.cmake -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_ABI="x86_64" -DANDROID_PLATFORM=android-21 ..
cmake --build . --parallel %NUMBER_OF_PROCESSORS%
cmake --build . --target install
popd

@ENDLOCAL
