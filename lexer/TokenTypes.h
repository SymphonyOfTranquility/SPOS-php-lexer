//
// Created by art on 6/11/20.
//

#ifndef LEXER_TOKENTYPES_H
#define LEXER_TOKENTYPES_H

#include <cstdio>

namespace lexer
{
    enum class TokenType : size_t
    {
        Identifier,

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
        Variable,                  // starts with dollar
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
        VarReference,              // ->
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
        StringValue,
        IntValue,
        FloatValue,
        True,
        False,
        NULLValue,                  // null
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
        AndSymb,                    // &&
        AndWord,                    // and
        OrSymb,                     // ||
        OrWord,                     // or
        XorWord,                    // xor
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
        // comments
        SingleLineSlashComment,     // //
        SingleLineHashTagComment,   // #
        MultiLineComment,           // /* */
        // conditional assignment
        QuestMark,                  // ?
        Colon,                      // :
        NullCoalescing,             // ??
        // association in array
        ArrayKey,                   // =>
        // punctuation
        Comma,                      // ,
        Semicolon,                  // ;
        DoubleColon,                // ::
        LParen,                     // (
        RParen,                     // )
        LBracket,                   // [
        RBracket,                   // ]
        LBrace,                     // {
        RBrace,                     // }
        Ampersand,                  // &

        TOKEN_NUMBER
    };

    char const *TokenValue[static_cast<size_t>(TokenType::TOKEN_NUMBER)] =
    {
        "Identifier",

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
        "variable",                  // starts with dollar
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
        "->",                        // VarReference,
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
        "StringValue",
        "IntValue",
        "FloatValue",
        "true",
        "false",
        "null",                     // Null
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
        "&&",                       // AndSymb
        "and",                      // AndWord
        "||",                       // OrSymb
        "or",                       // OrWord
        "xor",                      // XorWord
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
        // comments
        "//",                       // SingleLineSlashComment
        "#",                        // SingleLineHashTagComment
        "/* */",                    // MultiLineComment
        // conditional assignment
        "?",                        // QuestMark
        ":",                        // Colon
        "??",                       // NullCoalescing
        // association in array
        "=>",                       // ArrayKey
        // punctuation
        ",",                        // Comma
        ";",                        // Semicolon
        "::",                       // DoubleColon
        "(",                        // LParen
        ")",                        // RParen
        "[",                        // LBracket
        "]",                        // RBracket
        "{",                        // LBrace
        "}",                        // RBrace
        "&"                        // Ampersand
    };
}

#endif //LEXER_TOKENTYPES_H
