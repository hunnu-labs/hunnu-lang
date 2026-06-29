# AGENTS.md - Hunnu Language Development Guidelines

Guidance for agentic coding agents working on hunnu-lang.

## Project Overview

Lightweight, expression-oriented programming language in C (interpreter + bytecode VM) and Rust (AOT compiler via LLVM). CMake build system.

**Current:** Post-v1.0.0 — LLVM codegen complete, benchmark suite refactored, CI/CD automated.

---

## Quick Start

```bash
mkdir -p build && cd build && cmake .. && make              # build
./build/hunnu run examples/main.hn                           # run
./build/hunnu examples/main.hn                               # shorter
./build/hunnu build examples/main.hn                         # emit bytecode
./build/hunnu run examples/main.hn --vm                      # run via VM
cd build && make && ctest                                     # test
```

---

## Architecture

### Directory Structure

```
hunnu-lang/
├── compiler-core/          # Compiler submodule
│   ├── compiler/           # C interpreter + bytecode VM
│   │   ├── ast/            # AST definitions (ast.h, ast.c)
│   │   ├── interpreter/    # Runtime: interpreter.c, builtins.c/h
│   │   ├── lexer/          # Tokenization (lexer.c, token.h)
│   │   ├── parser/         # Parser (parser.c)
│   │   ├── runtime/        # Shared runtime: value.c/h, scope.c/h, import.c/h, version.c/h
│   │   └── vm/             # Bytecode VM (compiler.c, vm.c, opcodes.h)
│   ├── compiler-rust/      # Rust AOT frontend (src/{lexer,parser,ast,codegen}.rs)
│   ├── vm-rust/            # Rust VM
│   ├── cli/                # CLI + package manager
│   ├── transpile/          # C transpiler backend (AOT via gcc)
│   └── tests/              # C unit tests (50 tests, minunit)
├── stdlib/                  # Standard library (.hn modules)
├── self/                    # Self-hosting compiler (token.hn, lexer.hn)
├── benchmarks/              # hunnu-benchmark submodule
├── bindings/python/         # Python bindings (PyO3)
├── examples/                # Sample .hn programs
├── build/                   # Build output (gitignored)
└── .github/workflows/       # CI/CD (build, test, release, project-board)
```

### Data Flow

```
Source (.hn) → Lexer (tokens) → Parser (AST) → Interpreter (tree-walk) → Output
                                                         ↕
                                              Bytecode Compiler → VM (stack)
                                                         ↕
                                              Rust Compiler Frontend → LLVM IR
```

### Key Source Files

| File | Purpose |
|------|---------|
| `compiler/lexer/token.h` | Token type enum |
| `compiler/lexer/lexer.c` | Lexical analyzer |
| `compiler/parser/parser.c` | Recursive-descent parser |
| `compiler/ast/ast.h` | AST node definitions |
| `compiler/interpreter/interpreter.c` | Tree-walk interpreter (eval + exec) |
| `compiler/interpreter/builtins.c/h` | Shared builtin functions |
| `compiler/vm/opcodes.h` | Bytecode instruction set |
| `compiler/vm/compiler.c` | AST → bytecode compiler |
| `compiler/vm/vm.c` | Stack-based VM executor |
| `compiler-rust/src/codegen.rs` | LLVM IR codegen |
| `cli/main.c` | Entry point + CLI dispatch |

---

## Code Style

**C:** 4-space indent, 100-char lines, snake_case, `stdint.h` types, `const`-qualified, NULL-initialized pointers, error return codes. Group includes: project → stdlib → system.

**Python:** PEP 8, type hints, dataclasses for structured data.

**Hunnu:** `let`/`let mut` for variables, `fn` for functions.

### Naming

| Type | Convention | Example |
|------|-----------|---------|
| Variables/functions/files | snake_case | `current_token`, `parse_expression`, `lexer.c` |
| Types/Structs | PascalCase | `Lexer`, `BenchmarkConfig` |
| Constants/enums/macros | UPPER_SNAKE_CASE | `MAX_TOKEN_LENGTH`, `TOKEN_LET`, `#define MAX_DEPTH 100` |

---

## Adding Features

1. **Token** → add to `token.h` enum + `lexer.c` keyword arrays
2. **AST node** → add to `ast.h` enum + struct, implement in `ast.c`/`ast_free.c`, handle in parser + interpreter
3. **Keyword** → add string + type to `lexer.c` arrays
4. **Builtin** → add C impl in `builtins.c`, register in `interpreter.c`

**Always add corresponding tests** in `compiler-core/tests/`.

---

## Testing

```bash
cd build && make && ctest                                   # all tests
./compiler-core/tests/hunnu_tests                            # C unit tests only
ctest -R hunnu_c_unit_tests                                  # specific suite
```

| Suite | File | Tests | Coverage |
|-------|------|-------|----------|
| Value | `test_value.c` | 12 | create/free/copy, type checks |
| Scope | `test_scope.c` | 6 | define/lookup/nested/shadowing |
| Lexer | `test_lexer.c` | 10 | int/float/string/keywords/operators |
| Parser | `test_parser.c` | 11 | decls/expressions/statements/errors |
| Interpreter | `test_interpreter.c` | 11 | arithmetic/fn calls/loops/scopes |

---

## Language Reference

### Variables, Control Flow, Functions
```hunnu
let mut x = 10; x = x + 1                       # mutable variable
fn add(a, b) { return a + b }                    # function
if x > 5 { print("big") } else if x > 0 { ... }
while x > 0 { x = x - 1 }
for i in 0..10 { print(i) }
let arr = [1, 2, 3]; arr[0] = 99                 # array + index assign
```

### OOP (Classes, Traits, Inheritance)
```hunnu
class Point { pub x, pub y
    fn new(self, x, y) { self.x = x; self.y = y }
    fn length(self) { return self.x * self.x + self.y * self.y }
}
class Circle : Point { pub r                     # inheritance
    fn area(self) { return 3 * self.r * self.r }
}
trait Shape { fn describe(self) }
impl Shape for Circle { fn describe(self) { print("Circle") } }
let p = new Point(3, 4); print(p.length())
```

### Type declarations
```hunnu
type Point = { x, y }
fn Point.new(x, y) { return Point(x: x, y: y) }
```

### AOT Compilation
```bash
hunnu compile file.hn -o output
```

---

## Agent Guidelines

- Build and test after every change (`make && ctest`)
- Keep changes small and focused; update AGENTS.md when project setup changes
- Run `ruff check .` for Python, `clang-format` for C
- **No Chinese characters anywhere** — code, comments, docs, commit messages (English only)
