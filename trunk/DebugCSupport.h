/* DebugCSupport.h */

#ifndef DEBUGCSUPPORT_H
#define DEBUGCSUPPORT_H 1

#ifndef NDEBUG
#ifdef assert

#define detect_C_concatenation_op(src,src_len)	(assert(NULL!=src),assert(0<src_len),detect_C_concatenation_op(src,src_len))
#define detect_C_stringize_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_C_stringize_op(src,src_len))
#define detect_C_left_bracket_op(src,src_len)	(assert(NULL!=src),assert(0<src_len),detect_C_left_bracket_op(src,src_len))
#define detect_C_right_bracket_op(src,src_len)	(assert(NULL!=src),assert(0<src_len),detect_C_right_bracket_op(src,src_len))
#define detect_C_left_brace_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_C_left_brace_op(src,src_len))
#define detect_C_right_brace_op(src,src_len)	(assert(NULL!=src),assert(0<src_len),detect_C_right_brace_op(src,src_len))

#define detect_CPP_and_op(src,src_len)			(assert(NULL!=src),assert(0<src_len),detect_CPP_and_op(src,src_len))
#define detect_CPP_or_op(src,src_len)			(assert(NULL!=src),assert(0<src_len),detect_CPP_or_op(src,src_len))
#define detect_CPP_xor_op(src,src_len)			(assert(NULL!=src),assert(0<src_len),detect_CPP_xor_op(src,src_len))
#define detect_CPP_not_op(src,src_len)			(assert(NULL!=src),assert(0<src_len),detect_CPP_not_op(src,src_len))
#define detect_CPP_and_eq_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_and_eq_op(src,src_len))
#define detect_CPP_or_eq_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_or_eq_op(src,src_len))
#define detect_CPP_xor_eq_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_xor_eq_op(src,src_len))
#define detect_CPP_not_eq_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_not_eq_op(src,src_len))
#define detect_CPP_bitand_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_bitand_op(src,src_len))
#define detect_CPP_bitor_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_bitor_op(src,src_len))
#define detect_CPP_compl_op(src,src_len)		(assert(NULL!=src),assert(0<src_len),detect_CPP_compl_op(src,src_len))

#endif	/* #ifdef assert */
#endif  /* #ifndef NDEBUG */

#endif
