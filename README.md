# Open Generic Lexical Analyzer

OGLA is a lexical analysis library for C++.

The main purpose of OGLA is to provide an easy to use lexical analyzer that is not tied to any particular language
grammar.  Its focus is on flexibility and ease of use, as opposed to speed and efficiency.  It's intended for projects
that only require **basic** lexical analysis.  It is **not intended for use in advanced applications** (e.g. a compiler).

Uses for this library:
+ parsing command line arguments
+ parsing custom config files
+ pretty printing


## `boost` branch

The `boost` branch of this repository has a variation of this library which uses
[Boost Regex](http://www.boost.org/doc/libs/1_60_0/libs/regex/doc/html/index.html) instead of `std::regex`. The reason
for maintaining this separate branch is that Boost Regex implements certain features (e.g. partial expression matching)
that are used to make the lexer more powerful. The branch may eventually get merged back into `devel` once the standard
library implements the same features.


## License

OGLA is distributed under the [Boost Software License](http://www.boost.org/users/license.html).
