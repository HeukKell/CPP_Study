#pragma once
#include <iostream>

using std::cout;
using std::endl;

/* 객체지향 언어 특징 
	
	1. 캡슐화 ( 은닉성 )
		: 목적으로 하는 기능을 수행하기 위해서 필요한 멤버들, 서포팅 해줄 수 있는 멤버함수들을 클래스라는곳에 한 데 모아서,
		그 클래스가 객체화 되어 수행할 수 있도록 하는것

		- 접근 제한 지정자를 통해서 숨길것은 숨김으로서 사용자의 필요 지식을 낮춘다. 사용시 편리하게 한다. 보안

	2. 상속
		: 클래스가 다른 클래스의 변수 및 함수 의 기능을 가져와 활용하거나 확장할 수 있는 기능.
		- 코드의 재사용성을 높인다.

	3. 다형성
		: 뭐든지 될 수 있는 성질.
		- 다른 객체지향 언어에도 있는 성질
		- 상속 과 포인터 를 사용하여 보여질 수 있다.

	4. 추상화

*/

// 부모 클래스
class CParent {
	
private:
	int m_Parent_PrivateInt;	// 4 byte

protected:
	int m_Parent_ProtectedInt; 


public:

	/* 부모 기본생성자 */
	CParent()
		: m_Parent_PrivateInt(0)
		, m_Parent_ProtectedInt(0)
	{
		std::cout << &(*this) << ":부모쪽 생성자(기본생성자) 명령줄 수행" << std::endl;
	}

	/* 인수를 받는 부모 생성자 */
	CParent(int parent_PrivateInt, int parent_ProtectedInt)
		: m_Parent_PrivateInt(parent_PrivateInt)
		, m_Parent_ProtectedInt(parent_ProtectedInt)
	{
		std::cout << &(*this) << ":부모쪽 생성자(인수를 받는 생성자) 명령줄 수행" << std::endl;
	}

	~CParent()
	{
		std::cout << &(*this) << ":부모쪽 소멸자 호출" << std::endl;
	}

public:

	void ParentFunction()
	{
		std::cout << &(*this) << ":부모의 함수 호출" << std::endl;
	}

	virtual void output() {
		std::cout << &(*this) << ":부모쪽 함수 output 호출" << std::endl;
	}

};


