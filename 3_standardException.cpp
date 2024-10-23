/***
 * 표준 예외 개체
 * =================================================================
 *  C++11~: 동적 예외 사양은 deprecate 되었습니다. 예외를 나열하는 것보다 noexcept로 예외를 
 *      방출하느냐 안하느냐만 관심을 둡니다.
 *  C++17~: 동적 예외 사양 관련해서 throw()가 deprecate 되었습니다. 이제 noexcept만 사용해야 합니다
 * 
 * 개요
 * throw 에서 예외를 발생시킬때 예외 개체를 만들어 예외를 대한 정보를 담아 절달합니다.
 *  (catch()에서 예외 개체를 전달받을때, 예외 개체가 복사 생성되지 않도록 참조자로 받는게 좋습니다.
 * 
 */

/*
exception
    logical_error
        invalid_arg
        domain_error
        length_error
        out_of_range
    runtime_error
        range_error
        overflow_error
        underflow_error
    bad_typeid
    bad_cast
    bad_alloc
    bad_exeption
*/