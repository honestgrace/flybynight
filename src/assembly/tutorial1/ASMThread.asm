.data

mutex dw 0   ; our mutex!

extern CreateThread: proc

.code

ThreadStartProc proc
	mov rax, 10000000
LoopHead:
	lock inc dword ptr [rcx]
	;inc dword ptr [rcx]
	dec rax
	jnz LoopHead

	ret
ThreadStartProc endp

ThreadStartProc2 proc
	mov rax, 1000000

SpinLoop:              ; spin lock with a mutex
	lock bts mutex, 0  ; test and set bit 0 of mutex
	jc spinloop     ; if it's already taken, spin
	;
	; if a thread executes "lock bts mutex, 0", one of two things will happen:
	; 1. The bit must have been 0 prior to the instruction, thus, this thread now owns the mutex
	; 2. The carry falg will be set to 1, and the bit was already 1, so it will not change. 
	;    The mutex must be owned already, this thread must block or sopin until the mutex is released.
	; So we can check the carry flag with jc after an atomic bts.
	
	; !!!!!!!!!!!!!!!!!!!!!!!!!!! critical section!!!!!!!!!!!!!!!!!!!!!!!!!!
	mov rdx, 10000
LoopHead:
	inc dword ptr [rcx]
	dec rdx
	jnz LoopHead

	; !!!!!!!!!!!!!!!!!!!!!!!!!!! Release the mutex!!!!!!!!!!!!!!!!!!!!!!!!!
	mov mutex, 0
	; We have left the critical section!

	sub rax, 1000
	jnz SpinLoop

	ret
ThreadStartProc2 endp


CreateThreadInASM proc
	push rbp; save the caller's RBP
	mov rbp, rsp; save rsp of stack frame to rbp
 
	push 0 ; thread id
	push 0 ; creation flags, start immediately
	
	sub rsp, 20h   ; reserv 4 quads of shadow space

	mov r9, rcx  ; move *i to  r9
	mov rcx, 0   ; security attributes
	mov rdx, 0  ; stack size, use the same stack size as the calling thread
	mov r8, ThreadStartProc2
	
	call CreateThread

	mov rsp, rbp
	pop rbp     ; restore the caller's rbp
	ret
CreateThreadInASM endp
end