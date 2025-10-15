import huffman

# 압축된 파일을 디코딩하여 텍스트 파일로 저장
def decodeFileWithTree(encoded_filename, decoded_filename, tree_filename):
    # 트리 로드
    root = huffman.loadHuffmanTree(tree_filename)
    
    # 압축된 파일 읽기
    with open(encoded_filename, 'rb') as fp:
        encoded_data = fp.read()
    
    # 이진 문자열로 변환
    binary_string = ''.join(f'{byte:08b}' for byte in encoded_data)
    
    # 트리를 사용하여 디코딩
    decoded_text = huffman.decodeWithTree(binary_string, root)
    
    # 디코딩된 텍스트 파일로 저장
    with open(decoded_filename, 'w', encoding='utf-8') as fp:
        fp.write(decoded_text)
    
    print(f"파일 '{decoded_filename}'에 디코딩된 내용이 저장되었습니다.")

# 디코딩 실행
def main():
    # 압축된 파일과 트리 파일을 지정
    encoded_filename = "outputfile.bin"  # 압축된 파일 이름
    decoded_filename = "decoded_output.txt"  # 복원된 파일 이름
    tree_filename = "huffman_tree.pkl"  # 트리 파일 이름
    
    decodeFileWithTree(encoded_filename, decoded_filename, tree_filename)

# 메인 함수 실행
main()
