#include "gtest/gtest.h"
#include "config_parser.h"



//General tests for the config parser
class ParseTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  // void TearDown() override {}
  NginxConfigParser parser;
  NginxConfig out_config;
  
};


//valid configs
TEST_F(ParseTest, ExampleTest) {
  EXPECT_TRUE(parser.Parse("test_files/example_config", &out_config));
}

TEST_F(ParseTest, TestNothing) {
  EXPECT_TRUE(parser.Parse("test_files/TestNothing", &out_config));
}

TEST_F(ParseTest, TestEmpty) {
  EXPECT_TRUE(parser.Parse("test_files/TestEmpty", &out_config));
}

TEST_F(ParseTest, TestBraces) {
  EXPECT_TRUE(parser.Parse("test_files/testBraces", &out_config));
}

TEST_F(ParseTest, BasicCase) {
  EXPECT_TRUE(parser.Parse("test_files/basic", &out_config));
}

TEST_F(ParseTest, ComplexCase) {
  EXPECT_TRUE(parser.Parse("test_files/complex", &out_config));
}

TEST_F(ParseTest, ComplexCase1) {
  EXPECT_TRUE(parser.Parse("test_files/complex1", &out_config));
}

TEST_F(ParseTest, ComplexCase2) {
  EXPECT_TRUE(parser.Parse("test_files/complex2", &out_config));
}

TEST_F(ParseTest, ComplexCase3) {
  EXPECT_TRUE(parser.Parse("test_files/complex3", &out_config));
}



//invalid configs
TEST_F(ParseTest, FalseComplexCase) {
  EXPECT_FALSE(parser.Parse("test_files/falsecomplex", &out_config));
}

TEST_F(ParseTest, FalseComplexCase1) {
  EXPECT_FALSE(parser.Parse("test_files/falsecomplex1", &out_config));
}

TEST_F(ParseTest, FalseComplexCase2) {
  EXPECT_FALSE(parser.Parse("test_files/falsecomplex2", &out_config));
}

TEST_F(ParseTest, FalseComplexCase3) {
  EXPECT_FALSE(parser.Parse("test_files/falsecomplex3", &out_config));
}


TEST_F(ParseTest, getPort) {
  EXPECT_TRUE(parser.Parse("staticConfigLocal", &out_config));
  EXPECT_EQ(out_config.get_port_from_config(), 8001);
}


TEST_F(ParseTest, staticInp) {
  EXPECT_TRUE(parser.Parse("staticConfigLocal", &out_config));
  std::unordered_map<std::string, std::string> umapPaths = out_config.get_paths_from_config();
  EXPECT_TRUE(umapPaths.find("/static1") != umapPaths.end());
  EXPECT_TRUE(umapPaths.find("/static2") != umapPaths.end());
  EXPECT_EQ(umapPaths["/static1"], "./files1");
  EXPECT_EQ(umapPaths["/static2"], "/files2");
}


TEST_F(ParseTest, namesTest) {
  EXPECT_TRUE(parser.Parse("staticConfigLocal", &out_config));
  std::unordered_map<std::string, std::string> names = out_config.getHandlerNames();
  EXPECT_NE(names.find("/echo"), names.end());
}
