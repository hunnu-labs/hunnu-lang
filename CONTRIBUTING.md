# Contributing to Hunnu

Thank you for your interest in contributing to Hunnu! This document provides guidelines and instructions for contributing.

## Code of Conduct

By participating, you agree to uphold the [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Contribute

### Reporting Bugs

- Check if the bug has already been reported in [Issues](https://github.com/hunnu-labs/hunnu-lang/issues)
- Use the [bug report template](https://github.com/hunnu-labs/hunnu-lang/issues/new?labels=bug&template=bug_report.md)
- Include: OS version, build output, minimal `.hn` reproduction, expected vs actual behavior

### Feature Requests

- Check existing issues for similar requests
- Use the [feature request template](https://github.com/hunnu-labs/hunnu-lang/issues/new?labels=enhancement&template=feature_request.md)
- Explain the use case and motivation clearly
- Include example Hunnu code showing the proposed feature

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feat/your-feature`)
3. Follow the code style guidelines (see below)
4. Write or update tests as needed
5. Build and test locally
6. Run linting if applicable: `./hunnu-lint.sh`
7. Submit a pull request against the `master` branch

#### PR Checklist

- [ ] Code follows style guidelines
- [ ] Tests added/updated and passing
- [ ] Build succeeds (`cd build && cmake .. && make`)
- [ ] No new compiler warnings
- [ ] Documentation updated if needed
- [ ] CHANGELOG.md updated if applicable

## Development Setup

### Prerequisites

- **C compiler** (GCC or Clang)
- **CMake** >= 3.10
- **Rust** (optional, for Rust compiler/VM components)
- **LLVM** (optional, for LLVM codegen)

### Build and Run

```bash
# Clone the repository
git clone --recursive https://github.com/hunnu-labs/hunnu-lang.git
cd hunnu-lang

# Build with CMake
mkdir -p build && cd build
cmake ..
make

# Run a program
./hunnu run ../examples/main.hn

# Or install system-wide
./install.sh

# Configure environment (add to shell profile)
export HUNNU_STDLIB_PATH="/usr/local/lib/hunnu"
export PATH="$PATH:/usr/local/bin"
```

### Optional Components

```bash
# Rust AOT compiler
cd compiler-core/compiler-rust && cargo build

# Rust bytecode VM
cd compiler-core/vm-rust && cargo build

# LLVM codegen (requires LLVM dev headers)
cd compiler-core/compiler-rust && cargo build --features llvm-codegen

# Python bindings
cd bindings/python && pip install -e .
```

### Mongolian Support

```bash
# Run with Mongolian keywords
HUNNU_LANG=mn ./hunnu run file.hn
./hunnu --lang mn run file.hn
```

## Project Structure

```
hunnu-lang/
├── compiler-core/           # Compiler submodule
│   └── compiler/           # C interpreter + bytecode VM
│       ├── lexer/          # Tokenizer (lexer.c, token.h)
│       ├── parser/         # Parser (AST builder)
│       ├── ast/            # AST node definitions
│       ├── interpreter/    # Tree-walk interpreter
│       ├── vm/             # Bytecode compiler + VM
│       └── transpile/      # C transpiler (AOT via gcc)
│   ├── compiler-rust/      # Rust AOT compiler frontend
│   ├── vm-rust/            # Rust bytecode VM
│   ├── cli/                # CLI + package manager
│   └── tests/              # C unit tests (50+ tests)
├── stdlib/                  # Standard library (.hn modules)
│   ├── array.hn, string.hn, io.hn
│   ├── math.hn, fs.hn, time.hn
│   └── libc.hn             # C FFI bindings
├── self/                    # Self-hosting compiler (Hunnu in Hunnu)
│   ├── token.hn            # Token definitions
│   └── lexer.hn            # Lexer implementation
├── benchmarks/              # Performance benchmarks (submodule)
├── bindings/python/         # Python bindings (PyO3)
├── examples/                # Sample .hn programs
├── docs/                    # Documentation
└── build/                   # Build output (gitignored)
```

## Code Style

See [AGENTS.md](AGENTS.md) for the full style guide. Key rules:

| Item | Convention |
|------|-----------|
| Indentation | 4 spaces (no tabs) |
| Max line length | 100 characters |
| Braces | Opening brace on same line |
| Variables/functions | `snake_case` |
| Constants/macros | `UPPER_SNAKE_CASE` |
| Types/Structs | `snake_case` (PascalCase for display) |
| Integer types | Fixed-width from `<stdint.h>` |
| Initialization | Always initialize pointers to NULL |

### Comments

- Comment "why", not "what"
- Use `const` whenever possible
- Prefer `static` for internal functions
- No Chinese characters in code, comments, or documentation
- Group includes: project header, standard C library, system headers

### Error Handling

- Return error codes (0 for success, negative for errors)
- Use `fprintf(stderr, ...)` for error messages
- Never silently ignore return values
- Clean up resources on error (free memory, close files)

## Testing

```bash
# Run full test suite
cd build && make && ctest

# Run C unit tests directly
./build/tests/hunnu_tests

# Run integration tests
./run_tests.sh

# Run specific test suite
cd build && ctest -R hunnu_c_unit_tests

# Rust tests
cd compiler-core/compiler-rust && cargo test
cd compiler-core/vm-rust && cargo test

# Test a single example
./build/hunnu examples/main.hn
```

### Test Suites

| Suite | File | Tests | Coverage |
|-------|------|-------|----------|
| Value | `tests/test_value.c` | 12 | create/free/copy values, type checks |
| Scope | `tests/test_scope.c` | 6 | define/lookup/nested/shadowing |
| Lexer | `tests/test_lexer.c` | 10 | tokens: int/float/string/keywords/operators |
| Parser | `tests/test_parser.c` | 11 | AST nodes: decls/expressions/statements/errors |
| Interpreter | `tests/test_interpreter.c` | 11 | runtime: arithmetic/fn calls/loops/scopes |

## Commit Guidelines

```
type(scope): brief description in imperative mood

Body explaining the "why", not the "what". Reference issues
and pull requests where applicable.
```

Types: `feat`, `fix`, `refactor`, `docs`, `test`, `style`, `chore`, `ci`

Examples:
- `feat(lexer): add Mongolian keyword for for-loop`
- `fix(interpreter): handle division by zero gracefully`
- `refactor(parser): split parse_decl into separate functions`
- `docs: update README with Mongolian keyword table`

## Issues and Roadmap

### Current Issues

View all [open issues](https://github.com/hunnu-labs/hunnu-lang/issues) and the [project board](https://github.com/orgs/hunnu-labs/projects/1) for roadmap tracking.

### Good First Issues

Look for issues labeled `good first issue` or `help wanted` — these are specifically scoped for new contributors.

### Feature Planning

Major features are tracked on the [project board](https://github.com/orgs/hunnu-labs/projects/1). Current development focus:

- **AOT Compiler**: Complete LLVM codegen in compiler-rust (#89)
- **Self-hosting**: Complete the Hunnu-in-Hunnu compiler (#94)
- **Package registry**: Registry server for package distribution (#95)
- **Tooling**: LSP server, formatter, debugger, VS Code extension (#96, #97, #102, #103)
- **Stdlib expansion**: regex, json, http, random modules (#99)
- **Language features**: Closures, pipelines, type inference (#100, #101)

## Questions?

Open a [Discussion](https://github.com/hunnu-labs/hunnu-lang/discussions), check the [docs](https://github.com/hunnu-labs/hunnu-lang/tree/master/docs), or visit [hunnu-lang.dev](https://hunnu-lang.dev).
