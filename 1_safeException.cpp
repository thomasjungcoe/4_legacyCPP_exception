/***
 * Safe Exeption 예외 안전 코드
 * ===============================================================
 * 예외 안전 (safe) 코드는 예외가 발생하지 않는 코드가 아니라, 예외가 발생해도
 *      안전하게 복원되고 계속 동작해도 무방하게 예외 보증하는 코드다.
 * 예외 상황을 사전에 예측하고 프로그래밍 하라.
 * 모든 함수는 실패할수 있다고 가정하고 프로그래밍하라
 * 예외가 발생했을때 메모리 릭이나 리소스 릭이 없게 하라. (기본 보증)
 * 예외가 방생했을떄 예외 발생 전의 상태로 복원하라. (강한 보증)
 * 예외를 처리 할수 없다면 상위 개체(parent obj)에 보고하라.
 */

/*  이란적인 예외 상황에서 대처 방안    */
/*  메모리 할당 실패    
    1. new_handler를 이용하여 미리 할당한 예약 영역을 해제하고, 프로그램의 정상
        종료를 위해 뭔가 시도해 볼수 있습니다 (NewHandler 참고)
    2. 100,000,000 Gbyte 와 같이 엄청난 크기를 할당 요청을 하는 바람에 실패했다면, 
        메시지로 메모리 부족을 알리고 요청전의 상태로 복원할수 있습니다.
*/

/*  예외 상황의 잘못된 대처     */
// 악마의 유혹으로 예외 상황에서 예외를 제대로 보고하지 않으면, 디버깅을 하느라 코딩 인생을 
// 낭비 하게 됩니다. 처리할수 없다면, throw로 예외를 방생시키고 전파해서 좀 더 상위 개체에서 
// try-catch()로 처리하게끔 미뤄두세요.

/*
프로그래밍을 종료합니다.
    -> 무책임합니다. 종료하더라도 상위 개체에서 메시지를 표시하고, 사용자 선택에
        의해 종료하게 만들어야 합니다.
아예 신경쓰지 않습니다.
    -> 프로그램이 죽던지, 나중에 프로그래머가 디버깅하다가 스트레스로 죽던지 하니,
        신경 쓰셔야 합니다.
오류 코드를 리턴합니다.
    -> 리턴된 오류는 대부분 무시될수 있습니다. 엔진 안쪽의 하위 개체들이라면 예외를 
        방생시키고, 이를 처리할수 있는 곳까지 전파하여, 상위 개체에서 처리할수 있도록 해줘야 합니다.
전역 오류 코드를 세팅합니다.
    -> 호출한 곳에서 오류를 확인할 확률은 0.000001% 입니다. 함수 호출 후 GetLastError() 를 다시
        검사하는 경우는 거의 없잖아요?
        예외를 방생시키는게 더 낫습니다.
자가 진단후 함수를 정상 종료 시킵니다.
    -> if (condition) return; 와 같이 인자를 검사하고 그냥 종료 시키면, 잘못된 인자에 
        대한 처리가 수행되지 않습니다. 시스템이 정말 계속 안전하게 돌아갈지 의문이고, 이게
        나중에 알수 없는 버그의 원인이 되기도 합니다.
예외가 발생할 경우 호출할 함수를 호출합니다.
    -> new_handler 가 이런 구조인데요, 사실 이런 구조는 사용법이 좀 어렵습니다.
*/

/*  예외 상황의 올바른 대처     */
/*  
    예외 방생의 원인이 명백한 코딩 실수라면, 잘못 사용하기엔 어렵게, 바르게 사용하기엔 쉽게 
        갭슐화 하여 코딩 계약을 단단하게 하시고, 최후의 수단으로 assert()로 잘못된 사용을 
        안내하시기 바랍니다.

        그래도 예외가 방생한다면, 함수 내부에서 예외를 처리할수 있는지 없는지를 판단하여
        예외에 대처하거나 throw로 예외를 방생시키고 상위 개체에 예외를 전파합니다. 그리고 
        예외를 처리할수 있는 상위 개체에서 try-catch()로 예외를 탐지하고요.

        탐지한 예외는 다음과 같이 처리할수 있습니다.
        * 작업을 중단하고 메시지 박스 표시 (현재 상태가 이러저러해서 못하니, 이렇게 해서 재시도
        해보세요 등 원인과 해결방법 안내)
        * 자원을 정돈하고 재시도(메모리 부족등) 
        * 추후 재시도 할수 있도록 상태 정리 (파일 다운로드등)
        * 인수를 보정하고 재시도 (잘못된 입력값. 시스템에서 보정하거나 사용자가 보정)
        * 정의된 기본 동작으로 재시도 (치명적이지 않은 경우 기본동작으로 자동 실행) 
*/  