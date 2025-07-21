#!/bin/bash

# go to the script's directory
cd "$(dirname "$0")"
# go to root directory, which is now two levels up
cd ../../..

# --- Configuration ---
BUILD_DIR="build_java"
DIST_DIR="platform/java/dist"
ROOT_DIR=$(pwd)

# --- Clean up previous builds ---
echo "Cleaning up previous build directories..."
rm -rf $BUILD_DIR
rm -rf $DIST_DIR

# --- Create directories ---
mkdir -p $BUILD_DIR
mkdir -p $DIST_DIR

# --- Build JNI Library using CMake ---
echo "Building JNI library with CMake..."
cd $BUILD_DIR
cmake .. -DUSE_LIBWEBSOCKETS=ON
make -j8 # You can adjust the number of jobs

# Check if make was successful
if [ $? -ne 0 ]; then
    echo "CMake build failed. Exiting."
    exit 1
fi

echo "JNI library built successfully."
cd $ROOT_DIR

# --- Find and copy the JNI library ---
# The actual name of the library might differ based on OS (e.g., .so, .dylib, .dll)
JNI_LIB_NAME="libnotificationcenter_jni.so" # Default to .so
if [[ "$OSTYPE" == "darwin"* ]]; then
    JNI_LIB_NAME="libnotificationcenter_jni.dylib"
fi
# Add more conditions for other OS if needed, like Windows (.dll)

JNI_LIB_PATH=$(find "$BUILD_DIR" -name "$JNI_LIB_NAME")

if [ -z "$JNI_LIB_PATH" ]; then
    echo "Error: Could not find the built JNI library ($JNI_LIB_NAME)."
    exit 1
fi

echo "Copying JNI library to $DIST_DIR..."
cp "$JNI_LIB_PATH" "$DIST_DIR/"

# --- Build Java JAR using Gradle ---
echo "Building Java JAR with Gradle..."
cd platform/java
./gradlew clean build

# Check if gradle build was successful
if [ $? -ne 0 ]; then
    echo "Gradle build failed. Exiting."
    exit 1
fi

# --- Find and copy the JAR file ---
echo "--- Debug Info ---"
echo "Current directory for JAR copy: $(pwd)"
echo "Listing contents of 'build/libs/':"
ls -l build/libs

JAR_PATH=$(find build/libs -name "*.jar")

if [ -z "$JAR_PATH" ]; then
    echo "Error: Could not find the built JAR file."
    exit 1
fi

echo "Found JAR file path: $JAR_PATH"
echo "Listing contents of destination 'dist/':"
ls -l dist

echo "Executing: cp '$JAR_PATH' dist/"
cp "$JAR_PATH" dist/
echo "--- End Debug Info ---"

cd $ROOT_DIR

echo "Build complete. Artifacts are in $DIST_DIR" 