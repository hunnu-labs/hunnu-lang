# CLAUDE.md

Guidance for Claude Code when working on hunnu-lang. For full agent guidelines, see [AGENTS.md](AGENTS.md).

## Quick Start

```bash
mkdir -p build && cd build && cmake .. && make
./build/hunnu run examples/main.hn
```

## Testing

```bash
./run_tests.sh                    # Integration tests
cd build && make && ctest         # C unit tests
```

## Known Gotchas

- Assignment (`AST_ASSIGN`) must live in `interpreter_evaluate()` (returns `Value`), **not** in `interpreter_execute_statement()` (void).
- `while` loops use no parentheses: `while x > 0 { ... }` (not `while(x > 0)`).
- Newlines are tokens (`TOKEN_NEWLINE`); `parser_skip_newlines()` must be called at statement boundaries.
- Module imports (`import std.math`) resolve to file paths (`stdlib/math.hn`) in `cli/main.c`.
- Classes inside functions cannot be AOT-compiled; only top-level classes work.

## Adding Features

| Step | Files |
|------|-------|
| Token | `token.h` enum + `lexer.c` keyword arrays + `i18n.c` translation |
| AST node | `ast.h` enum/struct + `ast.c` factory/free/print + `parser.c` build + `interpreter.c` eval |
| Builtin | `builtins.c` impl + `interpreter.c` registration |
| Keyword | `lexer.c` arrays (both EN + MN) + `i18n.c` translation |
