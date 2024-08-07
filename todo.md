## 30th July 2024
- Implemented parsing for where clause
- tables and columns are only alphnumeric values
    - follow this up to extend with table qualifiers.
- Add small error logging for parsing.

## 31st July 2024

- Initiate building relational operators.
- Try convertig ast to logical plan using relational operators.

## 5th August 2024

- Now basic parsing and conversion to relational algebra works. 
- Next need to add support for
    - aggregation, keywords to be added
        - (sum, count, min, max, average)
        - group by
    - aliasing keyword 'as', attributes and relations can be aliased
    - expressions in select list, (attribute/constant op attribute/constant)
    - selectionpredicate 
        - (attribute = constant, >=, <=, >, <)
    - sorting, 
        - order by, (asc or desc)
    - add all kinds of join (inner, outer, left, right)
- After this, we need to 
    - infer join operation by looking at the JoinPredicateExpr, it goes inside and changes the cartesian product to join operation.

Current grammar is as below
```
query := SELECT <selList> FROM <fromList>
      := SELECT <selList> FROM <fromList> WHERE <condition>

selList := attribute | attribute, <selList>

fromList := relation | relation, <fromList>

condition := <condition> and/or <condition> |
             attribute in <query> |
             attribute = attribute |
             attribute like <pattern> |
```

## 7th August 2024

- Added a basic pipelined code generation for pandas/dataframe like api. 
- This needs to be refined further to be represented as an SSA perhaps. 

Todo, next: 
- For the selList, we change the grammar as follows, NOTE: THIS IS NOT YET IMPLEMENTED.

```
selList := <expr> | <expr>, <selList>
expr := const | attribute | <expr> <op> <expr>
op := +|-|/|*
```
the operations are only allowed for numeric(float or int) type of columns. Grammar stays as it is, we parse using simple pratt precedence parsing. 

- Can we directly lower it to for loops and stuff (simple for, pseudo code like), it seems possible.