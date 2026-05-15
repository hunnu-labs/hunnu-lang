# Hunnu (Хүннү)

A lightweight, expression-oriented programming language written in C (interpreter) and Rust (AOT compiler). Supports English and Mongolian Cyrillic keywords.

## Quick Start

```bash
# Build from source
mkdir -p build && cd build && cmake .. && make

# Run a program
./build/hunnu run examples/main.hn

# Or install
./install.sh          # Linux/macOS
install.bat           # Windows
```

Add to your shell profile:
```sh
export HUNNU_STDLIB_PATH="/usr/local/lib/hunnu"
export PATH="$PATH:/usr/local/bin"
```

## Features

| Feature | Syntax |
|---------|--------|
| Variables | `let x = 10`, `let mut x = 10` |
| Functions | `fn add(a, b) { return a + b }` |
| If/Else/else if | `if x > 0 { ... } else { ... }` |
| While/For loops | `while x > 0 { ... }`, `for let i = 0; i < 3; i++ { ... }` |
| Arrays | `let arr = [1, 2, 3]`, `arr[0]` |
| Structs | `type Point = { x: int, y: int }`, `p.x` |
| Pointers | `&x`, `*p` |
| Classes | `class Point { pub x, pub y, fn new(self, x, y) { ... } }` |
| Inheritance | `class Dog : Animal { ... }` |
| Traits | `trait Area { fn area(self) }` + `impl Area for Circle { ... }` |
| Encapsulation | `pub` (public) vs private fields |
| Match | `match val { 0 -> "zero", _ -> "other" }` |
| Try/Catch | `try { ... } catch { ... }` |
| FFI | `extern fn puts(s) from "libc.so.6"` |
| AOT compilation | `hunnu compile file.hn -o output` |
| i18n | `--lang mn` / `HUNNU_LANG=mn` |


**Built-in functions:** `print(x)` `input()` `len(arr)` `to_int(x)` `to_float(x)` `to_str(x)`

## Usage

```bash
hunnu run file.hn                  # Run with interpreter
hunnu run file.hn --vm             # Run with C bytecode VM
hunnu run file.hn --vm-rust        # Run with Rust VM
hunnu run file.hn --debug          # Debug mode (tokens + AST)
hunnu build file.hn                # Compile to bytecode
hunnu compile file.hn -o output    # AOT compile to native binary
hunnu tokens file.hn               # Show token stream
hunnu ast file.hn                  # Show AST
```

## Examples

```hunnu
// Hello World
fn main() { print("Hello, World!") }

// Fibonacci
fn fib(n) {
    if n <= 1 { return n }
    return fib(n - 1) + fib(n - 2)
}
fn main() { print(fib(10)) }

// Classes
class Point {
    pub x: int
    pub y: int
    fn new(self, x, y) { self.x = x; self.y = y }
    fn length(self) { return self.x * self.x + self.y * self.y }
}
fn main() {
    let p = new Point(3, 4)
    print(p.length())
}

// Inheritance + Traits
class Dog : Animal { fn speak(self) { print("Woof!") } }
trait Area { fn area(self) }
impl Area for Circle { fn area(self) { return 3 * self.radius * self.radius } }

// FFI
extern fn pow(base, exp) -> float from "libm.so.6"
fn main() { print(pow(2.0, 3.0)) }
```

See [`docs/quickstart.md`](docs/quickstart.md) for a full tutorial and [`examples/mn/`](examples/mn/) for Mongolian-language examples.

## Mongolian Support

Run with Mongolian locale:
```bash
HUNNU_LANG=mn ./build/hunnu run hello.hn
./build/hunnu --lang mn run hello.hn
```

