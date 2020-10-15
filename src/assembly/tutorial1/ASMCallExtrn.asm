.data

myData db "abcd", 0

extrn MyCppProc:proc
extrn MyPritnf:proc

.code

CallMeProc proc
	;#############
    ; Prologue
	push rbp
	mov rbp, rsp
	sub rsp, 16                 ; 16 = 8 * Param count
	mov byte ptr [rbp - 8], 4   ; var1
	mov byte ptr [rbp - 16], 21 ; var2    

	; NOTE:  rbp - (negative)  -> local variable,   rbp + (positive) -> parameter
	;				e -> [rbp+30h]
	;				f -> [rbp+38h]
	;        

	;#############
	; Epilogue - Put everything back
	mov rsp, rbp
	pop rbp

	ret
CallMeProc endp

CallCppASM proc

    ; call CallMeProc
	sub rsp, 20h
	call CallMeProc
	add rsp, 20h

    ; call MyCppProc
	sub rsp, 30h
	mov dword ptr [rsp+20h], 555
	mov dword ptr [rsp+28h], 333
	call MyCppProc
	add rsp, 30h

	; call MyPritnf
	sub rsp, 20h
	lea rcx, myData 
	call MyPritnf
	add rsp, 20h

	ret
CallCppASM endp
end