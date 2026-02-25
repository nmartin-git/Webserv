# WebServ


*This project has been created as part of the 42 curriculum by efranco, joudafke and nmartin.*


## Description

**Webserv** is a custom HTTP web server written from scratch in C++98.
The goal of this project is to understand the underlying mechanics of web servers by implementing a fully non-blocking I/O server (using `poll`). It parses configuration files, handles multiple concurrent clients, file uploads, executes CGI scripts and session management through cookies.

The server hosts a dynamic task management application called "Nicelife" that allows users to:
- **Authenticate** with login/logout functionality and session persistence
- **Manage tasks** with create, read, update, and delete operations
- **Upload files** through HTTP POST requests
- **Interact** with a responsive web interface built with HTML, CSS, and JavaScript

### Project Goal

The primary goal of this project is to gain a deep understanding of:
- Low-level network programming with sockets and `poll()`
- HTTP/1.1 protocol structure, parsing, and response generation
- CGI interface implementation for dynamic content
- Session management and cookie handling
- Concurrent connection management without threading
- Building a production-ready web application from scratch

## Instructions

### Compilation

Clone the repository and compile the project using the provided Makefile:

```bash
git clone https://github.com/EmkFrc/Nicelife.git
cd Nicelife
make
```

This will:
- Compile all C++ source files with flags: `-Wall -Werror -Wextra -std=c++98 -g3`
- Create the `webserv` executable
- Generate object files in the `objs/` directory

### Running the Server

Start the web server:

```bash
./webserv [config_file.conf]
```

The server will listen on the configured port (default: typically 8080).

### Accessing the Application

Once the server is running, open your web browser and navigate to:

```
http://localhost:8080/
```

You will see the **Nicelife** homepage.

## Technical Choices

### Architecture Decisions

- **Event-Driven I/O**: Uses `poll()` instead of `select()` for better scalability and efficiency
- **Non-blocking Sockets**: All socket operations are non-blocking to prevent server stalls
- **State Machine**: Each client connection has a state (RECEIVING, READY, EXECUTING_CGI, SENDING, DONE)
- **Modular Design**: Separation of concerns between request parsing, response building, and CGI execution
- **Header-Only Utilities**: Inline functions in headers for cookie handling, string parsing, and CGI utilities

### HTTP Implementation

- **Request Parsing**: Manual parsing of HTTP request lines, headers, and body
- **Response Building**: Dynamic response construction with proper headers (Content-Type, Content-Length, Set-Cookie)
- **Content-Type Detection**: Automatic MIME type detection based on file extensions
- **Status Codes**: Comprehensive HTTP status code support (200, 201, 204, 301, 302, 400, 401, 403, 404, 500, 502, 503)

### CGI Execution

- **Fork/Exec Model**: CGI scripts are executed in child processes via `fork()` and `execve()`
- **Pipe Communication**: Output from CGI scripts is captured through pipes
- **Environment Variables**: CGI environment is set according to RFC 3875 (QUERY_STRING, REQUEST_METHOD, etc.)
- **Non-blocking Read**: CGI output is read asynchronously to prevent blocking the server

### Session Management

- **Cookie-Based Authentication**: Sessions are maintained using secure HTTP cookies
- **Cookie Attributes**: Support for Max-Age, Path, Domain, HttpOnly, Secure, and SameSite attributes
- **Request Parsing**: Custom cookie parser that extracts and stores cookie key-value pairs

## Resources

- https://www.cplusplus.com/reference/
- https://www.geeksforgeeks.org/socket-programming-cc/

### AI Usage

**AI was used for the following tasks:**

1. Research and Documentation

2. Debugging Assistance

3. Code Structure Planning

4. Testing
