; These functions implements the basic operations for _int128 type
; running on 64-bit intel CPU.
; They are (almost) identical to MS's workhorse-functions that do
; __int64-math using 32-bit registers. Every memory reference that uses
; dword ptr(4 byte) are replaced by qword ptr(8 byte), and registernames
; eax, ebx, ecx... are replaced by rax, rbx, rcx etc.
; Parameters are passed in rcx(1st argument) and rdx(2nd argument) and
; return value is in rax, where the runtime-stack is used in x86 mode.
; So the code is modified to reflect this environment
;

.CODE

;void int128add(_int128 &dst, const _int128 &x); do assignop dst += x;
int128add PROC
    mov         rax, qword ptr[rdx]
    add         qword ptr[rcx], rax
    mov         rax, qword ptr[rdx+8]
    adc         qword ptr[rcx+8], rax
    ret
int128add ENDP

;void int128sub(_int128 &dst, const _int128 &x); do assignop dst -= x;
int128sub PROC
    mov         rax, qword ptr[rdx]
    sub         qword ptr[rcx], rax
    mov         rax, qword ptr[rdx+8]
    sbb         qword ptr[rcx+8], rax
    ret
int128sub ENDP

;void int128mul(_int128 &dst, const _int128 &x); do assignop dst *= x;
int128mul PROC
    push        rbx
    mov         rax, qword ptr[rdx+8]          ; rax  = x.hi
    mov         rbx, qword ptr[rcx+8]          ; rbx  = dst.hi
    or          rbx, rax                       ; rbx |= x.hi
    mov         rbx, qword ptr[rcx]            ; rbx  = dst.lo
    jne         Hard                           ; if(x.hi|dst.hi) goto Hard
                                               ; else simple int64 multiplication
    mov         rax, qword ptr[rdx]            ; rax  = x.lo
    mul         rbx                            ; rdx:rax = rax * rbx
    mov         qword ptr[rcx]  , rax          ; dst.lo = rax
    mov         qword ptr[rcx+8], rdx          ; dst.hi = rdx
    pop         rbx
    ret
Hard:                                          ; assume rax = x.hi, rbx = dst.lo
    push        rsi
    mov         rsi, rdx                       ; need rdx for highend of mul, so rsi = &x
    mul         rbx                            ; rdx:rax = x.hi*dst.lo
    mov         r9 , rax                       ; r9      = lo(x.hi*dst.lo)
    mov         rax, qword ptr[rsi]            ; rax     = x.lo
    mul         qword ptr[rcx+8]               ; rdx:rax = x.lo*dst.hi
    add         r9, rax                        ; r9     += lo(x.lo*dst.hi);
    mov         rax, qword ptr[rsi]            ; rax     = x.lo
    mul         rbx                            ; rdx:rax = x.lo*dst.lo
    add         rdx, r9                        ; rdx    += lo(x.hi*dst.lo) + lo(x.lo*dst.hi)
    mov         qword ptr[rcx]  , rax
    mov         qword ptr[rcx+8], rdx
    pop         rsi
    pop         rbx
    ret
int128mul ENDP

;void int128div(_int128 &dst, _int128 &x); do assignop dst /= x; if (x < 0) x = -x; !!
int128div PROC
    push        rbx                            ;
    push        rdi                            ;
    push        rsi                            ;
    mov         r8, rcx                        ; r8 = &dst
    mov         r9, rdx                        ; r9 = &x
    xor         rdi, rdi                       ; rdi = 0
    mov         rax, qword ptr[r8+8]           ; check sign of dst
    or          rax, rax                       ;
    jge         L1                             ; if(dst >= 0) goto L1
    inc         rdi                            ; rdi++
    mov         rdx, qword ptr[r8]             ; dst is negative. Change sign
    neg         rax                            ;
    neg         rdx                            ;
    sbb         rax, 0                         ;
    mov         qword ptr[r8+8], rax           ; dst = -original dst
    mov         qword ptr[r8], rdx             ;
L1:                                            ; Assume dst >= 0, rdi = (original dst < 0)1:0
    mov         rax, qword ptr[r9+8]           ; check sign of x
    or          rax, rax                       ;
    jge         L2                             ;
    inc         rdi                            ; x is negative. change sign, and increment rdi
    mov         rdx, qword ptr[r9]             ;
    neg         rax                            ;
    neg         rdx                            ;
    sbb         rax, 0                         ;
    mov         qword ptr[r9+8], rax           ; x = -original x
    mov         qword ptr[r9], rdx             ;
L2:                                            ; Assume dst >= 0, x > 0, rdi = #original negative arguments
    or          rax, rax                       ;
    jne         L3                             ;
    mov         rcx, qword ptr[r9]             ;
    mov         rax, qword ptr[r8+8]           ;
    xor         rdx, rdx                       ;
    div         rcx                            ;
    mov         rbx, rax                       ;
    mov         rax, qword ptr[r8]             ;
    div         rcx                            ;
    mov         rdx, rbx                       ;
    jmp         L4                             ;
