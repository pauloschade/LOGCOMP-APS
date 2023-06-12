# EBNF

**PROGRAM** = { STATEMENT } ;

**STATEMENT** = ( DSL_STATEMENT | REG_STATEMENT ), `;`

------------------------------

**REG_STATEMENT** = DECLARATION | ASSIGNMENT | LOOP | IF | FUNCTION_DEC | FUNCTION_CALL | RETURN ;

**DECLARATION** = TYPE , IDENTIFIER {, `=` , EXPRESSION };

**ASSIGNMENT** = IDENTIFIER , `=` , EXPRESSION ;

**LOOP** = `loop` , EXPRESSION , `{` , { STATEMENT } , `}` ;

**IF** = `if` , EXPRESSION , `{` , { STATEMENT } , `}`

**FUNCTION_DEC** = TYPE , IDENTIFIER , `(` , [ DEC_PARAMS ] , `)` , `{` , { STATEMENT } , `}` ;

**FUNCTION_CALL** = IDENTIFIER , `(` , [ ARGUMENT_LIST ] , `)` ;

**RETURN** = `return` , EXPRESSION ;

------------------------------

**DSL_STATEMENT** = DEPOSIT | WITHDRAW | TRANSFER | CREATE | DELETE ;

**CREATE** = `create` , PERSON_IDENTIFIER , EXPRESSION ;

**DELETE** = `delete` , PERSON_IDENTIFIER ;

**DEPOSIT** = `deposit` , PERSON_IDENTIFIER , EXPRESSION ;

**WITHDRAW** = `withdraw` , PERSON_IDENTIFIER , EXPRESSION ;

**TRANSFER** = `transfer`, PERSON_IDENTIFIER , `to` ,PERSON_IDENTIFIER , EXPRESSION ;

------------------------------

**EXPRESSION** = TERM, { (`+` | `-`), TERM } ;

**TERM** = FACTOR, { (`*` | `/`), FACTOR } ;

**FACTOR** = ((`+` | `-`), FACTOR) | INTEGER | `(`, EXPRESSION, `)` | IDENTIFIER | FUNCTION_CALL ;

**FUNCTION_CALL** = IDENTIFIER , `(` , [ ARGUMENT_LIST ] , `)` ;

**ARGUMENT_LIST** = EXPRESSION , { `,` , EXPRESSION } ;

------------------------------

**TYPE** = `int` ;

**ARGUMENT_LIST** = FUNC_PARAMETER , { `,` ,   FUNC_PARAMETER } ;

**FUNC_PARAMETER** =  ( EXPRESSION ) ;

**DEC_PARAMS** = TYPE , IDENTIFIER, { `,` , TYPE , IDENTIFIER } ;

**PERSON_IDENTIFIER** = `_` , IDENTIFIER ;

**IDENTIFIER** = LETTER , { LETTER | DIGIT | `_`} ;

**INTEGER** = DIGIT , { DIGIT } ;

**LETTER** = "A" | "B" | ... | "Z" | "a" | "b" | ... | "z" ;

**DIGIT** = ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 ) ;