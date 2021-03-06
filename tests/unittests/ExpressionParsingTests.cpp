#include "Test.h"

TEST_CASE("Empty string") {
    auto& text = "";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::IdentifierName);
    CHECK(expr.as<IdentifierNameSyntax>().identifier.isMissing());
}

TEST_CASE("Name expression") {
    auto& text = "foo";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::IdentifierName);
    CHECK(!expr.as<IdentifierNameSyntax>().identifier.isMissing());
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Parenthesized expression") {
    auto& text = "(foo)";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::ParenthesizedExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("MinTypMax expression") {
    auto& text = "(foo:34+99:baz)";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::ParenthesizedExpression);
    CHECK(expr.as<ParenthesizedExpressionSyntax>().expression->kind ==
          SyntaxKind::MinTypMaxExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("String literal expression") {
    auto& text = "\"asdf\"";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::StringLiteralExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Integer literal expression") {
    auto& text = "34'd56";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::IntegerVectorExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Integer with question") {
    auto& text = "4'b?10?";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::IntegerVectorExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Real literal expression") {
    auto& text = "42.42";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::RealLiteralExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Time literal expression") {
    auto& text = "42ns";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::TimeLiteralExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Null literal expression") {
    auto& text = "null";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::NullLiteralExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Wildcard expression") {
    auto& text = "$";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::WildcardLiteralExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

void testPrefixUnary(TokenKind kind) {
    auto text = std::string(getTokenKindText(kind)) + "a";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == getUnaryPrefixExpression(kind));
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
    auto& us = expr.as<PrefixUnaryExpressionSyntax>();
    CHECK(us.operatorToken.kind == kind);
    CHECK(us.operand->kind == SyntaxKind::IdentifierName);
}

TEST_CASE("Unary prefix operators") {
    testPrefixUnary(TokenKind::Plus);
    testPrefixUnary(TokenKind::Minus);
    testPrefixUnary(TokenKind::And);
    testPrefixUnary(TokenKind::TildeAnd);
    testPrefixUnary(TokenKind::Or);
    testPrefixUnary(TokenKind::TildeOr);
    testPrefixUnary(TokenKind::Xor);
    testPrefixUnary(TokenKind::XorTilde);
    testPrefixUnary(TokenKind::TildeXor);
    testPrefixUnary(TokenKind::DoublePlus);
    testPrefixUnary(TokenKind::DoubleMinus);
    testPrefixUnary(TokenKind::Tilde);
    testPrefixUnary(TokenKind::Exclamation);
}

void testPostfixUnary(TokenKind kind) {
    auto text = "a" + std::string(getTokenKindText(kind));
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == getUnaryPostfixExpression(kind));
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
    auto& us = expr.as<PostfixUnaryExpressionSyntax>();
    CHECK(us.operatorToken.kind == kind);
    CHECK(us.operand->kind == SyntaxKind::IdentifierName);
}

TEST_CASE("Unary postfix operators") {
    testPostfixUnary(TokenKind::DoublePlus);
    testPostfixUnary(TokenKind::DoubleMinus);
}

void testBinaryOperator(TokenKind kind) {
    auto text = "a " + std::string(getTokenKindText(kind)) + " 4";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == getBinaryExpression(kind));
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
    auto& us = expr.as<BinaryExpressionSyntax>();
    CHECK(us.operatorToken.kind == kind);
    CHECK(us.left->kind == SyntaxKind::IdentifierName);
    CHECK(us.right->kind == SyntaxKind::IntegerLiteralExpression);
}

