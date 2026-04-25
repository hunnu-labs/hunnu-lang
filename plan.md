# Hunnu 2-Hour Stream Plan: Arrays & Strings

## Stream Overview

- **Topic:** Adding arrays and string operations to Hunnu
- **Duration:** 2 hours
- **Audience:** Beginner-friendly

---

## Pre-Stream Checklist

- [ ] Hunnu project open in editor
- [ ] Example file `main.hn` ready to demo
- [ ] Build verified (`cd build && make`)
- [ ] Whiteboard/diagram ready for concepts
- [ ] Terminal open, running `./build/hunnu run examples/main.hn`

---

## Current Language State (Completed)

The following features are now working:
- Variables with `let`
- Print statements
- Arithmetic: `+`, `-`, `*`, `/`
- Comparison: `>`, `<`, `>=`, `<=`
- If/else statements
- **While loops**: `while(condition) { body }`
- **For loops**: `for(init; condition; update) { body }`
- **Return statements**: `return expression`
- Variable reassignment: `x = new_value`

---

## Hour 1: Arrays (55 min)

| Time | Segment | Content |
|------|---------|---------|
| 0:00-0:05 | Intro | Welcome, demo current features |
| 0:05-0:15 | Concept | Draw arrays: contiguous memory, index concept |
| 0:15-0:25 | Code | **Add array token** - TOKEN_LBRACKET, TOKEN_RBRACKET |
| 0:25-0:35 | Code | **Add array AST node** - AST_ARRAY_EXPR, AST_INDEX_EXPR |
| 0:35-0:45 | Code | **Parser** - array literals, indexing |
| 0:45-0:55 | Demo | Live demo array creation and access |

**Break: 5 min**

---

## Hour 2: Strings (55 min)

| Time | Segment | Content |
|------|---------|---------|
| 1:00-1:15 | Code | **Interpreter** - array indexing execution |
| 1:15-1:25 | Concept | Strings as character arrays |
| 1:25-1:35 | Code | **String length** - `len(s)` function |
| 1:35-1:45 | Code | **String concatenation** - `s + t` |
| 1:45-1:55 | Demo | Combined demo - arrays + strings |
| 1:55-2:00 | Wrap | Summary, Q&A |

---

## Files to Modify

| File | Changes |
|------|---------|
| `compiler/lexer/token.h` | Add `TOKEN_LBRACKET`, `TOKEN_RBRACKET`, `TOKEN_COMMA` |
| `compiler/lexer/lexer.c` | Handle `[`, `]`, `,` tokens |
| `compiler/parser/parser.c` | Parse array literals, indexing, string concat |
| `compiler/ast/ast.h` | Add `AST_ARRAY_EXPR`, `AST_INDEX_EXPR`, `AST_STRING_CONCAT` |
| `compiler/interpreter/interpreter.c` | Array access, string operations |
| `examples/main.hn` | Demo with arrays and strings |

---

## Syntax Examples

### Array Creation
```hunnu
let numbers = [1, 2, 3, 4, 5]
print(numbers[0])  // 1
print(numbers[4])  // 5
```

### Array with Loop
```hunnu
let arr = [1, 2, 3, 4, 5]
let i = 0
while i < 5 {
    print(arr[i])
    i = i + 1
}
```

### String Concatenation
```hunnu
let greeting = "Hello, "
let name = "World"
let message = greeting + name
print(message)  // Hello, World
```

### String Length
```hunnu
let s = "Hunnu"
let length = len(s)
print(length)  // 4
```

### Final Demo Program
```hunnu
fn print_array(arr, size) {
    let i = 0
    while i < size {
        print(arr[i])
        i = i + 1
    }
}

fn main() {
    let numbers = [10, 20, 30, 40, 50]
    print("Array elements:")
    print_array(numbers, 5)
    
    let first = numbers[0]
    let last = numbers[4]
    print("First + Last:")
    print(first + last)
    
    let name = "Hunnu"
    let message = "Language: " + name
    print(message)
    print(len(message))
}
```

---

## Teaching Points

### Why Arrays?
- Store multiple values in one variable
- Access by index (0-based)
- Useful with loops for iteration

### Why Strings?
- Text handling is essential
- Strings are sequences of characters
- Useful for user input/output

### Beginner-Friendly Diagrams

```
ARRAY:
┌─────┬─────┬─────┬─────┬─────┐
│ [0] │ [1] │ [2] │ [3] │ [4] │
│  10 │  20 │  30 │  40 │  50 │
└─────┴─────┴─────┴─────┴─────┘
   │
   ▼
numbers[2] = 30
```

```
STRING:
┌───┬───┬───┬───┬───┐
│ H │ u │ n │ n │ u │
└───┴───┴───┴───┴───┘
[0] [1] [2] [3] [4]
```

---

## Next Stream Preview

- Break and continue statements
- Input/output basics
- Multi-dimensional arrays
- String slicing
- Function parameters by reference

---

## Notes

- Explain each step before coding
- Show errors and how to fix them
- Take it slow for beginners
- Q&A breaks after each segment