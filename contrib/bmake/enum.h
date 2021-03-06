/*	$NetBSD: enum.h,v 1.9 2020/09/01 20:34:51 rillig Exp $	*/

/*
 Copyright (c) 2020 Roland Illig <rillig@NetBSD.org>
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MAKE_ENUM_H
#define MAKE_ENUM_H

/* Generate string representations for bitmasks and simple enums. */

#include <stddef.h>

typedef struct {
	int es_value;
	const char *es_name;
} EnumToStringSpec;

const char *Enum_FlagsToString(char *, size_t, int, const EnumToStringSpec *);
const char *Enum_ValueToString(int, const EnumToStringSpec *);

/* For Enum_FlagsToString, the separator between flags. */
#define ENUM__SEP "|"

/* Generate the string that joins all possible flags, to see how large the
 * buffer must be. */
#define ENUM__JOIN_STR_1(v1) \
	#v1
#define ENUM__JOIN_STR_2(v1, v2) \
	ENUM__JOIN_STR_1(v1) ENUM__SEP \
	ENUM__JOIN_STR_1(v2)
#define ENUM__JOIN_STR_4(v1, v2, v3, v4) \
	ENUM__JOIN_STR_2(v1, v2) ENUM__SEP \
	ENUM__JOIN_STR_2(v3, v4)
#define ENUM__JOIN_STR_8(v1, v2, v3, v4, v5, v6, v7, v8) \
	ENUM__JOIN_STR_4(v1, v2, v3, v4) ENUM__SEP \
	ENUM__JOIN_STR_4(v5, v6, v7, v8)
#define ENUM__JOIN_STR_16(v01, v02, v03, v04, v05, v06, v07, v08, \
			  v09, v10, v11, v12, v13, v14, v15, v16) \
	ENUM__JOIN_STR_8(v01, v02, v03, v04, v05, v06, v07, v08) ENUM__SEP \
	ENUM__JOIN_STR_8(v09, v10, v11, v12, v13, v14, v15, v16)

#define ENUM__JOIN_2(part1, part2) \
	part1 ENUM__SEP part2
#define ENUM__JOIN_3(part1, part2, part3) \
	part1 ENUM__SEP part2 ENUM__SEP part3
#define ENUM__JOIN_4(part1, part2, part3, part4) \
	part1 ENUM__SEP part2 ENUM__SEP part3 ENUM__SEP part4
#define ENUM__JOIN_5(part1, part2, part3, part4, part5) \
	part1 ENUM__SEP part2 ENUM__SEP part3 ENUM__SEP part4 ENUM__SEP part5

/* List the pairs of enum value and corresponding name. */
#define ENUM__SPEC_1(v1) \
	{ v1, #v1 }
#define ENUM__SPEC_2(v1, v2) \
	ENUM__SPEC_1(v1), \
	ENUM__SPEC_1(v2)
#define ENUM__SPEC_4(v1, v2, v3, v4) \
	ENUM__SPEC_2(v1, v2), \
	ENUM__SPEC_2(v3, v4)
#define ENUM__SPEC_8(v1, v2, v3, v4, v5, v6, v7, v8) \
	ENUM__SPEC_4(v1, v2, v3, v4), \
	ENUM__SPEC_4(v5, v6, v7, v8)
#define ENUM__SPEC_16(v01, v02, v03, v04, v05, v06, v07, v08, \
		      v09, v10, v11, v12, v13, v14, v15, v16) \
	ENUM__SPEC_8(v01, v02, v03, v04, v05, v06, v07, v08), \
	ENUM__SPEC_8(v09, v10, v11, v12, v13, v14, v15, v16)

#define ENUM__SPECS_2(part1, part2) \
	{ part1, part2, { 0, "" } }
#define ENUM__SPECS_3(part1, part2, part3) \
	{ part1, part2, part3, { 0, "" } }
#define ENUM__SPECS_4(part1, part2, part3, part4) \
	{ part1, part2, part3, part4, { 0, "" } }
#define ENUM__SPECS_5(part1, part2, part3, part4, part5) \
	{ part1, part2, part3, part4, part5, { 0, "" } }

/* Declare the necessary data structures for calling Enum_ValueToString. */
#define ENUM__VALUE_RTTI(typnam, specs) \
	static const EnumToStringSpec typnam ## _ ## ToStringSpecs[] = specs

