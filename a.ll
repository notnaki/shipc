; ModuleID = 'm_mod'
source_filename = "m_mod"

%Hey = type { i32, i32 }

@.str = private constant [19 x i8] c"member access: %i\0A\00"
@.str.1 = private constant [19 x i8] c"array access: %i \0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %x = alloca %Hey, align 8
  store %Hey { i32 10, i32 20 }, ptr %x, align 4
  %x.value = load %Hey, ptr %x, align 4
  %mem_access = extractvalue %Hey %x.value, 1
  %0 = call i32 (ptr, ...) @printf(ptr @.str, i32 %mem_access)

  ret i32 0
}
