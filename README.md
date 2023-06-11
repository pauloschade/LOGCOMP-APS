# EZFIN DSL

## Motivation

EZFIN DSL is a powerful domain-specific language designed specifically for financial modeling. With EZFIN DSL, you can effortlessly track people's balances and expenses. Whether you need to create, delete, deposit, withdraw, transfer, or check balances, EZFIN DSL makes it easy to manage your financial data with confidence. Whether you are a financial professional or just starting out, EZFIN DSL is designed to be straightforward and user-friendly, without sacrificing functionality.

In addition to its core features, EZFIN DSL also boasts a Turing-complete programming language that enables users to create complex functions and loops. With this powerful tool at your disposal, you can create sophisticated financial models with ease, without sacrificing safety or reliability.

## Example

```
create _bob 100;
create _alice 100;

deposit _bob 100;
deposit _alice 100;

transfer _bob _alice 50;

withdraw _bob 50;

balance _bob; -> 100
balance _alice; -> 250
```

## References

* https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
* https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/
* https://raywang.tech/2017/03/07/Flex-and-Bison-for-a-simple-language-MITScript/
