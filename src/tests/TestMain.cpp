#include <QtTest/QTest>
#include "tests/testsqlobjects.h"
#include "tests/TestImport.h"

int main(int argc, char** argv)
{
   int status = 0;
   {
      TestTable tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      TestImport tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   return status;
}
