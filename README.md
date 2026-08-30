# EXECUTABLE PROTECTION SYSTEM

## Operating Systems Project – Team 14

---

## 1. Project Title

**Executable Protection System**

---

## 2. Team Details

**Team Number:** 14

### Team Members

 1  PRAVEEN - 2520030406
 2  DHARANI DHAR - 2520030163
 3  RISHI - 2520030268

**Supervisor:**   K.HARIKA DEVI

---

## 3. Abstract

An executable file contains machine-level instructions that are loaded into memory and executed by a process. During execution, it is important that the executable file remains consistent and is not modified unexpectedly by another process. If an executable is modified while it is being executed, it may lead to inconsistent program behavior, corrupted execution, or security and reliability issues.

The **Executable Protection System** is an Operating Systems project designed to demonstrate how the Linux operating system protects an executable file while it is actively being executed. The project creates a sample executable program and runs it as a process. While the executable is running, another process attempts to open and modify the same executable file.

The project uses Linux system calls such as `open()`, `write()`, and `close()` to perform file operations and observe how the operating system handles modification attempts on an active executable. The return values of these system calls are checked to determine whether the requested operation was successful or rejected by the operating system.

Error handling is an important part of the demonstration. The project uses `errno` and related error-reporting mechanisms to identify and display the reason when a system call fails. This allows the behavior of the Linux kernel to be observed from the application level.

The project also demonstrates the relationship between **process execution, file operations, system calls, and kernel-level protection**. By comparing successful file operations with an attempted modification of an active executable, the project provides a practical understanding of how an Operating System maintains the consistency and integrity of executable programs.

Overall, this project provides a simple and practical demonstration of executable file protection in Linux and helps understand how the operating system controls file access while programs are being executed.

---

## 4. Problem Statement

A Linux system must ensure that important executable programs remain consistent while they are being executed. Allowing another process to overwrite or modify an executable that is currently in use may result in corrupted or inconsistent program execution.

Therefore, this project aims to demonstrate how Linux handles an attempt to modify an executable file while it is actively running and how the operating system prevents or restricts such modifications.

---

## 5. Objectives

The main objectives of this project are:

- To create a sample executable program in C.
- To execute the sample executable in a Linux environment.
- To attempt to modify the executable while it is running.
- To observe the response of the Linux operating system.
- To demonstrate the behavior of write operations on an active executable.
- To use Linux system calls for opening, writing, and closing files.
- To check the return values of system calls.
- To use `errno` to identify the reason for failed operations.
- To study the role of the Linux kernel in protecting active executable files.
- To understand the relationship between processes, files, and system calls.
- To document and analyze the observed results.

---

## 6. Technologies Used

- **Programming Language:** C
- **Operating System:** Linux / Ubuntu
- **Compiler:** GCC
- **System Calls:** `open()`, `write()`, `close()` and process-related system calls
- **Error Handling:** `errno`
- **Version Control:** Git
- **Repository:** GitHub

---

## 7. System Working

The project follows the following general process:

1. A sample C program is created.
2. The program is compiled using GCC to generate an executable file.
3. The executable is started and kept running.
4. A separate process attempts to open the running executable for modification.
5. A write operation is attempted on the executable.
6. The operating system processes the request.
7. The return value of the system call is checked.
8. If the operation fails, `errno` is used to determine the reason.
9. The result is displayed and documented.
10. The behavior is analyzed to understand Linux executable protection.

---

## 8. System Calls and Concepts

### `open()`

The `open()` system call is used to open a file and obtain a file descriptor. Different access modes can be specified depending on whether the file needs to be read or written.

### `write()`

The `write()` system call is used to write data to a file using its file descriptor. In this project, it is used to demonstrate the attempt to modify an executable file.

### `close()`

The `close()` system call releases the file descriptor after the file operation is completed.

### `errno`

`errno` is used to identify the error associated with a failed system call. The project uses it to understand why an attempted file operation was unsuccessful.

### Process Execution

The project also demonstrates how an executable becomes associated with a running process and how file operations performed by another process are handled by the operating system.

---

## 9. Project Structure

```text
Executable-Protection-System/
│
├── README.md
│
├── src/
│   └── Source code files
│
├── docs/
│   └── Project documentation
│
├── data/
│   └── Data source information
│
├── results/
│   └── Program outputs and screenshots
│
└── reports/
    └── Project reports and review documents
