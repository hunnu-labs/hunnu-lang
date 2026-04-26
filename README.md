# Хүмүүн хэл

> Хөнгөн, илэрхийлэлд төвлөсөн, C хэл дээр бичигдсэн програмчлалын хэл юм.

---

## Онцлогууд

| Онцлог | Синтакс | Жишээ |
|---------|---------|--------|
| Хувьсагч | `let x = 5` | `let x = 5` |
| Функц | `fn add(a, b) { return a + b }` | `fn add(a, b) { return a + b }` |
| Нөхцөл | `if x > 0 { ... } else { ... }` | `if x > 0 { ... } else { ... }` |
| else if | `else if` гинж | `else if x > 5 { ... } else { ... }` |
| while давталт | `while x > 0 { ... }` | `while i < 10 { ... }` |
| for давталт | `for let i = 0; i < 3; i = i + 1 { ... }` | `for let i = 0; i < 3; i = i + 1 { ... }` |
| Арифметик | `+` `-` `*` `/` `%` | `x + y * z` |
| Харьцуулалт | `==` `!=` `<` `<=` `>` `>=` | `if x > 0 { ... }` |
| Логик | `and` `or` `not` | `if a and b { ... }` |
| Хэвлэх | `print("Hello")` | `print("Hello")` |
| Нийлэх оператор | `x += 1`, `x -= 2` | `x += 5` |
| Массив | `[1, 2, 3]` | `let arr = [1, 2, 3]` |
| Массив хандалт | `arr[i]` | `arr[0]` |
| Тэмдэгт нийлэх | `"a" + "b"` | `"Hello " + "World"` |
| Тэмдэгт орлоос | `\n`, `\t`, `\\`, `\"` | `"Hello\nWorld"` |
| Урт | `len(s)` | `len(s)` |
| Бодит тоо | `3.14159` | `let pi = 3.14` |
| null/nil | `null` / `nil` | `let x = null` |

---

## Барих

```bash
mkdir build && cd build
cmake ..
make
```

---

## Ажиллуулах

```bash
./build/hunnu run examples/main.hn
./build/hunnu run examples/main.hn --vm
./build/hunnu build examples/main.hn
./build/hunnu run examples/main.hn --debug
```

---

## Жишээнүүд

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

### for Давталт break continue-тай

```hunnu
fn main() {
    let i = 0
    let count = 0
    while i < 10 {
        i = i + 1
        if i == 5 { continue }
        if i == 8 { break }
        count = count + 1
    }
    print(count)  // 6
}
```

### Нийлэх Оператор

```hunnu
fn main() {
    let x = 10
    x += 5
    x -= 3
    x *= 2
    print(x)  // 24
}
```

### else if Гинж

```hunnu
fn main() {
    let score = 85
    if score >= 90 {
        print("A")
    } else if score >= 80 {
        print("B")
    } else if score >= 70 {
        print("C")
    } else {
        print("F")
    }
}
```

### Бодит Тоо

```hunnu
fn main() {
    let pi = 3.14159
    let r = 2.0
    print(pi * r * r)  // 12.5664
    print(10 + 3.5)    // 13.5
}
```

### Тэмдэгт Орлоос

```hunnu
fn main() {
    print("Hello\nWorld")       // шинэ мөр
    print("Tab\there")           // таб
    print("Quote: \"test\"")      // орчуулсан ил引用
    print("Backslash: \\")       // орчуулсан урвуу зууа
}
```

### Хүрээлэлтэй Хувьсагууд

```hunnu
fn main() {
    let x = 10
    {
        let x = 20          // гаднах x-г сөлдөн
        print(x)            // 20
    }
    print(x)                // 10
}
```

### Функцууд

```hunnu
fn add(a, b) {
    return a + b
}

fn main() {
    let result = add(5, 3)
    print(result)
}
```

### Массивнууд

```hunnu
fn main() {
    let numbers = [10, 20, 30, 40, 50]
    print(numbers[0])
    print(numbers[4])
}
```

### Тэмдэгтүүд

```hunnu
fn main() {
    let greeting = "Сайн уу, "
    let name = "Дэлхий"
    let message = greeting + name
    print(message)
    
    let s = "Хүмүүн"
    print(len(s))
}
```

---

## Төслийн Бүтэц

```
hunnu-lang/
├── compiler/
│   ├── lexer/          # Токенчлог
│   ├── parser/        # Парсер (AST)
│   ├── ast/          # AST зангилаа төрлүүд
│   ├── interpreter/  # Модоо ажиллуулагч
│   └── vm/          # Bytecode + VM
├── cli/              # CLI
├── examples/         # Жишээ код
└── CMakeLists.txt
```

---

## Төлөвлөгөө

[`plan.md`](plan.md) дээр бүрэн хөгжлийн төлөвлөгөөг харна уу.

### Дууссан
- ✅ Phase 1: Үндсэн засварууд (хүрээлэл, break/continue, санах ой засвар)
- ✅ Phase 2: Түлхүүр онцлогууд (нийлэх оператор, else if, бодит тоо, null/nil, тэмдэгт орлоос)
- ✅ Phase 3: Стандарт сан + Хөгжлийн туршлага (input, to_str/to_int/to_float, --debug)
- ✅ Phase 4: Bytecode Компилятор + VM (build命令, --vm туг)

---

## Лиценз

MIT