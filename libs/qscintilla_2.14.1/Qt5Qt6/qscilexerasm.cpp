// This module implements the abstract QsciLexerAsm class.
//
// Copyright (c) 2023 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexerasm.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.  Note that we choose not to support explicit fold points.
QsciLexerAsm::QsciLexerAsm(QObject *parent)
    : QsciLexer(parent),
      fold_comments(true), fold_compact(true), comment_delimiter('~'),
      fold_syntax_based(true)
{
}


// The dtor.
QsciLexerAsm::~QsciLexerAsm()
{
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerAsm::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
    case BlockComment:
        return QColor(0x00, 0x7f, 0x00);

    case Number:
        return QColor(0x00, 0x7f, 0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
        return QColor(0x7f, 0x00, 0x7f);

    case Operator:
    case UnclosedString:
        return QColor(0x00, 0x00, 0x00);

    case CPUInstruction:
        return QColor(0x00, 0x00, 0x7f);

    case FPUInstruction:
    case Directive:
    case DirectiveOperand:
        return QColor(0x00, 0x00, 0xff);

    case Register:
        return QColor(0x46, 0xaa, 0x03);

    case ExtendedInstruction:
        return QColor(0xb0, 0x00, 0x40);

    case CommentDirective:
        return QColor(0x66, 0xaa, 0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerAsm::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerAsm::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Operator:
    case CPUInstruction:
    case Register:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case Comment:
    case BlockComment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the background colour of the text for a style.
QColor QsciLexerAsm::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0, 0xc0, 0xe0);

    return QsciLexer::defaultPaper(style);
}


// Returns the set of keywords.
const char *QsciLexerAsm::keywords(int set) const
{
    if (set == 1)
        return
            "aaa aad aam aas daa das "
            "ja jae jb jbe jc jcxz je jg jge jl jle jmp jna jnae jnb jnbe jnc "
            "jne jng jnge jnl jnle jno jnp jns jnz jo jp jpe jpo js jz jcxz "
            "jecxz jrcxz loop loope loopne loopz loopnz call ret "
            "add sub adc sbb neg cmp inc dec and or xor not test shl shr sal "
            "sar shld shrd rol ror rcl rcr cbw cwd cwde cdq cdqe cqo bsf bsr "
            "bt btc btr bts idiv imul div mul bswap nop "
            "lea mov movsx movsxd movzx xlatb bound xchg xadd cmpxchg "
            "cmpxchg8b cmpxchg16b "
            "push pop pushad popad pushf popf pushfd popfd pushfq popfq "
            "seta setae setb setbe setc sete setg setge setl setle setna "
            "setnae setnb setnbe setnc setne setng setnge setnl setnle setno "
            "setnp setns setnz seto setp setpe setpo sets setz salc "
            "clc cld stc std cmc lahf sahf "
            "cmovo cmovno cmovb cmovc cmovnae cmovae cmovnb cmovnc cmove "
            "cmovz cmovne cmovnz cmovbe cmovna cmova cmovnbe cmovs cmovns "
            "cmovp cmovpe cmovnp cmovpo cmovl cmovnge cmovge cmovnl cmovle "
            "cmovng cmovg cmovnle "
            "lock rep repe repz repne repnz "
            "cmpsb cmpsw cmpsq movsb movsw movsq scasb scasw scasd scasq "
            "stosb stosw stosd stosq "
            "cpuid rdtsc rdtscp rdpmc xgetbv "
            "llwpcb slwpcb lwpval lwpins "
            "crc32 popcnt lzcnt tzcnt movbe pclmulqdq rdrand "
            "andn bextr blsi blsmk blsr "
            "bzhi mulx pdep pext rorx sarx shlx shrx";

    if (set == 2)
        return
            "f2xm1 fabs fadd faddp fbld fbstp fchs fclex fcom fcomp fcompp "
            "fdecstp fdisi fdiv fdivp fdivr fdivrp feni ffree fiadd ficom "
            "ficomp fidiv fidivr fild fimul fincstp finit fist fistp fisub "
            "fisubr fld fld1 fldcw fldenv fldenvw fldl2e fldl2t fldlg2 fldln2 "
            "fldpi fldz fmul fmulp fnclex fndisi fneni fninit fnop fnsave "
            "fnsavew fnstcw fnstenv fnstenvw fnstsw fpatan fprem fptan "
            "frndint frstor frstorw fsave fsavew fscale fsqrt fst fstcw "
            "fstenv fstenvw fstp fstsw fsub fsubp fsubr fsubrp ftst fwait "
            "fxam fxch fxtract fyl2x fyl2xp1 fsetpm fcos fldenvd fnsaved "
            "fnstenvd fprem1 frstord fsaved fsin fsincos fstenvd fucom fucomp "
            "fucompp fcomi fcomip fucomi fucomip ffreep fcmovb fcmove fcmovbe "
            "fcmovu fcmovnb fcmovne fcmovnbe fcmovnu";

    if (set == 3)
        return
            "al ah bl bh cl ch dl dh ax bx cx dx si di bp eax ebx ecx edx esi "
            "edi ebx esp st0 st1 st2 st3 st4 st5 st6 st7 mm0 mm1 mm2 mm3 mm4 "
            "mm5 mm6 mm7 xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7 ymm0 ymm1 "
            "ymm2 ymm3 ymm4 ymm5 ymm6 ymm7 fs "
            "sil dil bpl r8b r9b r10b r11b r12b r13b r14b r15b r8w r9w r10w "
            "r11w r12w r13w r14w r15w rax rcx rdx rbx rsp rbp rsi rdi r8 r9 "
            "r10 r11 r12 r13 r14 r15 xmm8 xmm9 xmm10 xmm11 xmm12 xmm13 xmm14 "
            "xmm15 ymm8 ymm9 ymm10 ymm11 ymm12 ymm13 ymm14 ymm15 gs";

    if (set == 4)
        return
            "db dw dd dq dt do dy resb resw resd resq rest reso resy incbin "
            "equ times safeseh __utf16__ __utf32__ %+ default cpu float start "
            "imagebase osabi ..start ..imagebase ..gotpc ..gotoff ..gottpoff "
            "..got ..plt ..sym ..tlsie section segment __sect__ group "
            "absolute .bss .comment .data .lbss .ldata .lrodata .rdata "
            ".rodata .tbss .tdata .text alloc bss code exec data noalloc "
            "nobits noexec nowrite progbits rdata tls write private public "
            "common stack overlay class extern global common import export "
            "%define %idefine %xdefine %ixdefine %assign %undef %? %?? "
            "%defstr %idefstr %deftok %ideftok %strcat %strlen %substr %macro "
            "%imacro %rmacro %exitmacro %endmacro %unmacro %if %ifn %elif "
            "%elifn %else %endif %ifdef %ifndef %elifdef %elifndef %ifmacro "
            "%ifnmacro %elifmacro %elifnmacro %ifctx %ifnctx %elifctx "
            "%elifnctx %ifidn %ifnidn %elifidn %elifnidn %ifidni %ifnidni "
            "%elifidni %elifnidni %ifid %ifnid %elifid %elifnid %ifnum "
            "%ifnnum %elifnum %elifnnum %ifstr %ifnstr %elifstr %elifnstr "
            "%iftoken %ifntoken %eliftoken %elifntoken %ifempty %elifempty "
            "%ifnempty %elifnempty %ifenv %ifnenv %elifenv %elifnenv %rep "
            "%exitrep %endrep %while %exitwhile %endwhile %include "
            "%pathsearch %depend %use %push %pop %repl %arg %local %stacksize "
            "flat flat64 large small %error %warning %fatal %00 .nolist "
            "%rotate %line %! %final %clear struc endstruc istruc at iend "
            "align alignb sectalign bits use16 use32 use64 __nasm_major__ "
            "__nasm_minor__ __nasm_subminor__ ___nasm_patchlevel__ "
            "__nasm_version_id__ __nasm_ver__ __file__ __line__ __pass__ "
            "__bits__ __output_format__ __date__ __time__ __date_num__ "
            "__time_num__ __posix_time__ __utc_date__ __utc_time__ "
            "__utc_date_num__ __utc_time_num__ __float_daz__ __float_round__ "
            "__float__ __use_altreg__ altreg __use_smartalign__ smartalign "
            "__alignmode__ __use_fp__ __infinity__ __nan__ __qnan__ __snan__ "
            "__float8__ __float16__ __float32__ __float64__ __float80m__ "
            "__float80e__ __float128l__ __float128h__";

    if (set == 5)
        return
            "a16 a32 a64 o16 o32 o64 strict byte word dword qword tword oword "
            "yword nosplit %0 %1 %2 %3 %4 %5 %6 %7 %8 %9 abs rel $ $$ seg wrt";

    if (set == 6)
        return
            "movd movq paddb paddw paddd paddsb paddsw paddusb paddusw psubb "
            "psubw psubd psubsb psubsw psubusb psubusw pand pandn por pxor "
            "pcmpeqb pcmpeqd pcmpeqw pcmpgtb pcmpgtd pcmpgtw pmaddwd pmulhw "
            "pmullw psllw pslld psllq psrlw psrld psrlq psraw psrad packuswb "
            "packsswb packssdw punpcklbw punpcklwd punpckldq punpckhbw "
            "punpckhwd punpckhdq emms "
            "pavgb pavgw pextrw pinsrw pmovmskb pmaxsw pmaxub pminsw pminub "
            "pmulhuw psadbw pshufw prefetchnta prefetcht0 prefetcht1 "
            "prefetcht2 maskmovq movntq sfence "
            "paddsiw psubsiw pmulhrw pmachriw pmulhriw pmagw pdistib paveb "
            "pmvzb pmvnzb pmvlzb pmvgezb "
            "pfacc pfadd pfsub pfsubr pfmul pfcmpeq pfcmpge pfcmpgt pfmax "
            "pfmin pfrcp pfrcpit1 pfrcpit2 pfrsqit1 pfrsqrt pi2fd pf2id "
            "pavgusb pmulhrw femms "
            "pfnacc pfpnacc pi2fw pf2iw pswapd "
            "pfrsqrtv pfrcpv "
            "prefetch prefetchw "
            "addss addps subss subps mulss mulps divss divps sqrtss sqrtps "
            "rcpss rcpps rsqrtss rsqrtps maxss maxps minss minps cmpss comiss "
            "ucomiss cmpps cmpeqss cmpltss cmpless cmpunordss cmpneqss "
            "cmpnltss cmpnless cmpordss cmpeqps cmpltps cmpleps cmpunordps "
            "cmpneqps cmpnltps cmpnleps cmpordps andnps andps orps xorps "
            "cvtsi2ss cvtss2si cvttss2si cvtpi2ps cvtps2pi cvttps2pi movss "
            "movlps movhps movlhps movhlps movaps movups movntps movmskps "
            "shufps unpckhps unpcklps ldmxcsr stmxcsr "
            "addpd addsd subpd subsd mulsd mulpd divsd divpd sqrtsd sqrtpd "
            "maxsd maxpd minsd minpd cmpsd comisd ucomisd cmppd cmpeqsd "
            "cmpltsd cmplesd cmpunordsd cmpneqsd cmpnltsd cmpnlesd cmpordsd "
            "cmpeqpd cmpltpd cmplepd cmpunordpd cmpneqpd cmpnltpd cmpnlepd "
            "cmpordpd andnpd andpd orpd xorpd cvtsd2ss cvtpd2ps cvtss2sd "
            "cvtps2pd cvtdq2ps cvtps2dq cvttps2dq cvtdq2pd cvtpd2dq cvttpd2dq "
            "cvtsi2sd cvtsd2si cvttsd2si cvtpi2pd cvtpd2pi cvttpd2pi movsd "
            "movlpd movhpd movapd movupd movntpd movmskpd shufpd unpckhpd "
            "unpcklpd movnti movdqa movdqu movntdq maskmovdqu movdq2q movq2dq "
            "paddq psubq pmuludq pslldq psrldq punpcklqdq punpckhqdq pshufhw "
            "pshuflw pshufd lfence mfence "
            "addsubps addsubpd haddps haddpd hsubps hsubpd movsldup movshdup "
            "movddup lddqu fisttp "
            "psignb psignw psignd pabsb pabsw pabsd palignr pshufb pmulhrsw "
            "pmaddubsw phaddw phaddd phaddsw phsubw phsubd phsubsw "
            "extrq insertq movntsd movntss "
            "mpsadbw phminposuw pmuldq pmulld dpps dppd blendps blendpd "
            "blendvps blendvpd pblendvb pblendw pmaxsb pmaxuw pmaxsd pmaxud "
            "pminsb pminuw pminsd pminud roundps roundss roundpd roundsd "
            "insertps pinsrb pinsrd pinsrq extractps pextrb pextrd pextrq "
            "pmovsxbw pmovsxbd pmovsxbq pmovsxwd pmovsxwq pmovsxdq pmovzxbw "
            "pmovzxbd pmovzxbq pmovzxwd pmovzxwq pmovzxdq ptest pcmpeqq "
            "packusdw movntdqa "
            "pcmpgtq pcmpestri pcmpestrm pcmpistri pcmpistrm "
            "aesenc aesenclast aesdec aesdeclast aeskeygenassist aesimc "
            "xcryptcbc xcryptcfb xcryptctr xcryptecb xcryptofb xsha1 xsha256 "
            "montmul xstore "
            "vaddss vaddps vaddsd vaddpd vsubss vsubps vsubsd vsubpd "
            "vaddsubps vaddsubpd vhaddps vhaddpd vhsubps vhsubpd vmulss "
            "vmulps vmulsd vmulpd vmaxss vmaxps vmaxsd vmaxpd vminss vminps "
            "vminsd vminpd vandps vandpd vandnps vandnpd vorps vorpd vxorps "
            "vxorpd vblendps vblendpd vblendvps vblendvpd vcmpss vcomiss "
            "vucomiss vcmpsd vcomisd vucomisd vcmpps vcmppd vcmpeqss vcmpltss "
            "vcmpless vcmpunordss vcmpneqss vcmpnltss vcmpnless vcmpordss "
            "vcmpeq_uqss vcmpngess vcmpngtss vcmpfalsess vcmpneq_oqss "
            "vcmpgess vcmpgtss vcmptruess vcmpeq_osss vcmplt_oqss vcmple_oqss "
            "vcmpunord_sss vcmpneq_usss vcmpnlt_uqss vcmpnle_uqss vcmpord_sss "
            "vcmpeq_usss vcmpnge_uqss vcmpngt_uqss vcmpfalse_osss "
            "vcmpneq_osss vcmpge_oqss vcmpgt_oqss vcmptrue_usss vcmpeqps "
            "vcmpltps vcmpleps vcmpunordps vcmpneqps vcmpnltps vcmpnleps "
            "vcmpordps vcmpeq_uqps vcmpngeps vcmpngtps vcmpfalseps "
            "vcmpneq_oqps vcmpgeps vcmpgtps vcmptrueps vcmpeq_osps "
            "vcmplt_oqps vcmple_oqps vcmpunord_sps vcmpneq_usps vcmpnlt_uqps "
            "vcmpnle_uqps vcmpord_sps vcmpeq_usps vcmpnge_uqps vcmpngt_uqps "
            "vcmpfalse_osps vcmpneq_osps vcmpge_oqps vcmpgt_oqps "
            "vcmptrue_usps vcmpeqsd vcmpltsd vcmplesd vcmpunordsd vcmpneqsd "
            "vcmpnltsd vcmpnlesd vcmpordsd vcmpeq_uqsd vcmpngesd vcmpngtsd "
            "vcmpfalsesd vcmpneq_oqsd vcmpgesd vcmpgtsd vcmptruesd "
            "vcmpeq_ossd vcmplt_oqsd vcmple_oqsd vcmpunord_ssd vcmpneq_ussd "
            "vcmpnlt_uqsd vcmpnle_uqsd vcmpord_ssd vcmpeq_ussd vcmpnge_uqsd "
            "vcmpngt_uqsd vcmpfalse_ossd vcmpneq_ossd vcmpge_oqsd vcmpgt_oqsd "
            "vcmptrue_ussd vcmpeqpd vcmpltpd vcmplepd vcmpunordpd vcmpneqpd "
            "vcmpnltpd vcmpnlepd vcmpordpd vcmpeq_uqpd vcmpngepd vcmpngtpd "
            "vcmpfalsepd vcmpneq_oqpd vcmpgepd vcmpgtpd vcmptruepd "
            "vcmpeq_ospd vcmplt_oqpd vcmple_oqpd vcmpunord_spd vcmpneq_uspd "
            "vcmpnlt_uqpd vcmpnle_uqpd vcmpord_spd vcmpeq_uspd vcmpnge_uqpd "
            "vcmpngt_uqpd vcmpfalse_ospd vcmpneq_ospd vcmpge_oqpd vcmpgt_oqpd "
            "vcmptrue_uspd vcvtsd2ss vcvtpd2ps vcvtss2sd vcvtps2pd vcvtsi2ss "
            "vcvtss2si vcvttss2si vcvtpi2ps vcvtps2pi vcvttps2pi vcvtdq2ps "
            "vcvtps2dq vcvttps2dq vcvtdq2pd vcvtpd2dq vcvttpd2dq vcvtsi2sd "
            "vcvtsd2si vcvttsd2si vcvtpi2pd vcvtpd2pi vcvttpd2pi vdivss "
            "vdivps vdivsd vdivpd vsqrtss vsqrtps vsqrtsd vsqrtpd vdpps vdppd "
            "vmaskmovps vmaskmovpd vmovss vmovsd vmovaps vmovapd vmovups "
            "vmovupd vmovntps vmovntpd vmovhlps vmovlhps vmovlps vmovlpd "
            "vmovhps vmovhpd vmovsldup vmovshdup vmovddup vmovmskps vmovmskpd "
            "vroundss vroundps vroundsd vroundpd vrcpss vrcpps vrsqrtss "
            "vrsqrtps vunpcklps vunpckhps vunpcklpd vunpckhpd vbroadcastss "
            "vbroadcastsd vbroadcastf128 vextractps vinsertps vextractf128 "
            "vinsertf128 vshufps vshufpd vpermilps vpermilpd vperm2f128 "
            "vtestps vtestpd vpaddb vpaddusb vpaddsb vpaddw vpaddusw vpaddsw "
            "vpaddd vpaddq vpsubb vpsubusb vpsubsb vpsubw vpsubusw vpsubsw "
            "vpsubd vpsubq vphaddw vphaddsw vphaddd vphsubw vphsubsw vphsubd "
            "vpsllw vpslld vpsllq vpsrlw vpsrld vpsrlq vpsraw vpsrad vpand "
            "vpandn vpor vpxor vpblendwb vpblendw vpsignb vpsignw vpsignd "
            "vpavgb vpavgw vpabsb vpabsw vpabsd vmovd vmovq vmovdqa vmovdqu "
            "vlddqu vmovntdq vmovntdqa vmaskmovdqu vpmovsxbw vpmovsxbd "
            "vpmovsxbq vpmovsxwd vpmovsxwq vpmovsxdq vpmovzxbw vpmovzxbd "
            "vpmovzxbq vpmovzxwd vpmovzxwq vpmovzxdq vpackuswb vpacksswb "
            "vpackusdw vpackssdw vpcmpeqb vpcmpeqw vpcmpeqd vpcmpeqq vpcmpgtb "
            "vpcmpgtw vpcmpgtd vpcmpgtq vpmaddubsw vpmaddwd vpmullw vpmulhuw "
            "vpmulhw vpmulhrsw vpmulld vpmuludq vpmuldq vpmaxub vpmaxsb "
            "vpmaxuw vpmaxsw vpmaxud vpmaxsd vpminub vpminsb vpminuw vpminsw "
            "vpminud vpminsd vpmovmskb vptest vpunpcklbw vpunpcklwd "
            "vpunpckldq vpunpcklqdq vpunpckhbw vpunpckhwd vpunpckhdq "
            "vpunpckhqdq vpslldq vpsrldq vpalignr vpshufb vpshuflw vpshufhw "
            "vpshufd vpextrb vpextrw vpextrd vpextrq vpinsrb vpinsrw vpinsrd "
            "vpinsrq vpsadbw vmpsadbw vphminposuw vpcmpestri vpcmpestrm "
            "vpcmpistri vpcmpistrm vpclmulqdq vaesenc vaesenclast vaesdec "
            "vaesdeclast vaeskeygenassist vaesimc vldmxcsr vstmxcsr vzeroall "
            "vzeroupper "
            "vbroadcasti128 vpbroadcastb vpbroadcastw vpbroadcastd "
            "vpbroadcastq vpblendd vpermd vpermq vperm2i128 vextracti128 "
            "vinserti128 vpmaskmovd vpmaskmovq vpsllvd vpsllvq vpsravd "
            "vpsrlvd vpsrldq vpgatherdd vpgatherqd vgatherdq vgatherqq "
            "vpermps vpermpd vgatherdpd vgatherqpd vgatherdps vgatherqps "
            "vfrczss vfrczps vfrczsd vfrczpd vpermil2ps vperlil2pd vtestps "
            "vtestpd vpcomub vpcomb vpcomuw vpcomw vpcomud vpcomd vpcomuq "
            "vpcomq vphaddubw vphaddbw vphaddubd vphaddbd vphaddubq vphaddbq "
            "vphadduwd vphaddwd vphadduwq vphaddwq vphaddudq vphadddq "
            "vphsubbw vphsubwd vphsubdq vpmacsdd vpmacssdd vpmacsdql "
            "vpmacssdql vpmacsdqh vpmacssdqh vpmacsww vpmacssww vpmacswd "
            "vpmacsswd vpmadcswd vpmadcsswd vpcmov vpperm vprotb vprotw "
            "vprotd vprotq vpshab vpshaw vpshad vpshaq vpshlb vpshlw vpshld "
            "vpshlq "
            "vcvtph2ps vcvtps2ph "
            "vfmaddss vfmaddps vfmaddsd vfmaddpd vfmsubss vfmsubps vfmsubsd "
            "vfmsubpd vnfmaddss vnfmaddps vnfmaddsd vnfmaddpd vnfmsubss "
            "vnfmsubps vnfmsubsd vnfmsubpd vfmaddsubps vfmaddsubpd "
            "vfmsubaddps vfmsubaddpd "
            "vfmadd132ss vfmadd213ss vfmadd231ss vfmadd132ps vfmadd213ps "
            "vfmadd231ps vfmadd132sd vfmadd213sd vfmadd231sd vfmadd132pd "
            "vfmadd213pd vfmadd231pd vfmaddsub132ps vfmaddsub213ps "
            "vfmaddsub231ps vfmaddsub132pd vfmaddsub213pd vfmaddsub231pd "
            "vfmsubadd132ps vfmsubadd213ps vfmsubadd231ps vfmsubadd132pd "
            "vfmsubadd213pd vfmsubadd231pd vfmsub132ss vfmsub213ss "
            "vfmsub231ss vfmsub132ps vfmsub213ps vfmsub231ps vfmsub132sd "
            "vfmsub213sd vfmsub231sd vfmsub132pd vfmsub213pd vfmsub231pd "
            "vfnmadd132ss vfnmadd213ss vfnmadd231ss vfnmadd132ps vfnmadd213ps "
            "vfnmadd231ps vfnmadd132sd vfnmadd213sd vfnmadd231sd vfnmadd132pd "
            "vfnmadd213pd vfnmadd231pd vfnmsub132ss vfnmsub213ss vfnmsub231ss "
            "vfnmsub132ps vfnmsub213ps vfnmsub231ps vfnmsub132sd vfnmsub213sd "
            "vfnmsub231sd vfnmsub132pd vfnmsub213pd vfnmsub231pd";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerAsm::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Number:
        return tr("Number");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case CPUInstruction:
        return tr("CPU instruction");

    case FPUInstruction:
        return tr("FPU instruction");

    case Register:
        return tr("Register");

    case Directive:
        return tr("Directive");

    case DirectiveOperand:
        return tr("Directive operand");

    case BlockComment:
        return tr("Block comment");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case UnclosedString:
        return tr("Unclosed string");

    case ExtendedInstruction:
        return tr("Extended instruction");

    case CommentDirective:
        return tr("Comment directive");
    }

    return QString();
}


