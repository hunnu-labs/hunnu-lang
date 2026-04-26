# Хүмүүн хэл — Хөгжлийн төлөвлөгөө

> Хүмүүн хэлний одоогийн байдал, тэргүүлэх ажлууд болон алсын харааг хянах баримт баримт бичиг юм.

---

## Одоогийн төлөв

### Ажиллаж байгаа онцлогууд

| Онцлог | Синтакс | Жишээ |
|---------|---------|--------|
| Хувьсагч | `let x = 5` | `let x = 5` |
| Арифметик | `+`, `-`, `*`, `/`, `%` | `x + y * z` |
| Харьцуулалт | `>`, `<`, `>=`, `<=`, `==`, `!=` | `if x > 0 { ... }` |
| Логик | `and`, `or`, `not` | `if a and b { ... }` |
| Нөхцөл | `if x > 0 { ... } else { ... }` | `if x > 0 { "A" } else { "B" }` |
| else if | `else if` гинж | `else if x > 5 { ... } else { ... }` |
| while давталт | `while(condition) { body }` | `while i < 10 { i = i + 1 }` |
| for давталт | `for(init; condition; update) { body }` | `for let i = 0; i < 3; i = i + 1 { ... }` |
| Функц | `fn name(params) { body }` | `fn add(a, b) { return a + b }` |
| Буцаах | `return expression` | `return a + b` |
| Хэвлэх | `print(value)` | `print("Hello")` |
| Дахин оноох | `x = new_value` | `x = 10` |
| Нийлэх | `+=`, `-=`, `*=`, `/=` | `x += 1` |
| Массив | `[1, 2, 3]` | `let arr = [1, 2, 3]` |
| Массив хандалт | `arr[i]` | `arr[0]` |
| Тэмдэгт нийлэх | `"a" + "b"` | `"Hello " + "World"` |
| Тэмдэгт орлоос | `\n`, `\t`, `\\`, `\"` | `"Hello\nWorld"` |
| Урт | `len(s)` | `len("abc")` |
| Оруулах | `input()` | `let name = input()` |
| Төрөл шилжүүлэх | `to_int(s)` | `to_int("42")` |
| Бодит тоо | `3.14159` | `let pi = 3.14` |
| Хоосон | `null` / `nil` | `let x = null` |
| Зогсоох | `break` | `while i < 10 { if i == 5 { break } }` |
| Үргэлжлүүлэх | `continue` | `while i < 10 { i = i + 1; if i == 3 { continue } }` |
| Хүрээлэл | `{ ... }` | Блокоор хүрээлсэн хувьсагууд |

---

## CLI хэрэглээ

```bash
# Ажиллуулах
./hunnu run examples/main.hn

# VM-ээр ажиллуулах
./hunnu run examples/main.hn --vm

# Bytecode гаргах
./buildhunnu build examples/main.hn

# Debug хэвлэх
./hunnu run examples/main.hn --debug
```

---

## Фазууд

### Phase 1: Үндсэн засварууд ✅
*2025 оны 4 сар*

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | Хувьсагч хүрээлэл (scope stack) | Блокоор хүрээлсэн хувьсагууд |
| 2 | break/continue | Давталтын удирдлага |
| 3 | Массив хязгаар шалгах | `arr[i]` IndexError |
| 4 | Тэмдэгт санах ой | Dangling pointer засвар |

**Файлууд:** `interpreter.c`, `parser.c`

### Phase 2: Түлхүүр онцлогууд ✅
*2025 оны 4 сар*

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | Нийлэх оператор `+=`, `-=`, г.м | `x += 1` |
| 2 | else if гинж | Олон нөхцөл шалгах |
| 3 | Бодит тоо | `3.14`, `2.0` |
| 4 | null/nil | `let x = null` |

**Файлууд:** `lexer.c`, `token.h`, `parser.c`, `interpreter.c`

