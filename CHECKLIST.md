# Webserv Mandatory Checklist

This checklist tracks only mandatory requirements (no bonus scope).

## Done

- [x] Build with C++98 flags and strict warnings (`-Wall -Wextra -Werror -std=c++98`)
- [x] Config file loading from argument or default path
- [x] Basic config parsing for:
  - [x] `server` blocks
  - [x] `location` blocks
  - [x] `listen`, `server_name`, `root`, `index`, `client_max_body_size`, `error_page`
  - [x] `allowed_methods`, `autoindex`, `upload_path`, `return`, `cgi_extension`
- [x] Non-blocking listening sockets
- [x] Single `select()`-based event loop for socket I/O
- [x] Read/write operations on sockets gated by readiness (`FD_ISSET`)
- [x] Accept client connections and manage connection lifecycle
- [x] Basic client timeout handling
- [x] HTTP request parsing baseline:
  - [x] request line
  - [x] headers
  - [x] body via `Content-Length`
  - [x] chunked completeness detection (basic)
- [x] HTTP response building baseline:
  - [x] status line
  - [x] headers
  - [x] body serialization
  - [x] common status messages
- [x] Route matching (longest prefix style)
- [x] Method restriction checks from route config
- [x] GET static file serving baseline
- [x] Basic autoindex HTML generation
- [x] Basic redirect response (`301` with `Location`)
- [x] Basic error responses (`404`, `405`, etc.)
- [x] Smoke-tested with `curl` for:
  - [x] `GET /` returns `200`
  - [x] missing file returns `404`
  - [x] disallowed method returns `405`

## Left To Do (Mandatory)

### Core HTTP correctness

- [ ] Strict request validation (malformed request line/headers -> `400`)
- [ ] Correct header validation rules for HTTP/1.1 (`Host` handling, duplicates, invalid forms)
- [ ] Proper chunked transfer decoding (not only completeness detection)
- [ ] Better request state handling for partial/incremental reads and pipelining safety

### Methods and behavior

- [ ] Full `POST` support
- [ ] Full `DELETE` support
- [ ] Implement request body size enforcement (`client_max_body_size` -> `413`)
- [ ] Correct method behavior per route and per resource (status code accuracy)

### Uploads

- [ ] Implement actual upload write flow to configured `upload_path`
- [ ] Validate upload errors and return proper status codes

### CGI (mandatory)

- [ ] Implement CGI request detection and dispatch
- [ ] Fork/exec CGI process correctly
- [ ] Build CGI environment variables from request/server context
- [ ] Forward request body to CGI when required
- [ ] Parse CGI output into valid HTTP response
- [ ] Add CGI timeout and failure handling

### Static files and routing completeness

- [ ] Harden filesystem path resolution against traversal edge cases
- [ ] Finalize index resolution behavior for directories
- [ ] Ensure autoindex behavior is fully compliant with route config
- [ ] Improve redirection handling semantics (status variants and headers)

### Error pages and status accuracy

- [ ] Serve configured custom error pages consistently
- [ ] Audit and correct status codes for all edge cases

### Connection management and resilience

- [ ] Keep-alive lifecycle correctness across multiple requests per connection
- [ ] Ensure requests never hang indefinitely (timeouts + parse failure exits)
- [ ] Robust cleanup on disconnects, short reads/writes, and socket errors

### Multi-server / listen behavior

- [ ] Handle duplicate or overlapping listen definitions more cleanly
- [ ] Ensure expected behavior when multiple server configs share address/port

### Mandatory quality and evaluation readiness

- [ ] Compare core behaviors against NGINX for parity in key scenarios
- [ ] Add repeatable test checklist/scripts for evaluator demos (GET/POST/DELETE, uploads, CGI, error pages)
- [ ] Stress test for stability under concurrent clients
- [ ] Verify no blocking socket I/O occurs outside readiness checks
- [ ] Validate full mandatory path end-to-end with provided config files

## Explicitly Out of Scope

- [ ] Bonus features (cookies, sessions, extra CGI types)
