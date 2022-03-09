
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.1)

# Declares and names the project.

project("kxffmpeg")

set(SWIG_I_FILE "swig/h264player.i")

#set(JAVA_PACKAGE "com.kevin.ffmpeg")
set(JAVA_GEN_PACKAGE "com.kevin.ffmpeg")
set(JAVA_JNI_GEN_PACKAGE "com.kevin.ffmpeg.jni")
string(REPLACE "." "/" JAVA_GEN_SUBDIR ${JAVA_GEN_PACKAGE})
string(REPLACE "." "/" JAVA_JNI_GEN_SUBDIR ${JAVA_JNI_GEN_PACKAGE})

set(JAVA_GEN_DIR ${PROJECT_SOURCE_DIR}/wrapper/${JAVA_GEN_SUBDIR})
set(JAVA_JNI_GEN_DIR ${PROJECT_SOURCE_DIR}/wrapper/${JAVA_JNI_GEN_SUBDIR})

set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../libs/${ANDROID_ABI})
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR}/include)
include_directories(${ffmpeg_head_dir})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
# 添加ffmpeg相关的so库
add_library(libijkffmpeg
SHARED
IMPORTED )
set_target_properties(libijkffmpeg
PROPERTIES IMPORTED_LOCATION
${ffmpeg_lib_dir}/libijkffmpeg.so )

add_library( # Sets the name of the library.
             kxffmpeg

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
        ffmpeg_decoder.cpp
        H2645Parser/h265Parser.cpp
        H2645Parser/H264Parser.cpp
        H2645Parser/CH2645PacketParser.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       kxffmpeg

                        #连接 FFmpeg 相关的库
                        libijkffmpeg
#                        libavutil
#                        libswresample
#                        libavresample
#                        libavcodec
#                        libavfilter
#                        libswscale
#                        libavformat
#                        libavdevice
                        -landroid

        # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
#
#file(REMOVE_RECURSE ${JAVA_JNI_GEN_DIR})
# Ensure file recognized as C++ (otherwise, exported as C file)
set_property(SOURCE ${SWIG_I_FILE} PROPERTY CPLUSPLUS ON)
# Setup SWIG flags and locations
set(CMAKE_SWIG_FLAGS -c++ -package ${JAVA_JNI_GEN_PACKAGE})
#指定.java文件目录
set(CMAKE_SWIG_OUTDIR ${PROJECT_SOURCE_DIR}/../java/${JAVA_JNI_GEN_SUBDIR})
#指定wraper.cxx文件目录
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Export a wrapper file to Java, and link with the created C++ library
# 生成libdecoder_jni.so
SWIG_ADD_LIBRARY(decoder_jni TYPE MODULE LANGUAGE java SOURCES ${SWIG_I_FILE})
swig_link_libraries(decoder_jni kxffmpeg)

#add_custom_command(TARGET decoder_jni POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SWIG_OUTDIR} ${PROJECT_SOURCE_DIR}/../java/${JAVA_JNI_GEN_SUBDIR})
