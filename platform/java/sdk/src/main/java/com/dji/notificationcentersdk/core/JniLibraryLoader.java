package com.dji.notificationcentersdk.core;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

/**
 * JNI库加载器
 * 用于从jar包中提取并加载JNI库文件
 */
public class JniLibraryLoader {
    
    private static final String LIBRARY_NAME = "notificationcenter";
    private static final String[] LIBRARY_EXTENSIONS = {".jnilib", ".so", ".dll", ".dylib"};
    private static boolean libraryLoaded = false;
    
    /**
     * 加载JNI库
     * 首先尝试从系统路径加载，如果失败则从jar包中提取并加载
     */
    public static synchronized void loadLibrary() {
        if (libraryLoaded) {
            return;
        }
        
        try {
            // 首先尝试从系统路径加载
            System.loadLibrary(LIBRARY_NAME);
            System.out.println("从系统路径成功加载JNI库: " + LIBRARY_NAME);
            libraryLoaded = true;
            return;
        } catch (UnsatisfiedLinkError e) {
            System.out.println("从系统路径加载JNI库失败，尝试从jar包中提取: " + e.getMessage());
        }
        
        // 从jar包中提取并加载
        try {
            String extractedPath = extractLibraryFromJar();
            System.load(extractedPath);
            System.out.println("从jar包成功加载JNI库: " + extractedPath);
            libraryLoaded = true;
        } catch (Exception e) {
            throw new RuntimeException("无法加载JNI库: " + e.getMessage(), e);
        }
    }
    
    /**
     * 从jar包中提取JNI库文件
     */
    private static String extractLibraryFromJar() throws IOException {
        String osName = System.getProperty("os.name").toLowerCase();
        String arch = System.getProperty("os.arch").toLowerCase();
        
        // 确定库文件扩展名
        String extension = null;
        if (osName.contains("mac")) {
            extension = ".jnilib";
        } else if (osName.contains("linux")) {
            extension = ".so";
        } else if (osName.contains("windows")) {
            extension = ".dll";
        } else {
            extension = ".so"; // 默认
        }
        
        String libraryFileName = "lib" + LIBRARY_NAME + extension;
        String jarPath = "/lib/" + libraryFileName;
        
        // 从jar包中读取库文件
        InputStream inputStream = JniLibraryLoader.class.getResourceAsStream(jarPath);
        if (inputStream == null) {
            throw new IOException("在jar包中找不到JNI库文件: " + jarPath);
        }
        
        // 创建临时目录
        Path tempDir = Files.createTempDirectory("notificationcenter-jni");
        tempDir.toFile().deleteOnExit();
        
        // 提取库文件到临时目录
        Path libraryFile = tempDir.resolve(libraryFileName);
        Files.copy(inputStream, libraryFile, StandardCopyOption.REPLACE_EXISTING);
        inputStream.close();
        
        // 设置可执行权限（在Unix系统上）
        if (!osName.contains("windows")) {
            libraryFile.toFile().setExecutable(true);
        }
        
        return libraryFile.toAbsolutePath().toString();
    }
    
    /**
     * 检查JNI库是否已加载
     */
    public static boolean isLibraryLoaded() {
        return libraryLoaded;
    }
} 