TEST_CASE("Binary operators") {
    testBinaryOperator(TokenKind::Plus);
    testBinaryOperator(TokenKind::Minus);
    testBinaryOperator(TokenKind::Star);
    testBinaryOperator(TokenKind::Slash);
    testBinaryOperator(TokenKind::Percent);
    testBinaryOperator(TokenKind::DoubleStar);
    testBinaryOperator(TokenKind::DoubleEquals);
    testBinaryOperator(TokenKind::ExclamationEquals);
    testBinaryOperator(TokenKind::TripleEquals);
    testBinaryOperator(TokenKind::ExclamationDoubleEquals);
    testBinaryOperator(TokenKind::DoubleEqualsQuestion);
    testBinaryOperator(TokenKind::ExclamationEqualsQuestion);
    testBinaryOperator(TokenKind::DoubleAnd);
    testBinaryOperator(TokenKind::DoubleOr);
    testBinaryOperator(TokenKind::MinusArrow);
    testBinaryOperator(TokenKind::LessThanMinusArrow);
    testBinaryOperator(TokenKind::LessThan);
    testBinaryOperator(TokenKind::LessThanEquals);
    testBinaryOperator(TokenKind::GreaterThan);
    testBinaryOperator(TokenKind::GreaterThanEquals);
    testBinaryOperator(TokenKind::And);
    testBinaryOperator(TokenKind::Or);
    testBinaryOperator(TokenKind::Xor);
    testBinaryOperator(TokenKind::XorTilde);
    testBinaryOperator(TokenKind::TildeXor);
    testBinaryOperator(TokenKind::RightShift);
    testBinaryOperator(TokenKind::TripleRightShift);
    testBinaryOperator(TokenKind::LeftShift);
    testBinaryOperator(TokenKind::TripleLeftShift);
    testBinaryOperator(TokenKind::Equals);
    testBinaryOperator(TokenKind::PlusEqual);
    testBinaryOperator(TokenKind::MinusEqual);
    testBinaryOperator(TokenKind::StarEqual);
    testBinaryOperator(TokenKind::SlashEqual);
    testBinaryOperator(TokenKind::PercentEqual);
    testBinaryOperator(TokenKind::AndEqual);
    testBinaryOperator(TokenKind::OrEqual);
    testBinaryOperator(TokenKind::XorEqual);
    testBinaryOperator(TokenKind::LeftShiftEqual);
    testBinaryOperator(TokenKind::TripleLeftShiftEqual);
    testBinaryOperator(TokenKind::RightShiftEqual);
    testBinaryOperator(TokenKind::TripleRightShiftEqual);
}

void testScopedName(string_view text) {
    auto& expr = parseExpression(std::string(text));

    REQUIRE(expr.kind == SyntaxKind::ScopedName);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Scoped identifiers") {
    testScopedName("$unit::stuff");
    testScopedName("$root.asdf");
    testScopedName("foo::bar");
    testScopedName("$unit::foo::bar");
    testScopedName("blah::foo::bar");
    testScopedName("local::foo::bar");
}

TEST_CASE("Class scoped name") {
    auto& text = "blah::foo #(stuff, .thing(3+9))::bar";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::ScopedName);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Empty queue") {
    auto& text = "{}";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::EmptyQueueExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Concatenation") {
    auto& text = "{3+4, foo.bar}";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::ConcatenationExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Concatenation (single)") {
    auto& text = "{3+4}";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::ConcatenationExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Multiple concatenation") {
    auto& text = "{3+4 {foo.bar, 9**22}}";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::MultipleConcatenationExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Streaming concatenation") {
    auto& text = "{<< 3+9 {foo, 24.32 with [3+:4]}}";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::StreamingConcatenationExpression);
    CHECK(expr.as<StreamingConcatenationExpressionSyntax>()
              .expressions[1]
              ->withRange->range->selector->kind == SyntaxKind::AscendingRangeSelect);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Element Access") {
    auto& text = "(foo)[3][9+4]";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::ElementSelectExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

void testElementRange(string_view text, SyntaxKind kind) {
    auto& expr = parseExpression(std::string(text));
    REQUIRE(expr.kind == SyntaxKind::ElementSelectExpression);
    CHECK(expr.as<ElementSelectExpressionSyntax>().select->selector->kind == kind);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Element range") {
    testElementRange("(foo)[3:4]", SyntaxKind::SimpleRangeSelect);
    testElementRange("(foo)[3+:4]", SyntaxKind::AscendingRangeSelect);
    testElementRange("(foo)[3-:4]", SyntaxKind::DescendingRangeSelect);
}

TEST_CASE("Member Access") {
    auto& text = "(foo).bar";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::MemberAccessExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Invocation expression") {
    auto& text = "foo.bar(5, 6, .param(9))";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::InvocationExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Inside expression") {
    auto& text = "34 inside { 34, [12:12], 19 }";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::InsideExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Tagged union expression") {
    auto& text = "tagged foo";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::TaggedUnionExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}

TEST_CASE("Bad argument recovery") {
    auto& text = "foo(]], 3 4,)";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::InvocationExpression);
    CHECK(expr.toString() == "foo(");
    CHECK(!diagnostics.empty());
}

TEST_CASE("Conditional expression") {
    // check proper precedence
    auto& text = "foo || bar ? 3 : 4";
    auto& expr = parseExpression(text);

    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
    REQUIRE(expr.kind == SyntaxKind::ConditionalExpression);

    auto& cond = expr.as<ConditionalExpressionSyntax>();
    REQUIRE(cond.predicate->conditions.size() == 1);
    CHECK(cond.predicate->conditions[0]->expr->kind == SyntaxKind::LogicalOrExpression);
}

