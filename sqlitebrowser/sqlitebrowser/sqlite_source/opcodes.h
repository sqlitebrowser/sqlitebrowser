/* Automatically generated.  Do not edit */
/* See the mkopcodeh.awk script for details */
#define OP_MemLoad                              1
#define OP_HexBlob                            125   /* same as TK_BLOB     */
#define OP_Column                               2
#define OP_SetCookie                            3
#define OP_IfMemPos                             4
#define OP_Real                               124   /* same as TK_FLOAT    */
#define OP_Sequence                             5
#define OP_MoveGt                               6
#define OP_Ge                                  71   /* same as TK_GE       */
#define OP_RowKey                               7
#define OP_Eq                                  67   /* same as TK_EQ       */
#define OP_OpenWrite                            8
#define OP_NotNull                             65   /* same as TK_NOTNULL  */
#define OP_If                                   9
#define OP_ToInt                              140   /* same as TK_TO_INT   */
#define OP_String8                             86   /* same as TK_STRING   */
#define OP_Pop                                 10
#define OP_CollSeq                             11
#define OP_OpenRead                            12
#define OP_Expire                              13
#define OP_AutoCommit                          14
#define OP_Gt                                  68   /* same as TK_GT       */
#define OP_IntegrityCk                         15
#define OP_Sort                                17
#define OP_Function                            18
#define OP_And                                 60   /* same as TK_AND      */
#define OP_Subtract                            78   /* same as TK_MINUS    */
#define OP_Noop                                19
#define OP_Return                              20
#define OP_Remainder                           81   /* same as TK_REM      */
#define OP_NewRowid                            21
#define OP_Multiply                            79   /* same as TK_STAR     */
#define OP_IfMemNeg                            22
#define OP_Variable                            23
#define OP_String                              24
#define OP_RealAffinity                        25
#define OP_ParseSchema                         26
#define OP_Close                               27
#define OP_CreateIndex                         28
#define OP_IsUnique                            29
#define OP_IdxIsNull                           30
#define OP_NotFound                            31
#define OP_Int64                               32
#define OP_MustBeInt                           33
#define OP_Halt                                34
#define OP_Rowid                               35
#define OP_IdxLT                               36
#define OP_AddImm                              37
#define OP_Statement                           38
#define OP_RowData                             39
#define OP_MemMax                              40
#define OP_Push                                41
#define OP_Or                                  59   /* same as TK_OR       */
#define OP_NotExists                           42
#define OP_MemIncr                             43
#define OP_Gosub                               44
#define OP_Divide                              80   /* same as TK_SLASH    */
#define OP_Integer                             45
#define OP_ToNumeric                          139   /* same as TK_TO_NUMERIC*/
#define OP_MemInt                              46
#define OP_Prev                                47
#define OP_Concat                              82   /* same as TK_CONCAT   */
#define OP_BitAnd                              73   /* same as TK_BITAND   */
#define OP_CreateTable                         48
#define OP_Last                                49
#define OP_IsNull                              64   /* same as TK_ISNULL   */
#define OP_IdxRowid                            50
#define OP_MakeIdxRec                          51
#define OP_ShiftRight                          76   /* same as TK_RSHIFT   */
#define OP_ResetCount                          52
#define OP_FifoWrite                           53
#define OP_Callback                            54
#define OP_ContextPush                         55
#define OP_DropTrigger                         56
#define OP_DropIndex                           57
#define OP_IdxGE                               58
#define OP_IdxDelete                           61
#define OP_Vacuum                              62
#define OP_MoveLe                              63
#define OP_IfNot                               72
#define OP_DropTable                           84
#define OP_MakeRecord                          87
#define OP_ToBlob                             138   /* same as TK_TO_BLOB  */
#define OP_Delete                              88
#define OP_AggFinal                            89
#define OP_ShiftLeft                           75   /* same as TK_LSHIFT   */
#define OP_Dup                                 90
#define OP_Goto                                91
#define OP_TableLock                           92
#define OP_FifoRead                            93
#define OP_Clear                               94
#define OP_IdxGT                               95
#define OP_MoveLt                              96
#define OP_Le                                  69   /* same as TK_LE       */
#define OP_VerifyCookie                        97
#define OP_AggStep                             98
#define OP_Pull                                99
#define OP_ToText                             137   /* same as TK_TO_TEXT  */
#define OP_Not                                 16   /* same as TK_NOT      */
#define OP_ToReal                             141   /* same as TK_TO_REAL  */
#define OP_SetNumColumns                      100
#define OP_AbsValue                           101
#define OP_Transaction                        102
#define OP_Negative                            83   /* same as TK_UMINUS   */
#define OP_Ne                                  66   /* same as TK_NE       */
#define OP_ContextPop                         103
#define OP_BitOr                               74   /* same as TK_BITOR    */
#define OP_Next                               104
#define OP_IdxInsert                          105
#define OP_Distinct                           106
#define OP_Lt                                  70   /* same as TK_LT       */
#define OP_Insert                             107
#define OP_Destroy                            108
#define OP_ReadCookie                         109
#define OP_ForceInt                           110
#define OP_LoadAnalysis                       111
#define OP_OpenVirtual                        112
#define OP_Explain                            113
#define OP_IfMemZero                          114
#define OP_OpenPseudo                         115
#define OP_Null                               116
#define OP_Blob                               117
#define OP_Add                                 77   /* same as TK_PLUS     */
#define OP_MemStore                           118
#define OP_Rewind                             119
#define OP_MoveGe                             120
#define OP_BitNot                              85   /* same as TK_BITNOT   */
#define OP_MemMove                            121
#define OP_MemNull                            122
#define OP_Found                              123
#define OP_NullRow                            126

/* The following opcode values are never used */
#define OP_NotUsed_127                        127
#define OP_NotUsed_128                        128
#define OP_NotUsed_129                        129
#define OP_NotUsed_130                        130
#define OP_NotUsed_131                        131
#define OP_NotUsed_132                        132
#define OP_NotUsed_133                        133
#define OP_NotUsed_134                        134
#define OP_NotUsed_135                        135
#define OP_NotUsed_136                        136

/* Opcodes that are guaranteed to never push a value onto the stack
** contain a 1 their corresponding position of the following mask
** set.  See the opcodeNoPush() function in vdbeaux.c  */
#define NOPUSH_MASK_0 0x7f58
#define NOPUSH_MASK_1 0xee5b
#define NOPUSH_MASK_2 0x9f76
#define NOPUSH_MASK_3 0xfff2
#define NOPUSH_MASK_4 0xffff
#define NOPUSH_MASK_5 0xdb3b
#define NOPUSH_MASK_6 0xcfdf
#define NOPUSH_MASK_7 0x49cd
#define NOPUSH_MASK_8 0x3e00
#define NOPUSH_MASK_9 0x0000
