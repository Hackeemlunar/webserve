#!/bin/bash

# 1. MULTIPLE SERVERS / DIFFERENT PORTS
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8081/
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8082/

# 2. MULTIPLE SERVERS / DIFFERENT HOSTNAMES
curl -s -o /dev/null -w "%{http_code}\n" --resolve example.com:8082:127.0.0.1 http://example.com:8082/
curl -s -o /dev/null -w "%{http_code}\n" --resolve www.example.com:8082:127.0.0.1 http://www.example.com:8082/
curl -s -o /dev/null -w "%{http_code}\n" --resolve localhost:8080:127.0.0.1 http://localhost:8080/

# 3. DEFAULT / CUSTOM ERROR PAGE (404)
curl -s http://127.0.0.1:8080/does_not_exist

# 4. CLIENT BODY LIMIT (8081 = 2MB limit)
curl -s -o /dev/null -w "%{http_code}\n" -X POST -H "Content-Type: plain/text" --data "BODY IS HERE" http://127.0.0.1:8081/
curl -s -o /dev/null -w "%{http_code}\n" -X POST -H "Content-Type: plain/text" --data "$(head -c 3000000 /dev/zero | tr '\0' a)" http://127.0.0.1:8081/

# 4b. CLIENT BODY LIMIT (test.conf /post_body = 100 bytes)
curl -s -o /dev/null -w "%{http_code}\n" -X POST --data "$(head -c 50 /dev/zero | tr '\0' a)" http://127.0.0.1:8080/post_body
curl -s -o /dev/null -w "%{http_code}\n" -X POST --data "$(head -c 200 /dev/zero | tr '\0' a)" http://127.0.0.1:8080/post_body

# 5. ROUTES TO DIFFERENT DIRECTORIES (test.conf /directory -> ./YoupiBanane)
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/directory/nop/other.pouic
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/

# 6. DEFAULT FILE FOR A DIRECTORY (test.conf index youpi.bad_extension)
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/directory/

# 7. ACCEPTED METHODS PER ROUTE (test.conf / = GET only)
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/
curl -s -o /dev/null -w "%{http_code}\n" -X POST --data x http://127.0.0.1:8080/
curl -s -o /dev/null -w "%{http_code}\n" -X DELETE http://127.0.0.1:8080/
curl -s -D - -o /dev/null -X DELETE http://127.0.0.1:8080/

# 7b. DELETE WITH / WITHOUT PERMISSION (default.conf /uploads)
curl -s -o /dev/null -w "%{http_code}\n" -X POST --data "content" http://127.0.0.1:8080/uploads/deltest.txt
curl -s -o /dev/null -w "%{http_code}\n" -X DELETE http://127.0.0.1:8080/uploads/anonymous~deltest.txt
curl -s -o /dev/null -w "%{http_code}\n" -X DELETE http://127.0.0.1:8080/uploads/ghost.txt
curl -s -o /dev/null -w "%{http_code}\n" -X DELETE http://127.0.0.1:8080/index.html

# STATUS CODE VERIFICATION (default.conf)
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/
curl -s -o /dev/null -w "%{http_code}\n" -X POST --data x http://127.0.0.1:8080/uploads/sc.txt
curl -s -o /dev/null -w "%{http_code}\n" -X DELETE http://127.0.0.1:8080/uploads/anonymous~sc.txt
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/redirect
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/nope
curl -s -o /dev/null -w "%{http_code}\n" -X DELETE http://127.0.0.1:8080/index.html
curl -s -D - -o /dev/null http://127.0.0.1:8080/nope
curl -s -D - -o /dev/null http://127.0.0.1:8080/redirect

# CGI 1+3. GET METHOD (default.conf /cgi-bin/echo.py)
curl -s -w "\n%{http_code}\n" "http://127.0.0.1:8080/cgi-bin/echo.py?foo=bar&x=1"

# CGI 2. POST METHOD (body delivered to stdin)
curl -s -w "\n%{http_code}\n" -X POST -H "Content-Type: text/plain" --data "hello from post" http://127.0.0.1:8080/cgi-bin/echo.py

# CGI 4. ENV / META-VARIABLES (default.conf /cgi-bin/env.py)
curl -s -w "\n%{http_code}\n" -X POST --data "abc" "http://127.0.0.1:8080/cgi-bin/env.py?q=1"

# CGI 5. SCRIPT ERROR HANDLING (expects 502)
curl -s -w "\n%{http_code}\n" http://127.0.0.1:8080/cgi-bin/error.py

# CGI 6. INFINITE LOOP -> TIMEOUT (expects 504, server survives)
curl -s -w "\n%{http_code}\n" --max-time 30 http://127.0.0.1:8080/cgi-bin/infinite.py
curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8080/cgi-bin/test.py