L3:                                            ;
    mov         rbx, rax                       ;
    mov         rcx, qword ptr[r9]             ;
    mov         rdx, qword ptr[r8+8]           ;
    mov         rax, qword ptr[r8]             ;
L5:                                            ;
    shr         rbx, 1                         ;
    rcr         rcx, 1                         ;
    shr         rdx, 1                         ;
    rcr         rax, 1                         ;
    or          rbx, rbx                       ;
    jne         L5                             ;
    div         rcx                            ;
    mov         rsi, rax                       ;
    mul         qword ptr[r9+8]                ;
    mov         rcx, rax                       ;
    mov         rax, qword ptr[r9]             ;
    mul         rsi                            ;
    add         rdx, rcx                       ;
    jb          L6                             ;
    cmp         rdx, qword ptr[r8+8]           ;
    ja          L6                             ;
    jb          L7                             ;
    cmp         rax, qword ptr[r8]             ;
    jbe         L7                             ;
L6:                                            ;
    dec         rsi                            ;
L7:                                            ;
    xor         rdx, rdx                       ;
    mov         rax, rsi                       ;
L4:                                            ;
    dec         rdi                            ;
    jne         L8                             ;
    neg         rdx                            ;
    neg         rax                            ;
    sbb         rdx, 0                         ;
L8:                                            ;
    pop         rsi                            ;
    pop         rdi                            ;
    pop         rbx                            ;
    mov         qword ptr[r8], rax             ;
    mov         qword ptr[r8+8], rdx           ;
    ret                                        ;
int128div ENDP

;void int128rem(_int128 &dst, _int128 &x); do assignop dst %= x; if (x < 0) x = -x; !!
int128rem PROC
    push        rbx                            ;
    push        rdi                            ;
    mov         r8, rcx                        ; r8 = &dst
    mov         r9, rdx                        ; r9 = &x
    xor         rdi, rdi                       ;
    mov         rax, qword ptr[r8+8]           ; check sign of dst
    or          rax, rax                       ;
    jge         L1                             ;
    inc         rdi                            ;
    mov         rdx, qword ptr[r8]             ; dst is negative. change sign
    neg         rax                            ;
    neg         rdx                            ;
    sbb         rax, 0                         ;
    mov         qword ptr[r8+8], rax           ; dst = -original dst
    mov         qword ptr[r8], rdx             ;
L1:                                            ; Assume dst>=0, rdi = (original dst < 0)1:0
    mov         rax, qword ptr[r9+8]           ; check sign of x
    or          rax, rax                       ;
    jge         L2                             ; if(x >= 0) goto L2
    mov         rdx, qword ptr[r9]             ; x is negative. Change sign
    neg         rax                            ;
    neg         rdx                            ;
    sbb         rax, 0                         ;
    mov         qword ptr[r9+8], rax           ;
    mov         qword ptr[r9], rdx             ;
L2:                                            ; Assume dst>=0 and x > 0, (original dst < 0)1:0. dont care about orignal sign of x
    or          rax, rax                       ;
    jne         L3                             ;
    mov         rcx, qword ptr[r9]             ;
    mov         rax, qword ptr[r8+8]           ;
    xor         rdx, rdx                       ;
    div         rcx                            ;
    mov         rax, qword ptr[r8]             ;
    div         rcx                            ;
    mov         rax, rdx                       ;
    xor         rdx, rdx                       ;
    dec         rdi                            ;
    jns         L4                             ;
    jmp         L8                             ;
L3:                                            ;
    mov         rbx, rax                       ;
    mov         rcx, qword ptr[r9]             ;
    mov         rdx, qword ptr[r8+8]           ;
    mov         rax, qword ptr[r8]             ;
L5:                                            ;
    shr         rbx, 1                         ;
    rcr         rcx, 1                         ;
    shr         rdx, 1                         ;
    rcr         rax, 1                         ;
    or          rbx, rbx                       ;
    jne         L5                             ;
    div         rcx                            ;
    mov         rcx, rax                       ;
    mul         qword ptr[r9+8]                ;
    xchg        rax, rcx                       ;
    mul         qword ptr[r9]                  ;
    add         rdx, rcx                       ;
    jb          L6                             ;
    cmp         rdx, qword ptr[r8+8]           ;
    ja          L6                             ;
    jb          L7                             ;
    cmp         rax, qword ptr[r8]             ;
    jbe         L7                             ;
L6:                                            ;
    sub         rax, qword ptr[r9]             ;
    sbb         rdx, qword ptr[r9+8]           ;
L7:                                            ;
    sub         rax, qword ptr[r8]             ;
    sbb         rdx, qword ptr[r8+8]           ;
    dec         rdi                            ;
    jns         L8                             ;
