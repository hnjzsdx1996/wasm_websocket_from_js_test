#!/bin/bash

# Go to the script's directory (platform/java/java_demo)
cd "$(dirname "$0")"

# Check if gradlew exists
if [ ! -f "./gradlew" ]; then
    # If not, try to find it in the parent directory
    if [ -f "../gradlew" ]; then
        # Use the parent directory's gradlew to run the task
        ../gradlew -p . run
        exit 0
    else
        echo "Gradle wrapper not found. Please run 'gradle wrapper' in this directory or the parent directory."
        exit 1
    fi
fi

# Run the 'run' task
./gradlew run
