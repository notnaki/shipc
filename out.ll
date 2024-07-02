; ModuleID = 'm_mod'
source_filename = "m_mod"

@.str = private constant [3 x i8] c"%i\00"

declare i32 @printf(ptr, ...)

define i32 @a() {
entry:
  %x = alloca [1 x i32], align 4
  store [1 x i32] [i32 20], ptr %x, align 4
  ret i32 20
}

define i32 @main() {
entry:
  %f = alloca i32, align 4
  %0 = call i32 @a()
  store i32 %0, ptr %f, align 4
  %x = alloca i32, align 4
  store i32 10, ptr %x, align 4
  %addtmp = add i32 10, %0
  %1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %addtmp)
  %addtmp1 = add i32 10, %0
  ret i32 %addtmp1
}
