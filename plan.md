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
| Garbage collection | Basic mark-and-sweep or ref counting | Hard |
| `len()` for strings | `len("hello")` returns char count | Easy |
| Array index assignment | `arr[0] = 5` | Medium |
| `--vm` function support | User-defined functions in VM | Hard |

### Medium Priority

| Feature | Description | Difficulty |
|--------|-------------|------------|
| Structs/Records | `type Point = { x: int, y: int }` | Hard |
| `else if` chains | Multiple conditions | ~~Easy~~ ✅ Done |
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
| No garbage collection | interpreter.c | High |
| VM user-defined functions | vm.c, vm/compiler.c | Medium |
| Array index assignment | VM missing OP_SET_INDEX | Medium |
| Lexer token type strings | token_type_to_string() inaccurate | Low |
| Parser uses while keyword for multiple tokens | Debug output misleading | Low |

---

## Tomorrow's Tasks

### 1. Garbage Collection
```
Priority: High
Files: interpreter.c, interpreter.h
Steps:
- Implement reference counting on Value struct
- Increment ref count on value_copy()
- Decrement ref count on value_free()
- Free string/array when ref count reaches 0
- Handle circular references (optional: mark-and-sweep)
```

### 2. Array Index Assignment
```
Priority: High  
Files: vm/vm.c, vm/compiler.c, parser/parser.c
Steps:
- Parse: arr[expr] = expr (AST_INDEX_ASSIGN node)
- Add OP_SET_INDEX opcode
- Implement in VM: pop value, pop index, pop array, set element
- Add bounds checking
```

### 3. len() for Strings
```
Priority: Easy
Files: interpreter.c
Steps:
- Already implemented in interpreter (len returns string length)
- Verify: len("hello") should return 5
- Add test case
```

### 4. VM User-Defined Functions
```
Priority: High
Files: vm/compiler.c, vm/vm.c
Steps:
- Compile fn declarations to bytecode (OP_DEFINE_FN)
- Add OP_CALL_FN opcode for user function calls
- Implement call frame stack in VM
- Handle parameters and return values
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