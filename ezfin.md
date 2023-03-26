# EZFIN DSL

## EBNF

------------------------------

**PROGRAM** = { STATEMENT } ;

**STATEMENT** = ( DSL_STATEMENT | REG_STATEMENT ), `;`

------------------------------

**REG_STATEMENT** = ASSIGNMENT | LOOP | IF | FUNCTION;

**ASSIGNMENT** = IDENTIFIER , `=` , EXPRESSION

**LOOP** = `loop` , EXPRESSION , `{` , { STATEMENT } , `}`

**IF** = `if` , EXPRESSION , `{` , { STATEMENT } , `}` , [ `else` , `{` , { STATEMENT } , `}` ]

**FUNCTION** = `function` , IDENTIFIER , `(` , [ PARAMETER_LIST ] , `)` , `{` , { STATEMENT } , `}`

**PARAMETER_LIST** = FUNC_PARAMETER , { `,` ,   FUNC_PARAMETER }

**FUNC_PARAMETER** =  ( IDENTIFIER | PERSON_IDENTIFIER | NUMBER )

------------------------------

**DSL_STATEMENT** = DEPOSIT | WITHDRAW | TRANSFER | BALANCE | CREATE | DELETE ;

**CREATE** = `create` , PERSON_IDENTIFIER , INTEGER

**DELETE** = `delete` , PERSON_IDENTIFIER

**DEPOSIT** = `deposit` , PERSON_IDENTIFIER , INTEGER

**WITHDRAW** = `withdraw` , PERSON_IDENTIFIER , INTEGER

**TRANSFER** = `transfer`, PERSON_IDENTIFIER , PERSON_IDENTIFIER , INTEGER

**BALANCE** = `balance` , PERSON_IDENTIFIER

------------------------------

**EXPRESSION** = TERM, { (`+` | `-`), TERM } ;

**TERM** = FACTOR, { (`*` | `/`), FACTOR } ;

**FACTOR** = ((`+` | `-`), FACTOR) | NUMBER | `(`, EXPRESSION, `)` | IDENTIFIER | FUNCTION_CALL;

**FUNCTION_CALL** = IDENTIFIER , `(` , [ ARGUMENT_LIST ] , `)`

**ARGUMENT_LIST** = EXPRESSION , { `,` , EXPRESSION }

------------------------------

**PERSON_IDENTIFIER** = `_` , IDENTIFIER

**IDENTIFIER** = LETTER , { LETTER | DIGIT | `_`}

**INTEGER** = DIGIT , { DIGIT } ;

**LETTER** = "A" | "B" | ... | "Z" | "a" | "b" | ... | "z"

**DIGIT** = ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 )