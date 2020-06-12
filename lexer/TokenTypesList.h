//
// Created by art on 6/11/20.
//

#ifndef LEXER_TOKENTYPESLIST_H
#define LEXER_TOKENTYPESLIST_H

#include <stdint.h>

namespace lexer
{
    enum class TokenType : size_t
    {
        Identifier,
        DollarIdentifier,          // $
        DollarIdentifierInString,  // ${

        // key words
        OpenTag,                   // <?php
        CloseTag,                  // ?>
        Include,                   // include
        Namespace,                 // namespace

        If,                        // if
        Else,                      // else
        ElseIf,                    // elseif
        While,                     // while
        For,                       // for
        Do,                        // do
        Foreach,                   // foreach
        As,                        // as
        Continue,                  // continue
        Break,                     // break

        Function,                  // function
        Return,                    // return
        Define,                    // define
        Static,                    // static
        Global,                    // global

        Echo,                      // echo
        Print,                     // print

        //data types cast
        Int,                       // int
        Integer,                   // integer
        Bool,                      // bool
        Boolean,                   // boolean
        Float,                     // float
        Double,                    // double
        Real,                      // real
        String,                    // string
        Array,                     // array
        Object,                    // object
        Binary,                    // binary

        //class properties
        Class,                     // class
        Public,                    // public
        Protected,                 // protected
        Private,                   // private
        This,                      // $this
        New,                       // new
        Instanceof,                // instanceof
        Extends,                   // extends
        Final,                     // final
        Const,                     // const
        Abstract,                  // abstract
        Trait,                     // trait
        Use,                       // use
        Self,                      // self
        Parent,                    // parent

        // key variables
        GLOBALS,                   // $GLOBALS
        SERVER,                    // $_SERVER
        REQUEST,                   // $_REQUEST
        POST,                      // $_POST
        GET,                       // $_GET
        FILES,                     // $_FILES
        ENV,                       // $_ENV
        COOKIE,                    // $_COOKIE
        SESSION,                   // $_SESSION

        // data types
        StringValueOneQuote,
        StringValueTwoQuotes,
        IntValue,
        FloatValue,
        True,
        False,
        NULLValue,                  // null
        AndWord,                    // and
        OrWord,                     // or
        XorWord,                    // xor
        // arithmetic tokens
        Add,                        // +
        Sub,                        // -
        Multi,                      // *
        Div,                        // /
        Mod,                        // %
        Exp,                        // **
        Assignment,                 // =
        AddAssignment,              // +=
        SubAssignment,              // -=
        MultiAssignment,            // *=
        DivAssignment,              // /=
        ModAssignment,              // %=
        Equal,                      // ==
        Identical,                  // ===
        NotEqualQuest,              // !=
        NotEqualBracket,            // <>
        NotIdentical,               // !==
        GreaterThan,                // >
        LessThan,                   // <
        GreaterThanEqual,           // >=
        LessThanEqual,              // <=
        Increment,                  // ++
        Decrement,                  // --
        // logic
        Ampersand,                  // &
        AndSymb,                    // &&
        OrSymb,                     // ||
        Not,                        // !
        BitwiseOr,                  // |
        BitwiseXor,                 // ^
        LeftShift,                  // <<
        RightShift,                 // >>
        BitwiseOrAssignment,        // |=
        BitwiseXorAssignment,       // ^=
        LeftShiftAssignment,        // <<=
        RightShiftOrAssignment,     // >>=
        // operations with string
        Concat,                     // .
        ConcatAssignment,           // .=
        // conditional assignment
        QuestMark,                  // ?
        Colon,                      // :
        NullCoalescing,             // ??
        // association in array
        ArrayKey,                   // =>
        VarReference,               // ->
        DoubleColon,                // ::
        // comments
        SingleLineSlashComment,     // //
        SingleLineHashTagComment,   // #
        MultiLineComment,           // /* */
        // punctuation
        Comma,                      // ,
        Semicolon,                  // ;
        LParen,                     // (
        RParen,                     // )
        LBracket,                   // [
        RBracket,                   // ]
        LBrace,                     // {
        RBrace,                     // }

        INVALID,

        TOKEN_NUMBER
    };

