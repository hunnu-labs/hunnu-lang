# Hunnu Roadmap

Vision: A lightweight, expression-oriented programming language with C-level performance and modern ergonomics.

- **Repo:** [github.com/hunnu-labs/hunnu-lang](https://github.com/hunnu-labs/hunnu-lang)
- **Board:** [Project Board](https://github.com/orgs/hunnu-labs/projects/1)
- **Discussions:** [GitHub Discussions](https://github.com/orgs/hunnu-labs/discussions)

---

## ✅ v1.0.0 (Эрдэнэ — Jewel) — Released

### Month 6: Self-Hosting, Package Manager, Release Prep
- Package manager: `hunnu new`, `hunnu install`, project scaffolding with `hunnu.json` manifest (#41)
- Standard library v1: `stdlib/string.hn`, `stdlib/fs.hn`, `stdlib/array.hn`, shared builtin C module (#42)
- Self-hosting: `self/token.hn` + `self/lexer.hn` — token definitions and lexer in Hunnu (#43)
- CI/CD: release workflow with matrix builds (Linux/macOS), Rust + C test matrix (#44)
- Benchmark suite: submodule pointing to `hunnu-labs/hunnu-benchmark` with runner (#45)
- v1.0.0 release: version bump, changelog, binary distribution (#46)

### Month 4: Object-Oriented Programming
- Classes: `class Point { pub x, pub y, fn new(self, x, y) { ... } }`, `new` keyword
- Inheritance: `class Dog : Animal { ... }` with parent method dispatch
- Polymorphism: method dispatch walks inheritance chain
- Encapsulation: `pub` keyword for public fields, warnings on private access
- Traits/Impl: `trait Area { fn area(self) }` + `impl Area for Circle { ... }`
- AOT: instance methods, new expressions, field assignment transpiled to C runtime
- Examples: test_class, test_inheritance, test_polymorphism, test_trait, test_oop

### Month 3: AOT Compiler Foundation
- Structs/records: `type Point = { x, y }`, field access (`point.x`), pointer support (`&x`, `*p`)
- Rust compiler frontend: lexer.rs, parser.rs, ast.rs, codegen.rs (LLVM IR skeleton)
- Rust VM: bytecode VM in Rust (`vm-rust/`)
- Value type extensions: `VALUE_STRUCT`, `VALUE_POINTER`
- CLI: `compile` command (`./hunnu compile file.hn -o output`), C compiler as fallback

### Month 2: FFI Ecosystem + Standard Library
- FFI improvements: string returns, float arguments, Rust FFI boundary
- Error handling: `try`/`catch`/`finally` syntax
- Module system: file imports and module imports (`import std.math`)
- Standard library: libc, math, io, array, string, fs, time modules
- Python bindings (PyO3)

### Phase 4: Bytecode Compiler + VM
- Bytecode instruction set: `opcodes.h` with full instruction enumeration
- Bytecode compiler: `compiler.c` — AST to bytecode compilation
- Virtual machine: `vm.c` — stack-based VM with 256-slot value stack
- CLI: `build` command for bytecode output, `--vm` flag for VM execution

### Phase 3-1: Foundation
- Standard library builtins: `input()`, `to_str()`, `to_int()`, `to_float()`
- CLI: `--debug` / `-d` flag for tokens and AST
- Compound assignment: `+=`, `-=`, `*=`, `/=`
- `else if` chains, floating-point numbers, `null`/`nil`, string escapes
- Variable scoping (scope chain), `break`/`continue`, memory fixes
- Mongolian keywords (bilingual lexer)
- Core: variables, arithmetic, comparison, boolean logic, if/while/for, functions, arrays, print

---

## 🟢 Short Term (v1.1.0 — Q3 2026)

### AOT Compiler Completion ([#89](https://github.com/hunnu-labs/hunnu-lang/issues/89))
**Priority: High | Depends on: compiler-rust skeleton (done)**
- Full LLVM IR code generation in `compiler-rust/src/codegen.rs`
- Register-based codegen with function compilation and module output
- Rust frontend pipeline: lexer -> parser -> AST -> LLVM IR
- Feature parity between interpreter and AOT-compiled output
- **Success criteria:** All `examples/*.hn` programs compile to native binaries that produce identical output

### Package Registry ([#95](https://github.com/hunnu-labs/hunnu-lang/issues/95))
**Priority: High | Depends on: package manager CLI (done)**
- Registry server for package distribution (`hunnu-registry`)
- `hunnu publish` and `hunnu search` commands
- Version resolution and dependency management
- Package metadata format and validation
- **Success criteria:** `hunnu install <package>` fetches and resolves dependencies from registry

### Self-Hosting Compiler Completion ([#94](https://github.com/hunnu-labs/hunnu-lang/issues/94))
**Priority: Medium | Depends on: self/token.hn + self/lexer.hn (done)**
- Complete Hunnu-in-Hunnu compiler pipeline
- Parser in Hunnu (`self/parser.hn`)
- AST definitions in Hunnu (`self/ast.hn`)
- Codegen/interpret in Hunnu
- Dogfooding: compiler can compile itself
- **Success criteria:** `./build/hunnu run self/compiler.hn examples/main.hn` produces correct output

### Stdlib: HTTP + JSON ([#99](https://github.com/hunnu-labs/hunnu-lang/issues/99))
**Priority: Medium | Depends on: FFI ecosystem (done)**
- `std.http` — HTTP client with GET/POST, header support
- `std.json` — JSON parsing and serialization
- **Success criteria:** Full round-trip JSON serialization, HTTP request from Hunnu

---

## 🟡 Medium Term (v1.2.0 — Q4 2026)

### Language Features ([#100](https://github.com/hunnu-labs/hunnu-lang/issues/100), [#101](https://github.com/hunnu-labs/hunnu-lang/issues/101))
**Priority: High**
- **Closures** — first-class anonymous functions with lexical scoping
- **Pipelines** — `|>` operator for chained function calls: `data |> parse |> transform |> output`
- **Destructuring** — pattern-based variable binding: `let (x, y) = point`, `let [a, b] = arr`
- **Immutability by default** — functional programming idioms, `mut` keyword for mutations
- **Success criteria:** Closures capture environment correctly, pipeline operator chains 5+ functions

### LSP + IDE Tooling ([#96](https://github.com/hunnu-labs/hunnu-lang/issues/96), [#97](https://github.com/hunnu-labs/hunnu-lang/issues/97), [#102](https://github.com/hunnu-labs/hunnu-lang/issues/102), [#103](https://github.com/hunnu-labs/hunnu-lang/issues/103))
**Priority: High**
- **Language Server Protocol (LSP)** — diagnostics, completion, hover, go-to-definition
- **Formatter** — automatic code formatting (`hunnu fmt`)
- **Debugger** — source-level debugging support (breakpoints, step, inspect)
- **VS Code Extension** — syntax highlighting, snippets, LSP integration
- **Success criteria:** VS Code provides autocomplete, error squiggles, and formatting for `.hn` files

### Standard Library Expansion ([#99](https://github.com/hunnu-labs/hunnu-lang/issues/99))
**Priority: Medium**
- `std.regex` — regular expression matching and replacement
- `std.random` — random number generation (PRNG + crypto-safe)
- `std.crypto` — basic cryptographic primitives (hash, HMAC, AES)
- **Success criteria:** Each module has 5+ documented functions with tests

### Type System Enhancements ([#101](https://github.com/hunnu-labs/hunnu-lang/issues/101))
**Priority: Medium**
- **Type inference** — Hindley-Milner style type inference
- **Generics** — fully parametric polymorphism (beyond basic generics)
- Type annotations on variables, parameters, and returns: `let x: int = 42`
- **Success criteria:** Type inference eliminates all explicit type annotations in examples

---

## 🔵 Long Term (v2.0.0 — 2027)

### Performance
| Item | Description | Depends on |
|------|-------------|------------|
| JIT compilation | LRU-based hot-path JIT for frequently executed functions | Bytecode VM (done) |
| Multi-threaded VM | Parallel bytecode execution with work-stealing | JIT |
| AOT optimization passes | Constant folding, dead code elimination, inlining | LLVM codegen (v1.1) |
| SIMD intrinsics | `std.simd` module for vector operations via FFI | FFI (done) |

### Platform Support
| Item | Description | Depends on |
|------|-------------|------------|
| Windows | MSVC build, Win32 API bindings, installer | Cross-platform CLI (partial) |
| WebAssembly | `hunnc compile --target wasm` — compile to WASM | LLVM codegen (v1.1) |
| ARM/aarch64 | LLVM backend target for ARM processors | LLVM codegen (v1.1) |
| Cross-compilation | `--target` flag for cross-platform builds | LLVM codegen (v1.1) |

### Ecosystem
| Item | Description |
|------|-------------|
| Plugin system | Language-agnostic plugin system via shared libraries |
| Web framework | `std.web` for HTTP services |
| GUI bindings | GTK, Qt, or native GUI toolkit bindings |
| Game libraries | Game development foundation libraries |
| Embedded scripting | Low-footprint interpreter for embedding in other apps |
| Transpilation | Hunnu-to-{Go, Rust, Zig} transpilers |

### Community
| Item | Description |
|------|-------------|
| Playground | Web-based REPL (run Hunnu in browser) |
| Registry website | Official package registry with search and docs |
| Interactive tutorial | Learn Hunnu in the browser |
| Hunnu book | Comprehensive language guide (mdBook) |

---

## 📅 Timeline

| Version | Target | Focus | Key Deliverables |
|---------|--------|-------|-----------------|
| v1.0.0 | Released | Foundation | Core lang, OOP, stdlib, VM, AOT, i18n, CI/CD |
| v1.1.0 | Q3 2026 | AOT + Registry | Complete LLVM codegen, package registry, self-hosting, std.http + std.json |
| v1.2.0 | Q4 2026 | DX + Features | Closures, pipelines, LSP, formatter, debugger, VS Code, regex/random/crypto |
| v2.0.0 | 2027 | Maturity | Type inference, generics, JIT, WASM, Windows, ARM |

---

## 📊 Priority Summary

| Priority | Count | Areas |
|----------|-------|-------|
| 🔴 High | 4 | AOT LLVM codegen, Package registry, LSP/tooling, Closures/pipelines |
| 🟡 Medium | 4 | Self-hosting, Stdlib HTTP/JSON, Type system, Stdlib regex/random/crypto |
| 🟢 Low | ~15 | JIT, WASM, Windows, GUI, game libs, playground, book, etc. |

---

## ⚠️ Risks & Dependencies

1. **LLVM codegen complexity** — Rust/LLVM integration requires `inkwell` crate compatibility. Mitigation: pin LLVM version, CI tests for LLVM 14-17.
2. **Closure implementation** — Heap-allocated capture environments require GC or reference counting. Mitigation: simple ref-count first, GC later.
3. **LSP server** — Full language server is a large undertaking. Mitigation: incremental delivery (diagnostics first, then completion, then hover).
4. **Self-hosting performance** — Hunnu-in-Hunnu may be 10-100x slower than C interpreter. Mitigation: acceptable for bootstrapping; optimize after v2.0.
5. **Windows support** — Cross-platform portability requires handling MSVC, path separators, and Win32 API differences. Mitigation: CI with Windows runner.

---

Track progress on the [project board](https://github.com/orgs/hunnu-labs/projects/1) and [discuss ideas](https://github.com/orgs/hunnu-labs/discussions).
