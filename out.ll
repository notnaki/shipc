; ModuleID = 'm_mod'
source_filename = "m_mod"

@.str = private constant [3 x i8] c"%i\00"

declare i32 @printf(i8*, ...)

declare i32 @strcmp(i8*, i8*)

define void @main() {
entry:
  %mTrue = alloca i1, align 1
  store i1 true, i1* %mTrue, align 1
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 110)
  ret void
}
