#ifndef BINDINGPOWER_HPP
#define BINDINGPOWER_HPP

enum class BindingPower
{
    Default,
    Comma,
    Assignment,
    Logical,
    Relational,
    Additive,
    Multiplicative,
    Unary,
    Call,
    Member,
    Primary
};

#endif