| English | Mongolian (Cyrillic) |
|---------|---------------------|
| `let` / `mut` | `хувьсагч` / `өөрчлөгдөх` |
| `fn` | `функц` |
| `if` / `else` | `хэрвээ` / `бусад` |
| `true` / `false` | `үнэн` / `худал` |
| `while` / `for` | `давталт` / `тооллого` |
| `return` / `break` / `continue` | `буцаах` / `зогсоох` / `үргэлжлүүлэх` |
| `print` | `хэвлэх` |
| `import` | `импорт` |
| `try` / `catch` | `турших` / `барих` |
| `match` | `тохирох` |
| `class` / `new` | `класс` / `шинэ` |
| `pub` / `self` | `нийт` / `өөрөө` |
| `trait` / `impl` | `шинж` / `хэрэгжүүлэх` |
| `type` / `struct` | `төрөл` / `бүтэц` |

```hunnu
// Mongolian Hello World
функц main() {
    хэвлэх("Сайн уу, Хүннү!")
}
```

## Standard Library

| Module | Description |
|--------|-------------|
| `std.libc` | C library FFI (puts, printf, strlen, etc.) |
| `std.math` | Math (pow, sqrt, sin, cos, tan, fabs) |
| `std.io` | I/O (println, read_line) |
| `std.array` | Array utilities (map, filter, reduce) |
| `std.string` | String utils (to_upper, to_lower, trim, split, join) |
| `std.fs` | Filesystem (exists, read_file, write_file) |
| `std.time` | Time (now, timestamp) |

```hunnu
import std.math
fn main() { print(pow(2.0, 3.0)) }
```

## Project Structure

```
hunnu-lang/
├── compiler-core/      # Compiler submodule (C interpreter, Rust AOT, Rust VM)
│   ├── compiler/       # C interpreter + bytecode VM
│   │   ├── lexer/     # Tokenizer
│   │   ├── parser/    # Parser (AST builder)
│   │   ├── ast/       # AST node definitions
│   │   ├── interpreter/ # Tree-walk interpreter
│   │   └── vm/        # Bytecode compiler + VM
│   ├── compiler-rust/ # Rust AOT compiler frontend (LLVM)
│   ├── vm-rust/       # Rust bytecode VM
│   ├── cli/           # CLI + package manager
│   └── tests/         # C unit tests (50 tests)
├── stdlib/             # Standard library modules
├── benchmarks/         # Benchmark suite (submodule)
├── bindings/python/    # Python bindings (PyO3)
├── docs/               # Documentation
├── examples/           # Example .hn programs
├── self/               # Self-hosting compiler (Hunnu in Hunnu)
└── build/              # Build output (gitignored)
```

## Versioning

Named after authentic Mongolian women (`compiler-core/compiler/version.h`):

| Version | Name | Meaning |
|---------|------|---------|
| 0.1.0 | Алтан (Altan) | Golden |
| 0.2.0 | Алтангэрэл (Altangerel) | Golden light |
| **1.0.0** | **Эрдэнэ (Erdene)** | **Jewel** |

## Roadmap

### Completed
- Core language (variables, functions, control flow, arrays, structs, pointers)
- OOP (classes, inheritance, traits/impl, encapsulation, `self`)
- Pattern matching (match with literals, wildcards, identifiers)
- Error handling (try/catch)
- Standard library (math, I/O, array, string, fs, time)
- FFI ecosystem (extern functions, C library bindings)
- Bytecode compiler + VM (C and Rust)
- AOT compiler frontend (Rust/LLVM skeleton)
- i18n (Mongolian keywords, `--lang mn`, localized errors)
- Package manager (`hunnu install`, `hunnu new`)
- Self-hosting (Hunnu lexer in Hunnu)
- CI/CD pipeline, benchmark suite

### Next Steps
- Complete LLVM codegen in compiler-rust
- Enums / ADTs, generics, unsafe blocks
- Functional features (immutability, first-class functions, closures, lambdas)
- v1.0.0 Эрдэнэ release

## License

MIT

## Links

- GitHub: https://github.com/hunnu-labs/hunnu-lang
- Web: https://hunnu-lang.dev
