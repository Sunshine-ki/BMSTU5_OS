.586P
; 286 страница.
; Разрешение трансляции всех комнд Pentium.
; Структура для описания дескрипторов сегментов.
descr struct	; Начало объялвения структуры.
	lim 	DW 0		
	base_1 	DW 0
	base_m 	DB 0
	attr_1 	DB 0
	attr_2 	DB 0
	base_h 	DB 0
descr ends
 
; Сегмент данных
data segment use16
	; Таблица глобальных дескрипторов GDT.
	gdt_null descr 		<0, 0, 0, 0, 0, 0>
	gdt_data descr 		<data_size-1, 0, 0, 92h, 0, 0>
	gdt_code descr 		<code_size-1, 0, 0, 98h, 0, 0>
	gdt_stack descr 	<255, 0, 0, 92h, 0, 0>
	gdt_screen descr 	<3999, 8000h, 0Bh, 92h, 0, 0>
	gdt_size=$-gdt_null
	; Различные данные программы.
	pdescr  DF 0
	sym     DB 1
	attr    DB 1Eh
	msg     DB 27, '[31;42m  Вернулись в реальный режим! ' ,27, '[0m$'
	data_size=$-gdt_null
data ends
 
text segment use16
    assume CS:text,DS:data
 
main proc
    XOR EAX, EAX
    MOV AX, data
    MOV DS, AX
    shl EAX, 4
    MOV EBP, EAX
    MOV BX, offset gdt_data
    MOV [BX].base_1, AX
    SHR EAX, 16
    MOV [BX].base_m, AL
    XOR EAX, EAX
    MOV AX, CS
    SHL EAX, 4
    MOV BX, offset gdt_code
    MOV [BX].base_1, AX
    SHR EAX, 16
    MOV [BX].base_m, AL

    XOR EAX, EAX
    MOV AX, SS
    SHL EAX, 4
    MOV BX, offset gdt_stack
    MOV [BX].base_1, AX
    SHR EAX, 16
    MOV [BX].base_m, AL
 
    MOV DWORD ptr pdescr+2, EBP
    MOV WORD  ptr pdescr, gdt_size-1
    LGDT pdescr
 
    MOV AX, 40h
    MOV ES, AX
    MOV WORD ptr ES:[67h], offset return
    MOV ES:[69h], CS
    MOV AL, 0Fh
    OUT 70h, AL
	MOV AL, 0Ah
    OUT 71h, AL
    CLI
 
    MOV EAX, CR0
    OR EAX, 1
    MOV CR0, EAX
 
    DB 0EAh
    DW offset continue
    DW 16
continue:
    MOV AX, 8
    MOV DS, AX
 
    MOV AX, 24
    MOV SS, AX
 
    MOV AX, 32
    MOV ES, AX
 
    MOV DI, 1920
    MOV CX, 80
    MOV AX, WORD ptr sym
scrn: 
    STOSW
    INC AL
    LOOP scrn
; Возвращаемся в реальный режим.
    MOV AL, 0FEh
    OUT 64h, AL
    HLT
return:
    MOV AX, data
    MOV DS, AX
    MOV AX, stk
    MOV SS, AX
    MOV SP, 256
    STI
 
    MOV AH, 09h
    MOV DX, offset msg
    INT 21h
    MOV AX, 4C00h
    INT 21h
main endp

code_size = $-main
text ends
 
; Стек сегмента.
stk segment stack use16
    DB 256 dup('^')
stk ends

end main