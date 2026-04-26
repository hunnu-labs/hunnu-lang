# Hunnu Language — Development Plan

> A living document tracking the state, priorities, and vision for Hunnu.

---

## Features

Hunnu is a lightweight programming language written in C. It supports both **English** and **Mongolian** (Cyrillic) keywords.

### Working Features

| Feature | English | Mongolian |
|---------|---------|-----------|
| Variables | `let x = 5` | `хувьсагч x = 5` |
| Functions | `fn add(a, b) { return a + b }` | `функц нэмэх(a, b) { буцаах a + b }` |
| If/else | `if x > 0 { ... } else { ... }` | `хэрвээ x > 5 { ... } бусад { ... }` |
| While loop | `while i < 10 { ... }` | `давталт i < 10 { ... }` |
| For loop | `for let i = 0; i < 3; i = i + 1 { ... }` | `тооллого хувьсагч i = 0; i < 3; i = i + 1 { ... }` |
| Print | `print("Hello")` | `хэвлэх("Сайн уу")` |
| Return | `return value` | `буцаах утга` |
| Break | `break` | `зогсоох` |
| Continue | `continue` | `үргэлжлүүлэх` |
| null/nil | `null`, `nil` | `хоосон` |

---

## CLI Usage

```bash
./hunnu run examples/main.hn
./hunnu run examples/main.hn --vm
./hunnu build examples/main.hn
./hunnu run examples/main.hn --debug
```

---

## Phases

### Phase 1: Foundation Fixes ✅

| # | Feature | Description |
|----|---------|-------------|
| 1 | Variable scoping (scope stack) | Block-scoped variables |
| 2 | break/continue | Loop control flow |
| 3 | Array bounds checking | `arr[i]` IndexError |
| 4 | String memory safety | Dangling pointer fixes |

### Phase 2: Core Language Features ✅

| # | Feature | Description |
|----|---------|-------------|
| 1 | Compound assignment: `+=`, `-=`, etc | `x += 1` |
| 2 | `else if` chains | Multiple conditions |
| 3 | Floating-point numbers | `3.14`, `2.0` |
| 4 | `null`/`nil` literal | `let x = null` |

### Phase 3: Standard Library + DX ✅

| # | Feature | Description |
|----|---------|-------------|
| 1 | `input()` | Read from stdin |
| 2 | `to_int()`, `to_float()`, `to_str()` | Type conversions |
| 3 | `--debug` flag | Show tokens and AST |

### Phase 4: Bytecode + VM ✅

| # | Feature | Description |
|----|---------|-------------|
| 1 | Bytecode compiler | AST → bytecode |
| 2 | Virtual Machine | Bytecode execution |
| 3 | `build` command | Output bytecode |
| 4 | `--vm` flag | Run with VM |

---

## Next Steps

### High Priority (Tomorrow)

| Feature | Description | Difficulty |
|--------|-------------|------------|
| Array memory fix | Deep copy arrays, proper free | Medium |
| `import` statement | External file loading | Medium |
| Error line numbers | Show source line in errors | Easy |

### Medium Priority

| Feature | Description | Difficulty |
|--------|-------------|------------|
| Structs/Records | `type Point = { x: int, y: int }` | Hard |
| `else if` chains | Multiple conditions | Easy |
| Standard library | Common functions | Medium |
| Pattern matching | `match x { ... }` | Hard |

### Long-term Vision

| Feature | Description | Difficulty |
|--------|-------------|------------|
| ADT (Sum types) | `type Maybe[T] = Just(T) \| Nothing` | Hard |
| Self-hosting | Write compiler in Hunnu | Very Hard |
| JIT compilation | Just-in-time execution | Very Hard |
| AOT binary | Native binary output | Very Hard |

---

## Technical Debt (Known Issues)

| Issue | Location | Severity |
|--------|----------|----------|
| Array shallow copy | value_copy() | Medium |
| Double-free in arrays | VM execution | Medium |
| No garbage collection | interpreter.c | High |

---

## Tomorrow's Tasks

### 1. Array Memory Fix
```
Priority: High
Files: interpreter.c, vm.c
Steps:
- Fix value_copy() to deep copy array elements
- Fix VM OP_CREATE_ARRAY to avoid double-free
- Add array element cleanup in value_free()
```

### 2. Import Statement
```
Priority: High  
Files: parser.c, lexer.c, cli/main.c
Steps:
- Add TOKEN_IMPORT to token.h
- Add keyword "import" / "импорт"
- Parse import statement: import "module.hn"
- Load and concatenate source files
```

### 3. Error Line Numbers
```
Priority: Easy
Files: parser.c, interpreter.c
Steps:
- Pass source lines to AST nodes
- Show line number in parse errors
- Show line number in runtime errors
```

---

## File Structure

```
hunnu-lang/
├── compiler/
│   ├── lexer/          # Tokenizer
│   ├── parser/        # Parser (AST)
│   ├── ast/          # AST node types
│   ├── interpreter/  # Tree-walk interpreter
│   └── vm/          # Bytecode + VM
├── cli/              # CLI
├── examples/         # Example code
└── CMakeLists.txt
```

---

## Development Timeline

```
2025-04  Phase 1: Foundation Fixes
2025-04  Phase 2: Core Language Features
2025-04  Phase 3: Standard Library + DX
2025-04  Phase 4: Bytecode + VM
```

---

## Links

- Web: https://hunnu-lang.dev
- GitHub: https://github.com/hunnu-labs/hunnu-lang

MIT License © 2025 Hunnu