// Refresh all properties.
void QsciLexerAsm::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setCommentDelimiterProp();
    setSyntaxBasedProp();
}


// Read properties from the settings.
bool QsciLexerAsm::readProperties(QSettings &qs,const QString &prefix)
{
    fold_comments = qs.value(prefix + "foldcomments", true).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    comment_delimiter = qs.value(prefix + "commentdelimiter",
            QChar('~')).toChar();
    fold_syntax_based = qs.value(prefix + "foldsyntaxbased", true).toBool();

    return true;
}


// Write properties to the settings.
bool QsciLexerAsm::writeProperties(QSettings &qs,const QString &prefix) const
{
    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "commentdelimiter", comment_delimiter);
    qs.setValue(prefix + "foldsyntaxbased", fold_syntax_based);

    return true;
}


// Return true if comments can be folded.
bool QsciLexerAsm::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerAsm::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.asm.comment.multiline" property.
void QsciLexerAsm::setCommentProp()
{
    emit propertyChanged("fold.asm.comment.multiline",
            (fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerAsm::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact.
void QsciLexerAsm::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerAsm::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}


// Return the comment delimiter.
QChar QsciLexerAsm::commentDelimiter() const
{
    return comment_delimiter;
}


// Set the comment delimiter.
void QsciLexerAsm::setCommentDelimiter(QChar delimiter)
{
    comment_delimiter = delimiter;

    setCommentDelimiterProp();
}


// Set the "lexer.asm.comment.delimiter" property.
void QsciLexerAsm::setCommentDelimiterProp()
{
    emit propertyChanged("lexer.asm.comment.delimiter",
            textAsBytes(QString(comment_delimiter)).constData());
}


// Return true if folds are syntax-based.
bool QsciLexerAsm::foldSyntaxBased() const
{
    return fold_syntax_based;
}


// Set if folds are syntax-based.
void QsciLexerAsm::setFoldSyntaxBased(bool syntax_based)
{
    fold_syntax_based = syntax_based;

    setSyntaxBasedProp();
}


// Set the "fold.asm.syntax.based" property.
void QsciLexerAsm::setSyntaxBasedProp()
{
    emit propertyChanged("fold.asm.syntax.based",
            (fold_syntax_based ? "1" : "0"));
}