### Phase 3: Стандарт сан + DX ✅
*2025 оны 4 сар*

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | input() | Стандарт оролдсон |
| 2 | to_int(), to_float(), to_str() | Төрөл шилжүүлэх |
| 3 | --debug | Токен, AST хэвлэх |

**Файлууд:** `interpreter.c`, `cli/main.c`

### Phase 4: Bytecode + VM ✅
*2025 оны 4 сар*

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | Bytecode компилятор | AST → bytecode |
| 2 | Virtual Machine | Bytecode ажиллуулах |
| 3 | build命令 | Bytecode гаргах |
| 4 | --vm туг | VM ажиллуулах |

**Файлууд:** `compiler/vm/`

---

## Дараагийн алхсууд

### Түлхүүр (2025)

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | Модуль/import | Файлууд хооронд импорт |
| 2 | Стандарт сан | Нийтлэг функцүүд |

### Дунд (2026+)

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | Struct/Record | `type Point = { x: int, y: int }` |
| 2 | Pattern matching | `match x { ... }` |
| 3 | ADT (Sum төрөл) | `type Maybe[T] = Just(T) \| Nothing` |

### Алсын хараа

| # | Онцлог | Тайлбар |
|----|--------|---------|
| 1 | Self-hosting | Хэлээ өөрөө бичигдсэн |
| 2 | JIT | JIT компиляц |
| 3 | AOT | Бинар гаралт |

---

## Жишээ код

### Сайн уу дэлхий

```hunnu
fn main() {
    print("Сайн уу, Дэлхий!")
}
```

### Фибоначчи

```hunnu
fn fib(n) {
    if n <= 1 {
        return n
    }
    return fib(n - 1) + fib(n - 2)
}

fn main() {
    print(fib(10))  // 55
}
```

### Массивтай ажиллах

```hunnu
fn main() {
    let numbers = [10, 20, 30, 40, 50]
    print(numbers[0])  // 10
    print(len(numbers))  // 5
}
```

### while Давталт

```hunnu
fn main() {
    let i = 0
    let sum = 0
    while i < 10 {
        i = i + 1
        sum = sum + i
    }
    print(sum)  // 55
}
```

### break/continue

```hunnu
fn main() {
    let i = 0
    let count = 0
    while i < 10 {
        i = i + 1
        if i == 3 { continue }
        if i == 8 { break }
        count = count + 1
    }
    print(count)  // 6
}
```

### Төрөл шилжүүлэх

```hunnu
fn main() {
    let x = "42"
    let n = to_int(x)
    print(n + 1)  // 43

    let f = to_float("3.14")
    print(f * 2)  // 6.28

    let s = to_str(123)
    print(s + "456")  // 123456
}
```

---

## Файлын бүтэц

```
hunnu-lang/
├── compiler/
│   ├── lexer/          # Токенчлог
│   │   ├── lexer.c
│   │   └── token.h
│   ├── parser/        # Парсер (AST)
│   │   ├── parser.c
│   │   └── parser.h
│   ├── ast/          # AST төрөл
│   │   ├── ast.c
│   │   └── ast.h
│   ├── interpreter/  # Модоо ажиллуулагч
│   │   ├── interpreter.c
│   │   └── interpreter.h
│   └── vm/          # Bytecode + VM
│       ├── opcodes.h
│       ├── compiler.c
│       ├── compiler.h
│       ├── vm.c
│       └── vm.h
├── cli/              # CLI
│   ├── main.c
│   └── cli.h
├── examples/         # Жишээ код
└── CMakeLists.txt
```

---

## Хөгжлийн түүх

```
2025-04  Phase 1: Үндсэн засварууд
2025-04  Phase 2: Түлхүүр онцлогууд
2025-04  Phase 3: Стандарт сан + DX
2025-04  Phase 4: Bytecode + VM
```

---

## Холбоо

- Вэб: https://hunnu-lang.dev
- GitHub: https://github.com/hunnu-labs/hunnu-lang

MIT License © 2025 Хүмүүн