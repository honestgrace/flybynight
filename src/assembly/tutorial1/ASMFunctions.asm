.data

myByteArray db 1000 dup (0)

.code

; int Multiply (int a, int b)
; returns a * b
Multiply proc
    ; Single operand version 
	; mov eax, ecx   
	; imul edx   
	;

	; Two operands version
	imul ecx, edx   ; Mulity the operands
	mov eax, ecx   ; Move the answer into eax to return!
	ret
Multiply endp

; int Divide(int ecx, int edx)
; return ecx/edx
; We are dividing with 32 bit integers so
; the implied dividened will be EDX:EAX, 64 bit pair!
Divide proc
    ; We need to free EDX
	mov r8d, edx   ; r8d is now the divisor
	; We need the dividend in EAX
	; We need EDX to be it's sign!
	mov eax, ecx   ; Move the dividened into EAX
	
	cdq                   ; Convert to sign extended, fills ESX with
	                      ; the sign bit of EAX
	idiv r8d              ; Perform the division
	; The anser will be placed into eax

	ret
Divide endp

; int Remainder(int ecx, int edx)
; return ecx%edx
Remainder proc
    ; We need to free EDX
	mov r8d, edx   ; r8d is now the divisor
	; We need the dividend in EAX
	; We need EDX to be it's sign!
	mov eax, ecx   ; Move the dividened into EAX
	
	cdq                   ; Convert to sign extended, fills ESX with
	                      ; the sign bit of EAX
	idiv r8d              ; Perform the division
	; The anser will be placed into edx

	mov eax, edx          ; Chuck it into EAX to return
	ret
Remainder endp

ReturnMyArray proc
	lea rax, myByteArray
	ret
ReturnMyArray endp

ZeroArrayASM proc
    cmp edx, 0
	jle Finished

	cmp edx, 1
	jle SetFinalByte
	
	mov ax, 0     ; Set AX to 00
	mov r8d, edx  ; Save the original count to r8d
	shr edx, 1     ; Halve the count because we're using AX, not AL
	
MainLoop:
	mov word ptr [rcx], ax  ; Set two bytes to 0
	add rcx, 2              ; Move RCX to the next two bytes
	dec edx                 ; Decrement counter
	jnz MainLoop            ; Jump if we've more to set
	
	and r8d, 1              ; Check if there was an even number
	jz Finished             ; If there was, we're done!
	
SetFinalByte:
	mov byte ptr [rcx], 0

Finished:
	ret
ZeroArrayASM endp

GetValueFromASM proc
    mov eax, 1
	mov rax, 3
	ret
GetValueFromASM endp
end