# Rust-inline Asm To GCC-inline Asm Translation

A tool to translate Rust inline assembly to GCC inline assembly. This was 
written in about a week so it is very unstable and extremely barebones.
It is done as a Proof Of Concept for my attempt at GCC's GSOC 2024 project of 
implementing inline assembly in Rust (gcc-rs).

## Current Progress

#### Rust Styled (*Input*)
```
asm!(
     "mov {0}, {1}",
     "add {0}, 1",
     out(reg) dest,
     in(reg) src
)
```

#### GCC Styled (*Output*)
```
GCC Inline Assembly Node:
Templates:
	"mov %1, %0\n\t",
	"add $1, %0"
Output Operands: "=r" (dest)
Input Operands: "r" (src)
Clobbers:
```

**Note:** Only handles a subset of Rust's inline assembly syntax and proper 
validation of macro is not implented yet.