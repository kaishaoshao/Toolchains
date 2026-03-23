; ModuleID = 'target.c'
source_filename = "target.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @add(i32 noundef %0, i32 noundef %1) #0 !dbg !10 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
    #dbg_declare(ptr %4, !15, !DIExpression(), !16)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !17, !DIExpression(), !18)
  %6 = load i32, ptr %4, align 4, !dbg !19
  %7 = load i32, ptr %5, align 4, !dbg !21
  %8 = icmp sgt i32 %6, %7, !dbg !22
  br i1 %8, label %9, label %13, !dbg !23

9:                                                ; preds = %2
  %10 = load i32, ptr %4, align 4, !dbg !24
  %11 = load i32, ptr %5, align 4, !dbg !26
  %12 = sub nsw i32 %10, %11, !dbg !27
  store i32 %12, ptr %3, align 4, !dbg !28
  br label %17, !dbg !28

13:                                               ; preds = %2
  %14 = load i32, ptr %5, align 4, !dbg !29
  %15 = load i32, ptr %4, align 4, !dbg !31
  %16 = sub nsw i32 %14, %15, !dbg !32
  store i32 %16, ptr %3, align 4, !dbg !33
  br label %17, !dbg !33

17:                                               ; preds = %13, %9
  %18 = load i32, ptr %3, align 4, !dbg !34
  ret i32 %18, !dbg !34
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 20.0.0git (https://github.com/llvm/llvm-project.git 983869a0365c685049ea9015a2b4d7241b98fd4f)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "target.c", directory: "/home/shaokai/Desktop/test/klee", checksumkind: CSK_MD5, checksum: "d0a3f84586d907f50e26a03c1671d1d0")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 2}
!8 = !{i32 7, !"frame-pointer", i32 2}
!9 = !{!"clang version 20.0.0git (https://github.com/llvm/llvm-project.git 983869a0365c685049ea9015a2b4d7241b98fd4f)"}
!10 = distinct !DISubprogram(name: "add", scope: !1, file: !1, line: 2, type: !11, scopeLine: 2, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13, !13, !13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocalVariable(name: "a", arg: 1, scope: !10, file: !1, line: 2, type: !13)
!16 = !DILocation(line: 2, column: 13, scope: !10)
!17 = !DILocalVariable(name: "b", arg: 2, scope: !10, file: !1, line: 2, type: !13)
!18 = !DILocation(line: 2, column: 20, scope: !10)
!19 = !DILocation(line: 3, column: 7, scope: !20)
!20 = distinct !DILexicalBlock(scope: !10, file: !1, line: 3, column: 7)
!21 = !DILocation(line: 3, column: 11, scope: !20)
!22 = !DILocation(line: 3, column: 9, scope: !20)
!23 = !DILocation(line: 3, column: 7, scope: !10)
!24 = !DILocation(line: 4, column: 12, scope: !25)
!25 = distinct !DILexicalBlock(scope: !20, file: !1, line: 3, column: 14)
!26 = !DILocation(line: 4, column: 16, scope: !25)
!27 = !DILocation(line: 4, column: 14, scope: !25)
!28 = !DILocation(line: 4, column: 5, scope: !25)
!29 = !DILocation(line: 6, column: 12, scope: !30)
!30 = distinct !DILexicalBlock(scope: !20, file: !1, line: 5, column: 10)
!31 = !DILocation(line: 6, column: 16, scope: !30)
!32 = !DILocation(line: 6, column: 14, scope: !30)
!33 = !DILocation(line: 6, column: 5, scope: !30)
!34 = !DILocation(line: 8, column: 1, scope: !10)
