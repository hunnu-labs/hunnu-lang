use crate::opcodes::OpCode;
use crate::value::Value;

const STACK_MAX: usize = 256;
const MAX_LOCALS: usize = 256;

/// Bytecode program
pub struct Program {
    pub bytecode: Vec<u8>,
    pub constants: Vec<Value>,
}

/// Stack-based virtual machine for Hunnu bytecode
pub struct VM {
    stack: Vec<Value>,
    locals: Vec<Value>,
    ip: usize,
}

impl VM {
    pub fn new() -> Self {
        VM {
            stack: Vec::with_capacity(STACK_MAX),
            locals: Vec::with_capacity(MAX_LOCALS),
            ip: 0,
        }
    }

    fn push(&mut self, value: Value) -> Result<(), String> {
        if self.stack.len() >= STACK_MAX {
            return Err(format!("Stack overflow (max {} values)", STACK_MAX));
        }
        self.stack.push(value);
        Ok(())
    }

    fn pop(&mut self) -> Result<Value, String> {
        self.stack
            .pop()
            .ok_or_else(|| "Stack underflow - nothing to pop".to_string())
    }

    fn peek(&self) -> Option<&Value> {
        self.stack.last()
    }

    fn read_i64(&mut self, bytecode: &[u8]) -> i64 {
        let mut val: i64 = 0;
        for i in 0..8 {
            val |= (bytecode[self.ip + i] as i64) << (i * 8);
        }
        self.ip += 8;
        val
    }

