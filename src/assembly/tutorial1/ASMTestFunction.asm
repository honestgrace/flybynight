.data
; [label] [size] [initialvalue/?]

; 
; Byte: db, byte, sbyte
; Word: dw, word, sword
; Double Words: dd, dword, sdword
; Quad words: dq, qword, sqword

; xmmword: xmmword - SSE, 128 bits, 16 bytes
; ymmword: ymmword - AVX, 256 bits, 32 bytes

; float: real4 - Single
; double: real8 - Double
; ??: real10 - x87 floating point unit
; Ex) myFloat real4 10.0
; Ex) myDoubleFloat real8 ?

myByteArray db 1000 dup (0)        ; one dimentional array char[1000]
myByteArray2 db 1, 2, 3, 4         ; one dimentional array char[4] = {1, 2, 3, 4}
myByteArray3 db 'abcd', 0          ; null terminated string char[4] = "abcd"
myDWORD dd 'ABCD'                  ; 

myWordArray dw 10 dup (7 dup (6))  ; two dimentional array [10][7]
myByteArray4 db 10 dup (1, 2, 3, 4, 5)

mybyte db 0
mybyte2 db 9

.code

AddASM proc
    mov eax, ecx
	add eax, edx

	ret
AddASM endp

TestFunction proc

	; General purpose
	;	Ax Accumulator
	;	Bx base
	;	Cx Counter
	;	Dx data

	; String 
	; SI Source index
	; DI Desitination Index
	; Bp Base Pointer
	; SP Stack pointer

	; CS Code segment  -> Code
	; DS data segment -> Data
	; SS stack segment -> Stack
	; ES Extra segment

	; IP instruction pointer
	
	; AX 1 Word (2 bytes) -> AH AL
	; BX                  -> BH BL
	; ...

	; ###################
	; Eax => 32 bit   (486)
	; ...

	; ###################
	; RAx => 64 bit   (amd64)
	; R8~R15
	; DS,ES,SS is not used anymore
	; FS, GS newly added

	; mov [reg/mem], [reg/mem/imm]
	; add [reg/mem], [reg/mem/imm]
	; sub [reg/mem], [reg/mem/imm]
	; neg [reg/mem]
	; inc [reg/mem]
	; dec [reg/mem]
	; xchg [reg/mem], [reg/mem]  - Exchange
	; ret  - POP EIP to go back to the caller
	; jmp [reg/mem/label+offset]  - Unconditional jump
	; cmp [reg/mem], [reg/mem/imm]
	; je/jz - Jump equal (jz -> jump if zero)
	; jne/jnz - Jump not equal 
	; jl/jb - Jump if less   (jb -> jump if below)     (cf. jb and ja is used for unsigned int; jl and jg is used for signed int)
	; jle - Jump if less or equal
	; jnl - Jump if not less
	; jg/ja - Jump if greater (ja -> jump if above)
	; jng - Jump if not greater
	; jge - Jump if greater or equal
	; shr [reg/mem], [imm8/cl]
	; shl
	; sar  -> for signed negative value fill with 1 if signed bit was 1
	; sal  
	; rol [reg/mem], [imm/cl] -> Rotate
	; ror
	; rcl                     -> Rotate through carry
	; rcr
	;
	; ********************************************************************
	;
	; === Single operand version
	; mul [reg/mem]   -> Unsigned
	; imul   -> Signed
	;
	;          op1   ImpliedOp2    Answer
	;          byte  al            ax
	;          word  ax            dx:ax
	;          dword eax           edx:eax
	;          qword rax           rdx:rax
	;
	; === Two operands version
	; imul  [reg], [reg/mem]         ; op1 = op1 * op2
	;
	; === Three operands version
	; imul  [reg], [reg/mem], [imm8] ; op1 = op2 * op3
	;
	; ********************************************************************
	; div                   dividedend/divisor
	; idiv
	;           
	;          divisor  ImpliedDivended  Answer       Remaineder
	;          byte     ax               al           ah
	;          word     dx:ax            ax           dx
	;          dword    edx:eax          eax          edx
	;          qword    rdx:rax          rax          rdx
	;

	; Conditional verb with compare
	; cmovne   -> Conditional move if Not Equal
	; Ex)
	;       mov ebx, 20
	;       mov eax, 20
	;       cmp ebx, eax
	;       cmovne ecx, ebx
	; 
	; Conditional verb with <carry> flag   (11111111b + 1 => carry flag is set to 1)
	; 
	;   Name   Abbrev.  Jumps   Conditional Moves
	;   Carry  CF/C1    JC/JNC  CMOVC, CMOVNC
	;   Parity PF/PE	JP/JNP	CMOVP, CMOVNP
	;                           CMOVPE, CMOVPO    (E:Even/O:Odd)
	;   Zero   ZF/ZR    JE/JNE  CMOVZ, CMOVNZ
	;                   JZ/JNZ  CMOVE, CMOVNE
	;   Sign   SF/PL/NF JS/JNS  CMOVS, CMOVNS
	;   Overflow OF/OU  JO,JNO	CMOVO, CMOVNO
	;   Ex)
	;        mov al, 01111111b
	;        add al, 1b ; 10000000b   (overflow)
	
	; Addressing Modes
	; register
	; immediate      FFFFh    =  X     (immediatenumber can't be started with alphabet character)
	;                0FFFFh   =  0   
	; 
	;                hexa: h   
	;                octal(0~7): o    
	;                binary: b
	;                decimal: empty or d
	; label/variable
	; [label +/- offset]
	;    Ex)    a db 3 
	;			mov byte ptr [a+1], 6
	; [reg + offset]
	; [reg + reg]
	; SIB (Scale Index Base)
	; [reg + reg * scale]      scales => 1, 2, 4, 8

	; pop/push [imm/reg/mem] - 16 or 64 bit data only
	; push x
	;       mov ptr [rsp - sizeof(x)], x
	;       sub rsp, sizeof(x)
	; pop x
	;       mov x, ptr [rsp]
	;       add rsp, sizeof(x)

	; MyProc proc
	;       call TestProc    ->   push nextline; mov rip, TestProc
	;       ret              ->   pop rip
	; MyProc endp
	;
	; pushf/popf    -> push/pop 16bit flag register
	; pushfq/popfq  ->          64bit

	; Microsoft C++ compiler FastCall calling convention
    ;          QW     DWORD   WORD   BYTE(or bool)	        
	; Param1 - RCX    ECX     CX     CL
	; Param2 - RDX    EDX     DX     DL
	; Param3 - R8     R8D     R8W    R8B
	; Param4 - R9     R9D     R9W    R9B
	; Param5 and above - Stack

	; &  (and)
	; |  (or)
	; ^  (xor)  - output 1 only when they are differnt each other
	; ! (cf ~)  (not)

	; (byte|word|dward|qword|xmmword|ymmword ptr)
	; lea Load effective address

	; C++  
	; void Func(int a, int b, int c, int d)
	;        sub rsp, 20h  -> (20h = 32 = 8 * 4)  Make 32 bytes of shadow space; Note even there was only parameter, it still makes 32 bytes shadow space
	;        mov ecx, a
	;        mov edx, b
	;        mov r8d, c
	;        mov r9d, d
    ;
	;
	; Call Func
	;        add rsp, 20h   - deallocate shadow space
	;
	; void FuncWith6Param(int a, int b, int c, int d, int e, int f)
	;        push f
	;        push e
	;        make 32 bytes of shadow space
	;        ...
	; 
	; Call FuncWith6Param
	;        add rsp, 48d  (48 = 8 * (4 + 2))
	;
	; *** This is actually converted like this
	;
	; void FuncWith6Param(int a, int b, int c, int d, int e, int f)
	;        sub rsp, 48d
	;        mov ecx, a
	;        mov edx, b
	;        mov r8d, c
	;        mov r9d, d
	;        mov [rsp+20h], e
	;        mov [rsp+28h], f
	;        ...
	; 
	; Call FuncWith6Param
	;        add rsp, 48d  (48 = 8 * (4 + 2))
	
	; From Callee
	;        
	; If No Local Data
	;        1) Prologue - Setup the base pointer
	;
    ;			push rbp        -> Save rbp 
	;			mov rbp, rsp    -> Set rsp to rbp

	;			NOTE: after this 
	;				e -> [rbp+30h]
	;				f -> [rbp+38h]
	;        
	;        2) Epilogue - Put everything back
	;
	;			mov rsp, rbp -> Automatically POP all my rubbish
	;			pop rbp
	;   
	;        3) ret
	;
	; If Local Data:  
	;            var1 = byte
	;            var2 = dword

	;        1) Prologue - Setup the base pointer
	;
    ;			push rbp        -> Save rbp 
	;			mov rbp, rsp    -> Set rsp to rbp
	;           sub rsp, 16     -> 16 = 8 * Param count
	;           mov [rbp - 8], 4   -> var1
	;           mov [rbp - 16], 21 -> var2          NOTE:  rbp - (negative)  -> local variable,   rbp + (positive) -> parameter
	;
	;			NOTE: after this 
	;				e -> [rbp+30h]
	;				f -> [rbp+38h]
	;        
	;        2) Epilogue - Put everything back
	;
	;			mov rsp, rbp -> Automatically POP all my rubbish
	;			pop rbp
	;   
	;        3) ret
	;
	;
	; test [reg/mem], [reg/imm]    - perform AND and set flags only
	; comp                         - perform substract and set flags only
	;
	; bt [reg/mem], [reg/imm] - bit test
	; btc - bit test then complement
	; btr - bit test then reset to 0
	; bts - bit test then set to 1
	;        Ex)  mov ax,2 
	;             bt ax, 1   -> Set carry flag to bit[i] of ax
	;
	; bswap [reg32/64]
	;	Endieness
	;		Little Endien
	;		Big Endien
	;	word 2 bytes
	;
	;	bswap eax	 : dcba -> abcd
	;   xchg al, ah
	;
	; bsf [reg 16,32,64], [reg/mem 16,32,64] - bit scan forwards
	; bsr - bit scan reverse
	;
	; Store string
	; stosb, stosw, stosd, stosq

	lea rbx, mybyte
	mov rcx, rbx
	mov ecx, dword ptr [rbx]
	mov qword ptr [rbx], 55h
	mov rcx, qword ptr [rbx]

	mov rcx, -2
	inc rcx
	inc rcx
	
	lea rax, mybyte
	mov rax, 267
	add al, mybyte2
	sub ax, 5
	mov ecx, ebx
	mov bl, mybyte
	mov mybyte, ch
	mov mybyte, 45
	neg eax
	inc eax
	inc mybyte2
	add al, mybyte2
	dec eax
	dec eax
	mov rax, 2
	mov rbx, 5
	xchg rax, rbx

	mov ax, 3
	mov bx, 3
	cmp ax, bx
	jl MyLabel
	jg MyLabel2
	mov ax, 555
    jmp MyLabel

MyLabel2:
	mov ax, 333
	jmp MyLabel

MyLabel:
    mov ecx, 10
MyLoop:
    dec ecx
	jnz MyLoop	
	
	ret
TestFunction endp
end