/* 상속
 
  # 코드의 재사용성
  - 부모클래스의 기능을 가져오겠다.
  - 부모 클래스의 기능을 받아와 확장하겠다.
 
  # 접근제한 지정자
  - 자식이라도 부모의 private 멤버는 접근할 수 없다.
  - protected, public 멤버는 자식에서 접근할 수 있다.
  - protected 는 외부에서는 접근할수 없지만 상속하는 클래스에서는 접근할 수 있도록 하는 접근제한 지정자 이다.
  - 함수또한 사용할 수 있다.
 
  # 메모리
  - 객체선언시 부모 클래스 영역부터 순차적으로 잡힌다.
  - 순서또한 부모 부터 순차적으로 선언된다.	
  - [ 부모멤버변수 메모리 영역, 자식멤버 메모리 영역, 자식의자식 메모리 영역 ]
 
  # 생성자가 동작하는 방식
  - 자식 or 부모 클래스는 상속관계에서 다른 클래스의 멤버를 초기화 할 수 없다.
  - 부모의 생성자, 자식의 생성자가 모두 호출되어, 멤버의 초기화를 각자 담당한다.
  - 부모멤버 변수의 초기화는 부모의 생성자가 담당하고, 자식멤버 변수의 초기화는 자식의 생성자가 담당한다.
  - 생성자의 호출 순서는 자식의 생성자가 우선 호출되고 부모의 생성자가 호출되는데, 
	 생성자의 실행 순서(초기화 순서)는 부모의 생성자가 실행되고, 자식의 생성자가 실행된다. 
  - 자식의 이니셜라이저에는 가장먼저 부모의 생성자를 호출하도록 생략되어 있다. 명시적으로 순서를 바꾸어도 부모의 생성자부터 호출이 된다.
 
  # 소멸자가 동작하는 방식
  - 자식쪽 소멸자가 다 수행한 뒤 부모의 소멸자가 수행된다.
  - 자식 소멸자 명령줄 다 실행하고 끝나기 전에 부모쪽 소멸자로 이동하여 명령줄 다 실행하고 다시 자식으로 넘어와 끝이 난다.
 
  # 오버라이딩 ( 재정의 )
  - 부모클래스의 멤버함수를 자식쪽에서 재정의 함으로써, 자식 클래스에 구현된 기능이 호출되도록 한다.
  - 함수 오버로딩(함수명이 같지만 인자타입의 차이가 생겨 컴파일러가 구분할 여지가 생긴 함수들) 과 다르다.
  - 상속관계에서만 있는 개념이다.
  - 이름과 인자가 똑같은 부모의 함수를 자식쪽에서 덮어 쓰는 기능
  - CChild.CParent::function() 을 통해 명시적으로 부모의 함수를 호출할 수도 있다.

  # 다형성
  - 상속과 포인터를 사용하여 하나의 자료형이 여러종류의 자료형으로 해석될 수 있는 성질
  - 자식 클래스를 부모 클래스로 해석해도 문제가 없다. 역은 불가능하다. ( 자식 클래스는 부모 클래스의 변수를 함께 가지고 가기 때문이다. )
  - 부모 클래스가 4byte, 자식클래스가 ( 부모 4byte + 자식 4byte ) 만큼을 가진다면, 
		부모 클래스 포인터로 자식클래스를 가리키면 8byte 중 부모영역 4byte 만 읽으면 되지만. 
		자식 클래스 포인터로 부모 클래스를 가리킨다면, 4byte 할당되어 있는데 이를 4byte를 넘어 8byte 까지 읽어버리기 때문에 할당되지 않은 영역을 건드려 컴파일러 오류가 생긴다.
		Parent* pParent  ---> [ parent ][     child     ]   (o);
		Child* pChild    ---> [ parent ]					(x);
  - 강제 캐스팅도 필요없다. 문법적으로 문제가 없다.
  - 부모포인터 타입으로, 부모 클래스에서 파생되는 모든 클래스를 가리킬수 있다. 다 부모영역의 멤버 변수를 가질 것이니까.
  - 휴잭맨(배우)은 울버린(영화속 인물)도, 장발장(영화속 인물)도 될수 있지만, 울버린(영화속 인물)은 휴잭맨(배우)이 될 수 없다.
  - 자식클래스에서 함수가 오버라이딩 되었더라도, 부모 클래스 포인터로 가리킨다면 부모 함수로 호출된다. -> 즉 자식클래스를 그냥 부모클래스 취급을 해버린 것이다. 실제 객체가 무엇인지 알 수 없다(가상함수에 대한 개념이 필요)

  # 다형성 :: 가상함수 (virtual)
  - virtual 가상함수 를 통해 자식쪽에서 오버라이딩 된 함수를 호출할 수 있다. -> 부모 클래스로 가리켰지만 어떤 자식 클래스임을 알 수 있게 된다.
  - virtual 키워드를 통해서, 각 클래스는 자신만의 고유한 가상함수 테이블을 가지게 된다.
		가상함수테이블에는 해당 클래스의 가상함수들이 등록된다. __vfptr
  - 가상 함수 테이블에, 자식 클래스의 함수 주소가 적혀있어, 실제 객체의 함수에 접근할 수 있다.
		객체가 생성될때 가상함수 테이블을 만들어 자신의 함수를 등록하게 된다. __vfptr 멤버변수에 Child::functionName 의 주소가 적혀지게 된다.
		접근하려는 함수가 가상함수 라면, 가상함수 테이블로 가서 어디 클래스의 함수의 주소로 가야할지 보고 이동하여 호출하게 된다.

  # 다운 캐스팅
  - 부모클래스에서 선언되지 않은, 오직 자식쪽에서만 추가된 함수를 호출하고 싶을 때
  - 자식 포인터 타입으로 일시적으로 캐스팅 해서 호출한다.
  - 문제가 발 생할 수 있기 때문에 dynamic_cast 로 안전하게 확인해 볼 수 있다. 실패시 nullptr 을 반환하는 template 함수.
  - RTTI ( Run Time Type Identification or Information ) : 런터임중에 타입을 식별하는 것

  # 추상화
  - 실제 객체를 생성할 목적의 클래스가 아닌, 상속을 통해서 구현해야 할 내용을 전달하는, 상속목적으로 만들어진 클래스
  - virtual, = 0 (1 개 이상의 순수 가상함수를 포함하면 추상 클래스가 된다. )
*/
class CChild : public CParent
{
/* inheritated ( 상속하는 변수, 물려받은 변수 )

private : 
	int m_Parent_PrivateInt;	// 부모의 멤버 변수 4 byte

protected :
	int m_Parent_ProtectedInt;	// 부모의 멤버 변수 4 byte
*/
private: 
	int m_Child_PrivateInt;	// 4 byte

public:

	CChild()
		: // CParent(),	// 생략되어 있는 부모 생성자 호출, 즉 생성자 호출은 자식이 우선이지만. 호출되면서 바로 부모의 생성자를 호출하고, 부모의 멤버변수를 초기화 하고, 이후 자식의 멤버변수를 초기화 한다. 명시적으로 밑으로 내려도 부모의 생성자가 먼저 호출된다.
		m_Child_PrivateInt(0)
	{
		m_Parent_ProtectedInt = 10; // 부모의 protected 접근 가능
		// m_Parent_PrivateInt = 10; // 부모의 private 는 접근 불가능
		std::cout << &(*this) << ":자식쪽 생성자(기본생성자) 명령줄 수행" << std::endl;
	}