L4:                                            ;
    neg         rdx                            ;
    neg         rax                            ;
    sbb         rdx, 0                         ;
L8:                                            ;
    pop         rdi                            ;
    pop         rbx                            ;
    mov         qword ptr[r8], rax             ;
    mov         qword ptr[r8+8], rdx           ;
    ret                                        ;
int128rem ENDP

;void int128neg(_int128 &x); set x = -x;
int128neg PROC
    mov         rax, qword ptr[rcx]
    neg         rax
    mov         qword ptr[rcx], rax
    mov         rax, qword ptr[rcx + 8]
    adc         rax, 0
    neg         rax
    mov         qword ptr[rcx + 8], rax
    ret
int128neg ENDP

;void int128inc(_int128 &x); set x = x + 1;
int128inc PROC
    add         qword ptr[rcx], 1
    adc         qword ptr[rcx+8], 0
    ret
int128inc ENDP

;void int128dec(_int128 &x); set x = x - 1;
int128dec PROC
    sub         qword ptr[rcx], 1
    sbb         qword ptr[rcx+8], 0
    ret
int128dec ENDP

;void int128shr(int shft, _int128 &x); do assignop x >>= shft; (if(x<0) shift 1-bits in from left, else 0-bits)
int128shr PROC
    mov         rax, qword ptr[rdx+8]          ; rax = x.hi
    cmp         cl, 40h
    jae         More64                         ; if(cl >= 64) goto More64;
    sar         qword ptr[rdx+8], cl           ; shift x.hi
    shrd        qword ptr[rdx], rax, cl        ; shift x.lo taking new bits from x.hi (rax)
    ret
More64:                                        ; assume rax = x.hi
    cmp         cl, 80h
    jae         RetSign                        ; if(cl >= 128) goto RetSign;
    sar         qword ptr[rdx+8], 3Fh          ; set all bits in x.hi to sign-bit
    and         cl, 3Fh                        ; cl %= 64
    sar         rax, cl                        ; rax = x.hi >> cl
    mov         qword ptr[rdx], rax            ; x.lo = rax
    ret
RetSign:                                       ; assume rax = x.hi
    sar         rax,3Fh                        ; set all bits in rax to sign-bit
    mov         qword ptr[rdx], rax
    mov         qword ptr[rdx+8], rax
    ret
int128shr ENDP

;void int128shl(int shft, _int128 &x); do assignop x <<= shft;
int128shl PROC
    cmp         cl, 40h
    jae         More64                         ; if(cl >= 64) goto More64;
    mov         rax, qword ptr[rdx]            ; rax = x.lo
    shl         qword ptr[rdx], cl             ; shift x.lo
    shld        qword ptr[rdx+8], rax, cl      ; shift x.hi taking new bits from x.lo (rax)
    ret
More64:
    cmp         cl, 80h
    jae         RetZero                        ; if(cl >= 128) goto RetZero;
    and         cl, 3Fh                        ; cl %= 64
    mov         rax, qword ptr[rdx]            ; rax = x.lo
    shl         rax, cl                        ; shift rax (x.lo)
    mov         qword ptr[rdx+8], rax          ; x.hi = rax
    xor         rax, rax
    mov         qword ptr[rdx], rax            ; x.lo = 0
    ret
RetZero:
    xor         rax, rax                       ; return 0
    mov         qword ptr[rdx], rax
    mov         qword ptr[rdx+8], rax
    ret
int128shl ENDP

;int int128cmp(const _int128 &x1, const _int128 &x2); return sign(x1 - x2);
int128cmp PROC
    mov         rax, qword ptr[rcx+8]          ; x1.hi
    cmp         rax, qword ptr[rdx+8]          ; x2.hi
    jl          lessthan                       ; signed compare of x1.hi and x2.hi
    jg          greaterthan
    mov         rax, qword ptr[rcx]            ; x1.lo
    cmp         rax, qword ptr[rdx]            ; x2.lo
    jb          lessthan                       ; unsigned compare of x1.lo and x2.lo
    ja          greaterthan
    xor         rax, rax                       ; they are equal
    ret
greaterthan:
    mov         rax, 1
    ret
lessthan:
    mov         rax, -1
    ret
int128cmp ENDP

;void uint128div(_uint128 &dst, const _uint128 &x); do assignop dst /= x;
uint128div PROC
     push       rbx                            ; same as signed division
     push       rsi                            ; but without sign check on arguments
     mov        r8, rcx                        ; r8 = &dst
     mov        r9, rdx                        ; r9 = &x
     mov        rax, qword ptr[r9+8]           ;
     or         rax, rax                       ;
     jne        L1                             ;
     mov        rcx, qword ptr[r9]
     mov        rax, qword ptr[r8+8]
     xor        rdx, rdx
     div        rcx
     mov        rbx, rax
     mov        rax, qword ptr[r8]
     div        rcx
     mov        rdx, rbx
     jmp        L2
