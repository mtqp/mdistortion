%include  "macros.asm"

section .text
	global suma
	global sumaDouble
	global mult_float
	global area_circ

area_circ:
	convencion_C
	push dword 0 		;ebp-16
	push dword 0		;ebp-20
	finit			;inicio la fpu
	fld qword [ebp+8]	;cargo radio
	fmul st0,st0		;r²
	fldpi
	fmul st0,st1		;pi.r²
	fst qword [ebp-16]
	mov eax,[ebp-16]
	mov edx,[ebp-20]
	add esp,8
	convencion_C_fin
	ret

mult_float:
	convencion_C
	push dword 0 		;ebp-16
	push dword 0		;ebp-20
	finit			;inicio la fpu
	fld qword [ebp+8]	;cargo los dos floats para sumar
	fld qword [ebp+16]
	fmul st0,st1
	fst qword [ebp-16]
	mov eax,[ebp-16]
	mov edx,[ebp-20]
	add esp,8
	convencion_C_fin
	ret

sumaDouble:
	convencion_C
	push dword 0 		;ebp-16
	push dword 0		;ebp-20
	finit			;inicio la fpu
	fld qword [ebp+8]	;cargo los dos floats para sumar
	fld qword [ebp+16]
	fadd st0,st1
	fst qword [ebp-16]
	mov eax,[ebp-16]
	mov edx,[ebp-20]
	add esp,8
	convencion_C_fin
	ret
suma:
	convencion_C
	push dword 0 		;ebp-16
	finit			;inicio la fpu
	fld dword [ebp+8]	;cargo los dos floats para sumar
	fld dword [ebp+12]
	fadd st0,st1
	fst dword [ebp-16]
	mov eax,[ebp-16]
	add esp,4
	convencion_C_fin

	ret
