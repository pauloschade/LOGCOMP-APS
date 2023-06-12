# EZFIN DSL

## Motivation

Introducing EZFIN DSL, the ultimate domain-specific language crafted exclusively for financial modeling. With EZFIN DSL, effortlessly track balances and expenses with utmost precision. Whether you're handling tasks like creating, deleting, depositing, withdrawing, transferring, or checking balances, EZFIN DSL empowers you to manage your financial data with confidence. Tailored for both financial professionals and beginners, EZFIN DSL ensures a seamless and user-friendly experience while delivering powerful functionality.

In addition to its core features, EZFIN DSL offers a Turing-complete programming language that enables users to create intricate functions and loops. Harnessing this robust tool, you can effortlessly build sophisticated financial models while maintaining safety and reliability.

It's important to note that EZFIN DSL prioritizes precise balance calculations due to its finance-oriented nature. Therefore, floating points are not supported, and only integers can be used when working with numbers.

## Example

```
create _bob 100;
create _alice 100;

deposit _bob 100;
deposit _alice 100;

transfer _bob to _alice 50;

withdraw _bob 50;
```

* more examples are on the `dsl_compiler/test` directory

## How to run

* Go into the the `dsl_compiler` directory
* run `make`
* run `./parser < <filename>`
* Make sure that you have llvm installed on your machine, and that the instalation path matches the one in the `codegen.h` file. If not, update the path in the `codegen.h` file to match your llvm installation path.

## References

* https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
* https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/
* https://raywang.tech/2017/03/07/Flex-and-Bison-for-a-simple-language-MITScript/
