import sys

# --- 헬퍼 함수 1: 다항식 파싱 ---
def parse_polynomial(poly_str: str) -> list[int]:
    """
    다항식 문자열(예: "x4+x+1")을 파싱하여 차수 리스트(예: [4, 1, 0])로 변환합니다.
    """
    poly_str = poly_str.replace(" ", "")
    terms = poly_str.split('+')
    powers = []
    
    for term in terms:
        if not term: 
            continue
            
        if term == "1":
            powers.append(0)
        elif term == "x":
            powers.append(1)
        elif term.startswith("x"):
            try:
                if len(term) > 1 and term[1:].isdigit():
                    power = int(term[1:])
                    powers.append(power)
                else:
                    raise ValueError
            except ValueError:
                print(f"오류: '{term}'는 'x<숫자>' 형식이 아닙니다. (예: x5)", file=sys.stderr)
                return []
        else:
            print(f"오류: '{term}'는 지원하지 않는 형식입니다.", file=sys.stderr)
            return [] 

    return sorted(list(set(powers)), reverse=True)

# --- 헬퍼 함수 2: 이진수 검증 ---
def is_binary(data_str: str) -> bool:
    """
    입력 문자열이 '0'과 '1'로만 구성된 이진수인지 확인합니다.
    """
    if not data_str:
        return False
    return all(c in '01' for c in data_str)

# --- 입력 함수 1: 콘솔 입력 (정의만 하고 사용 안 함) ---
def get_inputs() -> tuple[list[int] | None, str | None]:
    """
    사용자로부터 다항식과 원본 데이터를 입력받고 파싱/검증합니다.
    """
    print("--- 체크섬 계산기 입력 (콘솔) ---")
    
    poly_string = input("1. 다항식을 입력하세요 (예: x4+x+1): ")
    polynomial_powers = parse_polynomial(poly_string)
    
    if not polynomial_powers and poly_string != "1": 
        print("다항식 입력이 잘못되었습니다.", file=sys.stderr)
        return None, None
        
    data_string = input("2. 원본 데이터를 이진수로 입력하세요 (예: 101101001): ")
    
    if not is_binary(data_string):
        print("원본 데이터는 '0'과 '1'로만 구성된 이진수여야 합니다.", file=sys.stderr)
        return None, None

    print("\n--- 입력 확인 (콘솔) ---")
    print(f"  [다항식]   문자열: '{poly_string}'")
    #print(f"  [다항식]   파싱된 차수: {polynomial_powers}") # 원본 코드에서 주석 처리됨
    print(f"  [원본 데이터] 이진수: '{data_string}'")
    
    return polynomial_powers, data_string

# --- 입력 함수 2: 파일 입력 (Main에서 사용) ---
def read_inputs_from_file(filename: str) -> tuple[list[int] | None, str | None]:
    """
    파일에서 다항식과 원본 데이터를 읽어 파싱/검증합니다.
    """
    print(f"--- 입력 파일 '{filename}' 읽기 시작 ---")
    
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            poly_string = f.readline().strip() 
            if not poly_string:
                print(f"오류: 파일의 첫 번째 줄(다항식)이 비어있습니다.", file=sys.stderr)
                return None, None
                
            data_string = f.readline().strip()
            if not data_string:
                print(f"오류: 파일의 두 번째 줄(원본 데이터)이 비어있습니다.", file=sys.stderr)
                return None, None

    except FileNotFoundError:
        print(f"오류: 파일 '{filename}'을(를) 찾을 수 없습니다.", file=sys.stderr)
        return None, None
    except Exception as e:
        print(f"오류: 파일을 읽는 중 예외가 발생했습니다: {e}", file=sys.stderr)
        return None, None

    # --- 입력 검증 (헬퍼 함수 사용) ---
    polynomial_powers = parse_polynomial(poly_string)
    if not polynomial_powers and poly_string != "1": 
        print("파일에서 읽은 다항식 형식이 잘못되었습니다.", file=sys.stderr)
        return None, None
        
    if not is_binary(data_string):
        print("파일에서 읽은 원본 데이터는 '0'과 '1'로만 구성된 이진수여야 합니다.", file=sys.stderr)
        return None, None

    print("\n--- 입력 확인 (파일) ---")
    print(f"  [다항식]   문자열: '{poly_string}'")
    print(f"  [원본 데이터] 이진수: '{data_string}'")
    
    return polynomial_powers, data_string