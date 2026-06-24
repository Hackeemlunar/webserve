#!/usr/bin/env python3

import os
import sys

body = sys.stdin.read()

print("Content-Type: text/plain")
print()
print("method=" + os.environ.get("REQUEST_METHOD", ""))
print("query=" + os.environ.get("QUERY_STRING", ""))
print("content_length=" + os.environ.get("CONTENT_LENGTH", ""))
print("content_type=" + os.environ.get("CONTENT_TYPE", ""))
print("server_name=" + os.environ.get("SERVER_NAME", ""))
print("server_port=" + os.environ.get("SERVER_PORT", ""))
print("body=" + body)
