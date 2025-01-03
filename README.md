# Password Strength Checker (C & Ncurses)

A **Password Strength Checker** implemented in the **C programming language**, featuring a **text-based GUI** using the **Ncurses library**. This program evaluates the strength of a user-provided password and provides constructive feedback to improve it.

---

## Features

- **Password Analysis**:
  - Checks for minimum length.
  - Counts uppercase letters, lowercase letters, digits, and special characters.
  - Provides a strength evaluation (Weak, Moderate, Strong).

- **GUI with Ncurses**:
  - Interactive terminal-based interface.
  - Real-time suggestions for improving password strength.

- **Cross-Platform Compatibility**:
  - Designed for Linux and macOS systems.

---

## Installation

### Prerequisites

- **C Compiler** (e.g., GCC)
- **Ncurses Library**

### macOS
1. Install **Homebrew** if not already installed:
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. Install Ncurses using Homebrew:
   ```bash
   brew install ncurses
   ```

### Linux
1. Install Ncurses:
   ```bash
   sudo apt-get update && sudo apt-get install libncurses5-dev libncursesw5-dev
   ```

---

## Compilation

To compile the program, use the following command:

### macOS
```bash
gcc password_check.c -o password_check -lncurses -I/opt/homebrew/Cellar/ncurses/6.4/include -L/opt/homebrew/Cellar/ncurses/6.4/lib
```

### Linux
```bash
gcc password_check.c -o password_check -lncurses
```

---

## Usage

1. Run the program:
   ```bash
   ./password_check
   ```
2. Enter your password when prompted.
3. View the analysis and suggestions to improve your password.

---

## Password Strength Criteria

The program evaluates passwords based on the following:

1. **Length**: Minimum 8 characters.
2. **Uppercase Letters**: At least one.
3. **Lowercase Letters**: At least one.
4. **Digits**: At least one.
5. **Special Characters**: At least one (e.g., `!@#$%^&*()`).

---

## Example

### Input
```
Enter your password: P@ssw0rd123
```

### Output
```
Password Analysis:
Length: Sufficient
Uppercase letters: 1
Lowercase letters: 6
Digits: 3
Special characters: 1

Password Strength: Strong
```

---

## Contributing

Contributions are welcome! If you have ideas for improvements or bug fixes, feel free to submit a pull request.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Contact

If you have any questions or suggestions, feel free to reach out:

- **Email**: HAHAHAHAHHAHAHAHAHA
- **GitHub**: [TarPeg0](https://github.com/TarPeg007)

---

## Acknowledgments

Special thanks to the open-source community for providing excellent tools like Ncurses!
