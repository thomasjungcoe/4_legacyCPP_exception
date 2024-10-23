/***
 * 예외 메카니즘 (try-catch, throw와 스택 풀지, terminate)
 * ===================================================================
 * 해결할수 없는 예외 상황은 오류 코드 리턴보다는 강제성이 있는 throw로 보고하라.
 * 예회를 해결할수 있는 곳에서 탐지하라.
 * catch()에서 예외 개체가 복사 생성되지 않도록 참조자로 받아라.
 * 예외를 그대로 전파할 겨우에는 throw; 를 사용해라
 * 소멸자에서 예외를 방출하지 마라. (필요하다면 Release() 할수를 구현하라.)
 * 스택 풀기와 호환되도록 스마트 포인터를 사용하라.
 * 함수 정의시 동적 예외 사양은 사용하지 마라.
 * 
 * Modern C++:
 *  C++11~: noexcept가 추가되어 함수의 예외 방출 여부를 보증하며, 소멸자는 기본적으로 noexcept로 동작합니다.
 *  C++11~: 동적 예외 사양은 deprecate 되었습니다. 예외를 나열하는 것보다 noexpert로 예외를 방출하느냐 안하느냐만 관심을 둡니다.
 *  C++11~: unexpected()는 동적 예외 사양이 deprecate 되면서 함계 deprecate 되었습니다.
 *  C++17~: noexcept가 함수 유형에 포함되어 예외 처리에 대한 코딩 계약을 좀더 단단하게 할수 있습니다.
 *  C++17~: 동적 예외 사양 관련해서 throw() 가 deprecate 되었습니다. 이제 noexcept만 사용해야 합니다.
 *  C++17~: [[nodiscard]]가 추가되어 리턴값을 무시하지 않도록 컴파일 해줍니다.
 */

int f() {
    return 1;   // 함수 호출 결과로 오류 코드를 리턴합니다.
}

int g() {
    f();        // (~) 비권장. 함수를 호출하고 리턴된 오류 코드를 무시합니다.
}

/*  예외 발생과 탐지 (try-catch(), throw)   */
unsigned char ToChar(int val) {
    return static_cast<char>(val);  // 0~255 의 범위를 벗어나면 오동작합니다
}

unsigned char ToChar(int val) {
    if (!(0 <= val && val <= 255)) {
        throw std::out_of_range("Fail : val range");    // 예외를 방생시킵니다.
    }

    return static_cast<char>(val);
}
// 이때 진단 코드를 추가하여, 사용자가 실수하지 않도록 해주면 더 좋습니다.
unsigned char ToChar(int val) {
    assert(0 <= val && val < 256);      // 인자가 유효한지 진단합니다.

    if (!(0 <= val && val < 256)) {
        throw std::out_of_range("Fail : val range"); // 예외를 발생시킵니다.
    }
    return static_cast<char>(val)
}

int val = 'A';
unsigned char ch = ToChar(val + 1);
EXPECT_TRUE(ch == 'B');

// (x) 예외 발생. 예외를 처리하는 곳이 없으면 프로그램이 종료됨니다.
int val = 256;
ch = ToChar(val);

// (0) 호출전에 조건을 검사하여 실행할수 있습니다.
if (0 <= val && val < 256) {
    ch = ToChar(val);
}

// (0) 그냥 실행하고 예외가 발생하면 이를 탐지하여 뭐가를 처리할수 있습니다.
try {
    ch = ToChar(val);
}
catch(std::out_of_range& e) {   // 참조자를 사용합니다 
    // 예외가 발생했을때 뭐가 처리하는 코드
}

class MyError {};
class YourError {};

try {
    f();
}
catch (const MyError&) {}
catch (const YourError&) {}

class Base : public std::exception {};
class Derived : public Base {};
try {
    f();
}
catch (const std::exception&) {} (~)
catch (const Derived&) {}        (x)
catch (const Base&) {}

/*   예외 전파      */
// try-catch()로
//      1. 예외를 탐지하지 않으면, 예외는 상위 개체에 그대로 전파되고,
//      2. 예외를 탐지하면, 예외는 더이상 상위 개체에 전파되지 않는데요, 이를 다시 재전파하려면 throw; 를 사용합니다.
try {
    std::string str = std::string("abc").substr(10); // (x) 예외 발생. std::length_error
}
catch (cons std::exception& e) {
    std::cout << e.what() << '\n';
    // throw e; // (~) 비권장. std::exception 을 복제하여 리턴
    throw;      // (0) std::length_error를 재전파
}

/*      스택 풀기 (예외 복원)   */
// C++은 예외 상황이 발생하면, catch() 될때까지 거꾸로 스택을 하나하나 풀면서 (해제하면서, 소멸시키면서) 
//  생성한 개체의 소멸자를 호출하고, 메모리를 해제합니다. 이러한 과정을 거처 예외 발생 전의 상태로 복원합니다.
int f(int val) {
    throw std::out_of_range("Fail : f()");      // 예외를 발생시킵니다.
    return 0;
}

int g(int val) {
    throw std::out_of_range("Fail : g()");      // 예외를 발생시킵니다.
    return 0;
}

void h() {
    try {
        int a = 10;
        int b = f(a);   // f() 함수에서 예외가 발생하여 g()는 호출되지 않습니다. 스택 풀기를 하고 catch문으로 이동합니다.
        int c  = g(a);  // f() 에서 예외 발생시 호출되지 않습니다.
    }
    catch (std::out_of_range& e) {
        // 예외 처리를 합니다.
    }
}

void MyUnexpetedHandler() {
    throw;  // #3. 예외를 전파합니다
}

void f() throw(int, double, std::bad_exception) {
    throw 'e';  // (~) 비권장. 예외 명세에 없는 char 형 예외를 발생시킵니다.
}

std::unexpected_handler old = std::set_unexpected(MyUnexpectedHandler); // unexpected 핸들러를 설정하비다
try {
    f();    // (~) 비권장. int 와 double 예외 명세가 있지만, char 예외를 발생시킵니다.
}
catch (char) {
    std::cout << "throw : char" << std::endl;
}       
catch (int) {
    std::cout << "throw : int" << std::endl;
}
catch (double) {
    std::cout << "throw : double" << std::endl;
} 
catch (std::bad_exception&) {
    std::cout << "throw : bad_exception" << std::endl; // #4. bad_exception으로 예외를 탐지합니다.
} 
catch (...) {
    std::cout << "throw : ..." << std::endl; 
} 

std::set_unexpected(old); // #2

