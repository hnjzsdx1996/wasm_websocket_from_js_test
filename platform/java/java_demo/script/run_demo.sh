#!/bin/bash

# Navigate to the script's directory to make the script runnable from anywhere
cd "$(dirname "$0")"

# Go up to the root of the java project, which contains gradlew
cd ../..

echo ">>> Running the Java Demo..."

# Use gradle to run the application.
# The `run` task is defined in java_demo/build.gradle.
./gradlew :java_demo:run

if [ $? -ne 0 ]; then
    echo ">>> Demo finished with errors."
    exit 1
fi

echo ">>> Demo finished successfully." 