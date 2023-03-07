# 南京大学C--文法 NJU C-- Grammar

南京大学计算机科学与技术系 许畅等 2021春季版(略有改动)

by Prof. Chang Xu et al., Department of Computer Science and Technology of Nanjing University, spring 2021 edition(slightly modified)

## 终结符号 Tokens

- INT → /* a sequence of 0-9 */ $^1$
- FLOAT → /* a real number of 0-9 and .*/ $^2$
- ID → /* a string of a-z, A-Z, 0-9 and _ */ $^3$
- SEMI → ;
- COMMA → ,
- ASSIGNOP → =
- RELOP → > | < | >= | <= | == | !=
- PLUS → +
- MINUS → -
- STAR → *
- DIV → /
- AND → &&
- OR → ||
- DOT → .
- NOT → !
- TYPE → int
- LP → (
- RP → )
- LB → [
- RB → ]
- LC → {
- RC → }
- RETURN → return
- IF → if
- ELSE → else
- WHILE → while
- READ → read
- WRITE → write

$1$ *Size of integer is 32-bit.*

$2$ *The decimal point must be surrounded by at least one digit.*

$3$ *An identifier must not start with a digit.*

## 非终结符号 Non-terminals

- Program
- ExtDefList
- ExtDef
- DefList
- Def 
- DecList
- Dec
- FunDec
- VarList
- CompSt
- StmtList
- Stmt
- Exp Args

## 文法 Grammar

- Program →
  - ExtDefList
- ExtDefList →
  - ExtDef ExtDefList
  - $\epsilon$
- ExtDef →
  - TYPE DecList SEMI
  - TYPE FunDec CompSt
- DefList →
  - Def DefList
  - $\epsilon$
- Def →
  - TYPE DecList SEMI
- DecList →
  - Dec
  - Dec COMMA DecList
- Dec →
  - ID
  - Dec LB INT RB
- FunDec →
  - ID LP RP
  - ID LP VarList RP
- VarList →
  - TYPE Dec
  - TYPE Dec COMMA VarList
- CompSt →
  - LC DefList StmtList RC
- StmtList →
  - Stmt StmtList
  - $\epsilon$
- Stmt →
  - Exp SEMI
  - CompSt
  - RETURN Exp SEMI
  - IF LP Exp RP Stmt
  - IF LP Exp RP Stmt ELSE Stmt
  - WHILE LP Exp RP Stmt
- Exp →
  - LP Exp RP
  - Exp ASSIGNOP Exp
  - Exp PLUS Exp
  - Exp MINUS Exp
  - Exp STAR Exp
  - Exp DIV Exp
  - MINUS Exp
  - NOT Exp
  - Exp AND Exp
  - Exp OR Exp
  - Exp RELOP Exp
  - READ LP RP
  - WRITE LP Exp RP
  - ID LP RP
  - ID LP Args RP
  - Exp LB Exp RB
  - ID
  - INT
- Args →
  - Exp
  - Exp COMMA Args