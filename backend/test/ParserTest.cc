// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>
#include "gtest/gtest.h"

#include "model/Statement.h"
#include "parser/Parser.h"

using wikidata::primarysources::model::Statement;

using wikidata::primarysources::model::NewQuantity;
using wikidata::primarysources::model::NewTime;
using wikidata::primarysources::model::NewValue;

namespace wikidata {
namespace primarysources {
namespace {

// helper method: parse statements from a string and return a vector of
// statement objects.
std::vector<Statement> parseString(const std::string &data) {
    std::stringstream buf(data);
    std::vector<Statement> result;

    parser::parseTSV("freebase", 0, buf, [&result](Statement st) {
        result.push_back(st);
    });

    return result;
}

TEST(ParserTest, ParseEntity) {
    std::vector<Statement> result = parseString("Q123\tP123\tQ321\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewValue("Q321"));
}

TEST(ParserTest, ParseString) {
    std::vector<Statement> result = parseString("Q123\tP123\t\"Q321\"\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewValue("Q321", ""));
}

TEST(ParserTest, ParseLangString) {
    std::vector<Statement> result = parseString("Q123\tP123\ten:\"Q321\"\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewValue("Q321", "en"));
}

TEST(ParserTest, ParseLocation) {
    std::vector<Statement> result = parseString("Q123\tP123\t@47.11/-10.09\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewValue(47.11, -10.09));
}

TEST(ParserTest, ParseTime) {
    std::vector<Statement> result = parseString("Q123\tP123\t+00000001967-01-17T00:00:00Z/11\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewTime(1967, 1, 17, 0, 0, 0, 11));
}

TEST(ParserTest, ParseTimeYear) {
    std::vector<Statement> result = parseString("Q123\tP123\t+00000001967-00-00T00:00:00Z/9\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewTime(1967, 0, 0, 0, 0, 0, 9));
}

TEST(ParserTest, ParseQuantity) {
    std::vector<Statement> result = parseString("Q123\tP123\t+123.21\n");

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].qid(), "Q123");
    ASSERT_EQ(result[0].property_value().property(), "P123");
    ASSERT_EQ(result[0].property_value().value(), NewQuantity("123.21"));
}


TEST(ParserTest, ParseMulti) {
    std::vector<Statement> result = parseString(
            "Q123\tP123\tQ321\n"
                    "Q123\tP123\tQ322\n"
                    "Q123\tP123\tQ323\n"
                    "Q123\tP123\tQ324\n"
    );

    ASSERT_EQ(result.size(), 4);
}

}  // namespace
}  // namespace primarysources
}  // namespace wikidata
