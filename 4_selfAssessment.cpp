/***
 * 자가 진단과 진단 최소화
 * =================================================================
 * 사전 가정과 사후 가정을 진단하라
 * 진단 코드가 최소화 되도록, 포인터 보다는 참조자를 사용하고, 코딩 계약을 단단하게 만들어라
 * 
 * Modern C++:
 *  C++11~: static_assert()가 추가되어 컴파일 타임 진단이 가능해 젔습니다.
 * 
 */

#ifdef NDEBUG
#   define assert(condition) ((void) 0)
#else
#   define assert(condition) /* implementation defined */
#endif 

int a = 10;
assert(a == 10);    
assert(a == 11);

/*  공격적 자가진단     */
