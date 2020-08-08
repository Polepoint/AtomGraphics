# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_definitions(
        -Wall
        -Wfatal-errors
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB native_srcs
        "*.cpp"
        "android/*.cpp"
        "base/*.cpp"
        "base/android/*.cpp"
        "cache/*.cpp"
        "cache/android/*.cc"
        "gpu/*.cpp"
        "graphics/*.cpp"
        "graphics/android/*.cpp"
        "graphics/android/*.cc"
        "graphics/android/skia/*.cpp"
        "graphics/font/*.cpp"
        "graphics/font/android/*.cc"
        "io/*.cpp"
        "io/android/*.cc"
        "math/*.cpp"
        "math/android/*.cc"
        "memory/*.cpp"
        "node/*.cpp"
        "node/canvas/*.cpp"
        "node/canvas/android/*.cpp"
        "node/canvas/android/*.cc"
        "node/canvas/android/skia/*.cpp"
        "platform/*.cpp"
        "javascript/*.cpp"
        "javascript/android/*.cpp"
        "javascript/v8/*.cpp"
        "thread/*.cpp"
        "thread/android/*.cpp")

add_library(
        atomgraphics
        SHARED
        ${native_srcs})


set(THRID_PARTY_V8 ${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8)
set(THRID_PARTY_SKIA ${CMAKE_CURRENT_SOURCE_DIR}/third_party/skia)
set(SK_HEADER ${THRID_PARTY_SKIA}/skia)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${THRID_PARTY_V8}
        ${THRID_PARTY_V8}/v8
        ${THRID_PARTY_SKIA}
        ${SK_HEADER}/android
        ${SK_HEADER}/atlastext
        ${SK_HEADER}/c
        ${SK_HEADER}/codec
        ${SK_HEADER}/config
        ${SK_HEADER}/core
        ${SK_HEADER}/effects
        ${SK_HEADER}/encode
        ${SK_HEADER}/gpu
        ${SK_HEADER}/pathops
        ${SK_HEADER}/ports
        ${SK_HEADER}/private
        ${SK_HEADER}/svg
        ${SK_HEADER}/utils
)

target_link_libraries(
        atomgraphics
        z
        EGL
        GLESv2
        log
        android
        jnigraphics
        ${THRID_PARTY_SKIA}/libs/${ANDROID_ABI}/libskia.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libinspector.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_libplatform.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_initializers.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_init.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_base.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_libsampler.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_libbase.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libv8_snapshot.a
        ${THRID_PARTY_V8}/libs/${ANDROID_ABI}/libtorque_generated_initializers.a
)
