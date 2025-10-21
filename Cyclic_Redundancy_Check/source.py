import sys
import parser

# --- CRC 체크섬 계산 함수 ---
def calculate_crc(data: str, poly_binary: str, r: int) -> str:
    """
    CRC 나눗셈(XOR)을 수행하여 체크섬(나머지)을 계산합니다.
    """
    # 1. 데이터 준비 (Padding): 원본 데이터 뒤에 r개의 '0'을 추가합니다.
    try:
        # 비트를 정수 리스트로 변환
        data_list = [int(bit) for bit in data] + [0] * r
        poly_list = [int(bit) for bit in poly_binary]
    except ValueError:
        print("오류: 데이터 또는 다항식 이진수에 숫자가 아닌 값이 포함됨", file=sys.stderr)
        return ""
        
    k = len(poly_list) # 제수(다항식)의 길이 (k = r + 1)
    
    if k == 0:
        print("오류: 다항식이 비어있습니다.", file=sys.stderr)
        return ""
        
    # 2. 이진 다항식 나눗셈 (XOR 루프)
    #    원본 데이터의 길이(len(data))만큼만 반복합니다.
    for i in range(len(data)):
        
        # 현재 윈도우의 가장 왼쪽 비트(MSB)가 '1'일 때만 XOR 연산 수행
        if data_list[i] == 1:
            # 다항식(제수)의 길이(k)만큼 XOR 연산을 수행
            for j in range(k):
                # data_list[i+j] = data_list[i+j] XOR poly_list[j]
                data_list[i+j] = data_list[i+j] ^ poly_list[j]

    # 3. 결과(나머지) 추출
    #    모든 나눗셈이 끝나면, data_list의 맨 뒤 r 비트가 나머지가 됩니다.
    remainder_list = data_list[-r:]
    
    # 리스트 [0, 1, 0, 1] -> 문자열 "0101"로 변환하여 반환
    return "".join(map(str, remainder_list))

# --- 메인 코드 실행 ---
if __name__ == "__main__":
    
    input_filename = "input.txt" 
    
    print(f"지정된 파일 '{input_filename}'을(를) 사용합니다.")
    
    # --- 입력 처리 ---
    # 1. parser 모듈의 함수를 사용
    powers, data = parser.read_inputs_from_file(input_filename)
    # 2. 콘솔창 직접 입력
    #powers, data = parser.get_inputs()
    
    if powers is not None and data is not None:
        
        poly_binary_str = ""
        r = 0 # 다항식의 최고 차수 (remainder 비트 수)
        
        if powers: # 다항식이 유효할 때 (예: "1" -> [0])
            r = max(powers) # (예: [4, 1, 0] -> 4)
            
            # 다항식 이진수 변환
            coeffs = [0] * (r + 1)
            for p in powers:
                coeffs[r - p] = 1
            poly_binary_str = "".join(map(str, coeffs))
            
            print(f"  -> 입력된 다항식의 이진수 표현: {poly_binary_str}")
        else:
            # parse_polynomial이 빈 리스트를 반환한 경우 (오류)
            print("오류: 유효한 다항식이 없습니다.", file=sys.stderr)
            sys.exit(1)

        # --- 체크섬 계산 로직 호출 ---
        print("\n--- CRC 체크섬 계산 시작 ---")
        
        # 다항식이 "1" (r=0)인 경우, 나머지는 항상 없습니다.
        if r == 0:
            if poly_binary_str == "1":
                print("  -> 다항식이 '1' (r=0)이므로 체크섬은 없습니다.")
                checksum = ""
            else:
                print("오류: 알 수 없는 다항식 오류 (r=0)", file=sys.stderr)
                sys.exit(1)
        else:
            # CRC 계산 함수 호출 (이 파일 안에 있는 함수)
            checksum = calculate_crc(data, poly_binary_str, r)
        
        print(f"  -> 계산된 체크섬: {checksum}")
        print(f"  -> 전송할 전체 데이터: {data + checksum}")

    else:
        print("\n입력 처리 중 오류가 발생하여 프로그램을 종료합니다.")
        sys.exit(1)