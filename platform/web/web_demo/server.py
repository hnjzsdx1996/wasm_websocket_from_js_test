#!/usr/bin/env python3
import http.server
import socketserver
import os
import sys

class CORSHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        # 设置 CORS 头以支持 SharedArrayBuffer
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Resource-Policy', 'same-origin')
        super().end_headers()

    def log_message(self, format, *args):
        # 自定义日志格式
        sys.stderr.write("[%s] %s\n" % (self.log_date_time_string(), format % args))

def main():
    PORT = 8080
    
    # 切换到 web_demo 目录
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    with socketserver.TCPServer(("", PORT), CORSHTTPRequestHandler) as httpd:
        print(f"服务器启动在 http://localhost:{PORT}")
        print("支持 SharedArrayBuffer 的 CORS 头已设置")
        print("按 Ctrl+C 停止服务器")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n服务器已停止")

if __name__ == "__main__":
    main() 