TEST_CASE("Conditional expression (pattern matching)") {
    // check proper precedence
    auto& text = "foo matches 34 &&& foo ? 3 : 4";
    auto& expr = parseExpression(text);

    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
    REQUIRE(expr.kind == SyntaxKind::ConditionalExpression);

    auto& cond = expr.as<ConditionalExpressionSyntax>();
    REQUIRE(cond.predicate->conditions.size() == 2);
    CHECK(cond.predicate->conditions[0]->expr->kind == SyntaxKind::IdentifierName);
    CHECK(cond.predicate->conditions[0]->matchesClause->pattern->kind ==
          SyntaxKind::ExpressionPattern);
}

TEST_CASE("Big expression") {
    auto& text = R"(
module M; localparam foo = (stackDepth == 100) || ((stackDepth == 200) || ((stackDepth ==
300) || ((stackDepth == 400) || ((stackDepth == 501) || ((stackDepth == 502) ||
((stackDepth == 600) ||
((stackDepth == 701) || ((stackDepth == 702) || ((stackDepth == 801) || ((stackDepth ==
802) || ((stackDepth == 901) || ((stackDepth == 902) || ((stackDepth == 903) ||
((stackDepth == 10201) || ((stackDepth == 10202) || ((stackDepth == 10301) ||
((stackDepth == 10302) || ((stackDepth == 10401) || ((stackDepth == 10402) ||
((stackDepth == 10403) || ((stackDepth == 10501) || ((stackDepth == 10502) ||
((stackDepth == 10601) || ((stackDepth == 10602) || ((stackDepth == 10701) ||
((stackDepth == 10702) || ((stackDepth == 10703) || ((stackDepth == 10704) ||
((stackDepth == 10705) || ((stackDepth == 10706) || ((stackDepth == 10801) ||
((stackDepth == 10802) || ((stackDepth == 10803) || ((stackDepth == 10804) ||
((stackDepth == 10805) || ((stackDepth == 10806) || ((stackDepth == 10807) ||
((stackDepth == 10808) || ((stackDepth == 10809) || ((stackDepth == 10900) ||
((stackDepth == 11000) || ((stackDepth == 11100) || ((stackDepth == 11201) ||
((stackDepth == 11202) || ((stackDepth == 11203) || ((stackDepth == 11204) ||
((stackDepth == 11205) || ((stackDepth == 11206) || ((stackDepth == 11207) ||
((stackDepth == 11208) || ((stackDepth == 11209) || ((stackDepth == 11210) ||
((stackDepth == 11211) || ((stackDepth == 11212) || ((stackDepth == 11213) ||
((stackDepth == 11214) || ((stackDepth == 11301) || ((stackDepth == 11302) ||
((stackDepth == 11303) || ((stackDepth == 11304) || ((stackDepth == 11305) ||
((stackDepth == 11306) || ((stackDepth == 11307) || ((stackDepth == 11308) ||
((stackDepth == 11309) || ((stackDepth == 11401) || ((stackDepth == 11402) ||
((stackDepth == 11403) || ((stackDepth == 11404) || ((stackDepth == 11501) ||
((stackDepth == 11502) || ((stackDepth == 11503) || ((stackDepth == 11504) ||
((stackDepth == 11505) || ((stackDepth == 11601) || ((stackDepth == 11602) ||
((stackDepth == 11603) || ((stackDepth == 11604) || ((stackDepth == 11605) ||
((stackDepth == 11606) || ((stackDepth == 11701) || ((stackDepth == 11702) ||
((stackDepth == 11800) || ((stackDepth == 11901) || ((stackDepth == 11902) ||
((stackDepth == 11903) || ((stackDepth == 11904) || ((stackDepth == 11905) ||
((stackDepth == 12001) || ((stackDepth == 12002) || ((stackDepth == 12003) ||
((stackDepth == 12004) || ((stackDepth == 12101) || ((stackDepth == 12102) ||
((stackDepth == 12103) || ((stackDepth == 12104) || ((stackDepth == 12105) ||
((stackDepth == 12106) || ((stackDepth == 12107) || ((stackDepth == 12108) ||
((stackDepth == 12109) || ((stackDepth == 12110) || ((stackDepth == 12111) ||
((stackDepth == 12112) || ((stackDepth == 12113) || ((stackDepth == 12114) ||
((stackDepth == 12115) || ((stackDepth == 12116) || ((stackDepth == 12201) ||
((stackDepth == 12202) || ((stackDepth == 12203) || ((stackDepth == 12204) ||
((stackDepth == 12205) || ((stackDepth == 12301) || ((stackDepth == 12302) ||
((stackDepth == 12401) || ((stackDepth == 12402) || ((stackDepth == 12403) ||
((stackDepth == 12404) || ((stackDepth == 12405) || ((stackDepth == 12406) ||
((stackDepth == 12501) || ((stackDepth == 12502) || ((stackDepth == 12601) ||
((stackDepth == 12602) || ((stackDepth == 12603) || ((stackDepth == 12700) ||
((stackDepth == 12800) || ((stackDepth == 12900) || ((stackDepth == 13001) ||
((stackDepth == 13002) || ((stackDepth == 13003) || ((stackDepth == 13004) ||
((stackDepth == 13005) ||
((stackDepth == 13101) || ((stackDepth == 13102) || ((stackDepth == 13103) ||
((stackDepth == 13201) || ((stackDepth == 13202) || ((stackDepth == 13203) ||
((stackDepth == 13301) || ((stackDepth == 13302) || ((stackDepth == 13303) ||
((stackDepth == 13304) || ((stackDepth == 13401) || ((stackDepth == 13402) ||
((stackDepth == 13403) || ((stackDepth == 13404) || ((stackDepth == 13405) ||
((stackDepth == 13501) || ((stackDepth == 13502) || ((stackDepth == 13600) ||
((stackDepth == 13701) || ((stackDepth == 13702) || ((stackDepth == 13703) ||
((stackDepth == 13800) || ((stackDepth == 13901) || ((stackDepth == 13902) ||
((stackDepth == 13903) || ((stackDepth == 14001) || ((stackDepth == 14002) ||
((stackDepth == 14100) || ((stackDepth == 14200) || ((stackDepth == 14301) ||
((stackDepth == 14302) || ((stackDepth == 14400) || ((stackDepth == 14501) ||
((stackDepth == 14502) || ((stackDepth == 14601) || ((stackDepth == 14602) ||
((stackDepth == 14603) || ((stackDepth == 14604) || ((stackDepth == 14605) ||
((stackDepth == 14606) || ((stackDepth == 14607) || ((stackDepth == 14701) ||
((stackDepth == 14702) || ((stackDepth == 14703) || ((stackDepth == 14704) ||
((stackDepth == 14705) || ((stackDepth == 14706) || ((stackDepth == 14707) ||
((stackDepth == 14708) || ((stackDepth == 14709) || ((stackDepth == 14710) ||
((stackDepth == 14711) || ((stackDepth == 14712) || ((stackDepth == 14713) ||
((stackDepth == 14714) || ((stackDepth == 14715) || ((stackDepth == 14716) ||
((stackDepth == 14717) || ((stackDepth == 14718) || ((stackDepth == 14719) ||
((stackDepth == 14720 || ((stackDepth == 14717 || ((stackDepth == 14717) || ((stackDepth
== 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717)
|| ((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717 || ((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth
== 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717)
|| ((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717) || ((stackDepth == 14717) ||
((stackDepth == 14717) || ((stackDepth == 14717 || ((stackDepth == 14717);
)";

    Diagnostics diags;
    Preprocessor preprocessor(SyntaxTree::getDefaultSourceManager(), alloc, diags);
    preprocessor.pushSource(string_view(text));

    Bag options;
    ParserOptions parserOptions;
    parserOptions.maxRecursionDepth = 128;
    options.add(parserOptions);

    Parser parser(preprocessor, options);
    parser.parseCompilationUnit();

    std::string result = "\n" + report(diags);
    CHECK(result == R"(
<unnamed_buffer0>:23:43: error: language constructs are too deeply nested
((stackDepth == 11306) || ((stackDepth == 11307) || ((stackDepth == 11308) ||
                                          ^
)");
}

TEST_CASE("Arithmetic expressions") {
    auto& expr = parseExpression("3 + 4 / 2 * 9");
    REQUIRE(expr.kind == SyntaxKind::AddExpression);
    CHECK(expr.as<BinaryExpressionSyntax>().right->kind == SyntaxKind::MultiplyExpression);
}

TEST_CASE("Simple class new expression") {
    auto& text = "new";
    auto& expr = parseExpression(text);

    REQUIRE(expr.kind == SyntaxKind::NewClassExpression);
    CHECK(expr.toString() == text);
    CHECK_DIAGNOSTICS_EMPTY;
}