	CChild(int child_PrivateInt)
		: CParent(20, 20) // 인수를 받는 부모생성자를 호출하고 싶은 경우 이렇게 명시적으로 호출해주어야 한다. 생략시 부모의 기본 생성자를 호출하게 된다.
		, m_Child_PrivateInt(child_PrivateInt)
	{
		std::cout << &(*this) << ":자식쪽 생성자(인수를 받는 생성자) 명령줄 수행" << std::endl;
	}


	~CChild()
	{
		std::cout << &(*this) << ":자식쪽 소멸자 호출" << std::endl;
	}

public:

	void ChildFunction()
	{
		std::cout << &(*this) << ":자식의 함수 호출" << std::endl;

		ParentFunction(); // 부모의 함수 호출 가능
	}

	/* 부모의 함수 output 을 덮어쓰는 오버라이딩 함수 */
	void output() {
		std::cout << &(*this) << ":자식쪽 함수 output 호출" << std::endl;
	}

	/* 자식에게만 있는 함수 */
	void OnlyChildFunction()
	{
		std::cout << &(*this) << ":자식에게만 있는 함수 OnlyChildFunction 호출 " << std::endl;
	}
};



void useClassInherit()
{

	std::cout << std::endl << "--- 오버라이딩 사용 예시 ---" << std::endl << std::endl;

	CChild child(100);

	child.output();	// 오버라이딩한 함수 호출

	child.CParent::output(); // 명시적으로 부모 함수 호출

	// 다형성
	std::cout << std::endl << "--- 다형성 ---" << std::endl << std::endl;

	CParent parent1;
	CChild child1;

	CChild* pChild = &child;		// 문제없음
	CParent* pParent = &parent1;	// 문제없음

	CParent* pParent2 = &child1;	// 자식클래스를 부모 클래스로 바라보는것은 문제가 없다. 메모리상에 부모의 멤버변수와 자식의 멤버변수가 할당되어 있기때문에. 다 쓰지 않을뿐.
	// CChild* pChild2 = &parent;	// 부모클래스 를 자식클래스로 바라보는것은 문제가 있다. 메모리상으로 자식의 멤버 변수는 할당되어 지지 않았기 때문에.

	std::cout << std::endl << "--- 다형성 :: 가상함수 사용 예시 ---" << std::endl << std::endl;

	/* 다형성 - 가상함수 */
	CParent* pParent3 = nullptr;

	CParent parent3;
	CChild child3;

	pParent3 = &parent3;
	pParent3->output();	// "부모쪽 Output 함수 호출";

	pParent3 = &child3;
	pParent3->output(); // output 이 virtual 함수인가? 맞다면 "자식쪽 Output 함수 호출" : 아니라면 "부모쪽 Output 함수 호출" 

	std::cout << std::endl << "--- 다운 캐스팅 사용 예시 ---" << std::endl << std::endl;

	/* 다운 캐스팅 사용 예시 */ 
	CParent parent4;
	CChild child4;
	
	CParent* pParent4 = &child4;						// 부모 클래스 포인터로 파생클래스를 가리키는 변수 pParent4

	//pParent4 = &parent4 ;								// 활성화 하면 에러 발생하는 코드, CChild 가 아닌 진짜 CParent 객체의 주소를 저장하게 되면. 아래의 다운캐스팅에 실패한다.
	
	std::cout << "(CChild*) : 강제 다운캐스팅 " << std::endl;
	((CChild*)pParent4)->OnlyChildFunction();			// 위험한 다운 캐스팅, 파생클래스만의 함수를 호출하기 위해 강제 다운캐스팅 시도후 바로 함수 호출. 만약 pParent4 가 CChild 가 아닌 다른 클래스를 저장하고 잇었다면, err 발생한다.

	CChild* pChild4 = dynamic_cast<CChild*>(pParent4);	// 안전한 다운 캐스팅, 실패시 nullptr 을 반환하기 때문에 체크하여 안전한 함수 호출이 가능해진다. 
	if (nullptr != pChild4)
	{
		std::cout << "dynamic_cast : 캐스팅 성공" << std::endl;
		pChild4->OnlyChildFunction();
	}
	else if (nullptr == pChild4)
	{
		std::cout << "dynamic_cast : 캐스팅 실패" << std::endl;
	}
	
	std::cout << std::endl << "--- 함수 끝 ---" << std::endl << std::endl;
}