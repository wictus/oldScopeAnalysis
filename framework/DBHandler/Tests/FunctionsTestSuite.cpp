// Functions Test Suite - FunctionsTestSuite.cpp
#ifndef FUNCTIONS_TESTSUITE_H
#define FUNCTIONS_TESTSUITE_H

#define BOOST_TEST_MODULE FunctionsTestSuite
#include <boost/test/included/unit_test.hpp>
#include "../HeaderFiles/Functions.h"


namespace DB
{

namespace TEST
{

// Tests
BOOST_AUTO_TEST_SUITE(FunctionsTS)

// DBConfigControler class
BOOST_AUTO_TEST_CASE(readConfigFileAndFillDBConfigDataTest)
{
  std::string l_dbConfigDataFileName = "../../Config/configDB.cfg";
  DB::FUNCTIONS::DBConfigData l_dbConfigData;
  DB::FUNCTIONS::DBConfigControler::readConfigFileAndFillDBConfigData(l_dbConfigDataFileName, l_dbConfigData);

  BOOST_CHECK(l_dbConfigData.m_db_configFileName == "configDB.cfg");
  BOOST_CHECK(l_dbConfigData.m_db_name == "test_db");
  BOOST_CHECK(l_dbConfigData.m_db_username == "gruntowski");
  BOOST_CHECK(l_dbConfigData.m_db_password == "pet*11");
  BOOST_CHECK(l_dbConfigData.m_db_hostname == "149.156.90.190");
  BOOST_CHECK(l_dbConfigData.m_db_port == "5432");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace TEST

} // namespace DB

#endif // FUNCTIONS_TESTSUITE_H
