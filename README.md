# Trak 🚀

**Trak** is a lightweight, Git-inspired version control system built from scratch in **C++**. It’s designed to help you manage your projects with core VCS features like initializing repos, staging files, committing changes, viewing history, and checking out past versions—all with a simple, one-click installer for easy setup. Whether you’re a developer looking to explore VCS internals or just want a minimal tool for version control, Trak has you covered! 💻

---

## ✨ Features

- **Core VCS Commands**:
  - `init`: Start a new repository
  - `add <file>`: Stage a file for commit
  - `commit -m <msg>`: Save your changes with a message
  - `log`: View commit history
  - `checkout <hash>`: Restore a past commit
- **Built in C++**: Fast and efficient with filesystem handling 🛠️
- **Easy Setup**: Single-file installer (`trak-setup.exe`) adds Trak to your PATH ⚙️
- **Open Source**: Fork, contribute, and make it your own! 🌟

---

## 📂 Folder Structure

Here’s how the repository is organized:

```
trak-vc/
├── trak.cpp          # Main source code for Trak
├── trak.exe          # Compiled binary (Windows)
├── setup.bat         # Installation script for the SFX installer
├── config.txt        # Configuration for the 7-Zip SFX installer
├── trak-v1.0.7z      # Intermediate archive for the installer
├── trak-setup.exe    # Final single-file installer for Windows
└── README.md         # You're reading it! 📖
```

---

## 🛠️ Installation

### For Windows Users
1. **Download the Installer**:
   - Grab `trak-setup.exe` from the [Releases](https://github.com/AimeKelvin/trak-vc/releases) page.
2. **Run the Installer**:
   - Right-click `trak-setup.exe` and select "Run as Administrator".
   - Click "Install" to set up Trak in `C:\Users\<your-username>\bin` and add it to your PATH.
3. **Start Using Trak**:
   - Open a new Command Prompt and type `trak` to verify it’s working.

### Building from Source
1. **Clone the Repo**:
   ```bash
   git clone https://github.com/AimeKelvin/trak-vc.git
   cd trak-vc
   ```
2. **Compile**:
   - You’ll need a C++17-compatible compiler (e.g., g++ with MinGW on Windows).
   ```bash
   g++ trak.cpp -o trak -std=c++17 -lstdc++fs -O2
   ```
3. **Run**:
   - Move `trak.exe` to a directory in your PATH, or run it directly:
     ```bash
     ./trak
     ```

---

## 🚀 Usage

Once installed, Trak is ready to use from any Command Prompt! Here are the commands:

- **Initialize a Repository**:
  ```bash
  trak init
  ```
- **Stage a File**:
  ```bash
  trak add myfile.txt
  ```
- **Commit Changes**:
  ```bash
  trak commit -m "My first commit"
  ```
- **View Commit History**:
  ```bash
  trak log
  ```
- **Restore a Commit**:
  ```bash
  trak checkout <commit-hash>
  ```

Run `trak` without arguments to see the usage message:
```
Usage: trak <command> [args]
```

---

## 🤝 Contributing

We’d love your help to make Trak even better! Here’s how to contribute:

1. **Fork the Repo**:
   - Click the "Fork" button at the top of this page.
2. **Clone Your Fork**:
   ```bash
   git clone https://github.com/AimeKelvin/trak-vc.git
   ```
3. **Make Changes**:
   - Add new features, fix bugs, or improve the code.
4. **Push and Create a Pull Request**:
   - Push your changes to your fork and submit a PR to this repo.

Ideas for contributions:
- Add new commands like `status` or `diff`.
- Improve the installer with a custom icon.
- Create a Linux/macOS version of the installer.

---

## 📜 License

This project is licensed under the MIT License—see the [LICENSE](LICENSE) file for details. Basically, you’re free to use, modify, and share Trak, as long as you include the license and copyright notice. 📝

---

## 🌟 Acknowledgments

- Built with ❤️ by [Aime Kelvin](https://github.com/AimeKelvin).
- Inspired by Git and the open-source community.
- Thanks to my network for the support and feedback! 🙌

Got questions or ideas? Open an issue or reach out—I’d love to hear from you! 💬

---

### ⭐ **If you find this project useful, please [star the repo](https://github.com/AimeKelvin/trak-vc) and follow me on GitHub! Your support means a lot!** 🚀
