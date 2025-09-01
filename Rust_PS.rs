
// 현 강의 진도 : Char2


// 주석1
/* 주석2 */

fn main() {

   let num = 100; 
  // 정수는 기본 i32 로 인식한다. (범용성)
  // 타입 지정 시 콜론 & 타입

  // i:8~128 (int, long.. 간소화)  isize : PC 에따라다름
  // u:8~128 (unsigned .. 간소화)  usize : PC 에따라다름0 
  // 다른 타입의 연산은 불가능하다.
  
   let x : i8 = 10; // 사용하지 않으면 unused warning
   let _x : i16 = 10; // unused warning 무시
   let _한글변수 : i16 = num + _x; // 한글명 변수 사용가능

   let _casting = x + _x as i8; 
   //  casting = simple type change, as type 으로 쉽게 가능함


   // casting & char
   let _c = 'a' as u8; // C와 동일하게 ASCII 따름 (-> 97 나옴)
   println!("{}", _c); // 매크로, 변수 출력방법.
   println!("Hello, world!"); 
  // string 은 무조건 "", char는 ''. C랑똑같음 UTF8 씀
  
}
