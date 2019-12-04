#include <QString>
#include <QtTest>
#define matrixtest
#include "../lib/headers/primitive/matrix.h"

class MatrixTest : public QObject
{
    Q_OBJECT

public:
    MatrixTest();

private Q_SLOTS:
    void constructorMatrixTypeInt();
    void constructorMatrixTypeChar();
    void constructorMatrixTypeShort();
    void constructorMatrixTypeFloat();
    void constructorMatrixTypeLongDouble ();

    //Testes utilizando o tipo Double
    void constructorMatrixTypeDouble ();
    void constructorMatrixTypeCStringAndDouble ();
    void constructorMatrixTypeStringAndDouble ();
    void constructorSizedVoidMatrixDouble ();
    void constructorNULLMatrixDouble ();
    void copyconstructorDouble ();
    void copyconstructorOtherTypeDoubleAndInt ();
    void destructorDouble ();
    void removeRowDoubleFirstTest ();
    void removeRowDoubleSecondTest ();
    void removeRowDoubleThirdTest ();
    void removeColumnDoubleFirstTest ();
    void removeColumnDoubleSecondTest ();
    void removeColumnDoubleThirdTest ();
    void getNumberOfRowsDouble ();
    void getNumberOfColumnsDouble ();
    void getRowDouble ();
    void getColumnDouble ();
    void lengthDouble ();
    void sizeDouble ();
    void isNullDouble ();
    void isSquareDouble ();
    void operatorParenthesisInputTwoUnsignedDouble ();
    void operatorParenthesisInputTwoUnsignedOutputConstDouble ();
    void operatorAtributionDoubleReturningVoid ();
    void operatorAtributionDoubleReturningDouble ();
    void operatorAtributionDoubleReturningInt ();
    void operatorAtributionDoubleReturningShort ();
    void operatorAtributionDoubleReturningLongInt ();
    void operatorAtributionDoubleReturningLongFloat ();
    void operatorSumAccumulatorScalarDouble ();
    void operatorSumAccumulatorMatrixDouble ();
    void operatorSubtractionAccumulatorScalarDouble ();
    void operatorSubtractionAccumulatorMatrixDouble ();
    void operatorMultiplicationAccumulatorScalarDouble ();
    void operatorMultiplicationAccumulatorMatrixDouble ();
    void operatorDivisionAccumulatorScalarDouble ();
    void horizontalConcatenationOperatorScalarDouble ();
    void horizontalConcatenationOperatorMatrixDouble ();
    void verticalConcatenationOperatorScalarDouble ();
    void verticalConcatenationOperatorMatrixDouble ();
    void bufferOperatorDoubleCase1 ();
    void bufferOperatorDoubleCase2 ();
    void bufferOperatorDoubleCase3 ();
    void bufferOperatorDoubleCase4 ();
    void bufferOperatorDoubleCase5 ();
    void bufferOperatorDoubleCase6 ();
    void bufferOperatorDoubleCase7 ();
    void bufferOperatorDoubleCase8 ();
    void bufferOperatorDoubleCase9 ();
    void bufferOperatorDoubleCase10 ();
    void operatorSumMatrixScalarDouble ();
    void operatorSumScalarMatrixDouble ();
    void operatorSumMatrixMatrixDouble ();
    void operatorSubtractionMatrixScalarDouble ();
    void operatorSubtractionScalarMatrixDouble ();
    void operatorSubtractionMatrixMatrixDouble ();
    void operatorMultiplicationMatrixScalarDouble ();
    void operatorMultiplicationScalarMatrixDouble ();
    void operatorMultiplicationMatrixMatrixDouble ();
    void operatorDivisionMatrixScalarDouble ();
    void operatorNegativeMatrixDouble ();
    void operatorTransposeMatrixDouble();
    void zerosMatrixDouble ();
    void identityMatrixDouble ();
    void onesMatrixDouble ();
    void LineVector ();
    void operatorBufferStringShow ();
    void operatorBufferStringBack ();
};

MatrixTest::MatrixTest()
{
}

void MatrixTest::constructorMatrixTypeChar()
{
    QBENCHMARK {
        LinAlg::Matrix<char> A('a');
    }
    LinAlg::Matrix<char> A('a');
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 'a', "Falhou ao comparar o elemento da matriz com valor char");
    QVERIFY2(sizeof(A(0,0)) == sizeof(char), "Falhou ao comparar o tipo do elemento da matriz com valor char");
}

