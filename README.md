# Renegade Engine

**Renegade** is a fast, modular C++20 game engine meticulously built from scratch for developers who demand **flexibility, uncompromising performance, and absolute control.** Tired of bloated abstractions and rigid frameworks, we engineered Renegade to provide minimal layers, clean APIs, and a complete understanding of what's under the hood.

This isn't just another engine; it's a foundation for crafting high-performance games with a focus on core engineering excellence.

---

### **Key Features**

* **Reflection System:** A fully custom, static-init driven reflection system with **zero RTTI and no generated files**. Powers dynamic data access with minimal overhead.
* **Binary Serialization:** Robust, pointer-safe, and editor-ready. Supports future type expansions by default for seamless data persistence.
* **Entity-Component System (ECS):** A lightweight, scalable, and clean ECS designed for optimal cache performance and flexible game logic.
* **Event-Driven Core:** A type-safe, callback-based, and fully decoupled event system for clean communication between engine modules.
* **ImGui-Based Editor Tools:** Real-time inspector UI and tooling powered by the engine's integrated reflection system.

---

### **Tech Stack**

* **Language:** C++20
* **Platform:** Windows (Linux support planned)
* **Development Environment:** Visual Studio 2022
* **Dependencies:** ImGui, spdlog, stb, assimp, GLFW, GLAD

---

### **Currently In Progress**

* 3D OpenGL Renderer (Deferred pipeline)
* Bullet Physics Integration
* Robust Asset Management (with automatic unloading)
* ECS Memory Pooling
* Hot-reloading via DLLs
* Audio System

---

### **License**

Renegade Engine is released under the Apache 2.0 License.
