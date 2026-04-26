# Өөрчлөлийн тэмдэглэг

> Хүмүүн хэлний өөрчлөлүүд.

---

## [Хэзээ ч тодорхойгүй]

### Phase 4: Bytecode Компилятор + VM ✅
*2025 оны 4 сар*

#### Bytecode Заавар
- `opcodes.h` — заавар цуглуулага нэмсэн
- Constants, арифметик, харьцуулалт, удирдлага заавар
- Хувьсагч хандалт, массив, индекс, функц дуудах

```c
// Жишээ bytecode
OP_CONSTANT_INT 10       // тоо оруулах
OP_CONSTANT_INT 20       // өөр тоо оруулах
OP_ADD                 // нэмэх
OP_SET_LOCAL 0          // хувьсагч 0-д хадгалах
OP_GET_LOCAL 0          // хувьсагч 0-г аврах
OP_GET_INDEX           // массив индекс
OP_CALL                // функц дуудох
OP_RETURN             // буцаах
```

#### Bytecode Компилятор
- `compiler/compiler.c` нэмсэн — AST → bytecode
- AST-аар гүйлдэж bytecode гаргах
- Тогтмол хотолбор (strings)
- Програм, функц, блок, хувьсагч, if/while, массив, print дэмжлэх

#### Виртуал Машин
- `vm.c` нэмсэн — стек дээр суурилсан VM
- Удамлах стек (256 слот хүртэл)
- Хувьсагч хадгалалт
- Суурилсан функц (print, input, to_int, to_float, to_str)
- Bytecode тайлбарлах давталт

#### CLI Нэгдэл
- `build` команды — bytecode гаргах
- `--vm` туг — VM-ээр ажиллуулах

```bash
./hunnu build examples/main.hn   # bytecode гаргах
./hunnu run examples/main.hn --vm  # VM-ээр ажиллуулах
```

#### Файлууд
- `compiler/vm/opcodes.h` - заавар enum
- `compiler/vm/compiler.h` - компилятор төрөл
- `compiler/vm/compiler.c` - bytecode компилятор
- `compiler/vm/vm.h` - VM толгой
- `compiler/vm/vm.c` - VM ажиллуулах
- `cli/main.c`, `cli/cli.h` - команды, тугнууд
- `interpreter/interpreter.c`, `interpreter/interpreter.h` - утга үүсгэх тусламжууд

---

### Phase 3: Стандарт сан + DX ✅
*2025 оны 4 сар*

#### Стандарт Сан функцүүд
- `input()` - стандарт оролдоос унших
- `to_str()` - тэмдэгт болгох
- `to_int()` - тоо болгох
- `to_float()` - бодит тоо болгох

#### CLI Сайжруулалт
- `--debug` / `-d` туг — токен болон AST хэвлэх

---

### Phase 2: Түлхүүр онцлогууд ✅
*2025 оны 4 сар*

#### Нийлэх Оператор
- `+=`, `-=`, `*=`, `/=` оператор нэмсэн
- Парсер дээр `x = x + y` болгон

```hunnu
let x = 10
x += 5   // x = 15
x -= 3   // x = 12
x *= 2   // x = 24
x /= 4   // x = 6
```

#### else if Гинж
- `if` өгүүлбэрийг гинжлэгдсэн `else if` дэмжлэхтэй болгосон

```hunnu
if x > 90 {
    print("A")
} else if x > 80 {
    print("B")
} else {
    print("C")
}
```

#### Бодит Тоо
- `TOKEN_FLOAT_LITERAL` болон `VALUE_FLOAT` төрөл нэмсэн
- int/float хольсонтой бүрэн арифметик дэмжлэх

```hunnu
let pi = 3.14159
let r = 2.0
print(pi * r * r)  // 12.5664
print(10 + 3.5)    // 13.5
```

#### null/nil Т literals
- `null` болон `nil` түлхүүр үгс нэмсэн

```hunnu
let x = null
let y = nil
```

#### Тэмдэгт Орлоос
- Бүрэн орлоос дараалал: `\n`, `\t`, `\\`, `\"`

```hunnu
print("Hello\nWorld")       // шинэ мөр
print("Tab\there")          //.tab
print("Quote: \"test\"")     // орчуулсан ил引用
print("Backslash: \\")      // орчуулсан урвуу зууа
```

---

### Phase 1: Үндсэн засварууд ✅
*2025 оны 4 сар*

#### Хувьсагч Хүрээлэл (Scope Stack)
-.Global нэрийн орон replacing хийх хүрээллийн гинжээр
- Блокоор хүрээлсэн хувьсагууд зөв сөлдөхөөр

```hunnu
let x = 10              // глобаль хүрээлэл
{
    let x = 20          // гаднах x-г сөлдөн
    print(x)            // 20
}
print(x)                // 10 (гаднах x хадгалагдсан)
```

#### break/continue
-Сигнал туг ашиглан fully буюу бүрэн хэрэгжлэх

```hunnu
let i = 0
while i < 10 {
    i = i + 1
    if i == 5 { break }
    if i == 3 { continue }
    print(i)
}
// Хэвлэх: 1, 2, 4
```

#### Санах Ой засварууд
- Парсер дээрх холбогдох зааг засварлагдсан
- Тэмдэгт гүнзгий хуулах implemented
- Массив элемент эзэмшил тодорхойлогдсон

---

## Өмнөх онцлогууд
- Хувьсагууд: `let x = 5`
- Арифметик: `+`, `-`, `*`, `/`, `%`
- Харьцуулалт: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Логик: `and`, `or`, `not`
- if/else өгүүлбэрүүд
- while давталт: `while(condition) { body }`
- for давталт: `for(init; condition; update) { body }`
- Функц: `fn нэр(параметр) { body }`
- return өгүүлбэрүүд: `return илэрхийлэл`
- print: `print(утга)`
- Дахин оноох: `x = шинэ_утга`
- Массив: `[1, 2, 3]` + хандалт `arr[i]`
- Тэмдэгт нийлэх: `"a" + "b"`
- len() суурилсан функц
- Нэрээр-first class функц дуудлага (by name)