    std::string const TokenValue[static_cast<size_t>(TokenType::TOKEN_NUMBER)] =
    {
        "Identifier",
        "$",                       // DollarIdentifier
        "${}",                     // DollarIdentifierInString

        // key words
        "<?php",                   // OpenTag
        "?>",                      // CloseTag
        "include",                 // Include
        "namespace",               // Namespace

        "if",                      // If
        "else",                    // Else
        "elseif",                  // ElseIf
        "while",                   // While
        "for",                     // For
        "do",                      // Do
        "foreach",                 // Foreach
        "as",                      // As
        "continue",                // Continue
        "break",                   // Break

        "function",                // Function
        "return",                  // Return
        "define",                    // Define
        "static",                    // Static
        "global",                    // Global

        "echo",                      // Echo
        "print",                     // Print
        //data types cast
        "int",                       // Int
        "integer",                   // Integer
        "bool",                      // Bool
        "boolean",                   // Boolean
        "float",                     // Float
        "double",                    // Double
        "real",                      // Real
        "string",                    // String
        "array",                     // Array
        "object",                    // Object
        "binary",                    // Binary

        //class properties
        "class",                     // Class
        "public",                    // Public
        "protected",                 // Protected
        "private",                   // Private
        "$this",                     // This
        "new",                       // New
        "instanceof",                // Instanceof
        "extends",                   // Extends
        "final",                     // Final
        "const",                     // Const
        "abstract",                  // Abstract
        "trait",                     // Trait
        "use",                       // Use
        "self",                      // Self
        "parent",                    // Parent

        // key variables
        "$GLOBALS",                   // GLOBALS
        "$_SERVER",                    // SERVER
        "$_REQUEST",                   // REQUEST
        "$_POST",                      // POST
        "$_GET",                       // GET
        "$_FILES",                     // FILES
        "$_ENV",                       // ENV
        "$_COOKIE",                    // COOKIE
        "$_SESSION",                   // SESSION

        // data types
        "StringValueOneQuote",
        "StringValueTwoQuotes",
        "IntValue",
        "FloatValue",
        "true",
        "false",
        "null",                     // Null
        "and",                      // AndWord
        "or",                       // OrWord
        "xor",                      // XorWord

        // arithmetic tokens
        "+",                        // Add
        "-",                        // Sub
        "*",                        // Multi
        "/",                        // Div
        "%",                        // Mod
        "**",                       // Exp
        "=",                        // Assignment
        "+=",                       // AddAssignment
        "-=",                       // SubAssignment
        "*=",                       // MultiAssignment
        "/=",                       // DivAssignment
        "%=",                       // ModAssignment
        "==",                       // Equal
        "===",                      // Identical
        "!=",                       // NotEqualQuest
        "<>",                       // NotEqualBracket
        "!==",                      // NotIdentical
        ">",                        // GreaterThan
        "<",                        // LessThan
        ">=",                       // GreaterThanEqual
        "<=",                       // LessThanEqual
        "++",                       // Increment
        "--",                       // Decrement
        // logic
        "&",                        // Ampersand
        "&&",                       // AndSymb
        "||",                       // OrSymb
        "!",                        // Not
        "|",                        // BitwiseOr
        "^",                        // BitwiseXor
        "<<",                       // LeftShift
        ">>",                       // RightShift
        "|=",                       // BitwiseOrAssignment
        "^=",                       // BitwiseXorAssignment
        "<<=",                      // LeftShiftAssignment
        ">>=",                      // RightShiftOrAssignment
        // operations with string
        ".",                        // Concat
        ".=",                       // ConcatAssignment
        // conditional assignment
        "?",                        // QuestMark
        ":",                        // Colon
        "??",                       // NullCoalescing
        // association in array, class
        "=>",                       // ArrayKey
        "->",                       // VarReference,
        "::",                       // DoubleColon
        // comments
        "//",                       // SingleLineSlashComment
        "#",                        // SingleLineHashTagComment
        "/* */",                    // MultiLineComment
        // punctuation
        ",",                        // Comma
        ";",                        // Semicolon
        "(",                        // LParen
        ")",                        // RParen
        "[",                        // LBracket
        "]",                        // RBracket
        "{",                        // LBrace
        "}",                        // RBrace
        "INVALID"
    };
}

#endif //LEXER_TOKENTYPESLIST_H