void MatrixTest::constructorMatrixTypeShort()
{
    QBENCHMARK {
        LinAlg::Matrix<short> A(1);
    }
    LinAlg::Matrix<short> A(1);
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1, "Falhou ao comparar o elemento da matriz com valor short");
    QVERIFY2(sizeof(A(0,0)) == sizeof(short), "Falhou ao comparar o tipo do elemento da matriz com valor short");
}

void MatrixTest::constructorMatrixTypeInt()
{
    QBENCHMARK {
        LinAlg::Matrix<int> A(1);
    }
    LinAlg::Matrix<int> A(1);
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1, "Falhou ao comparar o elemento da matriz com valor inteiro");
    QVERIFY2(sizeof(A(0,0)) == sizeof(int), "Falhou ao comparar o tipo do elemento da matriz com valor inteiro");
}

void MatrixTest::constructorMatrixTypeFloat()
{
    QBENCHMARK {
        LinAlg::Matrix<float> A(1.5);
    }
    LinAlg::Matrix<float> A(1.5);
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.5, "Falhou ao comparar o elemento da matriz com valor float");
    QVERIFY2(sizeof(A(0,0)) == sizeof(float), "Falhou ao comparar o tipo do elemento da matriz com valor float");
}

void MatrixTest::constructorMatrixTypeDouble()
{
    QBENCHMARK {
        LinAlg::Matrix<double> A(1.1);
    }
    LinAlg::Matrix<double> A(1.1);
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.1, "Falhou ao comparar o elemento da matriz com valor double");
    QVERIFY2(sizeof(A(0,0)) == sizeof(double), "Falhou ao comparar o tipo do elemento da matriz com valor double");
}

void MatrixTest::constructorMatrixTypeLongDouble()
{
    QBENCHMARK {
        LinAlg::Matrix<long double> A(1.1);
    }
    LinAlg::Matrix<long double> A(1.1);
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.1, "Falhou ao comparar o elemento da matriz com valor long double");
    QVERIFY2(sizeof(A(0,0)) == sizeof(long double), "Falhou ao comparar o tipo do elemento da matriz com valor long double");
}

