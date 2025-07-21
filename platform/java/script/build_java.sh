#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# --- Configuration ---
# Get the root directory of the project
ROOT_DIR=$(git rev-parse --show-toplevel)
BUILD_DIR="$ROOT_DIR/build_java"
DIST_DIR="$ROOT_DIR/platform/java/dist"
PLATFORM_JAVA_DIR="$ROOT_DIR/platform/java"

# --- Cleanup ---
echo ">>> Cleaning up previous build directories..."
rm -rf "$BUILD_DIR"
rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"

# --- Build JNI Library using CMake ---
echo ">>> Building JNI library with CMake..."
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -G "Unix Makefiles" -DUSE_LIBWEBSOCKETS=ON
cmake --build "$BUILD_DIR" -- -j8

echo ">>> JNI library built successfully."

# --- Find and copy the JNI library ---
# On macOS, the swig-generated JNI lib is .jnilib
JNI_LIB_NAME="libnotificationcenter.jnilib"
JNI_LIB_PATH=$(find "$BUILD_DIR" -name "$JNI_LIB_NAME")

if [ -z "$JNI_LIB_PATH" ]; then
    echo ">>> Error: Could not find the built JNI library ($JNI_LIB_NAME)."
    exit 1
fi

echo ">>> Copying JNI library to $DIST_DIR..."
cp "$JNI_LIB_PATH" "$DIST_DIR/"

# --- Build Java JAR using Gradle ---
echo ">>> Building Java JAR with Gradle..."
cd "$PLATFORM_JAVA_DIR"
./gradlew clean build

cd "$ROOT_DIR"

# --- Post-build Info ---
echo ">>> Build complete. Artifacts are in $DIST_DIR"
ls -l "$DIST_DIR" 