    /// Execute a program
    pub fn run(&mut self, program: &Program) -> Result<(), String> {
        let bytecode = &program.bytecode;
        self.ip = 0;

        while self.ip < bytecode.len() {
            let op = OpCode::from_byte(bytecode[self.ip]);
            self.ip += 1;

            match op {
                Some(OpCode::ConstantInt) => {
                    let val = self.read_i64(bytecode);
                    self.push(Value::Int(val))?;
                }

                Some(OpCode::ConstantFloat) => {
                    let bits = self.read_i64(bytecode);
                    let val = f64::from_bits(bits as u64);
                    self.push(Value::Float(val))?;
                }

                Some(OpCode::ConstantString) => {
                    let idx = bytecode[self.ip] as usize;
                    self.ip += 1;
                    let value = program.constants[idx].clone();
                    self.push(value)?;
                }

                Some(OpCode::Nil) => {
                    self.push(Value::None)?;
                }

                Some(OpCode::True) => {
                    self.push(Value::Bool(true))?;
                }

                Some(OpCode::False) => {
                    self.push(Value::Bool(false))?;
                }

                Some(OpCode::Add) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Int(x + y),
                        (Value::Float(x), Value::Float(y)) => Value::Float(x + y),
                        (Value::Int(x), Value::Float(y)) => Value::Float(x as f64 + y),
                        (Value::Float(x), Value::Int(y)) => Value::Float(x + y as f64),
                        _ => Value::None,
                    };
                    self.push(result)?;
                }

                Some(OpCode::Subtract) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Int(x - y),
                        (Value::Float(x), Value::Float(y)) => Value::Float(x - y),
                        (Value::Int(x), Value::Float(y)) => Value::Float(x as f64 - y),
                        (Value::Float(x), Value::Int(y)) => Value::Float(x - y as f64),
                        _ => Value::None,
                    };
                    self.push(result)?;
                }

                Some(OpCode::Multiply) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Int(x * y),
                        (Value::Float(x), Value::Float(y)) => Value::Float(x * y),
                        (Value::Int(x), Value::Float(y)) => Value::Float(x as f64 * y),
                        (Value::Float(x), Value::Int(y)) => Value::Float(x * y as f64),
                        _ => Value::None,
                    };
                    self.push(result)?;
                }

                Some(OpCode::Divide) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => {
                            if y == 0 {
                                eprintln!("Error: Division by zero");
                                Value::None
                            } else {
                                Value::Int(x / y)
                            }
                        }
                        (Value::Float(x), Value::Float(y)) => {
                            if y == 0.0 {
                                eprintln!("Error: Division by zero");
                                Value::None
                            } else {
                                Value::Float(x / y)
                            }
                        }
                        (Value::Int(x), Value::Float(y)) => {
                            if y == 0.0 {
                                eprintln!("Error: Division by zero");
                                Value::None
                            } else {
                                Value::Float(x as f64 / y)
                            }
                        }
                        (Value::Float(x), Value::Int(y)) => {
                            if y == 0 {
                                eprintln!("Error: Division by zero");
                                Value::None
                            } else {
                                Value::Float(x / y as f64)
                            }
                        }
                        _ => Value::None,
                    };
                    self.push(result)?;
                }

                Some(OpCode::Modulo) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Int(x % y),
                        _ => Value::None,
                    };
                    self.push(result)?;
                }

                Some(OpCode::Greater) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Bool(x > y),
                        (Value::Float(x), Value::Float(y)) => Value::Bool(x > y),
                        (Value::Int(x), Value::Float(y)) => Value::Bool((x as f64) > y),
                        (Value::Float(x), Value::Int(y)) => Value::Bool(x > (y as f64)),
                        _ => Value::Bool(false),
                    };
                    self.push(result)?;
                }

                Some(OpCode::Less) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Bool(x < y),
                        (Value::Float(x), Value::Float(y)) => Value::Bool(x < y),
                        (Value::Int(x), Value::Float(y)) => Value::Bool((x as f64) < y),
                        (Value::Float(x), Value::Int(y)) => Value::Bool(x < (y as f64)),
                        _ => Value::Bool(false),
                    };
                    self.push(result)?;
                }

                Some(OpCode::GreaterEqual) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Bool(x >= y),
                        (Value::Float(x), Value::Float(y)) => Value::Bool(x >= y),
                        (Value::Int(x), Value::Float(y)) => Value::Bool((x as f64) >= y),
                        (Value::Float(x), Value::Int(y)) => Value::Bool(x >= (y as f64)),
                        _ => Value::Bool(false),
                    };
                    self.push(result)?;
                }

                Some(OpCode::LessEqual) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Bool(x <= y),
                        (Value::Float(x), Value::Float(y)) => Value::Bool(x <= y),
                        (Value::Int(x), Value::Float(y)) => Value::Bool((x as f64) <= y),
                        (Value::Float(x), Value::Int(y)) => Value::Bool(x <= (y as f64)),
                        _ => Value::Bool(false),
                    };
                    self.push(result)?;
                }

                Some(OpCode::Equal) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Bool(x == y),
                        (Value::Float(x), Value::Float(y)) => Value::Bool(x == y),
                        (Value::String(x), Value::String(y)) => Value::Bool(x == y),
                        (Value::Bool(x), Value::Bool(y)) => Value::Bool(x == y),
                        (Value::Int(x), Value::Float(y)) => Value::Bool((x as f64) == y),
                        (Value::Float(x), Value::Int(y)) => Value::Bool(x == (y as f64)),
                        _ => Value::Bool(false),
                    };
                    self.push(result)?;
                }

                Some(OpCode::NotEqual) => {
                    let b = self.pop()?;
                    let a = self.pop()?;
                    let result = match (a, b) {
                        (Value::Int(x), Value::Int(y)) => Value::Bool(x != y),
                        (Value::Float(x), Value::Float(y)) => Value::Bool(x != y),
                        (Value::String(x), Value::String(y)) => Value::Bool(x != y),
                        (Value::Bool(x), Value::Bool(y)) => Value::Bool(x != y),
                        (Value::Int(x), Value::Float(y)) => Value::Bool((x as f64) != y),
                        (Value::Float(x), Value::Int(y)) => Value::Bool(x != (y as f64)),
                        _ => Value::Bool(false),
                    };
                    self.push(result)?;
                }

                Some(OpCode::Not) => {
                    let v = self.pop()?;
                    let r = Value::Bool(!v.as_bool());
                    self.push(r)?;
                }

                Some(OpCode::Negate) => {
                    let v = self.pop()?;
                    let r = match v {
                        Value::Int(x) => Value::Int(-x),
                        Value::Float(x) => Value::Float(-x),
                        _ => Value::None,
                    };
                    self.push(r)?;
                }

                Some(OpCode::Jump) => {
                    let offset = self.read_i64(bytecode);
                    self.ip = (self.ip as i64 + offset) as usize;
                }

                Some(OpCode::JumpIfFalse) => {
                    let offset = self.read_i64(bytecode);
                    if let Some(v) = self.peek() {
                        if !v.as_bool() {
                            self.ip = (self.ip as i64 + offset) as usize;
                        }
                    }
                }

                Some(OpCode::Pop) => {
                    self.pop()?;
                }

                Some(OpCode::GetLocal) => {
                    let idx = bytecode[self.ip] as usize;
                    self.ip += 1;
                    if idx < self.locals.len() {
                        self.push(self.locals[idx].clone())?;
                    } else {
                        self.push(Value::None)?;
                    }
                }

                Some(OpCode::SetLocal) => {
                    let idx = bytecode[self.ip] as usize;
                    self.ip += 1;
                    if let Some(v) = self.peek() {
                        let val = v.clone();
                        while self.locals.len() <= idx {
                            self.locals.push(Value::None);
                        }
                        self.locals[idx] = val;
                    }
                }

                Some(OpCode::CreateArray) => {
                    let count = bytecode[self.ip] as usize;
                    self.ip += 1;
                    let mut elements = Vec::with_capacity(count);
                    for _ in 0..count {
                        elements.push(self.pop()?);
                    }
                    elements.reverse();
                    self.push(Value::Array(elements))?;
                }

                Some(OpCode::GetIndex) => {
                    let index = self.pop()?;
                    let arr = self.pop()?;
                    if let Value::Array(elements) = arr {
                        let idx: i64 = match &index {
                            Value::Float(f) => *f as i64,
                            Value::Int(n) => *n,
                            _ => 0,
                        };
                        let idx = if idx < 0 {
                            (idx + elements.len() as i64) as usize
                        } else {
                            idx as usize
                        };
                        if idx < elements.len() {
                            self.push(elements[idx].clone())?;
                        } else {
                            self.push(Value::None)?;
                        }
                    } else {
                        self.push(Value::None)?;
                    }
                }

                Some(OpCode::SetIndex) => {
                    let val = self.pop()?;
                    let index = self.pop()?;
                    let arr = self.pop()?;
                    if let Value::Array(mut elements) = arr {
                        let idx: i64 = match &index {
                            Value::Float(f) => *f as i64,
                            Value::Int(n) => *n,
                            _ => 0,
                        };
                        let idx = if idx < 0 {
                            (idx + elements.len() as i64) as usize
                        } else {
                            idx as usize
                        };
                        if idx < elements.len() {
                            elements[idx] = val.clone();
                            self.push(Value::Array(elements))?;
                        } else {
                            self.push(Value::None)?;
                        }
                    } else {
                        self.push(Value::None)?;
                    }
                }

                Some(OpCode::Call) => {
                    let _arg_count = bytecode[self.ip];
                    self.ip += 1;
                    // TODO: function calls
                }

                Some(OpCode::Return) | Some(OpCode::Halt) => {
                    return Ok(());
                }

                Some(_) | None => {
                    // Unknown opcode
                }
            }
        }

        Ok(())
    }
}