void MatrixTest::constructorMatrixTypeCStringAndDouble()
{
    QBENCHMARK {
        LinAlg::Matrix<double> A("1,2,3,4;5,6,7,8");
    }
    LinAlg::Matrix<double> A("1,2,3,4;5,6,7,8");
    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.0 && A(0,1) == 2.0 && A(0,2) == 3.0 && A(0,3) == 4.0 &&
             A(1,0) == 5.0 && A(1,1) == 6.0 && A(1,2) == 7.0 && A(1,3) == 8.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::constructorMatrixTypeStringAndDouble()
{
    std::string str = "1,2,3,4;5,6,7,8";
    QBENCHMARK {
        LinAlg::Matrix<double> A(str);
    }
    LinAlg::Matrix<double> A(str);
    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.0 && A(0,1) == 2.0 && A(0,2) == 3.0 && A(0,3) == 4.0 &&
             A(1,0) == 5.0 && A(1,1) == 6.0 && A(1,2) == 7.0 && A(1,3) == 8.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::constructorSizedVoidMatrixDouble()
{
    QBENCHMARK {
        LinAlg::Matrix<double> A(3,3);
    }
    LinAlg::Matrix<double> A(3,3);
    QVERIFY2(A.getNumberOfColumns() == 3 && A.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 0.0 && A(0,1) == 0.0 && A(0,2) == 0.0 &&
             A(1,0) == 0.0 && A(1,1) == 0.0 && A(1,2) == 0.0 &&
             A(2,0) == 0.0 && A(2,1) == 0.0 && A(2,2) == 0.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::constructorNULLMatrixDouble()
{
    QBENCHMARK {
        LinAlg::Matrix<double> A;
    }
    LinAlg::Matrix<double> A;
    QVERIFY2(A.getNumberOfColumns() == 0 && A.getNumberOfRows() == 0, "Falhou ao testar o tamanho da matriz");
}

void MatrixTest::copyconstructorDouble()
{
    LinAlg::Matrix<double> A = "1,2;3,4";
    QBENCHMARK {
        LinAlg::Matrix<double> B(A);
    }
    LinAlg::Matrix<double> B(A);
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1.0 && B(0,1) == 2.0 &&
             B(1,0) == 3.0 && B(1,1) == 4.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::copyconstructorOtherTypeDoubleAndInt()
{
    LinAlg::Matrix<int> A = "1,2;3,4";
    QBENCHMARK {
        LinAlg::Matrix<double> B(A);
    }
    LinAlg::Matrix<double> B(A);
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1.0 && B(0,1) == 2.0 &&
             B(1,0) == 3.0 && B(1,1) == 4.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::destructorDouble()
{
//    LinAlg::Matrix<int> A = "1,2;3,4";
//    A.~Matrix();
//    QBENCHMARK {
//        LinAlg::Matrix<int> B = "1,2;3,4";
//        B.~Matrix();
//    }

//    QVERIFY2(A.getNumberOfColumns() == 0 && A.getNumberOfRows() == 0, "Falhou ao testar o tamanho da matriz");
}

void MatrixTest::removeRowDoubleFirstTest()
{
    std::string str = "1,2,3,4";
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = str.c_str();
        A.removeRow(0);
    }
    QVERIFY2(A.getNumberOfColumns() == 0 && A.getNumberOfRows() == 0, "Falhou ao testar o tamanho da matriz");
}

void MatrixTest::removeRowDoubleSecondTest()
{
    std::string str = "1,2,3,4;5,6,7,8";
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = str.c_str();
        A.removeRow(0);
    }
    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 5.0 && A(0,1) == 6.0 && A(0,2) == 7.0 && A(0,3) == 8.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::removeRowDoubleThirdTest()
{
    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = str.c_str();
        A.removeRow(1);
    }
    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.0 && A(0,1) == 2.0 && A(0,2) == 3.0 && A(0,3) == 4.0 &&
             A(1,0) == 9.0 && A(1,1) == 10.0 && A(1,2) == 11.0 && A(1,3) == 12.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::removeColumnDoubleFirstTest()
{
    std::string str = "1;2;3;4";
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = str.c_str();
        A.removeColumn(0);
    }
    QVERIFY2(A.getNumberOfColumns() == 0 && A.getNumberOfRows() == 0, "Falhou ao testar o tamanho da matriz");
}

void MatrixTest::removeColumnDoubleSecondTest()
{
    std::string str = "1,5;2,6;3,7;4,8";
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = str.c_str();
        A.removeColumn(0);
    }
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 4, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 5.0 && A(1,0) == 6.0 && A(2,0) == 7.0 && A(3,0) == 8.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::removeColumnDoubleThirdTest()
{
    std::string str = "1,5,9;2,6,10;3,7,11;4,8,12";
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = str.c_str();
        A.removeColumn(1);
    }
    QVERIFY2(A.getNumberOfColumns() == 2 && A.getNumberOfRows() == 4, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1.0 && A(1,0) == 2.0 && A(2,0) == 3.0 && A(3,0) == 4.0 &&
             A(0,1) == 9.0 && A(1,1) == 10.0 && A(2,1) == 11.0 && A(3,1) == 12.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::getNumberOfRowsDouble()
{
    std::string str = "1;2;3;4;5;6;7;8;9;0";
    LinAlg::Matrix<double> A;
    A = str.c_str();
    QBENCHMARK {
        A.getNumberOfRows();
    }
    QVERIFY2(A.getNumberOfColumns() == 1 && A.getNumberOfRows() == 10, "Falhou ao testar o tamanho da matriz");
}

void MatrixTest::getNumberOfColumnsDouble()
{
    std::string str = "1,2,3,4,5,6,7,8,9,0";
    LinAlg::Matrix<double> A;
    A = str.c_str();
    QBENCHMARK {
        A.getNumberOfColumns();
    }
    QVERIFY2(A.getNumberOfColumns() == 10 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
}

void MatrixTest::getRowDouble()
{
    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
    LinAlg::Matrix<double> A,B;
    A = str.c_str();
    QBENCHMARK {
        B = A.getRow(0);
    }
    QVERIFY2(B.getNumberOfColumns() == 4 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1.0 && B(0,1) == 2.0 && B(0,2) == 3.0 && B(0,3) == 4.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::getColumnDouble()
{
    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
    LinAlg::Matrix<double> A,B;
    A = str.c_str();
    QBENCHMARK {
        B = A.getColumn(0);
    }
    QVERIFY2(B.getNumberOfColumns() == 1 && B.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1.0 && B(1,0) == 5.0 && B(2,0) == 9.0
             , "Falhou ao comparar todos os elementos da matriz com valor double");
}

//void MatrixTest::swapRowsDouble()
//{
//    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
//    LinAlg::Matrix<double> A;
//    QBENCHMARK {
//        A = str.c_str();
//        A.swapRows(0,1);
//    }
//    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
//    QVERIFY2(A(0,0) == 5.0 && A(0,1) == 6.0 && A(0,2) == 7.0 && A(0,3) == 8.0 &&
//             A(1,0) == 1.0 && A(1,1) == 2.0 && A(1,2) == 3.0 && A(2,4) == 4.0 &&
//             A(2,0) == 9.0 && A(2,1) == 10.0 && A(2,2) == 11.0 && A(3,4) == 12.0
//             , "Falhou ao comparar todos os elementos da matriz com valor double");
//}

//void MatrixTest::swapColumnsDouble()
//{
//    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
//    LinAlg::Matrix<double> A;
//    QBENCHMARK {
//        A = str.c_str();
//        A.swapColumns(0,1);
//    }
//    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
//    QVERIFY2(A(0,0) == 2.0 && A(0,1) == 1.0 && A(0,2) == 3.0 && A(0,3) == 4.0 &&
//             A(1,0) == 6.0 && A(1,1) == 5.0 && A(1,2) == 7.0 && A(2,4) == 8.0 &&
//             A(2,0) == 10.0 && A(2,1) == 9.0 && A(2,2) == 11.0 && A(3,4) == 12.0
//             , "Falhou ao comparar todos os elementos da matriz com valor double");
//}

void MatrixTest::lengthDouble()
{
    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
    LinAlg::Matrix<double> A = str;
    QBENCHMARK {
        A.length();
    }
    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A.length() == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::sizeDouble()
{
    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
    LinAlg::Matrix<double> A = str,B;
    QBENCHMARK {
        B = A.size();
    }
    QVERIFY2(A.getNumberOfColumns() == 4 && A.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 3 && B(0,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::isNullDouble ()
{
    std::string str = "1,2,3,4;5,6,7,8;9,10,11,12";
    LinAlg::Matrix<double> A = str,B;
    QBENCHMARK {
        A.isNull();
        B.isNull();
    }
    QVERIFY2(B.isNull(), "Matriz deveria ser vazia");
    QVERIFY2(!A.isNull(), "Matriz não deveria ser vazia");
}

void MatrixTest::isSquareDouble ()
{
    LinAlg::Matrix<double> A = LinAlg::Zeros<double>(1,5), B = LinAlg::Zeros<double>(5,5);
    QBENCHMARK {
        A.isSquare();
        B.isSquare();
    }
    QVERIFY2(B.isSquare(), "Matriz deveria ser quadrada");
    QVERIFY2(!A.isSquare(), "Matriz não deveria ser quadrada");
}

void MatrixTest::operatorParenthesisInputTwoUnsignedDouble ()
{
    LinAlg::Matrix<double> A = LinAlg::Zeros<double>(1,5);
    QBENCHMARK {
        A(0,0) = 2.0;
    }
    A(0,0) = 2.0;
    QVERIFY2(A(0,0) == 2.0, "Atribuição não realizada com sucesso");
}

void MatrixTest::operatorParenthesisInputTwoUnsignedOutputConstDouble ()
{
    LinAlg::Matrix<double> A = LinAlg::Zeros<double>(1,5);
    bool flag;
    QBENCHMARK {
        flag = (A(0,0) == 0.0);
    }
    QVERIFY2(flag, "Acesso ao elemento da matriz não realizado com sucesso");
}


void MatrixTest::operatorAtributionDoubleReturningVoid ()
{
    LinAlg::Matrix<double> B;
    QBENCHMARK {
        B = "3,2";
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 3 && B(0,1) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorAtributionDoubleReturningDouble ()
{
    std::string str = "1,2;3,4";
    LinAlg::Matrix<double> A = str,B;
    QBENCHMARK {
        B = A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 3 && B(0,1) == 2 && B(1,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorAtributionDoubleReturningInt ()
{
    std::string str = "1,2;3,4";
    LinAlg::Matrix<double> A = str;
    LinAlg::Matrix<int> B;
    QBENCHMARK {
        B = A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 3 && B(0,1) == 2 && B(1,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorAtributionDoubleReturningShort ()
{
    std::string str = "1,2;3,4";
    LinAlg::Matrix<double> A = str;
    LinAlg::Matrix<short> B;
    QBENCHMARK {
        B = A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 3 && B(0,1) == 2 && B(1,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorAtributionDoubleReturningLongInt ()
{
    std::string str = "1,2;3,4";
    LinAlg::Matrix<double> A = str;
    LinAlg::Matrix<long int> B;
    QBENCHMARK {
        B = A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 3 && B(0,1) == 2 && B(1,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorAtributionDoubleReturningLongFloat ()
{
    std::string str = "1,2;3,4";
    LinAlg::Matrix<double> A = str;
    LinAlg::Matrix<float> B;
    QBENCHMARK {
        B = A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 3 && B(0,1) == 2 && B(1,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}




void MatrixTest::operatorSumAccumulatorScalarDouble ()
{
    LinAlg::Matrix<double> B = LinAlg::Zeros<double>(2,2);
    QBENCHMARK {
        B = LinAlg::Zeros<double>(2,2);
        B += 1;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 1 && B(0,1) == 1 && B(1,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSumAccumulatorMatrixDouble ()
{
    LinAlg::Matrix<double> B = LinAlg::Zeros<double>(2,2), A = LinAlg::Ones<double>(2,2);
    QBENCHMARK {
        B = LinAlg::Zeros<double>(2,2);
        B += A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 1 && B(0,1) == 1 && B(1,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSubtractionAccumulatorScalarDouble ()
{
    LinAlg::Matrix<double> B = LinAlg::Zeros<double>(2,2);
    QBENCHMARK {
        B = LinAlg::Zeros<double>(2,2);
        B -= 1;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == -1 && B(1,0) == -1 && B(0,1) == -1 && B(1,1) == -1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSubtractionAccumulatorMatrixDouble ()
{
    LinAlg::Matrix<double> B = LinAlg::Zeros<double>(2,2), A = LinAlg::Ones<double>(2,2);
    QBENCHMARK {
        B = LinAlg::Zeros<double>(2,2);
        B -= A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == -1 && B(1,0) == -1 && B(0,1) == -1 && B(1,1) == -1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorMultiplicationAccumulatorScalarDouble ()
{
    LinAlg::Matrix<double> B = 2*LinAlg::Ones<double>(2,2);
    QBENCHMARK {
        B = 2*LinAlg::Ones<double>(2,2);
        B *= 2;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 4 && B(1,0) == 4 && B(0,1) == 4 && B(1,1) == 4, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorMultiplicationAccumulatorMatrixDouble ()
{
    LinAlg::Matrix<double> B = LinAlg::Ones<double>(2,2), A = LinAlg::Ones<double>(2,2);
    QBENCHMARK {
        B = LinAlg::Ones<double>(2,2);
        B *= A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2 && B(1,0) == 2 && B(0,1) == 2 && B(1,1) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorDivisionAccumulatorScalarDouble ()
{
    LinAlg::Matrix<double> B = 2*LinAlg::Ones<double>(2,2);
    QBENCHMARK {
        B = 2*LinAlg::Ones<double>(2,2);
        B /= 2;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 1 && B(0,1) == 1 && B(1,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::horizontalConcatenationOperatorScalarDouble ()
{
    LinAlg::Matrix<double> A = "1,2", B;

    QBENCHMARK {
        B = A|3;
    }
    QVERIFY2(B.getNumberOfColumns() == 3 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(0,1) == 2 && B(0,2) == 3, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::horizontalConcatenationOperatorMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", B;

    QBENCHMARK {
        B = A|LinAlg::Matrix<double>("5;6");
    }
    QVERIFY2(B.getNumberOfColumns() == 3 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(0,1) == 2 && B(0,2) == 5 &&
             B(1,0) == 3 && B(1,1) == 4 && B(1,2) == 6, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::verticalConcatenationOperatorScalarDouble()
{
    LinAlg::Matrix<double> A = "1;2", B;

    QBENCHMARK {
        B = A||3;
    }
    QVERIFY2(B.getNumberOfColumns() == 1 && B.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 2 && B(2,0) == 3, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::verticalConcatenationOperatorMatrixDouble()
{
    LinAlg::Matrix<double> A = "1,2;3,4", B;

    QBENCHMARK {
            B = A||LinAlg::Matrix<double>("5,6");
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 3 && B(2,0) == 5 &&
             B(0,1) == 2 && B(1,1) == 4 && B(2,1) == 6, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase1 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "1"; B = "2,3";
        B << A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 3 && B(0,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase2 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2"; B = "1";
        B << A;
    }
    QVERIFY2(B.getNumberOfColumns() == 1 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase3 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2;2"; B = "1;1";
        B << A;
    }
    QVERIFY2(B.getNumberOfColumns() == 1 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2 && B(1,0) == 2 , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase4 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2;2"; B = "1,1;1,1";
        B << A;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 1 && B(0,1) == 2 && B(1,1) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase5 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2,2;2,2;2,2"; B = "1,1,1;1,1,1;1,1,1";
        B << A;
    }
    QVERIFY2(B.getNumberOfColumns() == 3 && B.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(1,0) == 1 && B(2,0) == 1 &&
             B(0,1) == 2 && B(1,1) == 2 && B(2,1) == 2 &&
             B(0,2) == 2 && B(1,2) == 2 && B(2,2) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase6 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "1"; B = "2,3";
        A >> B;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 1 && B(0,1) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase7 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2"; B = "1";
        A >> B;
    }
    QVERIFY2(B.getNumberOfColumns() == 1 && B.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase8 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2;2"; B = "1;1";
        A >> B;
    }
    QVERIFY2(B.getNumberOfColumns() == 1 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2 && B(1,0) == 2 , "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase9 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2;2"; B = "1,1;1,1";
        A >> B;
    }
    QVERIFY2(B.getNumberOfColumns() == 2 && B.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2 && B(1,0) == 2 && B(0,1) == 1 && B(1,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::bufferOperatorDoubleCase10 ()
{
    LinAlg::Matrix<double> A, B;

    QBENCHMARK {
        A = "2,2;2,2;2,2"; B = "1,1,1;1,1,1;1,1,1";
        A >> B;
    }
    QVERIFY2(B.getNumberOfColumns() == 3 && B.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(B(0,0) == 2 && B(1,0) == 2 && B(2,0) == 2 &&
             B(0,1) == 2 && B(1,1) == 2 && B(2,1) == 2 &&
             B(0,2) == 1 && B(1,2) == 1 && B(2,2) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSumMatrixScalarDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = A+2;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 3 && C(0,1) == 4 &&
             C(1,0) == 5 && C(1,1) == 6, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSumScalarMatrixDouble()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = 2+A;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 3 && C(0,1) == 4 &&
             C(1,0) == 5 && C(1,1) == 6, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSumMatrixMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;
    LinAlg::Matrix<int> B = "4,3;2,1";
    QBENCHMARK {
        C = A+B;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 5 && C(0,1) == 5 &&
             C(1,0) == 5 && C(1,1) == 5, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSubtractionMatrixScalarDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = A-2;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == -1 && C(0,1) == 0 &&
             C(1,0) == 1 && C(1,1) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSubtractionScalarMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = 2-A;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 1 && C(0,1) == 0 &&
             C(1,0) == -1 && C(1,1) == -2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorSubtractionMatrixMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;
    LinAlg::Matrix<int> B = "4,3;2,1";
    QBENCHMARK {
        C = A-B;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == -3 && C(0,1) == -1 &&
             C(1,0) == 1 && C(1,1) == 3, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorMultiplicationMatrixScalarDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = A*2;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 2 && C(0,1) == 4 &&
             C(1,0) == 6 && C(1,1) == 8, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorMultiplicationScalarMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = 2*A;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 2 && C(0,1) == 4 &&
             C(1,0) == 6 && C(1,1) == 8, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorMultiplicationMatrixMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4;5,6", C;
    LinAlg::Matrix<int> B = "1,2;3,4";
    QBENCHMARK {
        C = A*B;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 7  && C(0,1) == 10 &&
             C(1,0) == 15 && C(1,1) == 22 &&
             C(2,0) == 23 && C(2,1) == 34, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorDivisionMatrixScalarDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4", C;

    QBENCHMARK {
        C = A/2;

    }
    QVERIFY2(C.getNumberOfColumns() == 2 &&C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 0.5 && C(0,1) == 1 &&
             C(1,0) == 1.5 && C(1,1) == 2, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorNegativeMatrixDouble ()
{
    LinAlg::Matrix<double> A = "1,2;3,4;5,6", C;
    QBENCHMARK {
        C = -A;
    }
    QVERIFY2(C.getNumberOfColumns() == 2 && C.getNumberOfRows() == 3, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == -1  && C(0,1) == -2 &&
             C(1,0) == -3  && C(1,1) == -4 &&
             C(2,0) == -5  && C(2,1) == -6, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorTransposeMatrixDouble()
{
    LinAlg::Matrix<double> A = "1,2;3,4;5,6", C;
    QBENCHMARK {
        C = ~A;
    }
    QVERIFY2(C.getNumberOfColumns() == 3 && C.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(C(0,0) == 1  && C(0,1) == 3 && C(0,2) == 5 &&
             C(1,0) == 2  && C(1,1) == 4 && C(1,2) == 6, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::zerosMatrixDouble ()
{
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = LinAlg::Zeros<double>(2,2);
    }
    QVERIFY2(A.getNumberOfColumns() == 2 && A.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 0 && A(0,1) == 0 &&
             A(1,0) == 0 && A(1,1) == 0, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::identityMatrixDouble ()
{
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = LinAlg::Eye<double>(2);
    }
    QVERIFY2(A.getNumberOfColumns() == 2 && A.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1 && A(0,1) == 0 &&
             A(1,0) == 0 && A(1,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}
void MatrixTest::onesMatrixDouble ()
{
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = LinAlg::Ones<double>(2,2);
    }
    QVERIFY2(A.getNumberOfColumns() == 2 && A.getNumberOfRows() == 2, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1 && A(0,1) == 1 &&
             A(1,0) == 1 && A(1,1) == 1, "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::LineVector ()
{
    LinAlg::Matrix<double> A;
    QBENCHMARK {
        A = LinAlg::LineVector<double>(1,5,1);
    }
    QVERIFY2(A.getNumberOfColumns() == 5 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
    QVERIFY2(A(0,0) == 1 && A(0,1) == 2 && A(0,2) == 3 && A(0,3) == 4 && A(0,4) == 5,
             "Falhou ao comparar todos os elementos da matriz com valor double");

    QBENCHMARK {
        A = LinAlg::LineVector<double>(5,1,-1);
    }
    QVERIFY2(A.getNumberOfColumns() == 5 && A.getNumberOfRows() == 1, "Falhou ao testar o tamanho da matriz");
//    std::cout << A;
    QVERIFY2(A(0,0) == 5 && A(0,1) == 4 && A(0,2) == 3 && A(0,3) == 2 && A(0,4) == 1,
             "Falhou ao comparar todos os elementos da matriz com valor double");
}

void MatrixTest::operatorBufferStringShow ()
{
    LinAlg::Matrix<double> A = "1,2;3,4";
    std::string B;
    QBENCHMARK {
        B << A;
    }
//    std::cout << B;
    QVERIFY2(B.find("1.000 ") != -1 && B.find("2.000 ") != -1 && B.find("3.000 ") != -1 && B.find("4.000 ") != -1,
             "Falhou ao comparar a string com valor double");
}

void MatrixTest::operatorBufferStringBack ()
{
    LinAlg::Matrix<double> A = "1,2;3,4";
    std::string B;
    QBENCHMARK {
        B <<= A;
    }
//    std::cout << B;
    QVERIFY2(B.find("1.000,") != -1 && B.find("2.000;") != -1 && B.find("3.000,") != -1 && B.find("4.000") != -1,
             "Falhou ao comparar a string com valor double");
}

QTEST_APPLESS_MAIN(MatrixTest)

#include "tst_matrixtest.moc"
