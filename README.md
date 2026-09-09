# C++ Multithreaded Task Processor

A producer/consumer task queue demonstrating:
- `std::thread`
- `std::mutex`
- `std::condition_variable`
- Graceful shutdown
- Work distribution across workers

This is intentionally small but interview-friendly.

## Build
```bash
cmake -S . -B build
cmake --build build
./build/task_processor
```

Personal portfolio project demonstrating concurrency and systems programming fundamentals.