L1:
     mov        rcx, rax
     mov        rbx, qword ptr[r9]
     mov        rdx, qword ptr[r8+8]
     mov        rax, qword ptr[r8]
L3:
     shr        rcx, 1
     rcr        rbx, 1
     shr        rdx, 1
     rcr        rax, 1
     or         rcx, rcx
     jne        L3
     div        rbx
     mov        rsi, rax
     mul        qword ptr[r9+8]
     mov        rcx, rax
     mov        rax, qword ptr[r9]
     mul        rsi
     add        rdx, rcx
     jb         L4
     cmp        rdx, qword ptr[r8+8]
     ja         L4
     jb         L5
     cmp        rax, qword ptr[r8]
     jbe        L5
L4:
     dec        rsi
L5:
     xor        rdx, rdx
     mov        rax, rsi
L2:
     pop        rsi
     pop        rbx
     mov        qword ptr[r8], rax
     mov        qword ptr[r8+8], rdx
     ret
uint128div ENDP

; calculates unsigned remainder
;void uint128rem(_uint128 &dst, const _uint128 &x); do assignop dst %= x;
uint128rem PROC
     push       rbx
     mov        r8, rcx
     mov        r9, rdx
     mov        rax, qword ptr[r9+8]
     or         rax, rax
     jne        L1
     mov        rcx, qword ptr[r9]
     mov        rax, qword ptr[r8+8]
     xor        rdx, rdx
     div        rcx
     mov        rax, qword ptr[r8]
     div        rcx
     mov        rax, rdx
     xor        rdx, rdx
     jmp        L2
L1:
     mov        rcx, rax
     mov        rbx, qword ptr[r9]
     mov        rdx, qword ptr[r8+8]
     mov        rax, qword ptr[r8]
L3:
     shr        rcx, 1
     rcr        rbx, 1
     shr        rdx, 1
     rcr        rax, 1
     or         rcx, rcx
     jne        L3
     div        rbx
     mov        rcx, rax
     mul        qword ptr[r9+8]
     xchg       rax, rcx
     mul        qword ptr[r9]
     add        rdx, rcx
     jb         L4
     cmp        rdx, qword ptr[r8+8]
     ja         L4
     jb         L5
     cmp        rax, qword ptr[r8]
     jbe        L5
L4:
     sub        rax, qword ptr[r9]
     sbb        rdx, qword ptr[r9+8]
L5:
     sub        rax, qword ptr[r8]
     sbb        rdx, qword ptr[r8+8]
     neg        rdx
     neg        rax
     sbb        rdx, 0
L2:
     pop        rbx
     mov        qword ptr[r8], rax
     mov        qword ptr[r8+8], rdx
     ret
uint128rem ENDP

;void uint128shr(int shft, void *x); do assignop x >>= shft. always shift 0-bits in from left
uint128shr PROC
    cmp         cl, 40h
    jae         More64                         ; if(cl >= 64) goto More64;
    mov         rax, qword ptr[rdx+8]          ; rax = x.hi
    shr         qword ptr[rdx+8], cl           ; shift x.hi
    shrd        qword ptr[rdx], rax, cl        ; shift x.lo taking new bits from x.hi (rax)
    ret
More64:
    cmp         cl, 80h
    jae         RetZero                        ; if(cl >= 128) goto RetZero;
    and         cl, 3Fh                        ; cl %= 64
    mov         rax, qword ptr[rdx+8]          ; rax = x.hi
    shr         rax, cl                        ; rax >>= cl
    mov         qword ptr[rdx], rax            ; x.lo = rax
    xor         rax, rax
    mov         qword ptr[rdx+8], rax          ; x.hi = 0
    ret
RetZero:
    xor         rax, rax                       ; return 0
    mov         qword ptr[rdx], rax
    mov         qword ptr[rdx+8], rax
    ret
uint128shr ENDP

;int uint128cmp(const _uint128 &x1, const _uint128 &x2); return sign(x1 - x2);
uint128cmp PROC
     mov        rax, qword ptr[rcx+8]          ; x1.hi
     cmp        rax, qword ptr[rdx+8]          ; x2.hi
     jb         lessthan                       ; unsigned compare of x1.hi and x2.hi
     ja         greaterthan
     mov        rax, qword ptr[rcx]            ; x1.lo
     cmp        rax, qword ptr[rdx]            ; x2.lo
     jb         lessthan                       ; unsigned compare of x1.lo and x2.lo
     ja         greaterthan
     xor        rax, rax                       ; they are equal
     ret
greaterthan:
     mov        rax, 1
     ret
lessthan:
     mov        rax, -1
     ret
uint128cmp ENDP

END
