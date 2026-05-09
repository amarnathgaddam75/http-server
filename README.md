# HTTP Server in C

A simple HTTP server built from scratch in C using sockets.

## Features
- Handles HTTP GET requests
- Serves static HTML files
- Parses HTTP request headers
- File handling for webpage serving
- Basic modular architecture

## Technologies
- C
- Linux / WSL
- BSD Sockets
- GCC

## Project Structure
server.c          -> Socket setup and connection handling
http_parser.c     -> HTTP request parsing
file_handler.c    -> Reading and serving files

## How to Run

Compile:
gcc server.c http_parser.c file_handler.c -o server

Run:
./server

Open browser:
http://localhost:8080

## What I Learned
- Socket programming
- HTTP fundamentals
- Client-server architecture
- Low-level systems programming
- File I/O in C
