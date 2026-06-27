# 👹 ProcGuard — A New Daemon for Your Security

> A lightweight userspace daemon written in **C** that monitors suspicious processes by watching `/proc`, without ever touching kernel mode.

---

## 🧠 What Is It?

ProcGuard is a security daemon that silently observes process behavior on a Linux system. It's written entirely in **C** with careful attention to memory safety, race conditions, and secure coding practices.

---

## ⚙️ What Does It Do?

- 🔗 **Hooks into the system via `/proc`** — fully userspace, no kernel modules needed
- 🔍 **Monitors** process forks, access to sensitive files, and privilege escalation attempts
- 🚨 **Detects anomalies**, such as:
  1. A process opening `/etc/passwd`
  2. An unexpected shell being spawned
  3. PID injection attempts
- 📝 **Writes a log file** with each entry hashed using **SHA-256** for integrity
- 📡 **Sends real-time alerts** via a Unix socket when an anomaly is detected

---

## 📐 Project Structure

| 🗃️ Data Structures & Anomaly Detection | 📋 Log File Management | 📁 `/proc` Directory Handling | 📡 Alert System & Refactoring |
|---|---|---|---|
| Uses a **HashTable** where each process has a `snap` (snapshot) attribute to detect state changes | Thread-safe functions for writing process status to the log file | Continuously reads `/proc` to screen process states; calls `send_alert()` and removes the PID on anomaly | A Unix socket dispatches alerts containing the process info that triggered the detection |

---

## ⚠️ Foreword

> In this project, an **anomaly** is defined simply as a **change of process state** — not based on what the process is doing, what it can access, its permissions, etc. In a real-world scenario, this logic would be significantly more complex.
>
> The core idea was straightforward: build a system where **every state is marked and durable**.

---

## 🎓 Credits

The **SHA-256 implementation** used in this project was not written by me — it was taken from a YouTube tutorial.
👉 [Click here to watch the video](#)

---

## 🔧 Built With

- **Language:** C
- **Monitoring:** `/proc` filesystem (userspace only)
- **Hashing:** SHA-256
- **IPC:** Unix Domain Sockets
- **Thread Safety:** Mutex-protected log writes