/* Declare the necessary data structures for calling Enum_FlagsToString. */
#define ENUM__FLAGS_RTTI(typnam, specs, joined) \
	static const EnumToStringSpec typnam ## _ ## ToStringSpecs[] = specs; \
	enum { typnam ## _ ## ToStringSize = sizeof joined }

/* Declare the necessary data structures for calling Enum_FlagsToString
 * for an enum with 3 flags. */
#define ENUM_FLAGS_RTTI_3(typnam, v1, v2, v3) \
	ENUM__FLAGS_RTTI(typnam, \
	    ENUM__SPECS_2( \
	    	ENUM__SPEC_2(v1, v2), \
	    	ENUM__SPEC_1(v3)), \
	    ENUM__JOIN_2( \
	    	ENUM__JOIN_STR_2(v1, v2), \
	    	ENUM__JOIN_STR_1(v3)))

/* Declare the necessary data structures for calling Enum_FlagsToString
 * for an enum with 8 flags. */
#define ENUM_FLAGS_RTTI_8(typnam, v1, v2, v3, v4, v5, v6, v7, v8) \
	ENUM__FLAGS_RTTI(typnam, \
	    ENUM__SPECS_2( \
		ENUM__SPEC_4(v1, v2, v3, v4), \
		ENUM__SPEC_4(v5, v6, v7, v8)), \
	    ENUM__JOIN_2( \
		ENUM__JOIN_STR_4(v1, v2, v3, v4), \
		ENUM__JOIN_STR_4(v5, v6, v7, v8)))

/* Declare the necessary data structures for calling Enum_ValueToString
 * for an enum with 8 constants. */
#define ENUM_VALUE_RTTI_8(typnam, v1, v2, v3, v4, v5, v6, v7, v8) \
	ENUM__VALUE_RTTI(typnam, \
	    ENUM__SPECS_2( \
		ENUM__SPEC_4(v1, v2, v3, v4), \
		ENUM__SPEC_4(v5, v6, v7, v8)))

/* Declare the necessary data structures for calling Enum_FlagsToString
 * for an enum with 10 flags. */
#define ENUM_FLAGS_RTTI_10(typnam, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	ENUM__FLAGS_RTTI(typnam, \
	    ENUM__SPECS_2( \
		ENUM__SPEC_8(v1, v2, v3, v4, v5, v6, v7, v8), \
		ENUM__SPEC_2(v9, v10)), \
	    ENUM__JOIN_2( \
		ENUM__JOIN_STR_8(v1, v2, v3, v4, v5, v6, v7, v8), \
		ENUM__JOIN_STR_2(v9, v10)))

/* Declare the necessary data structures for calling Enum_FlagsToString
 * for an enum with 31 flags. */
#define ENUM_FLAGS_RTTI_31(typnam, \
		     v01, v02, v03, v04, v05, v06, v07, v08, \
		     v09, v10, v11, v12, v13, v14, v15, v16, \
		     v17, v18, v19, v20, v21, v22, v23, v24, \
		     v25, v26, v27, v28, v29, v30, v31) \
    ENUM__FLAGS_RTTI(typnam, \
        ENUM__SPECS_5( \
            ENUM__SPEC_16(v01, v02, v03, v04, v05, v06, v07, v08, \
			  v09, v10, v11, v12, v13, v14, v15, v16), \
	    ENUM__SPEC_8(v17, v18, v19, v20, v21, v22, v23, v24), \
	    ENUM__SPEC_4(v25, v26, v27, v28), \
	    ENUM__SPEC_2(v29, v30), \
	    ENUM__SPEC_1(v31)), \
	ENUM__JOIN_5( \
	    ENUM__JOIN_STR_16(v01, v02, v03, v04, v05, v06, v07, v08, \
			      v09, v10, v11, v12, v13, v14, v15, v16), \
	    ENUM__JOIN_STR_8(v17, v18, v19, v20, v21, v22, v23, v24), \
	    ENUM__JOIN_STR_4(v25, v26, v27, v28), \
	    ENUM__JOIN_STR_2(v29, v30), \
	    ENUM__JOIN_STR_1(v31)))

/* Declare the necessary data structures for calling Enum_FlagsToString
 * for an enum with 32 flags. */
#define ENUM_FLAGS_RTTI_32(typnam, \
		     v01, v02, v03, v04, v05, v06, v07, v08, \
		     v09, v10, v11, v12, v13, v14, v15, v16, \
		     v17, v18, v19, v20, v21, v22, v23, v24, \
		     v25, v26, v27, v28, v29, v30, v31, v32) \
    ENUM__FLAGS_RTTI(typnam, \
        ENUM__SPECS_2( \
            ENUM__SPEC_16(v01, v02, v03, v04, v05, v06, v07, v08, \
			  v09, v10, v11, v12, v13, v14, v15, v16), \
	    ENUM__SPEC_16(v17, v18, v19, v20, v21, v22, v23, v24, \
			  v25, v26, v27, v28, v29, v30, v31, v32)), \
	ENUM__JOIN_2( \
	    ENUM__JOIN_STR_16(v01, v02, v03, v04, v05, v06, v07, v08, \
			      v09, v10, v11, v12, v13, v14, v15, v16), \
	    ENUM__JOIN_STR_16(v17, v18, v19, v20, v21, v22, v23, v24, \
	    		      v25, v26, v27, v28, v29, v30, v31, v32)))

#endif
